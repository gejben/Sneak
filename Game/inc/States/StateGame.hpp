#ifndef STATEGAME_HPP
#define STATEGAME_HPP

#include <iostream>
#include <fstream>
#include <list>
#include "Timer.hpp"
#include "GameState.hpp"
#include "level.hpp"
#include "StateChangeLevel.hpp"
#include "StatePlay.hpp"
#include "StateFinish.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

enum{
	NO_CHANGE,
	GO_TO_NEXT_LEVEL,
	GO_TO_PLAY,
	END_GAME,
	GAME_FINISHED
};

class Bullet;

class StateGame : public GejbEngine::GameState {
private:
	StatePlay play;
	StateChangeLevel changeLevel;
	StateFinish finish;

	GejbEngine::GameState *states[3];
	GejbEngine::GameState **currentState;
	GejbEngine::Timer GameTimer;

	GejbEngine::Texture playerTexture;
	GejbEngine::Texture enemyTexture;
	GejbEngine::Texture bulletTexture;
	GejbEngine::Texture itemTexture;

	Player player;
	std::list<Enemy*> enemyList;
	Level *currentLevel;
	double score;
	bool cont;

	std::fstream file;
	unsigned int highScore[10];

	void StateGame::changeState(int toState);
	void updateEnemies();
	void updateBullets();

public:
	StateGame();
	~StateGame();
	bool InitState() override;
	void KeyPressed(int key) override;
	void KeyReleased(int key) override;
	void updateController(const XINPUT_STATE &controllerState) override;
	int UpdateState() override;
	void Draw() override;
	void ShutdownState() override;
	void UpdateHighScore();
	void WriteHighScore();
	void ReadHighScore();

	void CreateItem(double x, double y);

	void CreateEnemy(double x, double y);
	void ClearEnemyList();

	void CreateBullet(double bulletSpeed, GejbEngine::Vector3 position, double rotation);

	Player &getPlayer(){ return player; }
	const double getScore(){ return score; }
	void setScore(double s){ score = s; }

	void setContinue(bool val){ cont = val; }
	void setCurrentLevel(Level*level){ currentLevel = level; }
	Level *getCurrentLevel(){ return currentLevel; }
	DWORD getGameTimeMillis(){ return GameTimer.getStartTimerMillis(); }


	Enemy* FindEnemyByID(int enemyId);
};


#endif