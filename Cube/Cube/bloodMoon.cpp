#include "bloodMoon.h"
#include <iostream>

BloodMoon::BloodMoon() : m_isActive(false), m_duration(BLOODMOON_DURATION), m_currTime(0), m_isStarted(false), monsterMultiplier(1) { }
BloodMoon::~BloodMoon() { }

void BloodMoon::Activate() { m_isActive = true; }
void BloodMoon::Deactivate() { m_isActive = false; }
void BloodMoon::Start() { m_isStarted = true; monsterMultiplier = MONSTER_MULTIPLIER; }
void BloodMoon::Stop() { m_isActive = false; }
bool BloodMoon::GetActiveState() const { return m_isActive; }
bool BloodMoon::GetStartedState() const { return m_isStarted; }
float BloodMoon::GetDuration() const { return m_currTime; }

void BloodMoon::AddElapsedUnit()
{
	m_currTime++;
}

bool BloodMoon::GetCompletionState()
{
	if (m_currTime >= m_duration) 
	{
		m_isActive = false;
		m_isStarted = false;
		Reset();
		return true;
	}
	else
		return false;
	return false;
}

void BloodMoon::Reset()
{
	m_isActive = false;
	m_currTime = 0;
	m_isStarted = false;
	monsterMultiplier = 0;
}