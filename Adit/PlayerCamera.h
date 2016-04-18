#pragma once

#include <osgGA\NodeTrackerManipulator>

class GameEngine;
class Player;

class PlayerCamera
{
public:
	PlayerCamera(GameEngine* eng);
	~PlayerCamera();

	void attach(Player * player);

	osgGA::CameraManipulator* getManipulator() { return manipulator; }

	void update();

private:
	GameEngine* eng;
	osgGA::NodeTrackerManipulator* manipulator;

	float _distance;

	bool _firstPerson;
};
