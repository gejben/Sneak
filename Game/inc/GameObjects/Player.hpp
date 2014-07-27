#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Sprite.hpp"
#include "Graphics.hpp"

class Level;

class Player : public GejbEngine::Sprite{
private:
	Level *currentLevel;
	bool immortal;
	int steps;
	const int tileWidth = 16;
	GejbEngine::Timer immortalTimer;
	GejbEngine::Timer waitTimer;
	GejbEngine::Vector3 nextVelocity;
public:
	Player();
	~Player();
	void Init(const GejbEngine::Texture *texture, Level *level);
	void Update() override;
	void Collide(int objectType) override;
	void CollideWithWall();

	bool Move() override;

	void setLevel(Level *level){ currentLevel = level; }

	void setYvelocity(int y){ nextVelocity.setY(y);}
	void setXvelocity(int x){ nextVelocity.setX(x); }
	void setNextVelocity(int x, int y){ nextVelocity.Set(x, y); }
};

#endif