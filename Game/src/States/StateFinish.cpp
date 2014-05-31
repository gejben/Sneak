#include "StateFinish.hpp"
#include "StateGame.hpp"

StateFinish::StateFinish(const StateGame *g) :
game(const_cast<StateGame*>(g)),
finished(false){
}

StateFinish::~StateFinish(){

}

bool StateFinish::InitState(){
	finished = false;
	game->WriteHighScore();
	return true;
}

void StateFinish::KeyPressed(int key){

}

void StateFinish::KeyReleased(int key){
	if(key == DIK_RETURN) {
		finished = true;
	}
}

void StateFinish::updateController(const XINPUT_STATE &controllerState) {
	if(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A
		 || controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) {
		finished = true;
	}
}

int StateFinish::UpdateState(){
	if(finished == true){
		return GAME_FINISHED;
	} else return NO_CHANGE;
}

void StateFinish::Draw(){
	char FINALSCORE[30];
	sprintf_s(FINALSCORE, "final score: %d", (int)game->getScore());
	GejbEngine::DrawString("Congratulations you finished the game!", 100, 100);
	GejbEngine::DrawString(FINALSCORE, 100, 150);
}

void StateFinish::ShutdownState(){

}