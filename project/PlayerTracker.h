#pragma once
class PlayerTracker
{
public:
	PlayerTracker();
	~PlayerTracker();

	deque<Elite::Vector2>& GetDeque() { return m_pTargetDeque; }

private:
	deque<Elite::Vector2> m_pTargetDeque;
};

