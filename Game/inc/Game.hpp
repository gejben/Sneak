#ifndef GAME_HPP
#define GAME_HPP
#include "Engine.hpp"

enum GameEntityTypes{
	NONE,
	PLAYER,
	ENEMY,
	BULLET,
	ITEM
};

class Enemy;

bool game_preload();
bool game_init(HWND);
void game_update();
void game_end();
void game_graphics_update();
void game_keyPress(int key);
void game_keyRelease(int key);
void game_controllerUpdate(const XINPUT_STATE &controllerState);

Enemy* game_getEnemyById(int id);

#endif