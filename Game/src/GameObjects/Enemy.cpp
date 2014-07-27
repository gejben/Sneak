#include "Enemy.hpp"
#include "Bullet.hpp"
#include "level.hpp"
#include "StateGame.hpp"
#include "lua.hpp"
#include <math.h>
#include <memory.h>
#include <algorithm>
#include <stdio.h>

const double M_PI = 3.14159265358979323846;

using std::shared_ptr;
using std::make_shared;

double Enemy::radToDeg(double rad) {
	return rad*(180 / M_PI);
}

int lua_checkForPlayer(lua_State *lua){
	int id = lua_tointeger(lua, 1);
	Enemy* enemy = game_getEnemyById(id);
	if(enemy != nullptr){
		lua_pushboolean(lua, enemy->getPlayerFound());
		return 1;
	}
	lua_pushboolean(lua, false);
	return 1;
}

int lua_chase(lua_State *lua){
	int id = lua_tointeger(lua, 1);
	Enemy *enemy = game_getEnemyById(id);

	if(enemy != nullptr){
		enemy->chase();
		lua_pushboolean(lua, true);
		return 1;
	}
	lua_pushboolean(lua, false);
	return 1;
}

int lua_pathFind(lua_State *lua){
	int id = lua_tointeger(lua, 1);
	Enemy *enemy = game_getEnemyById(id);

	if(enemy != nullptr){
		
		enemy->pathFind();
		lua_pushboolean(lua, true);
		return 1;
	}
	lua_pushboolean(lua, false);
	return 1;
}

int lua_nextTile(lua_State *lua){
	int id = lua_tointeger(lua, 1);
	Enemy *enemy = game_getEnemyById(id);

	if(enemy != nullptr){
		enemy->nextTile();
		lua_pushboolean(lua, true);
		return 1;
	}
	lua_pushboolean(lua, false);
	return 1;
}

int lua_returnedToStart(lua_State *lua){
	int id = lua_tointeger(lua, 1);
	Enemy *enemy = game_getEnemyById(id);

	if(enemy != nullptr){
		lua_pushboolean(lua, enemy->atStartPos());
		return 1;
	}
	lua_pushboolean(lua, false);
	return 1;
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
enemyId(eId),
startPos(0,0){
	lua = luaL_newstate();
	luaL_openlibs(lua);
}

Enemy::~Enemy() {
	lua_close(lua);
}

void Enemy::Init(const GejbEngine::Texture *enemyTexture, double x, double y){
	setTexture(enemyTexture);
	startPos.Set(x, y);
	setPosition(x, y);
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

	lua_register(lua, "checkForPlayer", lua_checkForPlayer);
	lua_register(lua, "chase", lua_chase);
	lua_register(lua, "searchStartpos", lua_pathFind);
	lua_register(lua, "nextTile", lua_nextTile);
	lua_register(lua, "returnedToStart", lua_returnedToStart);
}

enum States{
	PATROLLING = 0,
	CHASING = 1,
	RETURNING = 2
};

void Enemy::Update() {
	checkForPlayer(&game->getPlayer());
	switch(state){
		case PATROLLING:
			setRotation(atan2(getVelocity().getY(), getVelocity().getX()));
			setCurrentRow(0);
			setAnimated(true);
			break;
		case CHASING:
			setRotation(atan2(getVelocity().getY(), getVelocity().getX()));
			setCurrentRow(1);
			setCurrentColumn(0);
			setAnimated(false);
			if(readyToFire) {
				game->CreateBullet(bulletSpeed, getPosition()+getVelocity(), rotationToPlayer);
				readyToFire = false;
				reloadTimer.resetStopwatch();
			}
			break;
		case RETURNING:
			setRotation(atan2(getVelocity().getY(), getVelocity().getX()));
			setCurrentRow(0);
			setAnimated(true);
			break;
	}

	if(reloadTimer.stopwatch(1000)) {
		readyToFire = true;
	}

}

bool Enemy::checkWallCollision(void){
	GejbEngine::Vector3 directionX(getVelocity().getX(), 0);

	GejbEngine::Vector3 directionY(0, getVelocity().getY());


	if(level->getTile(getPosition(), directionX) == L_WALL){
		CollideWithWall();
	}

	if(level->getTile(getPosition(), directionY) == L_WALL){
		CollideWithWall();
	}

	return true;
}

void Enemy::CollideWithWall() {
	setVelocity(-getVelocity().getY(), getVelocity().getX());
}

void Enemy::Collide(int objectType) {
	switch(objectType) {
		case 1:
			//setAlive(false);
			break;
		default:
			break;
	}
}

bool Enemy::Move(){
	if(steps == tilewidth){

		checkWallCollision();

		/* the function name */
		lua_getglobal(lua, "update");

		int result = lua_pcall(lua, 0, 1, 0);

		if(result != 0){
			std::string str = "Error in enemy.lua update function:\n";
			str.append(lua_tostring(lua, -1));
			GejbEngine::FatalError(str);
		}

		state = lua_tonumber(lua, -1);

		steps = 0;
	}
	if(Sprite::Move()){
		steps += getVelocity().Length();
		return true;
	}
	else{
		return false;
	}
}

bool Enemy::atStartPos(){
	if(getPosition() == startPos){
		setVelocity(0, -speed);
		return true;
	}
	return false;
}

void Enemy::chase(){
	GejbEngine::Vector3 playerPos(game->getPlayer().getPosition());
	GejbEngine::Vector3 directionVector((getPosition() - playerPos));
	double dirX(directionVector.getX());
	double dirY(directionVector.getY());

	if(abs(dirX) >= abs(dirY)){
		if(dirX > 0) setVelocity(-speed, 0);
		else setVelocity(speed, 0);
	} else{
		if(dirY > 0) setVelocity(0, -speed);
		else setVelocity(0, speed);
	}
}

void Enemy::nextTile(){
	if(getPosition() != startPos){
		GejbEngine::Vector3 directionVector((getPosition() - *path.begin()).Normal());

		double dirX(directionVector.getX());
		double dirY(directionVector.getY());

		if(abs(dirX) >= abs(dirY)){
			if(dirX > 0) setVelocity(-speed, 0);
			else setVelocity(speed, 0);
		} else{
			if(dirY > 0) setVelocity(0, -speed);
			else setVelocity(0, speed);
		}

		path.pop_front();
	}
	else setVelocity(0, 0);
}

void Enemy::checkForPlayer(Player *player) {

	GejbEngine::Vector3 playerPos = player->getPosition();

	if(getPosition().Distance(player->getPosition()) < 150) {

		rotationToPlayer = atan2(playerPos.getY() - getPosition().getY(),
														 playerPos.getX() - getPosition().getX());

		if(radToDeg(getRotation()) - radToDeg(rotationToPlayer) <= 50
			 && radToDeg(getRotation()) - radToDeg(rotationToPlayer) >= -50) {
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

bool Enemy::tileSorter(shared_ptr<TileState> const lhs, shared_ptr<TileState> const rhs){
	return lhs->f < rhs->f;
}

double Enemy::lengthToGoal(GejbEngine::Vector3 pos){
	return pos.Distance(startPos);
}


void Enemy::testDirection(GejbEngine::Vector3 pos, shared_ptr<TileState> state){
	bool previouslyVisited = false;
	for each(shared_ptr<TileState> state in visited){
		if(pos == state->position){
			previouslyVisited = true;
			break;
		}
	}
	if(previouslyVisited == false){
		shared_ptr<TileState> newState = make_shared<TileState>();
		newState->position = pos;
		newState->parentState = state;
		newState->g = newState->parentState->g + 1;
		newState->h = lengthToGoal(pos);
		newState->f = newState->g + newState->h;
		open.push_back(newState);
		visited.push_back(newState);

		double bajs = newState->h;
	}
}


void Enemy::checkSurrounding(shared_ptr<TileState> state){

	GejbEngine::Vector3 direction(tilewidth, 0);
	if(level->getTile(state->position, direction) != L_WALL){
		testDirection((state->position + direction), state);
	}

	direction = GejbEngine::Vector3(-tilewidth, 0);
	if(level->getTile(state->position, direction) != L_WALL){
		testDirection((state->position + direction), state);
	}

	direction = GejbEngine::Vector3(0, tilewidth);
	if(level->getTile(state->position, direction) != L_WALL){
		testDirection((state->position + direction), state);
	}

	direction = GejbEngine::Vector3(0, -tilewidth);
	if(level->getTile(state->position, direction) != L_WALL){
		testDirection((state->position + direction), state);
	}


	std::sort(open.begin(), open.end(), &tileSorter);
	int size = open.size();
}

void Enemy::pathFind(){
	setVelocity(GejbEngine::Vector3(0, 0));
	open.clear();
	visited.clear();
	path.clear();
	open.push_back(make_shared<TileState>());
	open.at(0)->position = getPosition();
	while(path.size() == 0){
		shared_ptr<TileState> pState = open.at(0);
		open.erase(open.begin());
		checkSurrounding(pState);
		if(pState->position == startPos){
			GejbEngine::Vector3 position = pState->position;
			shared_ptr<TileState> tempState = pState;
			while(position != getPosition()){
				path.push_front(position);
				tempState = tempState->parentState;
				position = tempState->position;
			}
		}
	}
}

