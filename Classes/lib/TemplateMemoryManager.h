#ifndef __FAIRY_GAMES_GARDEN_RUSH_LIB_TEMPLATE_MEMORY_MANAGER_
#define	__FAIRY_GAMES_GARDEN_RUSH_LIB_TEMPLATE_MEMORY_MANAGER_

/* 
 * File:   TemplateMemoryManager.h
 * Author: einhornimmond
 *
 * \date: 25.10.2018
 * 
 * ObjectClass needs this functions:
 * setMemoryManager(TemplateMemoryManager*)
 * reinit(u8 index)
 * getIndex() <- return index
 *
 */

#include "DRHashList.hpp"
#include "ErrorLog.h"
#include <assert.h>

template<typename ObjectClass>
class TemplateMemoryManager
{
public:
	
	TemplateMemoryManager()
		: mMemoryBlock(nullptr), mFreePlaces(nullptr), mFreePlaceCursor(0), mMaxIndexCount(0) {}
	~TemplateMemoryManager()
	{
		DR_SAVE_DELETE_ARRAY(mFreePlaces);
		DR_SAVE_DELETE_ARRAY(mMemoryBlock);
	}
	// \param maxIndexCount not greater than 255
	// \return true if all is good
	bool init(int maxIndexCount)
	{
		if (maxIndexCount <= 0 || maxIndexCount > 255) {
			LOG_ERROR("error, invalid index count, max 255, more than 0", false);
		}
		mMaxIndexCount = maxIndexCount;
		mFreePlaceCursor = 0;
		mMemoryBlock = new ObjectClass[maxIndexCount];
		if (!mMemoryBlock) LOG_ERROR("error allocating memory for memory block", false);
		mFreePlaces = new u8[maxIndexCount];
		if (!mFreePlaces) LOG_ERROR("error allocating memory for indices", false);
		
		// fill free places with indices
		for (u8 i = 0; i < maxIndexCount; i++) {
			mFreePlaces[i] = i;
			mMemoryBlock[i].setMemoryManager(this);
		}

		return true;
	}
    
	ObjectClass*  getFree() {
		assert(mMaxIndexCount > 0 && mMemoryBlock && mFreePlaces);
		if (mFreePlaceCursor >= mMaxIndexCount) LOG_ERROR("no space left", nullptr); 
		u8 index = mFreePlaces[mFreePlaceCursor];
		if (!mMemoryBlock[index].reinit(index)) {
			LOG_ERROR("error, reinit of Object failed", nullptr);
		}
		mFreePlaceCursor++;
		return &mMemoryBlock[index];
	}

	void free(ObjectClass* object) {
		u8 index = object->getIndex();
		assert(index < mMaxIndexCount);
		mFreePlaceCursor--;
		mFreePlaces[mFreePlaceCursor] = index;
	}

	unsigned long getMemoryConsumption() {
		unsigned long memory = sizeof(TemplateMemoryManager);
		// free places
		memory += mMaxIndexCount;
		// memory block
		memory += sizeof(ObjectClass) * mMaxIndexCount;
		return memory;
	}
    
private:
    
	ObjectClass*    mMemoryBlock;
	u8*				mFreePlaces;
    u8        mFreePlaceCursor;
    u8        mMaxIndexCount;
    
};


#endif	/* __FAIRY_GAMES_GARDEN_RUSH_LIB_TEMPLATE_MEMORY_MANAGER_ */

