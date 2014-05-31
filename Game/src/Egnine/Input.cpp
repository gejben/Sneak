

#include "Input.hpp"

namespace GejbEngine {

	//private variables
	HWND window = NULL;
	LPDIRECTINPUT8 di = NULL;
	LPDIRECTINPUTDEVICE8 keyboard = NULL;
	char keyState[256];
	LPDIRECTINPUTDEVICE8 mouse = NULL;
	DIMOUSESTATE mouseState;
	POINT position;

	bool mouseInitialized, keyboardInitialized;

	void InitInput(HWND hwnd) {
		window = hwnd;
		DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
											 IID_IDirectInput8, (void **)&di, NULL);

		HRESULT result;

		//init keyboard
		di->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
		keyboard->SetDataFormat(&c_dfDIKeyboard);
		keyboard->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		result = keyboard->Acquire();

		//init mouse
		di->CreateDevice(GUID_SysMouse, &mouse, NULL);
		mouse->SetDataFormat(&c_dfDIMouse);
		mouse->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		mouse->Acquire();
	}

	void ShutdownInput() {
		if(di) {
			di->Release();
		}
		if(keyboard) {
			keyboard->Release();
		}
		if(mouse) {
			mouse->Release();
		}
	}

	void UpdateInput() {
		//poll keyboard-state
		keyboard->Poll();
		if(!SUCCEEDED(keyboard->GetDeviceState(256, (LPVOID)&keyState))) {
			//keyboard-device lost
			keyboard->Acquire(); //retry to aquire
		}

		//poll mouse-state
		mouse->Poll();
		if(!SUCCEEDED(mouse->GetDeviceState(sizeof(DIMOUSESTATE), &keyState))) {
			//mouse-device lost
			mouse->Acquire(); //retry to aquire
		}
	}


	bool GetMouseButton(char button) {
		return (mouseState.rgbButtons[button] & 0x80);
	}

	char GetKeyState(int key) {
		return keyState[key];
	}

	long GetPosX() {
		return position.x;
	}
	long GetPosY() {
		return position.y;
	}

	long GetdX() {
		return mouseState.lX;
	}
	long GetdY() {
		return mouseState.lY;
	}
	long GetdWheel() {
		return mouseState.lZ;
	}
}