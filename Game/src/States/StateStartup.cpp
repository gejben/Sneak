#include "StateStartup.hpp"
#include "Engine.hpp"

using namespace GejbEngine;

StateStartup::StateStartup() {
}

StateStartup::~StateStartup() {
}

bool StateStartup::InitState() {
	timer.resetStopwatch();
	if(!LoadTexture("Resources/Images/splash.png", &sp)) {
		return false;
	}
	return true;
}

void StateStartup::KeyPressed(int key) {

}

void StateStartup::KeyReleased(int key) {

}

int StateStartup::UpdateState() {
	if(timer.stopwatch(2000)) {
		return 1;
	}
	return 0;
}

void StateStartup::Draw() {
	DrawTexture(&sp, 0, 0);
}

void StateStartup::ShutdownState() {
	ReleaseTexture(&sp);
}