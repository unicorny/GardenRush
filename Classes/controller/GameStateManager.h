#ifndef __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_GAME_STATE_MANAGER_H
#define __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_GAME_STATE_MANAGER_H


/*
* \author: einhornimmond
*
* \date: 06.01.2019
*
* \brief: controll game states, szene switchs u.a.
*
*/

#include "scenes/iScene.h"
#include "lib/TemplateMemoryManager.h"
#include "model/SpriteAnimationState.h"

class Player;
class GameScene;
class MainMenuScene;
class StoryScene;

enum GameSceneTypes {
	GAME_SCENE_MAIN_GAME,
	GAME_SCENE_MAIN_MENU,
	GAME_SCENE_STORY,
	GAME_SCENE_SIZE
};

class GameStateManager
{
public:
	static GameStateManager* getInstance();
	~GameStateManager();

	bool switchToScene(GameSceneTypes type);

	bool init();

	static const char* gameSceneTypeToString(GameSceneTypes type);

protected:
	GameStateManager();

	// scenes
	iScene*		mScenes[GAME_SCENE_SIZE];

	//
	bool				mFirstScene;
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_GAME_STATE_MANAGER_H