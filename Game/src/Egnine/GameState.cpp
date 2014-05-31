#include "GameState.hpp"

namespace GejbEngine {

	GameState::GameState() {
	}

	GameState::~GameState() {
	}

	bool GameState::InitState() {
		return 1;
	}

	void GameState::KeyPressed(int key) {

	}

	void GameState::KeyReleased(int key) {

	}

	void GameState::updateController(const XINPUT_STATE &controllerState) {

	}

	int GameState::UpdateState() {
		return 0;
	}

	void GameState::Draw() {
	}

	void GameState::ShutdownState() {
	}
}