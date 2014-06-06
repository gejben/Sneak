#include <sstream>
#include "StateGame.hpp"
#include "Engine.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Engine.hpp"
#include "Bullet.hpp"
#include "Item.hpp"

enum States{
	PLAY = 0,
	NEXT_LEVEL = 1,
	FINISHED = 2
};

StateGame::StateGame() :
score(0),
play(this),
changeLevel(this),
finish(this),
player(),
cont(false),
enemyList(){
	states[0] = &play;
	states[1] = &changeLevel;
	states[2] = &finish;
}

StateGame::~StateGame() {

	//ShutdownState();
}

bool StateGame::InitState() {
	ReadHighScore();
	score = 0;

	if(!GejbEngine::LoadSprite("Resources/Images/player.png", playerTexture))
	{
		GejbEngine::FatalError("Failed to initialize Player Sprite");
	}
	if(!GejbEngine::LoadSprite("Resources/Images/item.png", itemTexture))
	{
		GejbEngine::FatalError("Failed to initialize Item Sprite");
	}
	if(!GejbEngine::LoadSprite("Resources/Images/enemy.png", enemyTexture))
	{
		GejbEngine::FatalError("Failed to initialize Enemy Sprite");
	}
	if(!GejbEngine::LoadSprite("Resources/Images/bullet.png", bulletTexture))
	{
		GejbEngine::FatalError("Failed to initialize Bullet Sprite");
	}

	currentState = &states[NEXT_LEVEL];
	changeLevel.setContinue(cont);
	player.Init(&playerTexture, currentLevel);
	return true;
}

void StateGame::KeyPressed(int key) {
	(*currentState)->KeyPressed(key);
}

void StateGame::updateController(const XINPUT_STATE &controllerState) {
	(*currentState)->updateController(controllerState);
}

void StateGame::KeyReleased(int key) {
	(*currentState)->KeyReleased(key);
}

void StateGame::changeState(int toState){
	(*currentState)->ShutdownState();
	currentState = &states[toState];
	if(!(*currentState)->InitState()){
		GejbEngine::FatalError("Could not initialize Game State");
	}
}

int StateGame::UpdateState() {
	int result((States)(*currentState)->UpdateState());

	switch(result){
		case GO_TO_NEXT_LEVEL:
			changeState(NEXT_LEVEL);
			player.setAlive(false);
			break;
		case GO_TO_PLAY:
			changeState(PLAY);
			player.setLevel(currentLevel);
			player.setAlive(true);
			GejbEngine::RegisterSprite(&player);
			break;
		case END_GAME:
			changeState(FINISHED);
			break;
		case GAME_FINISHED:
			return -1;
		case NO_CHANGE:
		default:
			break;
	}
	return 0;
}

void StateGame::Draw() {
	(*currentState)->Draw();
}


void StateGame::CreateItem(double x, double y){
	Item *item = new Item();
	item->Init(&itemTexture, GejbEngine::Vector3(x,y));
	GejbEngine::RegisterSprite(item);

}

void StateGame::CreateEnemy(double x, double y){
	static int enemyId = 0;
	++enemyId;
	Enemy *enemy = new Enemy(currentLevel, this, enemyId);
	enemy->setPosition(x, y);
	enemy->Init(&enemyTexture);
	GejbEngine::RegisterSprite(enemy);
	enemyList.push_back(enemy);
}

Enemy* StateGame::FindEnemyByID(int enemyId){
	for each (Enemy* enemy in enemyList)
	{
		if(enemy->getEnemyId() == enemyId)
			return enemy;
	}
	return nullptr;
}

void StateGame::CreateBullet(double bulletSpeed, GejbEngine::Vector3 position, double rotation){
	Bullet *bullet = new Bullet();
	bullet->Init(&bulletTexture, bulletSpeed, position, rotation);
	GejbEngine::RegisterSprite(bullet);
}

void StateGame::ClearEnemyList(){
	std::list<Enemy*>::iterator it(enemyList.begin());
	while(it != enemyList.end()){
		Enemy *enemy(*it);
		GejbEngine::RemoveSprite(enemy->getId());
		enemyList.erase(it++);
		delete(enemy);
	}
}

void StateGame::ShutdownState() {
	GejbEngine::ReleaseSprite(playerTexture);
	file.close();
}

void StateGame::UpdateHighScore() {
	int i = 0;
	int temp1 = 0, temp2 = 0;
	while(highScore[i] > score) {
		++i;
	}

	temp1 = highScore[i];
	highScore[i] = score;

	while(i < 10) {
		temp2 = highScore[i + 1];
		highScore[i + 1] = temp1;
		temp1 = temp2;
		++i;
	}
}

void StateGame::WriteHighScore() {
	std::ofstream highscore;
	highscore.open("save/highscore.txt");

	char output[20];

	for(int i = 0; i < 10; i++) {
		sprintf_s(output, "%d\n", highScore[i]);
		highscore << output;
	}

	highscore.close();
}

void StateGame::ReadHighScore() {
	int i = 0;
	char ch;
	char input[50];
	std::ifstream fin("save/Highscore.txt");
	if(fin) {
		while(!fin.eof() && i < 10) {
			int a = 0;
			bool read = true;

			while(read) {
				fin.get(ch);
				if(ch == '\n') {
					read = false;
				} else {
					input[a] = ch;
					++a;
				}
			}
			input[a] = '\0';

			std::stringstream convert(input);
			if(!(convert >> highScore[i])) {
				highScore[i] = 0;
			}

			++i;
		}
		if(i < 9) {
			while(i <= 9) {
				highScore[i] = 0;
			}
		}
	} else {
		(std::ofstream("save/Highscore.txt"));
	}
}