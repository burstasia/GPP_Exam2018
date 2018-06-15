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

	float width = info.Size.x;
	float depth = info.Size.y;
	float widthDiv2 = width / 4.0f;
	float depthDiv2 = depth / 4.0f;

	if (!found) m_HouseVec.push_back(House{ info, false, {} , {Elite::Vector2{info.Center.x + widthDiv2, info.Center.y + depthDiv2 }
															,  Elite::Vector2{ info.Center.x - widthDiv2, info.Center.y + depthDiv2 }
															,  Elite::Vector2{ info.Center.x - widthDiv2, info.Center.y - depthDiv2 }
															,  Elite::Vector2{ info.Center.x + widthDiv2, info.Center.y - depthDiv2 } } });
}

void HouseTracker::AddItemToHouse(const Item& item, const Elite::Vector2& currHouseCenter)
{
	bool found = false;

	for (auto &house : m_HouseVec)
	{
		if (house.info.Center == currHouseCenter)
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
	House houseReturn{ m_HouseVec.at(0) };

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

const vector<Elite::Vector2>& HouseTracker::GetSearchPoints(const Elite::Vector2 & currHouseCenter)
{
	// TODO: insert return statement here
	for (auto &house : m_HouseVec)
	{
		if (house.info.Center == currHouseCenter)
		{
			return house.searchPositions;
		}
	}

	return vector<Elite::Vector2>{};
}

//vector<Elite::Vector2, bool>& HouseTracker::GetSearchPoints(const Elite::Vector2 & currHouseCenter)
//{
//	for (auto house : m_HouseVec)
//	{
//		if (house.info.Center == currHouseCenter)
//		{
//			return house.searchPositions;
//		}
//	}
//}
