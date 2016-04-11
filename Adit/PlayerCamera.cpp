#include "PlayerCamera.h"

#include <osg/ObserverNodePath>

#include "GameEngine.h"

#include "Player.h"

PlayerCamera::PlayerCamera(GameEngine* eng) : eng(eng)
{
	manipulator = new osgGA::NodeTrackerManipulator();
	manipulator->setDistance(20.0f);
	manipulator->setAutoComputeHomePosition(false);
}

PlayerCamera::~PlayerCamera()
{
	delete manipulator;
}

void PlayerCamera::attach(Player* player)
{
	manipulator->setTrackerMode(osgGA::NodeTrackerManipulator::TrackerMode::NODE_CENTER_AND_ROTATION);
	manipulator->setRotationMode(osgGA::NodeTrackerManipulator::RotationMode::ELEVATION_AZIM);
	manipulator->setTrackNode(player->getEyeNode());
}