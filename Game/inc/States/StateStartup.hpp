#ifndef StateStartup_HPP
#define StateStartup_HPP

#include "Engine.hpp"

class StateStartup : public GejbEngine::GameState {
private:
	GejbEngine::Texture sp;
	GejbEngine::Timer timer;
public:
	StateStartup();
	~StateStartup();
	bool InitState() override;
	void KeyPressed(int key) override;
	void KeyReleased(int key) override;
	int UpdateState() override;
	void Draw() override;
	void ShutdownState() override;
};

#endif