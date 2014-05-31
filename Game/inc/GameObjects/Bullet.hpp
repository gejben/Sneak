#ifndef BULLET_HPP
#define BULLET_HPP

#include "Sprite.hpp"

namespace GejbEngine {
	struct Texture;
}

class Bullet : public GejbEngine::Sprite{
private:
public:

	Bullet();
	~Bullet();
	void Update() override;
	void Init(GejbEngine::Texture *bulletTexture, double bulletSpeed, GejbEngine::Vector3 position, double rotationToPlayer);
	void Collide(int objectType) override;

	void CollideWithWall();
};

#endif