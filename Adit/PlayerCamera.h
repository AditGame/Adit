#pragma once

#include <osgGA\NodeTrackerManipulator>

class GameEngine;
class Player;

class PlayerCamera
{
public:
	PlayerCamera(GameEngine* eng);
	~PlayerCamera();

	void update();

private:
	GameEngine* eng;

	osg::Vec3d getBestCameraPosition(osg::Vec3d start, osg::Vec3d end);

	float _distance;

	bool _firstPerson;
};
