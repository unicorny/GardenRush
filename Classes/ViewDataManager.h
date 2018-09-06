#ifndef __FAIRY_GAMES_GARDEN_RUSH_PLANT_TYPES_MANAGER_H
#define __FAIRY_GAMES_GARDEN_RUSH_PLANT_TYPES_MANAGER_H

/*!
 *
 * \date: 04.09.18
 *
 * \author: Dario Rekowski
 * 
 * \brief: Manager storing all view data sets
 */

#include "lib/DRHashList.hpp"

class IViewData;


class ViewDataManager
{
public:
	ViewDataManager();
	virtual ~ViewDataManager();
protected:
	DRHashList mViewDataList;

};

#endif //__FAIRY_GAMES_GARDEN_RUSH_PLANT_TYPES_MANAGER_H

