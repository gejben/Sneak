#include "Bullet.hpp"

Bullet::Bullet(){
	setObjectType(BULLET);
	setAnimationDirection(-1);
	UpdateBounds();
}

Bullet::~Bullet() {
}

void Bullet::Init(GejbEngine::Texture *bulletTexture, double bulletSpeed, GejbEngine::Vector3 position, double rotationToPlayer){
	setTexture(bulletTexture);
	setPosition(position);
	setVelocity(bulletSpeed*cos(rotationToPlayer),
										 bulletSpeed*sin(rotationToPlayer));
	setRotation(rotationToPlayer);
	setColor(D3DCOLOR_RGBA(255, 0, 0, 255));
}

void Bullet::Update() {
	Move();
	if(getPosition().getX() < 0 || getPosition().getX() > GejbEngine::getScreenWidth()){
		setAlive(false);
	} else if(getPosition().getY() < 0 || getPosition().getY() > GejbEngine::getScreenHeight()){
		setAlive(false);
	}
}

void Bullet::Collide(int objectType) {
	setAlive(false);
}

void Bullet::CollideWithWall() {
	setAlive(false);
}
