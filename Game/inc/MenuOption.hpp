#ifndef MENUOPTION_HPP
#define MENUOPTION_HPP

#include "Sprite.hpp"

class MenuOption : public GejbEngine::Sprite{
private:
	bool selected;
	bool disabled;
	double direction;
	double colorG;
public:
	MenuOption();
	~MenuOption();
	void Init(GejbEngine::Texture &);

	void Update();

	void setSelected(bool value) { selected = value; }
	bool getSelected() { return selected; }

	void setDisabled(bool value) { disabled = value; selected = false; }
};

#endif