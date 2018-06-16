#pragma once
#include "Exam_HelperStructs.h"
struct InventorySlot
{
	ItemInfo item;
	int slotID;
	bool hasItem;

};

class InventoryTracker
{
public:
	InventoryTracker();
	~InventoryTracker();

	bool AddItem(int slotIndex, const ItemInfo& info);
	bool RemoveItem(int slotIndex);
	bool HasItem(int& slotIndex, const ItemInfo& info);

private:
	vector<InventorySlot> m_Inventory;
	InventorySlot m_InventoryArr[5];
};

