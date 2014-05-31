#include "Item.hpp"

Item::Item(){
	setObjectType(ITEM);
	setAnimationDirection(-1);
	UpdateBounds();
}

Item::~Item(){
	
}

void Item::Update(){
}
void Item::Init(GejbEngine::Texture *itemTexture, GejbEngine::Vector3 position){
	setTexture(itemTexture);
	setPosition(position);
	setColor(D3DCOLOR_RGBA(150, 150, 150, 255));
}
void Item::Collide(int objectType){
	if(objectType == PLAYER)
	{
		setAlive(false);
	}
}