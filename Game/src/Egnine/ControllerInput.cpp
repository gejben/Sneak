
#include "ControllerInput.hpp"

namespace GejbEngine {
	XINPUT_STATE controllerState;
	bool controllerInitialized;

	bool InitController() {
		XINPUT_CAPABILITIES caps;
		ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));
		XInputGetCapabilities(0/*controller number*/, XINPUT_FLAG_GAMEPAD, &caps);
		if(caps.Type != XINPUT_DEVTYPE_GAMEPAD) {
			return false;
		} else {
			return true;
		}
	}


	bool UpdateController() {
		//check controller state
		ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

		DWORD result = XInputGetState(0, &controllerState);

		if(result == 0) {
			return true;
		} else {
			return false;
		}
	}

	const XINPUT_STATE &getControllerState() {
		return controllerState;
	}
}