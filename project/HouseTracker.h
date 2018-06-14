#pragma once
#include "Exam_HelperStructs.h"
#include "ItemTracker.h"

struct House
{
	HouseInfo info;
	bool isSearched;
	vector<Item> items;
};

class HouseTracker
{
public:
	HouseTracker();
	~HouseTracker();

	void AddHouse(const HouseInfo& info);
	void AddItemsToHouse(const vector<Item>& items, const Elite::Vector2& pos);
	House GetClosestHouse(const Elite::Vector2& currPos, float& distance);

private:
	vector<House> m_HouseVec;
};

