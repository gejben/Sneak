#include "Game.hpp"
#include "StateGame.hpp"
#include "StateMainMenu.hpp"
#include "StateStartup.hpp"
#include <XInput.h>

using namespace GejbEngine;

GameState **gameState;

GameState *states[3];

StateStartup startup;
StateMainMenu mainMenu;
StateGame game;

bool gameover;

int i = 0;

static void changeState(int i) {
	(*gameState)->ShutdownState();
	gameState += i;
	if(!(*gameState)->InitState()) {
		FatalError("Failed to change state of game");
		--gameState;
	}
	if(*gameState == &game) {
		if(mainMenu.isContinuePressed()) {
			game.setContinue(true);
		}
	}
}


bool game_preload() {
	setTitle("WHOOP");
	setFullscreen(false);
	setScreenHeight(720);
	setScreenWidth(1280);
	setColorDepth(32);
	return true;
}

bool game_init(HWND hWnd) {
	states[0] = &startup;
	states[1] = &mainMenu;
	states[2] = &game;
	gameState = states;

	if((*gameState)->InitState()) {
		return true;
	}
	return false;
}

void game_update() {

	i = (*gameState)->UpdateState();

	if(i != 0) {
		changeState(i);
	}

}

void game_end() {
	(*gameState)->ShutdownState();
}

void game_graphics_update() {
	(*gameState)->Draw();
}

void game_keyPress(int key) {
	(*gameState)->KeyPressed(key);
}

void game_keyRelease(int key) {
	(*gameState)->KeyReleased(key);
}

void game_controllerUpdate(const XINPUT_STATE &controllerState) {
	if(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A
		 || controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B
		 || (controllerState.Gamepad.sThumbLX != 0)
		 || (controllerState.Gamepad.sThumbLY != 0)
		 || controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP
		 || controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN
		 || controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT
		 || controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT
		 || controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) {
		(*gameState)->updateController(controllerState);
	}
}

Enemy *game_getEnemyById(int id){
	return game.FindEnemyByID(id);
}