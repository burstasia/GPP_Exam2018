#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
#include "EnemyEvasion.h"

class IBaseInterface;
class IExamInterface;
class ItemTracker;
class HouseTracker;

class Plugin :public IExamPlugin
{
public:
	Plugin() {};
	virtual ~Plugin() {};

	void Initialize(IBaseInterface* pInterface, PluginInfo& info) override;
	void DllInit() override;
	void DllShutdown() override;

	void InitGameDebugParams(GameDebugParams& params) override;
	void ProcessEvents(const SDL_Event& e) override;

	SteeringPlugin_Output UpdateSteering(float dt) override;
	void Render(float dt) const override;

private:



	//Interface, used to request data from/perform actions with the AI Framework
	IExamInterface* m_pInterface = nullptr;
	vector<HouseInfo> GetHousesInFOV() const;
	vector<EntityInfo> GetEntitiesInFOV() const;

	vector<Enemy> m_VecEnemies;
	EnemyEvasion* m_pEnemyEvasion;
	ItemTracker* m_pItemTracker;
	HouseTracker* m_pHouseTracker;

	Elite::Vector2 m_Target = {};
	bool m_CanRun = false; 
	bool m_GrabItem = false; 
	bool m_UseItem = false; 
	bool m_RemoveItem = false; 
	bool m_DropItem = false; 
	float m_AngSpeed = 0.f; 

	//HELPER FUNCTIONS
	//this function fills my m_VecEnemies and updates my EnemyEvasion class
	void FillEnemyVec(float dt, const vector<EntityInfo>& entitiesFOV);
	//fill with items around me
	void FillItemVec(const vector<EntityInfo>& entitiesFOV, const vector<HouseInfo>& housesInFOV);

	//find next target pos
	Elite::Vector2 Evade();

	//movement behaviours
	Elite::Vector2 Flee(const Elite::Vector2& nextTargetPos, const AgentInfo& agentInfo);
	Elite::Vector2 SimpleSeeking(const Elite::Vector2& nextTargetPos, const AgentInfo& agentInfo);

	Elite::Vector2 CalculateFinalVelocityWithBlend(const Elite::Vector2& linVel01, const Elite::Vector2& linVel02, float blend01, float blend02);

};

//ENTRY
//This is the first function that is called by the host program
//The plugin returned by this function is also the plugin used by the host program
extern "C"
{
	__declspec (dllexport) IPluginBase* Register()
	{
		return new Plugin();
	}
}