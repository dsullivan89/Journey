#include "cApplication.h"

#include<sstream>

BOOL IsMouseTouchingTile(
	long TileX, long TileY, // tile coordinates
	long TileWidth, long TileHeight, // tile dimensions
	long MouseX, long MouseY) // mouse coordinates
{
	// check if mouse too far left from tile
	if (MouseX < TileX) return FALSE;
	// check if mouse too far right from tile
	if (MouseX >= TileX + TileWidth) return FALSE;
	// check if mouse too far above tile
	if (MouseY < TileY) return FALSE;
	// check if mouse too far below tile
	if (MouseY >= TileY + TileHeight) return FALSE;
	// mouse must be touching tile
	return TRUE; // return success
}

// KILL ME WHEN DONE TESTING GetTileClicked()
HWND g_WinHandle = NULL;
bool g_EscapePressed = false;

void GetTileClicked(int mouseX, int mouseY, int& gridX, int& gridY, int halfTileWidth, int halfTileHeight, int windowWidth, int windowHeight)
{
	RECT clientRect = RECT();
	GetClientRect(g_WinHandle, &clientRect);

	int clientWidth = clientRect.right;
	int clientHeight = clientRect.bottom;

	int x_trans = mouseX - (clientWidth / 2);
	int y_trans = mouseY - (clientHeight / 2);

	int tileWidth = halfTileWidth * 2;
	int tileHeight = halfTileHeight * 2;
	

	int posX = x_trans / halfTileWidth;
	int posY = y_trans / halfTileHeight;

	gridX = posX - posY;
	gridY = posX + posY;

	//gridX = (x_trans / halfTileWidth + y_trans / halfTileHeight) / 2;
	//gridY = (y_trans / halfTileHeight - (x_trans / halfTileWidth)) / 2;

	std::stringstream gridPosText;
	gridPosText << "Grid X: " << gridX << " Grid Y: " << gridY;

	std::stringstream mousePosText;
	mousePosText << "Mouse X: " << mouseX << " Mouse Y: " << mouseY;

	std::stringstream transformedMousePosText;
	transformedMousePosText << "Transformed Mouse X: " << x_trans << " Y: " << -y_trans;

	if(!g_EscapePressed)
		SetWindowText(g_WinHandle, gridPosText.str().c_str());

	//gridX = x_trans / halfTileWidth;
	//gridY = y_trans / halfTileHeight;
	//int x_norm = mouseX / (windowWidth / 2);
	//int y_norm = mouseY / (windowHeight / 2);
	return;
}

float g_Scale = 1;


bool cApplication::Initialize(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed)
{
	WNDCLASSEX wc;    //Create a new extended windows class

	wc.cbSize = sizeof(WNDCLASSEX);    //Size of our windows class
	wc.style = CS_HREDRAW | CS_VREDRAW;    //class styles
	wc.lpfnWndProc = WindowProc;    //Default windows procedure function
	wc.cbClsExtra = NULL;    //Extra bytes after our wc structure
	wc.cbWndExtra = NULL;    //Extra bytes after our windows instance
	wc.hInstance = hInstance;    //Instance to current application
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);    //Title bar Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);    //Default mouse Icon
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);    //Window bg color
	wc.lpszMenuName = NULL;    //Name of the menu attached to our window
	wc.lpszClassName = m_WinClassName;    //Name of our windows class
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); //Icon in your taskbar

	if (!RegisterClassEx(&wc))    //Register our windows class
	{
		//if registration failed, display error
		MessageBox(NULL, "Error registering class",
			"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	m_hWnd = CreateWindowEx(    //Create our Extended Window
		NULL,    //Extended style
		m_WinClassName,    //Name of our windows class
		"Journey",    //Name in the title bar of our window
		WS_OVERLAPPEDWINDOW,    //style of our window
		CW_USEDEFAULT, CW_USEDEFAULT,    //Top left corner of window
		width,    //Width of our window
		height,    //Height of our window
		NULL,    //Handle to parent window
		NULL,    //Handle to a Menu
		hInstance,    //Specifies instance of current program
		this    //used for an MDI client window
	);

	g_WinHandle = m_hWnd;

	if (!m_hWnd)    //Make sure our window has been created
	{
		//If not, display error
		MessageBox(NULL, "Error creating window",
			"Error", MB_OK | MB_ICONERROR);
		return false;
	}
	else
	{
		m_WindowWidth = width;
		m_WindowHeight = height;
		m_hInstance = hInstance;
	}

	ShowWindow(m_hWnd, ShowWnd);    //Shows our window

	bool result;

	RECT clientRect = RECT();
	GetClientRect(m_hWnd, &clientRect);

	int clientWidth = clientRect.right;
	int clientHeight = clientRect.bottom;
	
	result = m_Graphics.Initialize(m_hWnd, clientWidth, clientHeight, true);

	if (!result)
		return false;

	Initialize_Game();

	return true;
}

int cApplication::Run()
{
	MSG msg;    //Create a new message structure
	ZeroMemory(&msg, sizeof(MSG));    //clear message structure to NULL

	while (true)    //while there is a message
	{
		//if there was a windows message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)    //if the message was WM_QUIT
				break;    //Exit the message loop

			TranslateMessage(&msg);     //Translate the message			
			DispatchMessage(&msg);		//Send the message to default windows procedure
		}

		else {    //Otherewise, keep the flow going
			Frame();
		}
	}

	Shutdown_Game();

	m_Graphics.Shutdown();

	return (int)msg.wParam;        //return the message
}

void cApplication::OnEnterResize()
{
}

void cApplication::OnExitResize()
{
}

void cApplication::OnResize(int newWidth, int newHeight)
{
	m_Graphics.OnResize(newWidth, newHeight);
}

void cApplication::OnActivate(bool isActive)
{
}

void cApplication::OnPause(bool isPaused)
{
	m_bPaused = isPaused;
}

void cApplication::OnMinimize(bool isMinimized)
{
	m_bMinimized = isMinimized;
}

void cApplication::OnMaximize(bool isMaximized)
{
	m_bMaximized = isMaximized;
}

void cApplication::OnMouseMove(int posX, int posY)
{
	m_MouseState.MouseX = posX;// +posX / 2;
	m_MouseState.MouseY = posY;// +posY / 2;
}

void cApplication::OnMouseLeftDown(int posX, int posY)
{
	m_MouseState.MouseX = posX;// +posX / 2;
	m_MouseState.MouseY = posY;// +posY / 2;
	m_MouseState.LeftBtnDown = true;
}

void cApplication::OnMouseLeftUp(int posX, int posY)
{
	m_MouseState.MouseX = posX;// +posX / 2;
	m_MouseState.MouseY = posY;// +posY / 2;
	m_MouseState.LeftBtnDown = false;
}

void cApplication::OnMouseLeftDoubleClick(int posX, int posY)
{
}

void cApplication::OnMouseRightDown(int posX, int posY)
{
	m_MouseState.MouseX = posX;// +posX / 2;
	m_MouseState.MouseY = posY;// +posY / 2;
	m_MouseState.RightBtnDown = true;
}

void cApplication::OnMouseRightUp(int posX, int posY)
{
	m_MouseState.MouseX = posX;// +posX / 2;
	m_MouseState.MouseY = posY;// +posY / 2;
	m_MouseState.RightBtnDown = false;
}

void cApplication::OnMouseRightDoubleClick(int posX, int posY)
{
}

void cApplication::OnKeyDown(int vkCode)
{
	if (vkCode == VK_ESCAPE) {
		g_EscapePressed = true;
		if (MessageBox(0, "Are you sure you want to exit?",
			"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)

		{
			DestroyWindow(m_hWnd);
		}
		else
			g_EscapePressed = false;
			////Release the windows allocated memory  
			//if (app != nullptr)
			//{
			//	app->m_isRunning = false;
			//}
			
	}
	
	if (vkCode == 0x57) // W
	{
		//sprite1.Y += 5;
		DRECT rect = sprite1.Rect;
		rect.top -= 0.1;
		rect.bottom -= 0.1;
		rect.left = 0.0;
		rect.right = 0.1;
		sprite1.Rect = rect;
		
	}
		
	if (vkCode == 0x53) // S
	{
		DRECT rect = sprite1.Rect;
		rect.top += 0.1;
		rect.bottom += 0.1;
		rect.left = 0.0;
		rect.right = 0.1;
		sprite1.Rect = rect;
	}

	if (vkCode == 0x41) // A
	{
		sprite1.X -= 5;
		DRECT rect = sprite1.Rect;
		rect.top = (float)HumanoidAnimations::Run/10;
		rect.bottom = rect.top + 0.1;
		rect.right += 0.1;
		rect.left += 0.1;
		
		sprite1.Rect = rect;
		sprite1.FlipHorizontal = true;
	}
		
	if (vkCode == 0x44) // D
	{
		sprite1.X += 5;
		DRECT rect = sprite1.Rect;
		rect.top = (float)HumanoidAnimations::Run/10;
		rect.bottom = rect.top + 0.1;
		rect.right += 0.1;
		rect.left += 0.1;

		sprite1.Rect = rect;
		sprite1.FlipHorizontal = false;
	}

	if (vkCode == 0x31)
	{
		m_Graphics.RSSolidMode();
	}
	if (vkCode == 0x32)
	{
		m_Graphics.RSWireFrameMode();
	}

	float scale = 0.1;

	if (vkCode == VK_UP)
		g_Scale += scale;
	if (vkCode == VK_DOWN)
		g_Scale -= scale;
		
}

void cApplication::OnKeyUp(int vkCode)
{
}

void cApplication::Frame()
{
	Input();
	Render();
}

void cApplication::Input()
{
	if (m_MouseState.LeftBtnDown)
	{
		
	}
}

void cApplication::Render()
{
	float red[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
	float white[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	float black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	m_Graphics.Clear(black);

	// rendering terrain
	int _layers = m_Map.GetLayerCount();
	for (int layer = 0; layer < _layers; layer++)
	{
		float _maxMapWidth = m_Map.GetLayerWidth(0) * m_Tiles.GetWidth(0);
		float _maxMapHeight = m_Map.GetLayerHeight(0) * m_Tiles.GetHeight(0) * m_Tiles.GetScaleY(0);

		int _mapWidth = m_Map.GetLayerWidth(layer);
		int _mapHeight = m_Map.GetLayerHeight(layer);
		int _area = _mapWidth * _mapHeight;

		int xInterval = m_Tiles.GetWidth(0);
		int yInterval = m_Tiles.GetHeight(0);
		int halfX = xInterval / 2;
		int halfY = yInterval / 2;
		float xScale = m_Tiles.GetScaleX(0);
		float yScale = m_Tiles.GetScaleY(0);
		float adjXInt = xInterval * xScale;
		float adjYInt = yInterval * yScale;

		int _tileWidth = m_Tiles.GetWidth(layer);
		int _tileHeight = m_Tiles.GetHeight(layer);
		float _tileScaleX = m_Tiles.GetScaleX(layer);
		float _tileScaleY = m_Tiles.GetScaleY(layer);

		char* _layerInfo = new char[_area];
		SpriteData* terrainSprites = new SpriteData[_area];
		memset(terrainSprites, 0, sizeof(SpriteData)*_area);
		m_Map.GetLayerInfo(layer, _layerInfo);

		for (int row = 0; row < _mapWidth; row++)
		{
			for (int column = 0; column < _mapHeight; column++)
			{
				int i = row * _mapWidth + column;

				if (_layerInfo[i] != -1)
				{

					// Smooth scrolling goes here.
					DRECT rect = m_Tiles.GetTile(layer, _layerInfo[i]);

					// adj*Int = adjusted intervals. 
					float ScreenX = (row - column) * (adjXInt / 2);
					float ScreenY = (row + column) * (adjYInt / 2);// -(_maxMapHeight / 2);

					

					terrainSprites[i].X = ScreenX;
					terrainSprites[i].Y = ScreenY;
					terrainSprites[i].Width = _tileWidth * _tileScaleX;
					terrainSprites[i].Height = _tileHeight * _tileScaleY;
					terrainSprites[i].Rect = rect;
					terrainSprites[i].FlipHorizontal = false;
					terrainSprites[i].FlipVertical = false;
					terrainSprites[i].ClipPixels = false;


					int x = -1;
					int y = -1;
					GetTileClicked(m_MouseState.MouseX, m_MouseState.MouseY, x, y, adjXInt / 2, adjYInt / 2, m_WindowWidth, m_WindowHeight);


					if (m_MouseState.LeftBtnDown)
					{
						
						m_MouseState.LeftBtnDown = false;
					}
						
				}
			}
		}

		// tell the graphics system about the tiles we want and our sprite data.
		m_Tiles.SetTileset(layer);
		m_Graphics.Render(terrainSprites, _area);

		// we are done rendering terrain, free this memory.
		delete[] _layerInfo;
		delete[] terrainSprites;
	}

	// rendering objects.
	// m_Graphics.Update(0.0);
	m_Tiles.SetTileset(2);
	float _tileWidth = m_Tiles.GetWidth(2);
	float _tileHeight = m_Tiles.GetHeight(2);
	float _tileScaleX = m_Tiles.GetScaleX(2);
	float _tileScaleY = m_Tiles.GetScaleY(2);

	float _adjTileSizeX = _tileWidth * _tileScaleX;
	float _adjTileSizeY = _tileHeight * _tileScaleY;

	sprite1.Width = _adjTileSizeX;
	sprite1.Height = _adjTileSizeY;
	sprite1.ClipPixels = false;
	m_Graphics.Render(&sprite1, 1);
	m_Graphics.Present();
}

void cApplication::Initialize_Game()
{
	m_Tiles.Create(&m_Graphics, 3);

	m_Tiles.Load(0, L"Textures/seasons_tiles.png", 128, 64);
	m_Tiles.Load(1, L"Textures/seasons_trees.png", 384, 256);
	m_Tiles.Load(2, L"Textures/Warrior_Spritesheet.png", 32, 32);

	//m_Tiles.SetScaleX(0, 2);
	//m_Tiles.SetScaleY(0, 2);
	
	//m_Tiles.SetScaleX(2, 2.0f);
	//m_Tiles.SetScaleY(2, 2.0f);

	int layerTiles = 3 * 3;
	SpriteData terrainSprites[3 * 3];

	char layer0Info[256] = {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	char layer1Info[256] = { 0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0,
							-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1, 0,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1, 0, 0,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1, 0, 0,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	};

	// Map and it's Layers	//	//	//
	m_Map.Create(2);
	m_Map.AddLayer(0, layer0Info, 16, 16);
	m_Map.AddLayer(1, layer1Info, 16, 16);
	//	//	//	//	//	//	//	//	//

	sprite1.Width = 32;// m_Texture.GetWidth() / 10;
	sprite1.Height = 32;// m_Texture.GetHeight() / 10;
	sprite1.X = 0;
	sprite1.Y = 0;
	sprite1.FlipHorizontal = false;
	
	// I'm thinking a 10x10 tile spritesheet.
	DRECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.bottom = 0.1;
	rect.right = 0.1;

	sprite1.Rect = rect;

	// debugging:
	m_nToRender = 16;
}

void cApplication::Shutdown_Game()
{
	m_Tiles.Destroy();
	m_Map.Destroy();
}

LRESULT CALLBACK cApplication::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	cApplication* app = (cApplication*)GetWindowLongPtr(hwnd, GWL_USERDATA);

	switch (msg)    //Check message
	{
	case WM_NCCREATE:
	{
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);
		break;
	}

	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			app->OnActivate(false);
		}
		else
		{
			app->OnActivate(true);
		}
		return 0;

	case WM_ENTERSIZEMOVE:
	{
		/*Event sizeEvent;
		sizeEvent.id = EventID::EnterSizeMove;
		sizeEvent.x = -1;
		sizeEvent.y = -1;
		g_Events.push(sizeEvent);*/
		if (app != nullptr)
		{
			app->m_bPaused = true;
			app->m_bResizing = true;
			// timer stop
		}
		return 0;
	}
	case WM_EXITSIZEMOVE:
	{
		/*Event sizeEvent;
		sizeEvent.id = EventID::ExitSizeMove;
		sizeEvent.x = -1;
		sizeEvent.y = -1;
		g_Events.push(sizeEvent);*/
		if (app != nullptr)
		{
			app->m_bPaused = false;
			app->m_bResizing = false;
			// timer start
		}
		return 0;
	}
	case WM_SIZE:
	{
		/*Event sizeEvent;
		sizeEvent.id = EventID::NewWindowSize;
		sizeEvent.x = LOWORD(lParam);
		sizeEvent.y = HIWORD(lParam);
		g_Events.push(sizeEvent);*/
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);

		if (wParam == SIZE_MINIMIZED)
		{
			if (app != nullptr)
			{
				app->m_bPaused = true;
				app->m_bMinimized = true;
				app->m_bMaximized = false;
			}
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			if (app != nullptr)
			{
				app->m_bPaused = false;
				app->m_bMinimized = false;
				app->m_bMaximized = true;
				app->OnResize(width, height);
			}
		}
		else if (wParam == SIZE_RESTORED)
		{
			if (app != nullptr)
			{
				// Restoring from minimized state?
				if (app->m_bMinimized)
				{
					app->m_bPaused = false;
					app->m_bMinimized = false;
					app->OnResize(width, height);
				}

				// Restoring from maximized state?
				else if (app->m_bMaximized)
				{
					app->m_bPaused = false;
					app->m_bMaximized = false;
					app->OnResize(width, height);
				}
				else if (app->m_bResizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					app->OnResize(width, height);
				}
			}
			
		}

		
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		/*Event sizeEvent;
		sizeEvent.id = EventID::LeftMouseDown;
		sizeEvent.x = GET_X_LPARAM(lParam);
		sizeEvent.y = GET_Y_LPARAM(lParam);
		g_Events.push(sizeEvent);*/
		if (app != nullptr)
			app->OnMouseLeftDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		/*Event sizeEvent;
		sizeEvent.id = EventID::RightMouseDown;
		sizeEvent.x = GET_X_LPARAM(lParam);
		sizeEvent.y = GET_Y_LPARAM(lParam);
		g_Events.push(sizeEvent);*/
		if (app != nullptr)
			app->OnMouseRightDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	case WM_LBUTTONUP:
	{
		/*Event sizeEvent;
		sizeEvent.id = EventID::LeftMouseUp;
		sizeEvent.x = GET_X_LPARAM(lParam);
		sizeEvent.y = GET_Y_LPARAM(lParam);
		g_Events.push(sizeEvent);*/
		if (app != nullptr)
			app->OnMouseLeftUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	case WM_RBUTTONUP:
	{
		/*Event sizeEvent;
		sizeEvent.id = EventID::RightMouseUp;
		sizeEvent.x = GET_X_LPARAM(lParam);
		sizeEvent.y = GET_Y_LPARAM(lParam);
		g_Events.push(sizeEvent);*/
		if (app != nullptr)
			app->OnMouseRightUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	case WM_LBUTTONDBLCLK:
	{
		/*Event sizeEvent;
		sizeEvent.id = EventID::LeftMouseDoubleClick;
		sizeEvent.x = GET_X_LPARAM(lParam);
		sizeEvent.y = GET_Y_LPARAM(lParam);
		g_Events.push(sizeEvent);*/
		if (app != nullptr)
			app->OnMouseLeftDoubleClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	case WM_RBUTTONDBLCLK:
	{
		/*Event sizeEvent;
		sizeEvent.id = EventID::RightMouseDoubleClick;
		sizeEvent.x = GET_X_LPARAM(lParam);
		sizeEvent.y = GET_Y_LPARAM(lParam);
		g_Events.push(sizeEvent);*/
		if (app != nullptr)
			app->OnMouseRightDoubleClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if (app != nullptr)
			app->OnMouseMove(xPos, yPos);

		return 0;
	}

	case WM_KEYUP:
		if (app != nullptr)
			app->OnKeyUp(wParam);
		//g_KeyBoardState.KeyUp((unsigned)wParam);
		return 0;
	case WM_KEYDOWN:    //For a key down
						//if escape key was pressed, display popup box
		if (app != nullptr)
			app->OnKeyDown(wParam);
		
		return 0;

	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 800;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 600;
		return 0;

	case WM_DESTROY:    //if x button in top right was pressed
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

cApplication::cApplication()
{
	m_WinClassName = "JourneyClient";
	m_hInstance = NULL;
	m_hWnd = NULL;

	DefaultWindowHeight = 600;
	DefaultWindowWidth = 800;

	m_bPaused = false;
	m_bMinimized = false;
	m_bMaximized = false;
	m_bResizing = false;
}


cApplication::~cApplication()
{
}
