#pragma once
#include "Exam_HelperStructs.h"

struct Enemy
{
	EnemyInfo m_Info{};
	float m_CurrTime = 0.0f;
	Elite::Vector2 m_FirstPos{};
	Elite::Vector2 m_LinearVelocity{};
};

class EnemyEvasion
{
public:
	EnemyEvasion(float totalEnemyTime);
	~EnemyEvasion();

	void Update(float dt, const vector<EnemyInfo>& enemiesInFOV);

	
	Enemy GetAverageEnemy();
	vector<Enemy>& GetEnemyVec();

private:
	//DATA MEMBERS
	vector<Enemy> m_StoredEnemiesVec;
	float m_TotalEnemyTime;

	//HELPER FUNCTIONS
	Elite::Vector2 CalculateLinearVelocity(const Elite::Vector2& firstPos, const Elite::Vector2& currPos, float dt);
	void RemoveEnemiesAndIncrementTime(float dt);
	void AddEnemies(const vector<EnemyInfo>& enemiesInFOV);
	void UpdateLinearVelocityEnemies(float dt);
};

