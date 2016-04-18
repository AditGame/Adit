#pragma once
class CharacterStats
{
public:
	CharacterStats();
	~CharacterStats();

	int getWalkingSpeed() { return _walkingSpeed; }
	int getRunningSpeed() { return _runningSpeed; }
	int getSprintSpeed() { return _sprintSpeed; }

private:
	int _walkingSpeed;
	int _runningSpeed;
	int _sprintSpeed;
};

