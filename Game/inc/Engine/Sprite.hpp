#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "Graphics.hpp"
#include "Vector3.hpp"
#include "Timer.hpp"
#include "Game.hpp"

enum side {
	TOP,
	BOTTOM,
	LEFT,
	RIGHT
};

namespace GejbEngine {
	class Sprite {
	private:
		bool alive;
		bool animated;
		bool collidable;
		const Texture *texture;
		Timer lifetimeTimer;
		RECT bounds;
		Vector3 position;
		Vector3 velocity;
		D3DCOLOR color;

		int id;
		int frameWidth, frameHeight;
		int animationColumns;
		int framestart, frametimer;
		int movestart, movetimer;
		int objectType;
		int lifetimeLength;
		int currentRow, currentColumn, totalRows, totalColumns, animationDirection;
		int startColumn, endColumn;
		int startRow, endRow;
		int animationStartX, animationStartY;
		double faceAngle, moveAngle;
		double rotation, scaling;

	public:
		Sprite();
		~Sprite();
		void Animate();
		virtual void Move();
		void UpdateBounds();

		virtual void Update();
		virtual void Collide(int collisionType);

		void setTexture(const Texture *texture);

		bool  getAlive() const { return alive; }
		void setAlive(bool value) { alive = value; }

		int getLifetime() const { return lifetimeLength; }
		void setLifetime(int milliseconds) { lifetimeLength = milliseconds; lifetimeTimer.resetStopwatch(); }

		bool  lifetimeExpired() const { return lifetimeTimer.stopwatch(lifetimeLength); }
		int  getObjectType() const { return objectType; }

		void setObjectType(int value) { objectType = value; }

		RECT getBounds() const{ return bounds; }

		//screen position
		Vector3 getPosition() const { return position; }
		void setPosition(Vector3 value) { position = value; }
		void setPosition(double x, double y) { position.Set(x, y); }

		double getX() const { return position.getX(); }
		double getY() const { return position.getY(); }

		void setX(double x) { position.setX(x); }
		void setY(double y) { position.setY(y); }

		//movement velocity

		Vector3 getVelocity() const { return velocity; }
		double getXvelocity() const { return velocity.getX(); }
		double getYvelocity() const { return velocity.getY(); }

		void setVelocity(Vector3 value) { velocity = value; }
		void setVelocity(double x, double y) { velocity.Set(x, y); }
		void setXVelocity(double x){ velocity.setX(x); }
		void setYVelocity(double y){ velocity.setY(y); }

		int getWidth() const { return texture->width; }
		int getHeight() const { return texture->height; }

		int getColumns() const { return animationColumns; }
		void setColumns(int value) { animationColumns = value; }

		int getFrameTimer() const { return frametimer; }
		void setFrameTimer(int value) { frametimer = value; }

		int getCurrentColumn() const { return currentColumn; }
		void setCurrentColumn(int value) { currentColumn = value; }

		int getCurrentRow() const { return currentRow; }
		void setCurrentRow(int value) { currentRow = value; }

		int getTotalColumns() const { return totalColumns; }
		void setTotalColumns(int value) { totalColumns = value; }

		int getTotalRows() const { return totalRows; }
		void setTotalRows(int value) { totalRows = value; }

		int getAnimationDirection() const{ return animationDirection; }
		void setAnimationDirection(int value) { animationDirection = value; }

		double getRotation() const { return rotation; } const
		void setRotation(double value) { rotation = value; }

		double getScale() const { return scaling; }
		void setScale(double value) { scaling = value; }

		bool getCollidable() const { return collidable; }
		void setCollidable(bool value) { collidable = value; }

		bool getImageLoaded() const { return texture->imageLoaded; }

		int getTextureIndex() const { return texture->textureIndex; }

		void setStartColumn(int value) { startColumn = value; }
		void setEndColumn(int value) { endColumn = value; }

		void setStartRow(int value) { startRow = value; }
		void setEndRow(int value) { endRow = value; }

		void setFrameWidth(int width) { frameWidth = width; }
		void setFrameHeight(int height) { frameHeight = height; }

		int getFrameWidth() const { return frameWidth; }
		int getFrameHeight()const { return frameHeight; }

		void setAnimated(bool value) { animated = value; }
		bool isAnimated() const { return animated; }

		void setColor(D3DCOLOR col) { color = col; }
		D3DCOLOR getColor() const { return color; }

		void setId(int i){ id = i; }
		int getId(){ return id; }

		const Texture* getTexture() const{ return texture; }

	};
};

#endif