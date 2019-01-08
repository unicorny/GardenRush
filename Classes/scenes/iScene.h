#ifndef  __FAIRY_GAMES_GARDEN_RUSH_SCENES_i_SCENE_H
#define  __FAIRY_GAMES_GARDEN_RUSH_SCENES_i_SCENE_H

#include "cocos2d.h"


class iScene : public cocos2d::Scene
{
public:
	
	virtual bool reset() = 0;
	virtual unsigned long getMemoryConsumption() { return sizeof(cocos2d::Scene); };


protected:

};

#endif // __FAIRY_GAMES_GARDEN_RUSH_SCENES_i_SCENE_H
