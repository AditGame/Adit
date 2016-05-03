#pragma once

#include <osg/Timer>

#include <btBulletDynamicsCommon.h>

#include <PolyVox\Region.h>

class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	void update(btScalar);

	btDiscreteDynamicsWorld* getWorld() { return _dynamicsWorld; }
private:
	btBroadphaseInterface* _broadphase;
	btDefaultCollisionConfiguration* _collisionConfiguration;
	btCollisionDispatcher* _dispatcher;
	btSequentialImpulseConstraintSolver* _solver;
	btDiscreteDynamicsWorld* _dynamicsWorld;
};

