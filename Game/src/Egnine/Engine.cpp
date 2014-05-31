
#include "Engine.hpp"
#include <cstdlib>
#include <ctime>
#include <string>
#include <list>
#include "winmain.hpp"

namespace GejbEngine {
	int versionMajor, versionMinor, revision;
	HWND windowHandle;
	std::string title;
	bool fullscreen;
	int screenwidth;
	int screenheight;
	bool pauseMode;
	bool maximizeProcessor;
	Timer coreTimer;
	long frameCount_core;
	long frameRate_core;
	Timer realTimer;
	long frameCount_real;
	long frameRate_real;
	Timer collisionTimer;
	std::list<Sprite*>sprites;
	int inputMethod;
	bool controllerAvailable;

	//private methods
	void DrawEntities();
	void AnimateEntities();
	void UpdateEntities();
	void BuryEntities();

	std::string getVersionText() {
		char string[25];
		sprintf_s(string, "GejbEngine v%d.%d.%d", versionMajor, versionMinor, revision);
		return string;
	}

	void message(std::string message, std::string title) {
		MessageBox(0, message.c_str(), title.c_str(), 0);
	}

	void FatalError(std::string message, std::string title) {
		MessageBox(0, message.c_str(), title.c_str(), 0);
		ShutdownEngine();
	}

	void StartupEngine() {
		maximizeProcessor = false;
		frameCount_core = 0;
		frameCount_real = 0;
		frameRate_core = 0;
		frameRate_real = 0;
		windowHandle = 0;
		pauseMode = false;
		versionMajor = VERSION_MAJOR;
		versionMinor = VERSION_MINOR;
		revision = REVISION;
		srand((unsigned int)time(NULL));
		setTitle("Engine");
		setScreenWidth(1280);
		setScreenHeight(720);
		setFullscreen(false);
	}

	int InitEngine(int width, int height, bool fullscreen_value, int colordepth) {
		screenwidth = width;
		screenheight = height;
		fullscreen = fullscreen_value;

		if(!InitGraphics(windowHandle, screenwidth, screenheight, colordepth,
			fullscreen)) {
			return 0;
		}

		InitInput(windowHandle);

		controllerAvailable = InitController();
		controllerAvailable = true;

		if(!game_init(windowHandle)) {
			return 0;
		}

		return 1;
	}

	void ShutdownEngine() {
		game_end();
		ShutdownGraphics();
		ShutdownInput();
		gameover = true;
	}

	void UpdateEngine() {
		static Timer timedUpdate;

		//calculate core framerate
		frameCount_core++;
		if(coreTimer.stopwatch(999)) {
			frameRate_core = frameCount_core;
			frameCount_core = 0;
		}

		//fast update with no timing
		game_update();

		if(pauseMode == false) {
			BuryEntities();
			UpdateEntities();
			AnimateEntities();
		}

		UpdateInput();
		UpdateKeyboard();
		controllerAvailable = UpdateController();
		if(controllerAvailable) {
			game_controllerUpdate(getControllerState());
		}

		
		if(pauseMode == false && collisionTimer.stopwatch(50)){
			TestForCollisions();
		}

		//update with 60 fps timing
		if(!timedUpdate.stopwatch(14)) {
			if(getMaximizeProcessor() == false) {
				Sleep(1);
			}
		} else {
			//calcuate real framerate
			frameCount_real++;
			if(realTimer.stopwatch(999)) {
				frameRate_real = frameCount_real;
				frameCount_real = 0;
			}
		}

		//Perform grapics update
		if(BeginGraphics()) {
			ClearScreen(getClearColor());
			game_graphics_update();
			if(pauseMode == false) {
				DrawEntities();
			}
			EndGraphics();
		}

	}

	void AnimateEntities() {
		std::list<Sprite*>::iterator it = sprites.begin();

		while(it != sprites.end()) {
			Sprite *sprite(*it);
			if(sprite->getAlive()) {
				sprite->Animate();
			}
			++it;
		}
	}

	void DrawEntities() {
		std::list<Sprite*>::iterator it = sprites.begin();

		while(it != sprites.end()) {
			Sprite *sprite(*it);
			if(sprite->getAlive()) {
				DrawSprite(sprite);
			}
			++it;
		}
	}

	void UpdateEntities() {
		std::list<Sprite*>::iterator it = sprites.begin();

		while(it != sprites.end()) {
			Sprite *sprite(*it);
			if(sprite->getAlive()) {
				sprite->Move();
				sprite->Update();
			}
			++it;
		}
	}

	void BuryEntities(){
		std::list<Sprite*>::iterator it = sprites.begin();

		while(it != sprites.end()) {
			if((*it)->getAlive() == false) {
				sprites.erase(it++);
			}
			else{
				it++;
			}
		}
	}

	void RegisterSprite(Sprite *sprite) {
		static int id = 0;
		sprite->setId(id++);
		sprites.push_back(sprite);
	}

	void RemoveSprite(int id){
		for(std::list<Sprite*>::iterator it = sprites.begin(); it != sprites.end(); it++){
			if((*it)->getId() == id){
				sprites.erase(it);
				break;
			}
		}
	}


	void UpdateKeyboard() {
		static char old_keys[256];
		for(int n = 0; n < 255; n++) {
			if(GetKeyState(n) & 0x80) {
				game_keyPress(n);
				old_keys[n] = GetKeyState(n);
			} else if(old_keys[n] & 0x80) {
				game_keyRelease(n);
				old_keys[n] = GetKeyState(n);
			}

		}
	}

	static bool collision(Sprite *sprite1,
												Sprite *sprite2) {
		double radius1, radius2;
		double distance;

		if(sprite1->getFrameWidth() > sprite1->getFrameHeight()) {
			radius1 = sprite1->getFrameWidth() / 2;
		} else {
			radius1 = sprite1->getFrameHeight() / 2;
		}

		Vector3 vector1(sprite1->getX() + radius1, sprite1->getY() + radius1);

		if(sprite2->getFrameWidth() > sprite2->getFrameHeight()) {
			radius2 = sprite2->getFrameWidth() / 2;
		} else {
			radius2 = sprite2->getFrameHeight() / 2;
		}

		Vector3 vector2(sprite2->getX() + radius2, sprite2->getY() + radius2);

		distance = vector1.Distance(vector2);

		return (distance < radius1 + radius2);
	}

	void TestForCollisions() {
		std::list<Sprite*>::iterator first = sprites.begin();
		std::list<Sprite*>::iterator second;

		while(first != sprites.end()) {
			Sprite *sprite1(*first);
			if(sprite1->getAlive() && sprite1->getCollidable()) {
				second = sprites.begin();
				while(second != sprites.end()) {
					Sprite *sprite2(*second);

					if(sprite2->getAlive() && sprite2->getCollidable()
						 && first != second) {
						if(collision(sprite1, sprite2)) {
							sprite1->Collide(sprite2->getObjectType());
							sprite2->Collide(sprite1->getObjectType());
						}
					}
					second++;
				}
			}
			first++;
		}
	}

	bool isPaused() { return pauseMode; }
	void setPaused(bool paused) { pauseMode = paused; }
	void setWindowHandle(HWND hwnd) { windowHandle = hwnd; }
	HWND getWindowHandle() { return windowHandle; }
	std::string getTitle() { return title; }
	void setTitle(std::string value) { title = value; }
	int getVersionMajor() { return VERSION_MAJOR; }
	int getVersionMinor() { return VERSION_MINOR; }
	int getRevision() { return REVISION; }
	long getFrameRate_core() { return frameRate_core; }
	long getFrameRate_real() { return frameRate_real; }
	int getScreenWidth() { return screenwidth; }
	int getScreenHeight() { return screenheight; }
	void setScreenWidth(int width) { screenwidth = width; }
	void setScreenHeight(int height) { screenheight = height; }
	bool getFullscreen() { return fullscreen; }
	void setFullscreen(bool value) { fullscreen = value; }
	bool getMaximizeProcessor() { return maximizeProcessor; }
	void setMaximizeProcessor(bool value) { maximizeProcessor = value; }
	void setInputMethod(int value) { inputMethod = value; }
};
