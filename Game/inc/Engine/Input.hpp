
#ifndef INPUT_HPP
#define INPUT_HPP

#include "WinMain.hpp"
#include <dinput.h>



namespace GejbEngine {
	void InitInput(HWND window);

	void UpdateInput();
	void ShutdownInput();
	bool GetMouseButton(char button);
	char GetKeyState(int key);

	long GetPosX();
	long GetPosY();

	long GetdX();
	long GetdY();
	long GetdWheel();
}

#endif