#include "stdafx.h"
#include "HouseTracker.h"


HouseTracker::HouseTracker()
{
}


HouseTracker::~HouseTracker()
{
}

void HouseTracker::AddHouse(const HouseInfo & info)
{
	bool found = false;

	for (auto house : m_HouseVec)
	{
		if (house.info.Center == info.Center)
		{
			found = true;
		}
	}

	if (!found) m_HouseVec.push_back(House{ info, false, {} });
}

void HouseTracker::AddItemToHouse(const Item& item, const Elite::Vector2& pos)
{
	bool found = false;

	for (auto &house : m_HouseVec)
	{
		if (house.info.Center == pos)
		{
			for (auto itemRef : house.items)
			{
				if (itemRef.pos == item.pos)
				{
					found = true;
				}
			}
			if (!found) house.items.push_back(item);
		}
	}
}

House HouseTracker::GetClosestHouse(const Elite::Vector2 & currPos, float & distance)
{
	distance = Elite::Distance(m_HouseVec.at(0).info.Center, currPos);
	House houseReturn{};

	for (auto house : m_HouseVec)
	{
		if (Elite::Distance(house.info.Center, currPos) < distance)
		{
			houseReturn = house;
			distance = Elite::Distance(house.info.Center, currPos);
		}
	} 

	return houseReturn;
}
