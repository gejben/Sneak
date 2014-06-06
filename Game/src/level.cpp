#include "level.hpp"
#include <fstream>
#include <memory.h>
#include "Enemy.hpp"
#include "Player.hpp"
#include "StateGame.hpp"

Level::Level() :
filename(""),
startX(0),
startY(0),
startingPoint(0, 0),
wall(){
	game = nullptr;
	wall.collidable = 0;
	wall.color = 0;
	wall.height = 0;
	wall.imageLoaded = 0;
	wall.textureIndex = 0;
	wall.width = 0;
}

Level::~Level() {
}

bool Level::init(std::string filename, StateGame *game) {
	if(!LoadTexture("Resources/Images/wall.png", &wall)){
		return false;
	}
	wall.color = 0xFFFFFFFF;
	char ch;
	int i = 0, j = 0;
	std::ifstream fin(filename);
	while(!fin.eof()) {
		fin.get(ch);
		if(ch == '\n') {
			map[j][i] = NULL;
			i++; j = 0;
		} else {
			map[j][i] = ch;
			if(ch == 'e') {
					game->CreateEnemy(j*wall.width, i*wall.height);
			} else if(ch == 'i'){
				game->CreateItem(j*wall.width, i*wall.height);
			}else if(ch == 's') {
				startX = j*wall.width;
				startY = i*wall.height;
			}
			j++;
		}
	}
	startingPoint.Set(startX, startY);
	return true;
}

void Level::draw() {

	for(int i = 0; i < 45; i++) {
		for(int j = 0; j < 80; j++) {
			if(map[j][i] == L_WALL) {
				wall.color = 0xFFFFFFFF;
				DrawTexture(&wall, j*wall.width, i*wall.height);
			} else if(map[j][i] == L_FINISH) {
				wall.color = D3DCOLOR_RGBA(100, 100, 100, 255);
				DrawTexture(&wall, j*wall.width, i*wall.height);
			}
		}
	}
}

char Level::getTile(double xPos, double yPos) const{
	return map[(int)(xPos / tilewidth)][(int)(yPos / tilewidth)];
}

char Level::getTile(const GejbEngine::Vector3& position, const GejbEngine::Vector3& direction) const{
	
	int xPos = 0;
	int yPos = 0;

	if(direction.getY() > 0)
	{
		yPos = position.getY() + direction.getY() + tilewidth;
	}
	else
	{
		yPos = position.getY() + direction.getY();
	}

	if(direction.getX() > 0)
	{
		xPos = position.getX() + direction.getX() + tilewidth;
	} else
	{
		xPos = position.getX() + direction.getX();
	}


	return map[(int)(xPos / tilewidth)][(int)(yPos / tilewidth)];
}

void Level::checkWallCollision(Player &p) {
	RECT bounds = p.getBounds();

	if(map[(int)(p.getX() + (tilewidth / 2)) / 16][bounds.top / 16] ==
		 '1') {
		p.CollideWithWall();
	}

	if(map[(int)(p.getX() +
		(tilewidth / 2)) / 16][bounds.bottom / 16] == '1') {
		p.CollideWithWall();
	}

	if(map[bounds.left / 16][(int)(p.getY() +
		(tilewidth / 2)) / 16] == '1') {
		p.CollideWithWall();
	}

	if(map[bounds.right / 16][(int)(p.getY() +
		(tilewidth / 2)) / 16] == '1') {
		p.CollideWithWall();
	}
}

bool Level::checkFinish(Player &p) {
	RECT bounds = p.getBounds();

	if(map[(int)(p.getX() + (tilewidth)) / 16][bounds.top / 16] ==
		 'f') {
		return true;
	}

	else if(map[(int)(p.getX() +
		(tilewidth)) / 16][bounds.bottom / 16] == 'f') {
		return true;
	}

	else if(map[bounds.left / 16][(int)(p.getY() +
		(tilewidth)) / 16] == 'f') {
		return true;
	}

	else if(map[bounds.right / 16][(int)(p.getY() +
		(tilewidth)) / 16] == 'f') {
		return true;
	}

	return false;
}

//return true if there is a wall between positions
bool Level::testForWall(GejbEngine::Vector3 position1,
												GejbEngine::Vector3 position2) {
	GejbEngine::Vector3 direction;
	direction.setX(position1.getX() - position2.getX());
	direction.setY(position1.getY() - position2.getY());
	direction.setZ(0);
	GejbEngine::Vector3 poop = direction.Normal();

	direction = position1;
	//while( !(direction.getX() >  position2.getX()-5 || direction.getX() <  position2.getX()+5)
	//	&& !(direction.getY() >  position2.getY()-5 || direction.getY() <  position2.getY()+5))
	while(1) {
		if((direction.getX() > position2.getX() - 5
			&& direction.getX() <  position2.getX() + 5)
			&& (direction.getY() >  position2.getY() - 5
			&& direction.getY() < position2.getY() + 5)) {
			break;
		}

		direction -= poop;

		if(map[(int)direction.getX() / 16][(int)direction.getY() / 16] == '1') {
			return true;
		}
	}

	return false;
}

void Level::shutdown() {

}