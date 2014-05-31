#include "MenuOption.hpp"
#include "iostream"

MenuOption::MenuOption() :
selected(false),
disabled(false),
direction(0.5),
colorG(0.5) {
}

MenuOption::~MenuOption() {
}

void MenuOption::Init(GejbEngine::Texture &te) {
	setTexture(&te);
	setColor(D3DCOLOR_RGBA(255, 0, 0, 255));
}

void MenuOption::Update() {
	if(colorG >= 255 || colorG <= 0) {
		direction *= -1;
	}

	if(selected) {
		colorG += direction;
		setColor(D3DCOLOR_RGBA(255, (int)colorG, 0, 255));
	} else if(disabled) {
		setColor(D3DCOLOR_RGBA(50, 50, 50, 255));
	} else {
		setColor(D3DCOLOR_RGBA(255, 0, 0, 255));
	}
}