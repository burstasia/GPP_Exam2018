#pragma once
#include "Exam_HelperStructs.h"
#include "ItemTracker.h"

struct House
{
	HouseInfo info;
	bool isSearched;
	vector<Item> items;

	//map<const Elite::Vector2, bool> searchPositions;
};

class HouseTracker
{
public:
	HouseTracker();
	~HouseTracker();

	void AddHouse(const HouseInfo& info);
	void AddItemToHouse(const Item& item, const Elite::Vector2& currHouseCenter);
	House GetClosestHouse(const Elite::Vector2& currPos, float& distance);
	//map<Elite::Vector2, bool>& GetSearchPoints(const Elite::Vector2& currHouseCenter);

private:
	vector<House> m_HouseVec;
};

