#pragma once

#include <osg/Timer>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <osg\Vec3f>
#include <osg\Quat>
#include <osg\PositionAttitudeTransform>

class GameEngine;
class SyncedMotionState;

class Entity
{
public:
	Entity(osg::Group* parent, osg::Vec3d initPosition = osg::Vec3d(0,0,0));
	virtual ~Entity();

	//Update any logic
	virtual void update(GameEngine* eng, btScalar time) = 0;

	//Position Logic
	virtual void setPosition(const osg::Vec3d&, bool movePhysics = true);
	void movePosition(const osg::Vec3d&, bool movePhysics = true);
	osg::Vec3d getPosition() { return _baseNode->getPosition(); }

	//Rotation Logic
	virtual void setRotation(osg::Vec3f);
	void modRotation(osg::Vec3f);
	osg::Vec3f getRotation() { return _rotation; }

	//OSG Node Logic
	void attach(osg::Group* group);
	osg::PositionAttitudeTransform* getBaseNode() { return _baseNode; }

	//Bullet Rigid body logic
	btRigidBody* getRigidBody() { return _rigidBody; }

protected:
	osg::PositionAttitudeTransform* _baseNode;
	osg::Group* _parentNode;

	//Seperate out the rotation data from OSG to avoid dealing with quats
	osg::Vec3f _rotation;

	//bullet data
	btRigidBody* _rigidBody;
	SyncedMotionState* _motionState;
};

