#ifndef STATEPLAY_HPP
#define STATEPLAY_HPP

#include "GameState.hpp"

class StateGame;
class Player;
class Level;

class StatePlay : public GejbEngine::GameState {
private:
	StateGame *game;
	Player *player;
	Level *currentLevel;
public:
	StatePlay(const StateGame*);
	~StatePlay();
	bool InitState() override;
	void KeyPressed(int key) override;
	void KeyReleased(int key) override;
	int UpdateState() override;
	void Draw() override;
	void ShutdownState() override;
	void updateController(const XINPUT_STATE &controllerState) override;
};

#endif