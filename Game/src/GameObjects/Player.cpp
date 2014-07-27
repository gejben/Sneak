#include "Player.hpp"
#include "Level.hpp"

Player::Player():
immortal(false),
nextVelocity(0,0){
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
	waitTimer.resetStopwatch();
	currentLevel = level;
	setAnimated(false);

}

void Player::Update() {
	if(immortal){
		if(immortalTimer.stopwatch(3000)){
			immortal = false;
		}
	}
	if(nextVelocity.getY() || nextVelocity.getX()) {
		setRotation(atan2(nextVelocity.getY(), nextVelocity.getX()));
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

bool Player::Move(){

	if(Sprite::Move()){
		steps += getVelocity().Length();
		if(steps == tileWidth || steps == 0){
			setVelocity(0, 0);


			setAnimated(false);
				if(currentLevel->getTile(getPosition(), nextVelocity) == L_WALL){
					setVelocity(0, 0);
				} else if(nextVelocity.getX() || nextVelocity.getY()){
					OutputDebugString("move");
					setVelocity(nextVelocity);
					nextVelocity.Set(0, 0);
				}
			steps = 0;
		} else{
				setAnimated(true);
		}

		return true;
	}
	return false;
}