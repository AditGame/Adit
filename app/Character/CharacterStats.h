#pragma once
class CharacterStats
{
public:
	CharacterStats();
	~CharacterStats();

	int getWalkingSpeed() { return _walkingSpeed; }
	int getRunningSpeed() { return _runningSpeed; }
	int getSprintSpeed() { return _sprintSpeed; }

	int getJumpSpeed() { return _runningSpeed; }

	double getCreateBlockCooldown() { return _createCooldown; }
	double getDestroyBlockTime() { return _destroyTime; }

	int getManipulateDist() { return _manipulateDistance; }

private:
	int _walkingSpeed;
	int _runningSpeed;
	int _sprintSpeed;

	int _manipulateDistance;

	double _createCooldown;
	double _destroyTime;
};

