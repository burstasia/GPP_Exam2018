#pragma once
#include "Exam_HelperStructs.h"

struct Item
{
	eItemType type;
	Elite::Vector2 pos;

};

struct ItemSeen
{
	EntityInfo info;
	bool seen;
};
class ItemTracker
{
public:
	ItemTracker();
	~ItemTracker();

	void AddItem(eItemType type, Elite::Vector2 pos);

	vector<Item>& GetMedkitVec() { return m_MedkitVec; }

	vector<Item>& GetPistolVec() { return m_PistolVec; }

	vector<Item>& GetFoodVec() { return m_FoodVec; }

	float GetClosestDistance(eItemType type, const Elite::Vector2& currPos);
	Elite::Vector2 GetClosestPos() { return m_ClosestPos; }

	bool SetItemsInFOV(const vector<EntityInfo>& entities);
	const vector<EntityInfo>& GetItemsInFOV();
	const vector<EntityInfo>& GetNewEntities();

private:
	vector<Item> m_MedkitVec;
	vector<Item> m_PistolVec;
	vector<Item> m_FoodVec;
	vector<Item> m_MiscVec;

	Elite::Vector2 m_ClosestPos;
	vector<EntityInfo> m_ItemsInFOV;
	vector<EntityInfo> m_NewEntities;

	void AddItemToVec(vector<Item>& vec, eItemType type, const Elite::Vector2& pos);
	float GetClosest(const vector<Item>& vec, const Elite::Vector2& pos);

};

