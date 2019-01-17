#pragma once

//
// "The True Test of a Person's Character Is What They Do When No One Is Watching."
//

#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<windowsx.h>

#include"cGraphics.h"
#include"cTexture.h"

#include"cMap.h"

#define WARRIOR 0
#define RANGER 1
#define ROGUE 2
#define CLERIC 3
#define ORC 4

/* Rendering the map or any objects. 
- Map[][] array used to store tile info.
- SpriteData can be used to render each tile or object.
- Feed all SpriteData into the renderer as an array,
	sorted by texture used and z-order
*/

enum HumanoidAnimations
{
	Idle = 0,
	Dodge,
	Run,
	Attack,
	Die
};

class cApplication
{
private:
	LPCTSTR m_WinClassName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	int m_WindowWidth;
	int m_WindowHeight;
	int DefaultWindowHeight;
	int DefaultWindowWidth;
	bool m_bPaused;
	bool m_bResizing;
	bool m_bMinimized;
	bool m_bMaximized;

	void OnEnterResize();
	void OnExitResize();
	void OnResize(int newWidth, int newHeight);
	void OnActivate(bool isActive);
	void OnPause(bool isPaused);
	void OnMinimize(bool isMinimized);
	void OnMaximize(bool isMaximized);

	void OnMouseMove(int posX, int posY);
	void OnMouseLeftDown(int posX, int posY);
	void OnMouseLeftUp(int posX, int posY);
	void OnMouseLeftDoubleClick(int posX, int posY);
	void OnMouseRightDown(int posX, int posY);
	void OnMouseRightUp(int posX, int posY);
	void OnMouseRightDoubleClick(int posX, int posY);

	void OnKeyDown(int vkCode);
	void OnKeyUp(int vkCode);

	void Frame();

	cGraphics m_Graphics;
	cTexture m_WallsTexture;
	cTexture m_WarriorTexture;
	cTexture m_TerrainTexture;

	cMap m_Map;

	SpriteData sprite1;
	int m_nToRender;

	void Initialize_Game();
	void Shutdown_Game();
	
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool Initialize(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);

	int Run();

	

	cApplication();
	virtual ~cApplication();
};

