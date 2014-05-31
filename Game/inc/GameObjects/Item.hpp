#ifndef ITEM_H
#define ITEM_H

#include "Sprite.hpp"

namespace GejbEngine {
	struct Texture;
}

class Item : public GejbEngine::Sprite{
private:
public:
	Item();
	~Item();
	void Update() override;
	void Init(GejbEngine::Texture *itemTexture, GejbEngine::Vector3 position);
	void Collide(int objectType) override;
};


#endif