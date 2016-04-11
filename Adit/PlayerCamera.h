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

private:
	GameEngine* eng;
	osgGA::NodeTrackerManipulator* manipulator;
};

