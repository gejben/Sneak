#ifndef STATEMAINMENU_HPP
#define STATEMAINMENU_HPP

#include "GameState.hpp"
#include "MenuOption.hpp"
#include "Graphics.hpp"


class StateMainMenu : public GejbEngine::GameState {
private:
	GejbEngine::Timer timer;
	bool proceed;
	MenuOption options[5];
	GejbEngine::Texture textures[5];
	MenuOption *optionPtr;
	int bgy;
	int index;
	bool continueAvailable;
	bool continuePressed;
	int state;
	unsigned int highScore[10];

	void DrawHighscore();
	void readHighScore();


	void MenuUp();
	void MenuDown();
	void MenuSelect();

public:
	StateMainMenu();
	virtual ~StateMainMenu();
	bool InitState() override;
	void KeyPressed(int key) override;
	void KeyReleased(int key) override;
	void updateController(const XINPUT_STATE &controllerState) override;
	int UpdateState() override;
	void Draw() override;
	void ShutdownState() override;
	bool isContinuePressed() { return continuePressed; }
};

#endif