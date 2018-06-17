#pragma once
#include "stdafx.h"
#include "Blackboard.h"
#include "BehaviorTree.h"
#include "IExamInterface.h"
#include "EliteMath/EVector2.h"
#include "ItemTracker.h"
#include "HouseTracker.h"
#include "PlayerTracker.h"
#include "InventoryTracker.h"
//*** GENERAL BEHAVIORS ***

//conditionals
bool LowEnergy(Blackboard* pBlackboard)
{
	auto lowEnergy = false;
	IExamInterface* pInterface;

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable)
	{
		return false;
	}

	if (pInterface->Agent_GetInfo().Energy < 2.0f)
	{
		return true;
	}
	else return false;

}

bool LowHealth(Blackboard* pBlackboard)
{
	auto lowHealth = false;
	IExamInterface* pInterface;

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable)
	{
		return false;
	}

	if (pInterface->Agent_GetInfo().Health < 5.0f)
	{
		return true;
	}
	else return false;

}

bool HasFood(Blackboard* pBlackboard)
{
//	auto hasFood = false;
	IExamInterface* pInterface;
	InventoryTracker * pInventoryTracker{};

	ItemInfo iInfo{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface)&&
		pBlackboard->GetData("InventoryTracker", pInventoryTracker);

	if (!dataAvailiable)
	{
		return false;
	}

	int slot;
	eItemType type = eItemType::FOOD;
	if (pInventoryTracker->HasItem(slot, type))
	{
		return true;
	}
	else return false;

	
	return false;
}

bool HasMedkit(Blackboard* pBlackboard)
{
//	auto hasMedkit = false;
	IExamInterface* pInterface;
	InventoryTracker * pInventoryTracker{};
	ItemInfo iInfo{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("InventoryTracker", pInventoryTracker);

	if (!dataAvailiable)
	{
		return false;
	}

	int slot;
	eItemType type = eItemType::MEDKIT;
	if (pInventoryTracker->HasItem(slot, type))
	{
		return true;
	}
	else return false;

	return false;
}

bool EnemiesInFOV(Blackboard* pBlackboard)
{
	EnemyEvasion* pEnemyEvasion;
	ItemInfo iInfo{};

	auto dataAvailiable = pBlackboard->GetData("EnemyEvasion", pEnemyEvasion);

	if (!dataAvailiable)
	{
		return false;
	}

	if (pEnemyEvasion->GetEnemyVec().size() > 0)
	{
		return true;
	}
	else return false;

}

bool HasGun(Blackboard* pBlackboard)
{
	IExamInterface* pInterface;
	InventoryTracker * pInventoryTracker{};
	ItemInfo iInfo{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("InventoryTracker", pInventoryTracker);

	if (!dataAvailiable)
	{
		return false;
	}

	int slot;
	eItemType type = eItemType::PISTOL;
	if (pInventoryTracker->HasItem(slot, type))
	{
		pInterface->Inventory_GetItem(slot, iInfo);
		if (iInfo.Type == eItemType::PISTOL)
		{
			return true;
		}
	}
	else return false;

	return false;
}

bool InRangeItem(Blackboard* pBlackboard)
{
	IExamInterface* pInterface;
	ItemInfo * itemInRange{};
	//ItemInfo 
	Elite::Vector2* target = {};
	vector<EntityInfo> m_ItemsInFOVVec;

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("ItemsInFOV", m_ItemsInFOVVec) &&
		pBlackboard->GetData("target", target) &&
		pBlackboard->GetData("ItemInRange", itemInRange);

	
	if (!dataAvailiable)
	{
		return false;
	}

	for (auto item : m_ItemsInFOVVec)
	{
		if (Elite::Distance(item.Location, *target) < 0.1f)
		{
			if (Elite::Distance(pInterface->Agent_GetInfo().Position, item.Location) < (pInterface->Agent_GetInfo().GrabRange - 0.5f))
			{
				return true;
			}
			
		}
	}

	return false;
}

bool NewItemsInFOV(Blackboard* pBlackboard)
{
	//This conditional adds the items in the agents FOV to the tracker
	//then the tracker's SetItemsInFOV returns true if there are new items in the FOV
	//if not then it returns false
	ItemTracker * pItemTracker{};
	vector<EntityInfo> itemsInFOV;

	auto dataAvailiable = pBlackboard->GetData("ItemTracker", pItemTracker) &&
		pBlackboard->GetData("ItemsInFOV", itemsInFOV);

	if (!dataAvailiable) return false;

	if (pItemTracker->SetItemsInFOV(itemsInFOV))
	{
		return true;
	}
	else return false;
}

bool HaveAmmo(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	ItemInfo iInfo{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface);


	if (!dataAvailiable)
	{
		return false;
	}

	//Go through all the slots and check if any is gun
	for (UINT i = 0; i < pInterface->Inventory_GetCapacity(); i++)
	{
		pInterface->Inventory_GetItem(i, iInfo);
		if (iInfo.Type == eItemType::PISTOL)
		{
		
			return true;
		}
	}

	return false;
}

bool CloseItem(Blackboard * pBlackboard)
{
	IExamInterface* pInterface;
	ItemInfo iInfo{};
	ItemTracker* pItemTracker{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("ItemTracker", pItemTracker) && 
		pBlackboard->GetData("ItemToTrack", iInfo);

	if (!dataAvailiable)
	{
		return false;
	}

	//TODO: change max distance to variable the plugin sets 

	if (pItemTracker->GetClosestDistance(iInfo.Type, pInterface->Agent_GetInfo().Position) > 10.0f)
	{
		return false;
	}
	else return true;
}

bool InMiddleHouse(Blackboard * pBlackboard)
{
	HouseTracker houseTracker;
	IExamInterface* pInterface;

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("HouseTracker", houseTracker);

	if (!dataAvailiable) return false;

	float distance;

	House house = houseTracker.GetClosestHouse(pInterface->Agent_GetInfo().Position, distance);

	/*if (distance <= 2.0f && pInterface->Agent_GetInfo().IsInHouse)
	{
		return true;
	}*/

	return false;
}

bool InHouse(Blackboard * pBlackboard)
{
	IExamInterface* pInterface;

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable) return false;

	if (pInterface->Agent_GetInfo().IsInHouse)
	{
		//std::cout << "In house" << std::endl;
		return true;
	}
	
	return false;
}

bool TargetReached(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	Elite::Vector2* target = {};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("target", target);

	if (!dataAvailiable) return false;

	if (Elite::Distance(pInterface->Agent_GetInfo().Position, *target) < 1.0f)
	{
		//std::cout << "target reached" << std::endl;
		return true;
	}
	else return false;
}

bool WasNotInHousePrevFrame(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	bool inHousePrevFrame;

	auto dataAvailiable = pBlackboard->GetData("InHousePrevFrame", inHousePrevFrame) &&
		pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable) return false;

	if (inHousePrevFrame) return false;
	else return true;
}

bool IsCheckpointNotSet(Blackboard *pBlackboard)
{
	bool *isCheckpointSet;

	auto dataAvailiable = pBlackboard->GetData("CheckpointSet", isCheckpointSet);

	if (!*isCheckpointSet)
	{
		return true;
	}
	else return false;
}

bool StartGame(Blackboard* pBlackboard)
{
	IExamInterface* pInterface;

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable) return false;

	if (pInterface->World_GetStats().TimeSurvived < 100.0f)
	{
		return true;
	}
	else return false;

}

bool WasInHousePrevFrame(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	bool inHousePrevFrame;

	auto dataAvailiable = pBlackboard->GetData("InHousePrevFrame", inHousePrevFrame) &&
		pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable) return false;

	if (inHousePrevFrame) return true;
	else return false;
}

bool IsNotInHouse(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable) return false;

	if (pInterface->Agent_GetInfo().IsInHouse)
	{
		//std::cout << "In house" << std::endl;
		return false;
	}
	else return true;
	return false;
}
//actions
BehaviorState EatFood(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	ItemInfo iInfo{};
	InventoryTracker * pInventoryTracker{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("InventoryTracker", pInventoryTracker);

	if (!dataAvailiable)
	{
		return BehaviorState::Failure;
	}

	int index;

	if (pInventoryTracker->HasItem(index, eItemType::FOOD))
	{
		pInterface->Inventory_UseItem(index);
		pInterface->Inventory_RemoveItem(index);
		pInventoryTracker->RemoveItem(index);
		return BehaviorState::Success;
	}

	return BehaviorState::Failure;
}

BehaviorState UseMedkit(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	ItemInfo iInfo{};
	InventoryTracker * pInventoryTracker{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface)&&
		pBlackboard->GetData("InventoryTracker", pInventoryTracker);

	if (!dataAvailiable)
	{
		return BehaviorState::Failure;
	}
	int index;

	if (pInventoryTracker->HasItem(index, eItemType::MEDKIT))
	{
		pInterface->Inventory_UseItem(index);
		pInterface->Inventory_RemoveItem(index);
		pInventoryTracker->RemoveItem(index);
		return BehaviorState::Success;
	}



	return BehaviorState::Failure;
}

BehaviorState Shoot(Blackboard * pBlackboard)
{
	IExamInterface* pInterface;
	ItemInfo iInfo{}; //Nicole this is fine don't try to change it


	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable)
	{
		return BehaviorState::Failure;
	}

	for (UINT i = 0; i < pInterface->Inventory_GetCapacity(); i++)
	{
		pInterface->Inventory_GetItem(i, iInfo);
		if (iInfo.Type == eItemType::PISTOL)
		{
			float ammo = pInterface->Item_GetMetadata(iInfo, "ammo");
			pInterface->Inventory_UseItem(i);
			return BehaviorState::Success;
		}
	}
	return BehaviorState::Failure;
}

BehaviorState Aim(Blackboard* pBlackboard)
{
	IExamInterface* pInterface;
	Elite::Vector2 enemyPos;
	Elite::Vector2 vecBetweenAgentAndEnemy;
	float *angSpeed = new float{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("EnemyPos", enemyPos)&&
		pBlackboard->GetData("AngSpeed", angSpeed);

	if (!dataAvailiable)
	{
		return BehaviorState::Failure;
	}

	vecBetweenAgentAndEnemy =(enemyPos - pInterface->Agent_GetInfo().Position);
	Elite::Normalize(vecBetweenAgentAndEnemy);

	Elite::Vector2 forward{};
	AgentInfo agentInfo = pInterface->Agent_GetInfo();

	forward.x = agentInfo.LinearVelocity.x * cos(agentInfo.Orientation) - agentInfo.LinearVelocity.y * sin(agentInfo.Orientation);
	forward.y = agentInfo.LinearVelocity.x * sin(agentInfo.Orientation) + agentInfo.LinearVelocity.y * cos(agentInfo.Orientation);

	float angle = atan2(Elite::Cross(forward, vecBetweenAgentAndEnemy), Elite::Dot(forward, vecBetweenAgentAndEnemy));

	//float angle = (atan2(velocity.x, velocity.z) * 180 / XM_PI) + 180.f;

	if (angle < 0.0f)
	{
		*angSpeed = -0.2f;
	}
	else
	{
		*angSpeed = 0.2f;
	}

	if (angle > 2.0f)
	{
		return BehaviorState::Running;
	}
	else return BehaviorState::Success;


}

BehaviorState PopCheckpoint(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	PlayerTracker * pTargets{};
	bool * isCheckpointSet{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("targetDeque", pTargets) &&
		pBlackboard->GetData("CheckpointSet", isCheckpointSet);

	if (!dataAvailiable) return BehaviorState::Failure;

	pTargets->GetDeque().pop_front();
	*isCheckpointSet = false;

	return BehaviorState::Success;
}

BehaviorState Pop(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	PlayerTracker * pTargets{};
	bool * isCheckpointSet{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("targetDeque", pTargets)&&
		pBlackboard->GetData("CheckpointSet", isCheckpointSet);

	if (!dataAvailiable) return BehaviorState::Failure;

	pTargets->GetDeque().pop_front();

	return BehaviorState::Success;
}

BehaviorState PushFourCorners(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	HouseTracker* houseTracker = {};
	PlayerTracker * pTargets{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface)&&
		pBlackboard->GetData("HouseTracker", houseTracker) &&
		pBlackboard->GetData("targetDeque", pTargets);

	if (!dataAvailiable) return BehaviorState::Failure;
	float distance{};

	House house = houseTracker->GetClosestHouse(pInterface->Agent_GetInfo().Position, distance);

	auto tempVec = house.searchPositions;

	pTargets->GetDeque().push_front(tempVec.at(0));
	pTargets->GetDeque().push_front(tempVec.at(1));
	pTargets->GetDeque().push_front(tempVec.at(2));
	pTargets->GetDeque().push_front(tempVec.at(3));

	return BehaviorState::Success;

}

BehaviorState SetTarget(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	PlayerTracker * pTargets{};
	Elite::Vector2 * target{};
	bool * checkpointSet{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("targetDeque", pTargets) &&
		pBlackboard->GetData("CheckpointSet", checkpointSet) &&
		pBlackboard->GetData("target", target);

	if (!dataAvailiable)
	{
		return BehaviorState::Failure;
	}

	if (pTargets->GetDeque().size() > 0)
	{
		if (pTargets->GetDeque().at(0) == Elite::Vector2{0.0f, 0.0f})
		{
			auto checkpointLocation = pInterface->World_GetCheckpointLocation();

			//Use the navmesh to calculate the next navmesh point
			pTargets->GetDeque().push_front(pInterface->NavMesh_GetClosestPathPoint(checkpointLocation));

			*target = pInterface->NavMesh_GetClosestPathPoint(checkpointLocation);
			*checkpointSet = true;

			pBlackboard->ChangeData("target", target);

			return BehaviorState::Success;
		}
		*target = pTargets->GetDeque().at(0);
		pBlackboard->ChangeData("target", target);
		//std::cout << target->x  << " : x" << "\n" << target->y << " : y" << std::endl;
		return BehaviorState::Success;
	}

	else return BehaviorState::Failure;
}

BehaviorState PushCheckpoint(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	PlayerTracker * pTargets{};
	bool * checkpointSet{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("targetDeque", pTargets) &&
		pBlackboard->GetData("CheckpointSet", checkpointSet);

	if (!dataAvailiable) return BehaviorState::Failure;

	//Retrieve the current location of our CheckPoint
	auto checkpointLocation = pInterface->World_GetCheckpointLocation();

	//Use the navmesh to calculate the next navmesh point
	pTargets->GetDeque().push_front(pInterface->NavMesh_GetClosestPathPoint(checkpointLocation));

	*checkpointSet = true;

	return BehaviorState::Success;
}

BehaviorState PushItems(Blackboard *pBlackboard)
{
	//get item tracker 
	//get new entities
	//push them to the deque

	ItemTracker * pItemTracker{};
	PlayerTracker * pTargets{};

	auto dataAvailiable = pBlackboard->GetData("targetDeque", pTargets) &&
		pBlackboard->GetData("ItemTracker", pItemTracker);

	if (!dataAvailiable) return BehaviorState::Failure;

	
	for (auto item : pItemTracker->GetNewEntities())
	{
		pTargets->GetDeque().push_front(item.Location);
	}

	return BehaviorState::Success;
}

BehaviorState GrabItem(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	ItemInfo *itemInRange{};
	Elite::Vector2* target {};
	ItemGrabbed* prevFrameItem{};
	ItemTracker* pItemTracker{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("target", target)&&
		pBlackboard->GetData("ItemGrabbedPrevFrame", prevFrameItem) &&
		pBlackboard->GetData("ItemInRange", itemInRange)&&
		pBlackboard->GetData("ItemTracker", pItemTracker);

	vector<EntityInfo> vEntitiesInFOV = {};

	if (!dataAvailiable) return BehaviorState::Failure;

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (pInterface->Fov_GetEntityByIndex(i, ei))
		{
			vEntitiesInFOV.push_back(ei);
			continue;
		}

		break;
	}

	if (Elite::Distance(pInterface->Agent_GetInfo().Position, *target) < (pInterface->Agent_GetInfo().GrabRange - 0.5f))
	{
		for (auto item : vEntitiesInFOV)
		{
			if (item.Type == eEntityType::ITEM)
			{
				if (Elite::Distance(item.Location, *target) < 0.1f)
				{
					if (prevFrameItem->pos != item.Location)
					{
						if (pInterface->Item_Grab(item, *itemInRange))
						{
							*prevFrameItem = ItemGrabbed{ item.Location, true };
							pItemTracker->AddItem(itemInRange->Type, item.Location);
							//pBlackboard->ChangeData("ItemInRange", itemInRange); //will this change ItemInRange in the plugin too?
							//pBlackboard->ChangeData("ItemGrabbedPrevFrame", prevFrameItem);
							return BehaviorState::Success;
						}
						else
						{
							return BehaviorState::Failure;
						}
					}
					
				}
			}
			
		}
	}
	return BehaviorState::Failure;
}

BehaviorState PickUpItem(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	ItemInfo * itemInRange{};
	InventoryTracker * pInventoryTracker{};
	ItemTracker * pItemTracker{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("ItemInRange", itemInRange) &&
		pBlackboard->GetData("InventoryTracker", pInventoryTracker)&&
		pBlackboard->GetData("ItemTracker", pItemTracker);

	if (!dataAvailiable) return BehaviorState::Failure;

	if (itemInRange->Type == eItemType::FOOD)
	{
		
		if (pInventoryTracker->AddItem(0, *itemInRange))
		{
			if (pInterface->Inventory_AddItem(0, *itemInRange))
			{
				pItemTracker->RemoveItem(eItemType::FOOD, itemInRange->Location);
				return BehaviorState::Success;
			}	
		}
		if (pInventoryTracker->AddItem(3, *itemInRange))
		{
			if (pInterface->Inventory_AddItem(3, *itemInRange))
			{
				pItemTracker->RemoveItem(eItemType::FOOD, itemInRange->Location);
				return BehaviorState::Success;
			}
		}	
	}
	else if (itemInRange->Type == eItemType::PISTOL)
	{
		if (pInventoryTracker->AddItem(1, *itemInRange))
		{
			pItemTracker->RemoveItem(eItemType::PISTOL, itemInRange->Location);
			pInterface->Inventory_AddItem(1, *itemInRange);
			return BehaviorState::Success;
		}
	}
	else if (itemInRange->Type == eItemType::MEDKIT)
	{
		if (pInventoryTracker->AddItem(2, *itemInRange))
		{
			pItemTracker->RemoveItem(eItemType::MEDKIT, itemInRange->Location);
			pInterface->Inventory_AddItem(2, *itemInRange);
			return BehaviorState::Success;
		}
	}
	else if (itemInRange->Type == eItemType::GARBAGE)
	{
		if (pInventoryTracker->AddItem(4, *itemInRange))
		{
			pItemTracker->RemoveItem(eItemType::GARBAGE, itemInRange->Location);
			pInterface->Inventory_AddItem(4, *itemInRange);
			pInventoryTracker->RemoveItem(4);
			pInterface->Inventory_RemoveItem(4);
			return BehaviorState::Success;
		}
	}
	return BehaviorState::Failure;
}

BehaviorState ClearItemsFOV(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	ItemTracker *pItemTracker{};

	auto dataAvailiable = pBlackboard->GetData("ItemTracker", pItemTracker) &&
		pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable) return BehaviorState::Failure;

	pItemTracker->RemoveItemsFOV();
	return BehaviorState::Success;

}

BehaviorState PushClosestItem(Blackboard *pBlackboard)
{

}