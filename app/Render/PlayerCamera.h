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

	void setDistance(float dist);
	void modDistance(float dist) { setDistance(dist + _distance); }
	float getDistance() { return _distance; }

	void setFirstPerson(bool mode);

private:

	//at or below this distance we switch to first person
	static float _minDistance;

	static float _maxDistance;

	GameEngine* eng;

	osg::Vec3d getBestCameraPosition(osg::Vec3d start, osg::Vec3d end);

	float _distance;

	bool _firstPerson;
};
