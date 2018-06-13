#pragma once
#include "stdafx.h"
#include "Blackboard.h"
#include "BehaviorTree.h"
#include "IExamInterface.h"
#include "EliteMath/EVector2.h"

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

//bool HaveAmmo(Blackboard *pBlackboard)
//{
//	//IExamInterface* pInterface;
//	//ItemInfo iInfo{};
//
//	//auto dataAvailiable = pBlackboard->GetData("Interface", pInterface);
//
//	//if (!dataAvailiable)
//	//{
//	//	return false;
//	//}
//
//	////Go through all the slots and check if any is gun
//	//for (UINT i = 0; i < pInterface->Inventory_GetCapacity(); i++)
//	//{
//	//	pInterface->Inventory_GetItem(i, iInfo);
//	//	if (iInfo.Type == eItemType::PISTOL)
//	//	{
//	//		return true;
//	//	}
//	//}
//
//	if (HasGun(pBlackboard))
//	{
//
//	}
//}
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
	ItemInfo iInfo{};

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
			pInterface->Inventory_UseItem(i);
			return BehaviorState::Success;
		}
	}
	return BehaviorState::Failure;
}