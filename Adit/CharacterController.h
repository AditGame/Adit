#pragma once

class Player;

class CharacterController
{
public:
	CharacterController(Player* player);

	void update();

private:

	enum MotionStates
	{
		No_State, // Default state
		On_Ground,
		In_Air,

		Num_Motion_States
	};

	void setUpState(int newState);
	void updateState();

	Player* _player;
	int _currState;
};

