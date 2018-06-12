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

	
	vector<EnemyInfo> vEnemiesInFOV;

	for (auto entity : vEntitiesInFOV)
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
