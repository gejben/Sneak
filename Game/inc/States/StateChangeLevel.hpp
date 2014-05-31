#ifndef STATECHANGELEVEL_HPP
#define STATECHANGELEVEL_HPP

#include <iostream>
#include <fstream>
#include "GameState.hpp"
#include "level.hpp"

class StateGame;

enum{ NUMBER_OF_LEVELS = 3 };

class StateChangeLevel : public GejbEngine::GameState {
private:
	StateGame *game;

	Level **level;

	Level level1;
	Level level2;
	Level level3;

	Level *levelList[NUMBER_OF_LEVELS];

	int levelcounter;
	bool changeDone;
	bool finished;
	bool proceed;
	bool cont;
	bool first;
	char levelname[50];
	std::fstream file;
	void startLevel();
public:
	StateChangeLevel(const StateGame*);
	~StateChangeLevel();
	bool InitState() override;
	void KeyPressed(int key) override;
	void KeyReleased(int key) override;
	int UpdateState() override;
	void Draw() override;
	void ShutdownState() override;
	void updateController(const XINPUT_STATE &controllerState) override;
	void changeLevel();
	void setContinue(bool value){ cont = value; }

};

#endif