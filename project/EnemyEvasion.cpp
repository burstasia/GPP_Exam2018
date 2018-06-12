#include "stdafx.h"
#include "EnemyEvasion.h"


EnemyEvasion::EnemyEvasion(float totalEnemyTime):
	m_TotalEnemyTime(totalEnemyTime)
{
}


EnemyEvasion::~EnemyEvasion()
{
}

void EnemyEvasion::Update(float dt, const vector<EnemyInfo>& enemiesInFOV)
{
	AddEnemies(enemiesInFOV);
	RemoveEnemiesAndIncrementTime(dt);
	UpdateLinearVelocityEnemies(dt);
}

void EnemyEvasion::RemoveEnemiesAndIncrementTime(float dt)
{
	if (m_StoredEnemiesVec.size() > 0)
	{
		for (size_t i = 0; i < m_StoredEnemiesVec.size(); i++)
		{
			if (m_StoredEnemiesVec.at(i).m_CurrTime >= m_TotalEnemyTime) m_StoredEnemiesVec.erase(m_StoredEnemiesVec.begin() + i);
			else m_StoredEnemiesVec.at(i).m_CurrTime += dt;
		}
	}
}

void EnemyEvasion::AddEnemies(const vector<EnemyInfo>& enemiesInFOV)
{
	bool found{};

	if (enemiesInFOV.size() > 0)
	{
		for (size_t i = 0; i < enemiesInFOV.size(); i++)
		{
			for (size_t j = 0; j < m_StoredEnemiesVec.size(); j++)
			{
				//if the enemy is with a certain radius of itself then it is the same enemy
				if ((m_StoredEnemiesVec.at(j).m_Info.Location - enemiesInFOV.at(i).Location).Magnitude() < enemiesInFOV.at(i).Size * 3.0f)
				{
					m_StoredEnemiesVec.at(j).m_Info.Location = enemiesInFOV.at(i).Location;
					found = true;
				}
			}
			if (found == false) m_StoredEnemiesVec.push_back(Enemy{ enemiesInFOV.at(i), 0.01f, enemiesInFOV.at(i).Location,{ 0.0f, 0.0f } });
		}
	}

}

void EnemyEvasion::UpdateLinearVelocityEnemies(float dt)
{
	if (m_StoredEnemiesVec.size() > 0)
	{
		for (auto &enemy : m_StoredEnemiesVec)
		{
			enemy.m_LinearVelocity = CalculateLinearVelocity(enemy.m_FirstPos, enemy.m_Info.Location, dt);
		}
	}
	
}

Enemy EnemyEvasion::GetAverageEnemy()
{
	Enemy averageEnemy{};

	for (size_t i = 0; i < m_StoredEnemiesVec.size(); i++)
	{
		averageEnemy.m_FirstPos += m_StoredEnemiesVec.at(i).m_FirstPos;
		averageEnemy.m_CurrTime += m_StoredEnemiesVec.at(i).m_CurrTime;
		averageEnemy.m_Info.Location += m_StoredEnemiesVec.at(i).m_Info.Location;
	}

	averageEnemy.m_FirstPos /= float(m_StoredEnemiesVec.size());
	averageEnemy.m_CurrTime /= float(m_StoredEnemiesVec.size());
	averageEnemy.m_Info.Location /= float(m_StoredEnemiesVec.size());

	averageEnemy.m_LinearVelocity = CalculateLinearVelocity(averageEnemy.m_FirstPos, averageEnemy.m_Info.Location, averageEnemy.m_CurrTime) / float(m_StoredEnemiesVec.size());

	return averageEnemy;
}

vector<Enemy>& EnemyEvasion::GetEnemyVec()
{
	return m_StoredEnemiesVec;
}

Elite::Vector2 EnemyEvasion::CalculateLinearVelocity(const Elite::Vector2 & firstPos, const Elite::Vector2 & currPos, float dt)
{
	Elite::Vector2 linearVelocity{};

	linearVelocity = (currPos - firstPos) / dt;

	return linearVelocity;
}
