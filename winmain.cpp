#include"cApplication.h"



int WINAPI WinMain(HINSTANCE hInstance,    //Main windows function
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{

	cApplication app;

	//Initialize Window//
	if (!app.Initialize(hInstance, nShowCmd, 800, 600, true))
	{
		//If initialization failed, display an error message
		MessageBox(0, "Window Initialization - Failed",
			"Error", MB_OK);
		return 0;
	}

	return app.Run();
}