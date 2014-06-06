#include "Player.hpp"
#include "Level.hpp"

Player::Player():
immortal(false){
}

Player::~Player() {
}

void Player::Init(const GejbEngine::Texture *texture, Level *level) {
	setTexture(texture);
	setObjectType(PLAYER);
	setColor(D3DCOLOR_RGBA(0, 255, 0, 255));
	setAnimationDirection(1);
	setCurrentColumn(0);
	setEndColumn(7);
	setTotalColumns(8);
	setFrameWidth(16);
	setFrameTimer(50);
	UpdateBounds();
	immortalTimer.resetStopwatch();
	currentLevel = level;
}

void Player::Update() {
	if(immortal){
		if(immortalTimer.stopwatch(3000)){
			immortal = false;
		}
	}
	if(getXvelocity() || getYvelocity()) {
		setRotation(atan2(getYvelocity(), getXvelocity()));
		setAnimated(true);
		setVelocity(GejbEngine::Vector3(0, 0, 0));
	} else {
		setAnimated(false);
	}
}

void Player::Collide(int objectType) {
	switch(objectType) {
		case BULLET:
		case ENEMY:
			if(immortal == false){
				setAlive(false);
			}
			break;
		case ITEM:
			immortal = true;
			immortalTimer.resetStopwatch();
		default:
			break;
	}
}

void Player::CollideWithWall() {
	setPosition(getPosition() - getVelocity());
	setVelocity(0, 0);
	UpdateBounds();
}

void Player::Move(){
	GejbEngine::Vector3 directionX(getVelocity().getX(),0);

	GejbEngine::Vector3 directionY(0,getVelocity().getY());


	if(currentLevel->getTile(getPosition(), directionX) == L_WALL){
		setVelocity(0, getVelocity().getY());
	}

	if(currentLevel->getTile(getPosition(), directionY) == L_WALL){
		setVelocity(getVelocity().getX(), 0);
	}

	Sprite::Move();
}