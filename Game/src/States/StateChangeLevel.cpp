#include "StateChangeLevel.hpp"
#include "StateGame.hpp"
#include "Input.hpp"
#include "Graphics.hpp"

StateChangeLevel::StateChangeLevel(const StateGame *g) :
game(const_cast<StateGame*>(g)),
changeDone(false),
proceed(false),
finished(false),
levelcounter(1),
cont(0),
level1(),
level2(),
level3(),
first(true){
	levelList[0] = &level1;
	levelList[1] = &level2;
	levelList[2] = &level3;
	level = levelList;
}

StateChangeLevel::~StateChangeLevel(){

}

bool StateChangeLevel::InitState(){
	game->ClearEnemyList();
	changeDone = false;
	proceed = false;
	finished = false;
	cont = false;
	return true;
}

void StateChangeLevel::updateController(const XINPUT_STATE &controllerState) {
	if(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A
		 || controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) {
		startLevel();
	}
}

void StateChangeLevel::KeyPressed(int key){
	if(key == DIK_RETURN) {
		startLevel();
	}
}

void StateChangeLevel::KeyReleased(int key){

}

int StateChangeLevel::UpdateState(){
	if(changeDone == false){
		changeLevel();
		changeDone = true;
	}

	if(finished == true){
		return GAME_FINISHED;
	}
	if(proceed){
		return GO_TO_PLAY;
	} else{
		return NO_CHANGE;
	}
}

void StateChangeLevel::Draw(){
	int score(game->getScore());
	GejbEngine::DrawString("Level loaded, press enter to start next level", 100, 100);
	if(score){
		char SCORE[30];
		sprintf_s(SCORE, "score: %d", score);
		GejbEngine::DrawString(SCORE, 100, 150);
	}
}

void StateChangeLevel::ShutdownState(){

}


void StateChangeLevel::changeLevel() {
	GejbEngine::setPaused(true);

	if(first)
	{
		sprintf_s(levelname, "Resources/levels/level%d.lvl", levelcounter);
		game->setCurrentLevel((*level));
		(*level)->init(levelname, game);
		first = false;
	} else
	{
		if((100 - (game->getGameTimeMillis() / 1000)) > 0) {
			game->setScore(game->getScore() + 100 - game->getGameTimeMillis() / 1000);
		}
		++levelcounter;
		++level;
		if((level - levelList) >= 3) {
			if(std::ifstream("Save/save.sav")) {
				remove("Save/save.sav");
			}
			game->UpdateHighScore();
			finished = true;
		} else {
			//++level;
			file.open("Save/save.sav", std::ios::trunc | std::ios::out);
			file << levelcounter << std::endl << game->getScore() << std::endl;
			file.close();
			sprintf_s(levelname, "Resources/levels/level%d.lvl", levelcounter);
			game->setCurrentLevel((*level));
			(*level)->init(levelname, game);
		}
	}

}

void StateChangeLevel::startLevel() {
	if(cont){
		char arr[50];
		char *lc = arr, *sc = &arr[10];
		file.open("Save/save.sav", std::ios::in);
		if(!file.eof()) {
			file.getline(lc, 3);
			if(!file.eof()) {
				file.getline(sc, 15);
				int prevlvlcounter = levelcounter;

				levelcounter = atoi(lc);
				game->setScore(atoi(sc));

				level += levelcounter - prevlvlcounter;
				sprintf_s(levelname, "Resources/levels/level%d.lvl", levelcounter);
				game->setCurrentLevel((*level));
				(*level)->init(levelname, game);
			}
		}
	}
	proceed = true;
	GejbEngine::setPaused(false);
}
