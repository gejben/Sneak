#include "Sprite.hpp"
#include <string>

namespace GejbEngine {

	Sprite::Sprite() :
		currentRow(0),
		currentColumn(0),
		totalRows(1),
		totalColumns(1),
		startRow(0),
		startColumn(0),
		endRow(0),
		endColumn(0),
		animationDirection(1),
		animationColumns(1),
		framestart(0),
		frametimer(0),
		animationStartX(0),
		animationStartY(0),
		faceAngle(0),
		moveAngle(0),
		rotation(0),
		scaling(1.0f),
		movetimer(16),
		movestart(0),
		frameHeight(0),
		frameWidth(0),
		animated(false),
		velocity(0, 0),
		alive(true),
		lifetimeLength(0),
		position(0, 0),
		id(0),
		texture(nullptr){
		lifetimeTimer.resetStopwatch();

	}

	Sprite::~Sprite() {
	}

	void Sprite::setTexture(const Texture *te){
		texture = te;
		frameHeight = te->height;
		frameWidth = te->width;
		collidable = true;
	}


	void Sprite::Animate() {
		if(animated) {
			//update frames based on animation direction
			if(frametimer > 0) {
				if(timeGetTime() > (DWORD)(framestart + frametimer)) {
					//reset animation timer
					framestart = timeGetTime();
					currentColumn += animationDirection;

					//animation going forwards
					if(currentColumn > totalColumns) {
						++currentRow;
						currentColumn = 0;
					}
					if(currentColumn < startColumn && currentRow <= startRow) {
						currentColumn = endColumn;
						currentRow = endRow;
					}


					//animation going backwards
					if(currentColumn < 0) {
						--currentRow;
						currentColumn = totalColumns;
					}
					if(currentColumn > endColumn && currentRow >= startRow) {
						currentColumn = startColumn;
						currentRow = startRow;
					}

				}
			} else {
				//no animation timer -- update at cpu clock speed
				currentColumn += animationDirection;


				//animation going forwards
				if(currentColumn >= totalColumns) {
					++currentRow;
					currentColumn = 0;
				}
				if(currentColumn < startColumn && currentRow <= startRow) {
					currentColumn = endColumn;
					currentRow = endRow;
				}


				//animation going backwards
				if(currentColumn < 0) {
					--currentRow;
					currentColumn = totalColumns;
				}
				if(currentColumn > endColumn && currentRow >= startRow) {
					currentColumn = startColumn;
					currentRow = startRow;
				}
			}
		}
	}


	bool Sprite::Move() {
		bool retval = true;
		if(movetimer > 0) {
			if(timeGetTime() > (DWORD)(movestart + movetimer)) {
				//reset move timer
				movestart = timeGetTime();

				//move sprite by velocity amount
				this->setX(this->getX() + this->velocity.getX());
				this->setY(this->getY() + this->velocity.getY());
			}
			else{
				retval = false;
			}
		} else {
			//no movement timer--update at cpu clock speed
			this->setX(this->getX() + this->velocity.getX());
			this->setY(this->getY() + this->velocity.getY());
		}

		UpdateBounds();
		return retval;
	}

	void Sprite::UpdateBounds() {
		bounds.top = position.getY();
		bounds.bottom = bounds.top + frameHeight;
		bounds.left = position.getX();
		bounds.right = bounds.left + frameWidth;
	}

	void Sprite::Collide(int objType){
	}

	void Sprite::Update(){

	}
}