#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "ControllerInput.hpp"
#include <memory>

namespace GejbEngine {

	class GameState {
	public:
		GameState();
		virtual ~GameState();
		virtual bool InitState();
		virtual void KeyPressed(int key);
		virtual void KeyReleased(int key);
		virtual void updateController(const XINPUT_STATE &controllerState);
		virtual int UpdateState();
		virtual void Draw();
		virtual void ShutdownState();
	};
}

#endif