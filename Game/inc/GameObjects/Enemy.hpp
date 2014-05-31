#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Sprite.hpp"
#include "Player.hpp"
#include "level.hpp"

class Level;
class StateGame;
class lua_State;

const int tilewidth = 16;

class Enemy : public GejbEngine::Sprite{
private:
	GejbEngine::Timer reloadTimer;
	bool readyToFire;
	double rotationToPlayer;
	bool playerFound;
	double speed;
	double yVel, xVel;
	double tileWidth;
	double Enemy::radToDeg(double rad);
	Level *const level;
	GejbEngine::Texture *bulletTexture;
	const double bulletSpeed;
	StateGame *game;
	lua_State *lua;
	int state;
	int steps;

	int enemyId;

public:
	Enemy(Level *const l, StateGame *g, int eId);
	~Enemy();
	void Init(const GejbEngine::Texture *enemyTexture);
	void Update();
	void Collide(int objectType);
	void Move() override;
	bool checkWallCollision(void);
	void CollideWithWall();
	void move();

	void checkForPlayer(Player *player);

	int getSteps()const { return steps; }
	void ResetSteps(){ steps = 0; }

	int getEnemyId(){ return enemyId; }
};



#endif