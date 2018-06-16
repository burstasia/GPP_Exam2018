#include "stdafx.h"
#include "InventoryTracker.h"


InventoryTracker::InventoryTracker()
{
	m_InventoryArr[0] = InventorySlot{ {},0,false };
	m_InventoryArr[1] = InventorySlot{ {},1,false };
	m_InventoryArr[2] = InventorySlot{ {},2,false };
	m_InventoryArr[3] = InventorySlot{ {},3,false };
	m_InventoryArr[4] = InventorySlot{ {},4,false };
}


InventoryTracker::~InventoryTracker()
{
}

bool InventoryTracker::AddItem(int slotIndex, const ItemInfo & info)
{
	if (slotIndex <= 4 && slotIndex >= 0 && !m_InventoryArr[slotIndex].hasItem)
	{
		m_InventoryArr[slotIndex] = InventorySlot{ info, slotIndex, true };
		return true;
	}
	else return false;
}

bool InventoryTracker::RemoveItem(int slotIndex)
{
	if (slotIndex <= 4 && slotIndex >= 0)
	{
		if (m_InventoryArr[slotIndex].hasItem)
		{
			m_InventoryArr[slotIndex] = InventorySlot{ {},slotIndex,false };
			return true;
		}
	}
	return false;
}

bool InventoryTracker::HasItem(int & slotIndex, const eItemType & info)
{
	//HasItem goes through the array and checks for a certain item
	//slotIndex will = the index of this item
	
	for (size_t i = 0; i < 5; i++)
	{
		if (m_InventoryArr[i].item.Type == info && m_InventoryArr[i].hasItem)
		{
			slotIndex = i;
			return true;
		}
	}
	return false;
}
