#ifndef LEVEL_HPP
#define LEVEL_HPP


#include <string>
#include "Graphics.hpp"
#include "Vector3.hpp"

class Enemy;
class Player;
class StateGame;


enum TileTypes{
	L_FLOOR		= '0',
	L_ENEMY		= 'e',
	L_WALL		= '1',
	L_FINISH	= 'f'
};

class Level {
private:
	std::string filename;
	GejbEngine::Texture wall;

	StateGame *game;

	GejbEngine::Vector3 startingPoint;
	double startX;
	double startY;
	char map[80][45];
public:
	Level();
	~Level();
	bool init(std::string filename, StateGame *g);
	void draw();
	void shutdown();
	void checkWallCollision(Player &p);
	bool checkFinish(Player &p);
	bool testForWall(GejbEngine::Vector3 position1, GejbEngine::Vector3 position2);
	GejbEngine::Vector3 getStartingPoint(){ return startingPoint; }
	char getTile(double xPos, double yPos) const;
	char getTile(const GejbEngine::Vector3& position, const GejbEngine::Vector3& direction) const;
};

#endif