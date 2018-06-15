#include "stdafx.h"
#include "PlayerTracker.h"


PlayerTracker::PlayerTracker()
{
	m_pTargetDeque.push_front({ 0.0f,0.0f });
}


PlayerTracker::~PlayerTracker()
{
}
