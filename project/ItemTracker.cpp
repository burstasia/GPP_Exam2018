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
	AddItemToVec(m_AllItems, type, pos);

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

Elite::Vector2 ItemTracker::GetClosestDistance(eItemType type, const Elite::Vector2 & currPos)
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

bool ItemTracker::SetItemsInFOV(const vector<EntityInfo>& entities)
{
	bool found = false;

	vector<EntityInfo> newEntities;

	for (auto entity : entities)
	{
		for (auto item : m_ItemsInFOV)
		{
			if (entity.Location == item.Location)
			{
				found = true;
			}
		}
		if(!found) newEntities.push_back(entity);
	}
	
	m_NewEntities = newEntities;

	for (auto newEntity : newEntities)
	{
		m_ItemsInFOV.push_back(newEntity);
	}


	if (newEntities.size() > 0)
	{
		return true;
	}
	else return false;
}

const vector<EntityInfo>& ItemTracker::GetItemsInFOV()
{
	// TODO: insert return statement here
	return m_ItemsInFOV;
}

const vector<EntityInfo>& ItemTracker::GetNewEntities()
{
	// TODO: insert return statement here
	return m_NewEntities;
}

void ItemTracker::RemoveItemsFOV()
{
	if (m_ItemsInFOV.size() > 0)
		m_ItemsInFOV.clear();
}

void ItemTracker::RemoveItem(eItemType type, Elite::Vector2 pos)
{
	RemoveItemVec(m_AllItems, pos, type);

	switch (type)
	{
	case eItemType::PISTOL:
		RemoveItemVec(m_PistolVec, pos, type);
		break;
	case eItemType::MEDKIT:
		RemoveItemVec(m_MedkitVec, pos, type);
		break;
	case eItemType::FOOD:
		RemoveItemVec(m_FoodVec, pos, type);
		break;
	default:
		RemoveItemVec(m_MiscVec, pos, type);
		break;
	}
}

float ItemTracker::GetClosestDistance(const Elite::Vector2 & pos)
{
	float max = Elite::Distance(m_AllItems.at(0).pos, pos);

	for (auto item : m_AllItems)
	{
		if (Elite::Distance(item.pos, pos) < max)
		{
			max = Elite::Distance(item.pos, pos);
		}
	}

	return max;
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

Elite::Vector2 ItemTracker::GetClosest(const vector<Item>& vec, const Elite::Vector2 & pos)
{
	//helper function to get the closest item in a certain vector
	bool found = false;
	Item item{};
	float max{};
	Elite::Vector2 posClosest{};

	if (vec.size() > 0)
	{
		float max = Elite::Distance(vec.at(0).pos, pos);
		Elite::Vector2 posClosest = vec.at(0).pos;
	}

	for (auto itemVec : vec)
	{
		if (Elite::Distance(itemVec.pos, pos) < max)
		{
			max = Elite::Distance(itemVec.pos, pos);
			posClosest = itemVec.pos;
			item = itemVec;
		}
	}

	m_ClosestPos = item.pos;
	return posClosest;
}

void ItemTracker::RemoveItemVec(vector<Item>& vec, const Elite::Vector2 & pos, eItemType type)
{
	if (vec.size() > 0)
	{
		bool found = false;
		int indextoErase;

		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec.at(i).pos == pos)
			{
				found = true;
				indextoErase = i;
			}
		}

		if(found)vec.erase(vec.begin() + indextoErase);
	}
	
}


