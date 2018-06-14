#pragma once
#include "Exam_HelperStructs.h"

struct Item
{
	eItemType type;
	Elite::Vector2 pos;

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

	Item GetClosestItemOfType(eItemType type, const Elite::Vector2& currPos);

private:
	vector<Item> m_MedkitVec;
	vector<Item> m_PistolVec;
	vector<Item> m_FoodVec;
	vector<Item> m_MiscVec;

	void AddItemToVec(vector<Item>& vec, eItemType type, const Elite::Vector2& pos);
	Item GetClosest(const vector<Item>& vec, const Elite::Vector2& pos);

};
