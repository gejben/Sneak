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
	void Init(const GejbEngine::Texture *texture, Level *level);
	void Update() override;
	void Collide(int objectType) override;
	void CollideWithWall();

	void Move() override;

	void setLevel(Level *level){ currentLevel = level; }

	void setYvelocity(int y){ setVelocity(getVelocity().getX(), y); }
	void setXvelocity(int x){ setVelocity(x,getVelocity().getY()); }
};

#endif