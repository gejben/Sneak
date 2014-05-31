#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Sprite.hpp"
#include "Graphics.hpp"

class Level;

class Player : public GejbEngine::Sprite{
private:
	Level *currentLevel;
	bool immortal;
	GejbEngine::Timer immortalTimer;
public:
	Player();
	~Player();
	void Init(const GejbEngine::Texture *texture);
	void Update() override;
	void Collide(int objectType) override;
	void CollideWithWall();

	void setYvelocity(int y){ setVelocity(getVelocity().getX(), y); }
	void setXvelocity(int x){ setVelocity(x,getVelocity().getY()); }
};

#endif