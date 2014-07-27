#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Sprite.hpp"
#include "Player.hpp"
#include "level.hpp"
#include <vector>
#include <list>
#include <memory>

class Level;
class StateGame;
class lua_State;

const int tilewidth = 16;

class Enemy : public GejbEngine::Sprite{
private:
	GejbEngine::Timer reloadTimer;
	GejbEngine::Vector3 startPos;
	bool readyToFire;
	double rotationToPlayer;
	bool playerFound;
	double speed;
	double yVel, xVel;
	double Enemy::radToDeg(double rad);
	Level *const level;
	GejbEngine::Texture *bulletTexture;
	const double bulletSpeed;
	StateGame *game;
	lua_State *lua;
	int state;
	int steps;

	int enemyId;

	struct TileState{
		GejbEngine::Vector3 position;
		std::shared_ptr<TileState> parentState;
		double g = 0;
		double h = 0;
		double f = 0;
	};
	TileState startPosition;
	std::vector<std::shared_ptr<TileState>> open;
	std::vector<std::shared_ptr<TileState>> visited;

	std::list<GejbEngine::Vector3>path;

	

	void checkSurrounding(std::shared_ptr<TileState> state);
	void testDirection(GejbEngine::Vector3 direction, std::shared_ptr<TileState> state);
	double lengthToGoal(GejbEngine::Vector3 position);

	static bool tileSorter(std::shared_ptr<TileState> const lhs, std::shared_ptr<TileState> const rhs);

public:
	Enemy(Level *const l, StateGame *g, int eId);
	~Enemy();
	void Init(const GejbEngine::Texture *enemyTexture, double x, double y);
	void Update();
	void Collide(int objectType);
	bool Move() override;
	bool checkWallCollision(void);
	void CollideWithWall();
	void move();

	void chase();
	void pathFind();

	void nextTile();

	void checkForPlayer(Player *player);

	int getSteps()const { return steps; }
	void ResetSteps(){ steps = 0; }

	int getEnemyId(){ return enemyId; }
	bool getPlayerFound(){ return playerFound; }

	void setState(int s){ state = s; }

	bool atStartPos();
};



#endif