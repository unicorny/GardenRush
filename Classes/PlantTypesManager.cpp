#include "PlantTypesManager.h"



using namespace rapidjson;

PlantTypesManager::PlantTypesManager()
	: mNeigborFastAccessMap(NULL), mDirty(false)
{

}
PlantTypesManager::~PlantTypesManager()
{
	if (mNeigborFastAccessMap) {
		free(mNeigborFastAccessMap);
		mNeigborFastAccessMap = NULL;
	}
}

void PlantTypesManager::allocateMemoryForNPlantTypes(u32 N)
{
	mPlantTypes.initMemory(N);
}

bool PlantTypesManager::addPlantType(PlantType* plantType)
{
	if (!plantType) return false;
	
	bool result = mPlantTypes.addByHash(plantType->getNameHash(), plantType);
	if(result) mDirty = true;
	return result;
}

PlantType* PlantTypesManager::findPlantType(DHASH nameHash) const
{
	return (PlantType*)mPlantTypes.findByHash(nameHash);
}

bool PlantTypesManager::makeFastAccessMap()
{
	if (mNeigborFastAccessMap) {
		free(mNeigborFastAccessMap);
	}
	u32 itemCount = mPlantTypes.getNItems();
	assert(itemCount > 0);
	mNeigborFastAccessMap = (u8*)malloc(itemCount*itemCount);
	memset(mNeigborFastAccessMap, 0, itemCount*itemCount);
	for (u32 i = 0; i < itemCount; i++) {
		PlantType* plantType = (PlantType*)mPlantTypes.findByIndex(i);
		assert(plantType != NULL);
		plantType->setIndex(i);
		for (u32 j = 0; j < itemCount; j++) {
			if (i == j) continue;
			DHASH neighborPlantHash = mPlantTypes.findHashByIndex(j);
			mNeigborFastAccessMap[i*itemCount + j] = (u8)plantType->getNeighborType(neighborPlantHash);
		}
	}
	mDirty = false;
	return true;
}

bool PlantTypesManager::loadFromJson(const char* filename)
{
	// load json from file and parse
	std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
	Document document;
	document.Parse(content.data());

	// interpret json
	assert(document.IsObject());

	allocateMemoryForNPlantTypes(document.MemberCount());
	for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
	{
		// for every plant
		assert(itr->name.IsString());
		u32 neighborCount = 0;
		// count neighbors
		if (itr->value.HasMember("neighbors")) {
			for (Value::ConstMemberIterator iNeighbors = itr->value["neighbors"].MemberBegin();
				iNeighbors != itr->value["neighbors"].MemberEnd(); ++iNeighbors)
			{
				neighborCount += iNeighbors->value.Capacity();
			}
		}		

		PlantType* plant = new PlantType(itr->name.GetString(), neighborCount);

		// add neighbors
		if (itr->value.HasMember("neighbors")) {
			for (Value::ConstMemberIterator iNeighbors = itr->value["neighbors"].MemberBegin();
				iNeighbors != itr->value["neighbors"].MemberEnd(); ++iNeighbors)
			{
				assert(iNeighbors->name.IsString());
				PlantTypeNeighborType type = PlantType::getNeighborTypeFromString(iNeighbors->name.GetString());
				for (Value::ConstValueIterator iNeighbor = iNeighbors->value.Begin(); iNeighbor != iNeighbors->value.End(); ++iNeighbor) {
					assert(iNeighbor->IsString());
					plant->addNeighbor(iNeighbor->GetString(), type);
				}
			}
		}

		// add views
		if (itr->value.HasMember("graphics")) {
			for (Value::ConstMemberIterator iGraphics = itr->value["graphics"].MemberBegin();
				iGraphics != itr->value["graphics"].MemberEnd(); ++iGraphics) {
				assert(iGraphics->name.IsString());
				IViewData* view = NULL;
				PlantTypePhasisView type = PlantType::getPhasisViewFromString(iGraphics->name.GetString());
				if (iGraphics->value.IsString()) {
					view = IViewData::createFromJson(&iGraphics);
					plant->setViewData(view, type);
				}
				else if (iGraphics->value.IsArray()) {
					int count = 1;
					for (Value::ConstValueIterator itrArray = iGraphics->value.Begin(); itrArray != iGraphics->value.End(); ++itrArray) {
						std::string typeName = iGraphics->name.GetString();
						typeName += "_";
						typeName += std::to_string(count);
						type = PlantType::getPhasisViewFromString(typeName.data());
						assert(type < PLANT_PHASIS_SIZE);
						view = IViewData::createFromJson(&itrArray);
						plant->setViewData(view, type);
						count++;
					}
				}
				else if (iGraphics->value.IsObject() && strcmp(iGraphics->name.GetString(), "final") == 0) {
					for (Value::ConstMemberIterator itrObject = iGraphics->value.MemberBegin(); itrObject != iGraphics->value.MemberEnd(); ++itrObject) {
						assert(itrObject->name.IsString());
						std::string typeName = iGraphics->name.GetString();
						typeName += "_";
						typeName += itrObject->name.GetString();
						type = PlantType::getPhasisViewFromString(typeName.data());
						assert(type < PLANT_PHASIS_SIZE);
						view = IViewData::createFromJson(&itrObject);
						plant->setViewData(view, type);
					}
				}
				
			}
		}

		addPlantType(plant);
	}

	return true;
}