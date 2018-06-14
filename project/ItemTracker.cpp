#include "stdafx.h"
#include "ItemTracker.h"
#include "EliteMath/EVector2.h"


ItemTracker::ItemTracker()
{
}


ItemTracker::~ItemTracker()
{
}

void ItemTracker::AddItem(eItemType type, Elite::Vector2 pos)
{
	switch (type)
	{
	case eItemType::PISTOL:
		AddItemToVec(m_PistolVec, type, pos);
		break;
	case eItemType::MEDKIT:
		AddItemToVec(m_MedkitVec, type, pos);
		break;
	case eItemType::FOOD:
		AddItemToVec(m_FoodVec, type, pos);
		break;
	default:
		AddItemToVec(m_MiscVec, type, pos);
		break;
	}
}

Item ItemTracker::GetClosestItemOfType(eItemType type, const Elite::Vector2 & currPos)
{
	switch (type)
	{
	case eItemType::PISTOL:
		return GetClosest(m_PistolVec, currPos);
		break;
	case eItemType::MEDKIT:
		return GetClosest(m_MedkitVec, currPos);
		break;
	case eItemType::FOOD:
		return GetClosest(m_FoodVec, currPos);
		break;
	default:
		return GetClosest(m_MiscVec, currPos);
		break;
	}
}

void ItemTracker::AddItemToVec(vector<Item>& vec, eItemType type, const Elite::Vector2 & pos)
{
	bool found = false;

	for (auto item : vec)
	{
		if (item.pos == pos)
		{
			found = true;
		}
	}

	if (!found) vec.push_back(Item{ type, pos });
}

Item ItemTracker::GetClosest(const vector<Item>& vec, const Elite::Vector2 & pos)
{
	bool found = false;
	Item item{};
	float max = Elite::Distance(vec.at(0).pos, pos);

	for (auto itemVec : vec)
	{
		if (Elite::Distance(itemVec.pos, pos) < max)
		{
			item = itemVec;
		}
	}

	return item;
}


