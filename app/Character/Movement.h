#pragma once
struct Movement
{
public:
	Movement() : forwardBack(0), leftRight(0), turnLeftRight(0), turnUpDown(0) {}

	void clear()
	{
		forwardBack = 0;
		leftRight = 0;
		jump = 0;
		turnLeftRight = 0;
		turnUpDown = 0;
	}

	float forwardBack;
	float leftRight;
	float jump;

	float turnLeftRight;
	float turnUpDown;
};

