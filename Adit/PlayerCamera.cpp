#include "PlayerCamera.h"

#include <osg/ObserverNodePath>

#include "GameEngine.h"
#include "PhysicsEngine.h"

#include "Player.h"

PlayerCamera::PlayerCamera(GameEngine* eng) : eng(eng), _distance(15.0), _firstPerson(false)
{
}

PlayerCamera::~PlayerCamera()
{
}

void PlayerCamera::update()
{

	osg::Vec3d eye;
	osg::Vec3d lookAt;

	//First Person: Eye is eye node, look at is a vector from the eye node
	//Third Person: Look at is eye node, eye is vector behind eye node

	if (_firstPerson)
	{
		eye = osg::computeLocalToWorld(GameEngine::inst().getPlayer()->getEyeNode()->getParentalNodePaths().at(0)).getTrans();
		osg::Vec3d rot = GameEngine::inst().getPlayer()->getRotation();

		lookAt = osg::Vec3d(eye.x() + cos(rot.x())*_distance, eye.y() + sin(rot.x())*_distance, eye.z() + sin(-rot.y())*_distance);
	}
	else
	{
		lookAt = osg::computeLocalToWorld(GameEngine::inst().getPlayer()->getEyeNode()->getParentalNodePaths().at(0)).getTrans();
		osg::Vec3d rot = GameEngine::inst().getPlayer()->getRotation();

		float yaw = -rot.x() - osg::PI_2;
		float pitch = -rot.y();
		float x = cos(yaw)*cos(pitch);
		float y = sin(yaw)*cos(pitch);
		float z = sin(pitch);

		eye = osg::Vec3d(lookAt.x() + x*_distance, lookAt.y() + y*_distance, lookAt.z() + z*_distance);

		eye = getBestCameraPosition(lookAt, eye);
	}

	GameEngine::inst().getViewer()->getCamera()->setViewMatrixAsLookAt(eye, lookAt, osg::Vec3d(0, 0, 1));
}

osg::Vec3d PlayerCamera::getBestCameraPosition(osg::Vec3d start, osg::Vec3d end)
{
	btVector3 btFrom = osgbCollision::asBtVector3(start);
	btVector3 btTo = osgbCollision::asBtVector3(end);
	btCollisionWorld::ClosestConvexResultCallback res(btFrom, btTo);
	res.m_collisionFilterMask = btBroadphaseProxy::StaticFilter;

	btConvexShape* shape = new btSphereShape(0.25);

	btTransform startTransform(btMatrix3x3(btQuaternion(0, 0, 0, 1)), btFrom);
	btTransform endTransform(btMatrix3x3(btQuaternion(0, 0, 0, 1)), btTo);

	GameEngine::inst().getPhysics()->getWorld()->convexSweepTest(shape, startTransform, endTransform, res);

	if (res.hasHit())
	{
		return osgbCollision::asOsgVec3(res.m_hitPointWorld);
	}

	return end;
}
