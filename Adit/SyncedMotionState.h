#pragma once

#include <btBulletCollisionCommon.h>

#include <osgbCollision\Utils.h>

class Entity;

class SyncedMotionState : public btMotionState
{

public:
	SyncedMotionState(const btTransform &initialPosition, Entity* ent, osg::Vec3d centerOfMass = osg::Vec3d(0,0,0), bool syncRotation = false);

	virtual ~SyncedMotionState();

	void setEntity(Entity* ent)
	{
		_entity = ent;
	}

	virtual void getWorldTransform(btTransform &worldTrans) const
	{
		worldTrans = _initialPosition;
	}

	virtual void setWorldTransform(const btTransform &worldTrans);

protected:
	Entity* _entity;
	btTransform _initialPosition;
	osg::Vec3d _com;

	bool _syncRotation;

};