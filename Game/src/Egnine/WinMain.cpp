

#include <Windows.h>
#include <iostream>
#include "WinMain.hpp"
#include "Engine.hpp"

#define KET_DOWN(vk) ((GetAsyncKeyState(vk) & 0x8000)?1:0)

HINSTANCE g_hInstance;
HWND g_hWnd;
int g_nCmdShow;

extern bool gameover;


//window event callback function
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_QUIT:
		case WM_CLOSE:
		case WM_DESTROY:
			gameover = true;
			break;

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstace, LPSTR lpCmdLine,
									 int nCmdShow) {
	MSG msg;
	srand((unsigned int)time(NULL));
	g_hInstance = hInstance;
	g_nCmdShow = nCmdShow;
	DWORD dwStyle, dwExStyle;
	RECT windowRect;

	GejbEngine::StartupEngine();

	if(!game_preload()) {
		MessageBox(g_hWnd, "Error in game preload!", "Error", MB_OK);
		return 0;
	}

	//get window caption string from engine
	char title[255];
	sprintf_s(title, "%s", GejbEngine::getTitle().c_str());

	//set window dimensions
	windowRect.left = (long)0;
	windowRect.right = (long)GejbEngine::getScreenWidth();
	windowRect.top = (long)0;
	windowRect.bottom = (long)GejbEngine::getScreenHeight();

	//create the window class structure

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	//fill the struct with info
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = title;
	wc.hIconSm = NULL;

	//set up the window with the class info
	RegisterClassEx(&wc);

	//set up the screen in windowed or fullscreen mode

	if(GejbEngine::getFullscreen()) {
		DEVMODE dm;
		memset(&dm, 0, sizeof(dm));
		dm.dmSize = sizeof(dm);
		dm.dmPelsWidth = GejbEngine::getScreenWidth();
		dm.dmPelsHeight = GejbEngine::getScreenHeight();
		dm.dmBitsPerPel = GejbEngine::getColorDepth();
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			MessageBox(NULL, "Display mode failed", NULL, MB_OK);
			GejbEngine::setFullscreen(false);
		}

		dwStyle = WS_POPUP;
		dwExStyle = WS_EX_APPWINDOW;
		ShowCursor(false);
	} else {
		dwStyle = WS_OVERLAPPEDWINDOW;
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	}

	//adjust window to true requested size
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	//create the program window
	g_hWnd = CreateWindowEx(
		0,
		title,
		title,
		dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, //x,y coordinate
		windowRect.right - windowRect.left,//window width
		windowRect.bottom - windowRect.top, //window height
		0, //parent window
		0, //menu
		g_hInstance, //application instance
		0); //window parameters

	if(!g_hWnd) {
		MessageBox(g_hWnd, "Error creating program window!", "Error", MB_OK);
		return 0;
	}

	//display window
	ShowWindow(g_hWnd, g_nCmdShow);
	UpdateWindow(g_hWnd);

	//initialize the engine
	GejbEngine::setWindowHandle(g_hWnd);
	if(!GejbEngine::InitEngine(GejbEngine::getScreenWidth(),
		GejbEngine::getScreenHeight(),
		GejbEngine::getColorDepth(), GejbEngine::getFullscreen())) {
		MessageBox(g_hWnd, "Error initializing the engine", "Error", MB_OK);
		return 0;
	}

	//main message loop
	gameover = false;
	while(!gameover) {
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		GejbEngine::UpdateEngine();
	}

	if(GejbEngine::getFullscreen()) {
		ShowCursor(TRUE);
	}

	GejbEngine::ShutdownEngine();

	return 1;
}