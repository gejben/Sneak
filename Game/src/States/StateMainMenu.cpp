#include <fstream>
#include <sstream>
#include "StateMainMenu.hpp"
#include "Engine.hpp"

enum option {
	Continue,
	NewGame,
	HighScore,
	Exit
};

enum state {
	MAINMENU,
	SHOWHIGHSCORE
};

StateMainMenu::StateMainMenu() {
	int i = 0;
}

StateMainMenu::~StateMainMenu() {
}

bool StateMainMenu::InitState() {
	int i = 0;
	proceed = false;
	continueAvailable = false;
	continuePressed = false;
	GejbEngine::setClearColor(D3DCOLOR_RGBA(0x00, 0x00, 0x00, 255));
	state = MAINMENU;

	proceed = false;
	int x = 350;
	int y = 100;

	if(!GejbEngine::LoadSprite("Resources/Images/MenuOptions/Continue.png", textures[i++])) {
		GejbEngine::FatalError("Could Not load menu options");
	}

	if(!GejbEngine::LoadSprite("Resources/Images/MenuOptions/NewGame.png", textures[i++])) {
		GejbEngine::FatalError("Could Not load menu options");
	}

	if(!GejbEngine::LoadSprite("Resources/Images/MenuOptions/HighScore.png", textures[i++])) {
		GejbEngine::FatalError("Could Not load menu options");
	}

	if(!GejbEngine::LoadSprite("Resources/Images/MenuOptions/Exit.png", textures[i++])) {
		GejbEngine::FatalError("Could Not load menu options");
	}

	if(std::ifstream("Save/save.sav")) {
		continueAvailable = true;
	} else {
		continueAvailable = false;
	}
	i = 0;
	options[i].Init(textures[i]);
	options[i].setPosition(x, y);
	if(continueAvailable == false) {
		options[i].setDisabled(true);
	}
	++i;
	y += 100;

	options[i].Init(textures[i]);
	options[i].setPosition(x, y);
	++i;
	y += 100;

	options[i].Init(textures[i]);
	options[i].setPosition(x, y);
	++i;
	y += 100;

	options[i].Init(textures[i]);
	options[i].setPosition(x, y);
	++i;
	y += 100;

	optionPtr = &options[1];

	optionPtr->setSelected(true);
	index = 1;

	return 1;
}

void StateMainMenu::KeyPressed(int key) {
}

void StateMainMenu::KeyReleased(int key) {
	switch(state) {
		case MAINMENU:
			if(key == DIK_RETURN) {
				MenuSelect();
			}

			if(key == DIK_UP) {
				MenuUp();
			}

			if(key == DIK_DOWN) {
				MenuDown();
			}
			break;
		case SHOWHIGHSCORE:
			if(key == DIK_RETURN) {
				state = MAINMENU;
			}
			break;
	}
}

void StateMainMenu::updateController(const XINPUT_STATE &controllerState) {
}

int StateMainMenu::UpdateState() {
	for(int i = 0; i < 4; i++) {
		options[i].Update();
	}
	return proceed;
}

void StateMainMenu::Draw() {

	switch(state) {
		case MAINMENU:
			for(int i = 0; i < 4; i++) {
				GejbEngine::DrawSprite(&options[i]);
			}
			break;
		case SHOWHIGHSCORE:
			DrawHighscore();
			break;
	}
}

void StateMainMenu::ShutdownState() {
	for(int i = 0; i < 4; i++) {
		if(textures[i].imageLoaded)
		{
			GejbEngine::ReleaseTexture(&textures[i]);
		}
	}
}

void StateMainMenu::DrawHighscore() {
	int x = GejbEngine::getScreenWidth() / 2;
	int y = 50;
	readHighScore();

	GejbEngine::DrawString("High Score:", x, y);

	char hs[20];
	for(int i = 0; i < 10; i++) {
		y += 20;
		sprintf_s(hs, "%d: %d", i, highScore[i]);
		GejbEngine::DrawString(hs, x, y);
	}
	y += 50;
	GejbEngine::DrawString("Press Enter to return", x, y);
}

void StateMainMenu::readHighScore() {
	int i = 0;
	char ch;
	char input[50];
	std::ifstream fin("save/Highscore.txt");
	if(fin) {
		while(!fin.eof() && i < 10) {
			int a = 0;
			bool read = true;

			while(read) {
				fin.get(ch);
				if(ch == '\n') {
					read = false;
				} else {
					input[a] = ch;
					++a;
				}
			}
			input[a] = '\0';

			std::stringstream convert(input);
			if(!(convert >> highScore[i])) {
				highScore[i] = 0;
			}

			++i;
		}
		if(i < 9) {
			while(i <= 9) {
				highScore[i] = 0;
			}
		}
	} else { (std::ofstream("save/Highscore.txt")); }
}

void StateMainMenu::MenuUp() {
	optionPtr->setSelected(false);
	if(index == 0 || (continueAvailable == false && index == 1)) {
		index = 3;
		optionPtr = &options[3];
	} else {
		--index;
		--optionPtr;
	}
	optionPtr->setSelected(true);
}

void StateMainMenu::MenuDown() {
	optionPtr->setSelected(false);
	if(index == 3) {
		if(continueAvailable) {
			index = 0;
			optionPtr = &options[0];
		} else {
			index = 1;
			optionPtr = &options[1];
		}
	} else {
		++index;
		++optionPtr;
	}
	optionPtr->setSelected(true);
}

void StateMainMenu::MenuSelect() {
	switch(index) {
		case Continue:
			continuePressed = true;
			proceed = true;
			break;
		case NewGame:
			proceed = true;
			break;
		case HighScore:
			state = SHOWHIGHSCORE;
			break;
		case Exit:
			gameover = true;
			break;
	}
}

