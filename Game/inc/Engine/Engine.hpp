/*******************Engine Main Header*************************
* Simple 2D-Engine
*
* First version allmost exact copy of Jonathan  S. Harbours
* example in the Advanced 2D Game Development book(2005)
*
* First version based on DirectX 9
***************************************************************/

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <memory.h>
#include "GameState.hpp"
#include "Graphics.hpp"
#include "Input.hpp"
#include "Sprite.hpp"
#include "Timer.hpp"
#include "Vector3.hpp"
#include "WinMain.hpp"
#include "ControllerInput.hpp"

enum Version{
	VERSION_MAJOR = 0,
	VERSION_MINOR = 1,
	REVISION = 2
};


//External variables and functions

extern bool gameover;
extern bool game_preload();
extern bool game_init(HWND);
extern void game_update();
extern void game_end();
extern void game_graphics_update();
extern void game_keyPress(int key);
extern void game_keyRelease(int key);
extern void game_controllerUpdate(const XINPUT_STATE &controllerState);

namespace GejbEngine {

	class Sprite;

	void StartupEngine();
	int InitEngine(int width, int height, bool fullscreen, int colordepth);
	void UpdateEngine();
	void Message(std::string message, std::string title = "Engine");
	void FatalError(std::string message, std::string title = "Error");
	void ShutdownEngine();
	int Release();

	void TestForCollisions();

	void UpdateKeyboard();
	void RegisterSprite(Sprite *sprite);
	void RemoveSprite(int id);

	//Accessors/mutators
	bool isPaused();
	void setPaused(bool paused);
	void setWindowHandle(HWND hwnd);
	HWND getWindowHandle();
	std::string getTitle();
	void setTitle(std::string value);
	int getVersionMajor();
	int getVersionMinor();
	int getRevision();
	std::string getVersionText();
	long getFrameRate_core();
	long getFrameRate_real();
	int getScreenWidth();
	int getScreenHeight();
	void setScreenWidth(int width);
	void setScreenHeight(int height);
	bool getFullscreen();
	void setFullscreen(bool value);
	bool getMaximizeProcessor();
	void setMaximizeProcessor(bool value);
	void setInputMethod(int value);

};


#endif
