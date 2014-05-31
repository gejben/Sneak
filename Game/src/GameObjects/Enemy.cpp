#include "Enemy.hpp"
#include "Bullet.hpp"
#include "level.hpp"
#include "StateGame.hpp"
#include "lua.hpp"
#include <math.h>
#include <memory.h>

const double M_PI = 3.14159265358979323846;

using namespace GejbEngine;
using std::shared_ptr;
using std::make_shared;

double Enemy::radToDeg(double rad) {
	return rad*(180 / M_PI);
}

int lua_checkWallCollision(lua_State *lua){
	int id = lua_tointeger(lua, 1);
	Enemy* enemy = game_getEnemyById(id);
	if(enemy != nullptr){
		lua_pushboolean(lua, true);
		lua_pushboolean(lua, enemy->checkWallCollision());
	} else{	//don't really know how to solve this in another way
		lua_pushboolean(lua, false);
		lua_pushboolean(lua, false);
	}
	return 2;
}

int lua_collideWall(lua_State *lua){
	int id = lua_tointeger(lua, 1);
	Enemy* enemy = game_getEnemyById(id);
	if(enemy != nullptr){
		enemy->CollideWithWall();
	}
	return 0;
}

int lua_resetSteps(lua_State *lua){
	int id = lua_tointeger(lua, 1);
	Enemy* enemy = game_getEnemyById(id);
	if(enemy != nullptr){
		enemy->ResetSteps();
	}
	return 0; 
}

Enemy::Enemy(Level *const l, StateGame *g, int eId):
readyToFire(true),
playerFound(false),
rotationToPlayer(0),
speed(4),
level(l),
yVel(0),
xVel(0),
bulletSpeed(7),
game(g),
steps(0),
enemyId(eId){
	lua = luaL_newstate();
	luaL_openlibs(lua);
}

Enemy::~Enemy() {
	lua_close(lua);
}

void Enemy::Init(const GejbEngine::Texture *enemyTexture){
	setTexture(enemyTexture);

	yVel = -speed;
	xVel = 0;

	setObjectType(ENEMY);
	setColor(D3DCOLOR_RGBA(255, 0, 0, 255));
	setAnimationDirection(1);
	setCurrentColumn(0);
	setEndColumn(7);
	setTotalColumns(8);
	setFrameWidth(16);
	setFrameHeight(16);
	setFrameTimer(100);
	setAnimated(true);
	UpdateBounds();

	setVelocity(xVel, yVel);
	reloadTimer.resetStopwatch();

	int result = luaL_dofile(lua, "script/Enemy.lua");
	if(result != 0)
	{
		GejbEngine::FatalError("Could not load Enemy.lua");
	}

	lua_pushinteger(lua, enemyId);
	lua_setglobal(lua, "enemyId");

	lua_register(lua, "checkCollision", lua_checkWallCollision);
	lua_register(lua, "resetSteps", lua_resetSteps);
	lua_register(lua, "collide", lua_collideWall);
}

void Enemy::Update() {
	checkForPlayer(&game->getPlayer());

	/* the function name */
	lua_getglobal(lua, "update");

	lua_call(lua, 0, 0);

	if(playerFound) {
		setRotation(rotationToPlayer);
		setCurrentRow(1);
		setCurrentColumn(0);
		setAnimated(false);
		if(readyToFire) {
			game->CreateBullet(bulletSpeed, getPosition(), rotationToPlayer);
			readyToFire = false;
			reloadTimer.resetStopwatch();
		}
	} else {
		setRotation(atan2(getVelocity().getY(), getVelocity().getX()));
		setCurrentRow(0);
		setAnimated(true);
	}

	if(reloadTimer.stopwatch(1000)) {
		readyToFire = true;
	}

}

bool Enemy::checkWallCollision(void){
	double yVel = (getVelocity().getY() != 0) ? (getVelocity().getY() / abs(getVelocity().getY())) : 0;
	double xVel = (getVelocity().getX() != 0) ? (getVelocity().getX() / abs(getVelocity().getX())) : 0;

	if(xVel > 0){
		if(level->getTile((getPosition().getX() + (xVel*getFrameWidth())), getPosition().getY()) ==
			 '1') {
			return true;
		} else{
			return false;
		}
	} else if(xVel < 0){
		if(level->getTile(getPosition().getX(), getPosition().getY()) == '1') {
			return true;
		} else{
			return false;
		}
	} else if(yVel > 0){
		if(level->getTile(getPosition().getX() + (getFrameWidth() / 2), (getPosition().getY() + yVel * getFrameHeight())) == '1') {
			return true;
		} else{
			return false;
		}
	} else if(yVel < 0){
		if(level->getTile(getPosition().getX() + (getFrameWidth() / 2), (getPosition().getY() + (yVel * getFrameHeight() / 2))) == '1') {
			return true;
		} else{
			return false;
		}
	}
}

void Enemy::CollideWithWall() {
	setPosition(getPosition() - getVelocity());

	setVelocity(-getVelocity().getY(), getVelocity().getX());
}

void Enemy::Collide(int objectType) {
	switch(objectType) {
		case 1:
			setAlive(false);
			break;
		default:
			break;
	}

}

void Enemy::Move(){
	if(playerFound == false){
		Sprite::Move();
		steps += getVelocity().Length();
		lua_pushinteger(lua, steps);
		lua_setglobal(lua, "steps");
	}

}

void Enemy::checkForPlayer(Player *player) {

	GejbEngine::Vector3 playerPos = player->getPosition();

	if(getPosition().Distance(player->getPosition()) < 275) {

		rotationToPlayer = atan2(playerPos.getY() - getPosition().getY(),
														 playerPos.getX() - getPosition().getX());

		if(radToDeg(getRotation()) - radToDeg(rotationToPlayer) <= 60
			 && radToDeg(getRotation()) - radToDeg(rotationToPlayer) >= -60) {
			if(level->testForWall(getPosition(), player->getPosition()) == false) {
				playerFound = true;
			} else {
				playerFound = false;
			}
		}
	} else {
		playerFound = false;
	}
}