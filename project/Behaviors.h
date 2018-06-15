#pragma once
#include "stdafx.h"
#include "Blackboard.h"
#include "BehaviorTree.h"
#include "IExamInterface.h"
#include "EliteMath/EVector2.h"
#include "ItemTracker.h"
#include "HouseTracker.h"

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

	if (pInterface->Agent_GetInfo().Energy < 5.0f)
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
	ItemInfo iInfo{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable)
	{
		return false;
	}

	//Go through all the slots and check if any is food
	for (UINT i = 0; i < pInterface->Inventory_GetCapacity(); i++)
	{
		pInterface->Inventory_GetItem(i, iInfo);
		if (iInfo.Type == eItemType::FOOD)
		{
			return true;
		}
	}
	
	return false;
}

bool HasMedkit(Blackboard* pBlackboard)
{
//	auto hasMedkit = false;
	IExamInterface* pInterface;
	ItemInfo iInfo{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable)
	{
		return false;
	}

	//Go through all the slots and check if any is medkit
	for (UINT i = 0; i < pInterface->Inventory_GetCapacity(); i++)
	{
		pInterface->Inventory_GetItem(i, iInfo);
		if (iInfo.Type == eItemType::MEDKIT)
		{
			return true;
		}
	}

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

bool InRangeItem(Blackboard* pBlackboard)
{
	IExamInterface* pInterface;
	ItemInfo iInfo{};
	Elite::Vector2 itemPos{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("ItemPos", itemPos);

	if (!dataAvailiable)
	{
		return false;
	}

	return Elite::Distance(pInterface->Agent_GetInfo().Position, itemPos) < pInterface->Agent_GetInfo().GrabRange;
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

	if (pInterface->Agent_GetInfo().IsInHouse) return true;
	
	return false;
}

bool TargetReached(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	Elite::Vector2 target;

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("target", target);

	if (!dataAvailiable) return false;

	if (Elite::Distance(pInterface->Agent_GetInfo().Position, target) < 5.0f)
	{
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

	if (inHousePrevFrame) return true;
	else return false;
}
//actions
BehaviorState EatFood(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	ItemInfo iInfo{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable)
	{
		return BehaviorState::Failure;
	}

	for (UINT i = 0; i < pInterface->Inventory_GetCapacity(); i++)
	{
		pInterface->Inventory_GetItem(i, iInfo);
		if (iInfo.Type == eItemType::FOOD)
		{
			pInterface->Inventory_UseItem(i);
			return BehaviorState::Success;
		}
	}
	return BehaviorState::Failure;
}

BehaviorState UseMedkit(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	ItemInfo iInfo{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface);

	if (!dataAvailiable)
	{
		return BehaviorState::Failure;
	}

	for (UINT i = 0; i < pInterface->Inventory_GetCapacity(); i++)
	{
		pInterface->Inventory_GetItem(i, iInfo);
		if (iInfo.Type == eItemType::MEDKIT)
		{
			pInterface->Inventory_UseItem(i);
			return BehaviorState::Success;
		}
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

BehaviorState Pop(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	deque<Elite::Vector2> * targets{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("targetDeque", targets);

	if (!dataAvailiable) return BehaviorState::Failure;

	targets->pop_front();
	return BehaviorState::Success;
}

BehaviorState PushFourCorners(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	HouseTracker houseTracker;
	deque<Elite::Vector2> *targets{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface)&&
		pBlackboard->GetData("HouseTracker", houseTracker) &&
		pBlackboard->GetData("targetDeque", targets);

	if (!dataAvailiable) return BehaviorState::Failure;
	float distance{};

	House house = houseTracker.GetClosestHouse(pInterface->Agent_GetInfo().Position, distance);

	auto tempVec = house.searchPositions;

	targets->push_front(tempVec.at(0));
	targets->push_front(tempVec.at(1));
	targets->push_front(tempVec.at(2));
	targets->push_front(tempVec.at(3));

	return BehaviorState::Success;

}

BehaviorState SetTarget(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	deque<Elite::Vector2> targets;
	Elite::Vector2 *target{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("targetDeque", targets) &&
		pBlackboard->GetData("target", target);

	if (!dataAvailiable) return BehaviorState::Failure;

	if (targets.size() > 0)
	{
		*target = targets.at(0);
		return BehaviorState::Success;
	}

	else return BehaviorState::Failure;
}

BehaviorState PushCheckpoint(Blackboard *pBlackboard)
{
	IExamInterface* pInterface;
	deque<Elite::Vector2> targets{};

	auto dataAvailiable = pBlackboard->GetData("Interface", pInterface) &&
		pBlackboard->GetData("targetDeque", targets);

	if (!dataAvailiable) return BehaviorState::Failure;

	//Retrieve the current location of our CheckPoint
	auto checkpointLocation = pInterface->World_GetCheckpointLocation();

	//Use the navmesh to calculate the next navmesh point
	targets.push_front(pInterface->NavMesh_GetClosestPathPoint(checkpointLocation));

	pBlackboard->ChangeData("targetDeque", targets);

	return BehaviorState::Success;
}