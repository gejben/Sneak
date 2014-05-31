#ifndef LEVEL_HPP
#define LEVEL_HPP


#include <string>
#include "Graphics.hpp"
#include "Vector3.hpp"

class Enemy;
class Player;
class StateGame;


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
	char getTile(double xPos, double yPos);
};

#endif