#include "StatePlay.hpp"
#include "StateGame.hpp"
#include "Player.hpp"
#include "Input.hpp"
#include "Level.hpp"

StatePlay::StatePlay(const StateGame* g) :
game(const_cast<StateGame*>(g)){

}

StatePlay::~StatePlay(){

}

bool StatePlay::InitState(){
	player = &game->getPlayer();
	currentLevel = game->getCurrentLevel();
	player->setPosition(currentLevel->getStartingPoint());

	if(!GejbEngine::LoadSprite("Resources/Images/bg.png", background))
	{
		GejbEngine::FatalError("Failed to initialize background Texture");
		return false;
	}
	background.color = D3DCOLOR_RGBA(0, 0, 255, 255);

	return true;
}

void StatePlay::KeyPressed(int key){
	const int playerspeed = 4;
	if(key == DIK_UP) {
		player->setNextVelocity(0, -playerspeed);
	}
	if(key == DIK_DOWN) {
		player->setNextVelocity(0, playerspeed);
	}
	if(key == DIK_LEFT) {
		player->setNextVelocity(-playerspeed, 0);
	}
	if(key == DIK_RIGHT) {
		player->setNextVelocity(playerspeed, 0);
	}
	if(key == DIK_ESCAPE) {
	}
}

void StatePlay::KeyReleased(int key){

}

void StatePlay::updateController(const XINPUT_STATE &controllerState) {
	if(controllerState.Gamepad.sThumbLX > 10000
		 || controllerState.Gamepad.sThumbLX < -10000) {
		player->setXvelocity(((double)controllerState.Gamepad.sThumbLX / 32726) * 4);
	}
	if(controllerState.Gamepad.sThumbLY > 10000
		 || controllerState.Gamepad.sThumbLY < -10000) {
		player->setYvelocity(-((double)controllerState.Gamepad.sThumbLY / 32726) * 4);
	}
	if(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
		player->setYvelocity(-4);
	}
	if(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
		player->setYvelocity(4);
	}
	if(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
		player->setXvelocity(-4);
	}
	if(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
		player->setXvelocity(4);
	}
	if(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) {
	}
}

int StatePlay::UpdateState(){
	//currentLevel->checkWallCollision(*player);

	if(currentLevel->checkFinish(*player)) {

		return GO_TO_NEXT_LEVEL;
	}
	if(player->getAlive() == false) {
		player->setPosition(currentLevel->getStartingPoint());
		player->setAlive(true);
	}
	
	return 0;
}

void StatePlay::Draw(){
	currentLevel->draw();
	GejbEngine::DrawTexture(&background, 0, 0);
}

void StatePlay::ShutdownState(){
	GejbEngine::ReleaseTexture(&background);
}
