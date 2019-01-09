#ifndef  __FAIRY_GAMES_GARDEN_RUSH_SCENES_STORY_SCENE_H
#define  __FAIRY_GAMES_GARDEN_RUSH_SCENES_STORY_SCENE_H

#include "iScene.h"


class StoryScene : public iScene
{
public:
	StoryScene();
	~StoryScene();

	virtual bool init();
	virtual bool reset();

	// updated called every frame
	virtual void update(float delta);

	// profiling
	virtual unsigned long getMemoryConsumption();
	
	CREATE_FUNC(StoryScene);

protected:



};

#endif // __FAIRY_GAMES_GARDEN_RUSH_SCENES_STORY_SCENE_H
