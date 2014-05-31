#ifndef STATEFINISH_HPP
#define STATEFINISH_HPP

#include "GameState.hpp"

class StateGame;

class StateFinish : public GejbEngine::GameState {
private:
	StateGame *game;
	bool finished;
public:
	StateFinish(const StateGame*);
	~StateFinish();
	bool InitState() override;
	void KeyPressed(int key) override;
	void KeyReleased(int key) override;
	int UpdateState() override;
	void Draw() override;
	void ShutdownState() override;
	void updateController(const XINPUT_STATE &controllerState) override;
};

#endif