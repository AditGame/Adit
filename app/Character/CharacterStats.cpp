#include "CharacterStats.h"



CharacterStats::CharacterStats() :
	_walkingSpeed(5),
	_runningSpeed(10),
	_sprintSpeed(15),
	_manipulateDistance(10),
	_createCooldown(500.0/1000.0),
	_destroyTime(500.0/1000.0)
{
}


CharacterStats::~CharacterStats()
{
}
