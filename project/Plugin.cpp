#include "stdafx.h"
#include "Plugin.h"
#include "IExamInterface.h"
#include "EnemyEvasion.h"

//Called only once, during initialization
void Plugin::Initialize(IBaseInterface* pInterface, PluginInfo& info)
{
	//Retrieving the interface
	//This interface gives you access to certain actions the AI_Framework can perform for you
	m_pInterface = static_cast<IExamInterface*>(pInterface);

	//Bit information about the plugin
	//Please fill this in!!
	info.BotName = "EatSeekSurviveRepeat";
	info.Student_FirstName = "Nicole";
	info.Student_LastName = "Munro";
	info.Student_Class = "2DAE01";

	//Create EnemyEvasion class to use to store enemies
	m_pEnemyEvasion = new EnemyEvasion(2.0f);
}

//Called only once
void Plugin::DllInit()
{
	//Called when the plugin is loaded
}

//Called only once
void Plugin::DllShutdown()
{
	//Called wheb the plugin gets unloaded
}

//Called only once, during initialization
void Plugin::InitGameDebugParams(GameDebugParams& params)
{
	params.AutoFollowCam = true; //Automatically follow the AI? (Default = true)
	params.RenderUI = true; //Render the IMGUI Panel? (Default = true)
	params.SpawnEnemies = true; //Do you want to spawn enemies? (Default = true)
	params.EnemyCount = 20; //How many enemies? (Default = 20)
	params.GodMode = false; //GodMode > You can't die, can be usefull to inspect certain behaviours (Default = false)
							//params.LevelFile = "LevelTwo.gppl";
	params.AutoGrabClosestItem = true; //A call to Item_Grab(...) returns the closest item that can be grabbed. (EntityInfo argument is ignored)
	params.OverrideDifficulty = false; //Override Difficulty?
	params.Difficulty = 10.f; //Difficulty Override: 0 > 1 (Overshoot is possible, >1)
}

//Only Active in DEBUG Mode
//(=Use only for Debug Purposes)
void Plugin::ProcessEvents(const SDL_Event& e)
{
	//Demo Event Code
	//In the end your AI should be able to walk around without external input
	switch (e.type)
	{
	case SDL_MOUSEBUTTONUP:
	{
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			const Elite::Vector2 pos = Elite::Vector2(static_cast<float>(x), static_cast<float>(y));
			m_Target = m_pInterface->Debug_ConvertScreenToWorld(pos);
		}
		break;
	}
	case SDL_KEYDOWN:
	{
		if (e.key.keysym.sym == SDLK_SPACE)
		{
			m_CanRun = true;
		}
		else if (e.key.keysym.sym == SDLK_LEFT)
		{
			m_AngSpeed -= Elite::ToRadians(10);
		}
		else if (e.key.keysym.sym == SDLK_RIGHT)
		{
			m_AngSpeed += Elite::ToRadians(10);
		}
		else if (e.key.keysym.sym == SDLK_g)
		{
			m_GrabItem = true;
		}
		else if (e.key.keysym.sym == SDLK_u)
		{
			m_UseItem = true;
		}
		else if (e.key.keysym.sym == SDLK_r)
		{
			m_RemoveItem = true;
		}
		else if (e.key.keysym.sym == SDLK_d)
		{
			m_DropItem = true;
		}
		break;
	}
	case SDL_KEYUP:
	{
		if (e.key.keysym.sym == SDLK_SPACE)
		{
			m_CanRun = false;
		}
		break;
	}
	}
}

//Update
//This function calculates the new SteeringOutput, called once per frame
SteeringPlugin_Output Plugin::UpdateSteering(float dt)
{
	auto steering = SteeringPlugin_Output();

	//Use the Interface (IAssignmentInterface) to 'interface' with the AI_Framework
	auto agentInfo = m_pInterface->Agent_GetInfo();

	//Retrieve the current location of our CheckPoint
	auto checkpointLocation = m_pInterface->World_GetCheckpointLocation();

	//Use the navmesh to calculate the next navmesh point
	auto nextTargetPos = m_pInterface->NavMesh_GetClosestPathPoint(checkpointLocation);

	auto vEntitiesInFOV = GetEntitiesInFOV();

	FillEnemyVec(dt, vEntitiesInFOV);
	
	Elite::Vector2 evadeVelocity{};

	if (m_VecEnemies.size() > 0 || agentInfo.Bitten)
	{
		evadeVelocity = Evade();
		m_CanRun = true;

		Elite::Vector2 normalSeekVelocity = SimpleSeeking(nextTargetPos, agentInfo);


		steering.LinearVelocity = CalculateFinalVelocityWithBlend(normalSeekVelocity, evadeVelocity, 0.8f, 0.2f);


		steering.AutoOrientate = true;

		steering.RunMode = false;

		return steering;
	}

	if (agentInfo.IsInHouse)
	{
		//spin in circle
		//look at items around me
		//determine what I need
		FillItemVec(vEntitiesInFOV);

	}
	Elite::Vector2 normalSeekVelocity = SimpleSeeking(nextTargetPos, agentInfo);
	
	steering.LinearVelocity = normalSeekVelocity;

	if (m_AngSpeed <= 0.2f) m_AngSpeed += 0.01f;
	else m_AngSpeed -= 0.01f;

	steering.AngularVelocity = m_AngSpeed;

	steering.AutoOrientate = false;

	steering.RunMode = false;

	return steering;
}

//This function should only be used for rendering debug elements
void Plugin::Render(float dt) const
{
	//This Render function should only contain calls to Interface->Draw_... functions
	m_pInterface->Draw_SolidCircle(m_Target, .7f, { 0,0 }, { 1, 0, 0 });
}

vector<HouseInfo> Plugin::GetHousesInFOV() const
{
	vector<HouseInfo> vHousesInFOV = {};

	HouseInfo hi = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetHouseByIndex(i, hi))
		{
			vHousesInFOV.push_back(hi);
			continue;
		}

		break;
	}

	return vHousesInFOV;
}

vector<EntityInfo> Plugin::GetEntitiesInFOV() const
{
	vector<EntityInfo> vEntitiesInFOV = {};

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetEntityByIndex(i, ei))
		{
			vEntitiesInFOV.push_back(ei);
			continue;
		}

		break;
	}

	return vEntitiesInFOV;
}

void Plugin::FillEnemyVec(float dt, const vector<EntityInfo>& entitiesFOV)
{
	vector<EnemyInfo> vEnemiesInFOV;

	for (auto entity : entitiesFOV)
	{
		if (entity.Type == eEntityType::ENEMY)
		{
			EnemyInfo eInfo = {};
			m_pInterface->Enemy_GetInfo(entity, eInfo);

			vEnemiesInFOV.push_back(eInfo);
		}
	}

	m_pEnemyEvasion->Update(dt, vEnemiesInFOV);

	m_VecEnemies = m_pEnemyEvasion->GetEnemyVec();
}

void Plugin::FillItemVec(const vector<EntityInfo>& entitiesFOV)
{
	vector<ItemInfo> vItemsInFOV;
	ItemInfo iInfo = {};
	for (auto entity : entitiesFOV)
	{
		if (entity.Type == eEntityType::ITEM)
		{
			//vItemsInFOV.push_back(entity);
			

			m_pInterface->Item_Grab(entity, iInfo);
			vItemsInFOV.push_back(iInfo);
		}
	}


	//m_VecEnemies = m_pEnemyEvasion->GetEnemyVec();
}

Elite::Vector2 Plugin::Evade()
{
	Enemy averageEnemy{};
	averageEnemy = m_pEnemyEvasion->GetAverageEnemy();

	auto agentInfo = m_pInterface->Agent_GetInfo();
	auto distanceToTarget = (agentInfo.Position - averageEnemy.m_Info.Location).Magnitude();
	auto offsetFromTarget = distanceToTarget / agentInfo.MaxLinearSpeed;
	auto targetDirection = -0.5f * averageEnemy.m_LinearVelocity;

	auto newTarget = averageEnemy.m_Info.Location + (targetDirection * offsetFromTarget);

	return Flee(newTarget, agentInfo);
}

Elite::Vector2 Plugin::Flee(const Elite::Vector2 & nextTargetPos, const AgentInfo & agentInfo)
{
	Elite::Vector2 linearVelocity{};

	linearVelocity = nextTargetPos - agentInfo.Position;
	linearVelocity.Normalize();
	linearVelocity *= agentInfo.MaxLinearSpeed;
	linearVelocity = linearVelocity * -1.0f;

	return linearVelocity;
}

Elite::Vector2 Plugin::SimpleSeeking(const Elite::Vector2 & nextTargetPos, const AgentInfo & agentInfo)
{
	Elite::Vector2 linearVelocity{};

	//Simple Seek Behaviour (towards Target)
	linearVelocity = nextTargetPos - agentInfo.Position; //Desired Velocity
	linearVelocity.Normalize(); //Normalize Desired Velocity
	linearVelocity *= agentInfo.MaxLinearSpeed; //Rescale to Max Speed

	return linearVelocity;
}

Elite::Vector2 Plugin::CalculateFinalVelocityWithBlend(const Elite::Vector2 & linVel01, const Elite::Vector2 & linVel02, float blend01, float blend02)
{
	Elite::Vector2 finalLinearVelocity{};

	finalLinearVelocity = (blend01 * linVel01) + (blend02 * linVel02);

	return finalLinearVelocity;
}
