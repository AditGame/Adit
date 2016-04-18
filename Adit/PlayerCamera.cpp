#include "PlayerCamera.h"

#include <osg/ObserverNodePath>

#include "GameEngine.h"

#include "Player.h"

PlayerCamera::PlayerCamera(GameEngine* eng) : eng(eng), _distance(15.0), _firstPerson(false)
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

void PlayerCamera::update()
{

	osg::Vec3d eye;
	osg::Vec3d lookAt;

	//First Person: Eye is eye node, look at is a vector from the eye node
	//Third Person: Look at is eye node, eye is vector behind eye node

	if (_firstPerson)
	{
		eye = GameEngine::inst().getPlayer()->getEyeNode()->getPosition();
		osg::Vec3d rot = GameEngine::inst().getPlayer()->getRotation();

		lookAt = osg::Vec3d(eye.x() + cos(rot.x())*_distance, eye.y() + sin(rot.x())*_distance, eye.z() + sin(-rot.y())*_distance);
	}
	else
	{
		lookAt = GameEngine::inst().getPlayer()->getPosition();
		osg::Vec3d rot = GameEngine::inst().getPlayer()->getRotation();

		float yaw = -rot.x() - osg::PI_2;
		float pitch = -rot.y();
		float x = cos(yaw)*cos(pitch);
		float y = sin(yaw)*cos(pitch);
		float z = sin(pitch);

		eye = osg::Vec3d(lookAt.x() + x*_distance, lookAt.y() + y*_distance, lookAt.z() + z*_distance);
	}

	GameEngine::inst().getViewer()->getCamera()->setViewMatrixAsLookAt(eye, lookAt, osg::Vec3d(0, 0, 1));
}