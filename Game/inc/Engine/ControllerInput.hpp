#ifndef XINPUT_HPP
#define XINPUT_HPP

#include "WinMain.hpp"
#include <Xinput.h>

namespace GejbEngine {
	bool InitController();
	bool UpdateController();
	const XINPUT_STATE &getControllerState();
}

#endif