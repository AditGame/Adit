#pragma once

#include <osg/Timer>

#include "VoxTreeNode.h"

#include <btBulletDynamicsCommon.h>

#include <PolyVox\Region.h>

class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	void update(osg::ElapsedTime);

	void addVoxelTree(PolyVox::Region & reg);

	void addVoxelNode(VoxTreeNode * node);

	void addStaticBox(const btVector3 & pos, const btVector3 & halfSize);

	btDiscreteDynamicsWorld* getWorld() { return _dynamicsWorld; }
private:
	btBroadphaseInterface* _broadphase;
	btDefaultCollisionConfiguration* _collisionConfiguration;
	btCollisionDispatcher* _dispatcher;
	btSequentialImpulseConstraintSolver* _solver;
	btDiscreteDynamicsWorld* _dynamicsWorld;
};

