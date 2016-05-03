#include "PhysicsEngine.h"

#include "BlockMgmt/BlockGrid.h"

#include "Engine/GameEngine.h"

PhysicsEngine::PhysicsEngine()
{

	//Instantiate everything we need for the physics engine
	_broadphase = new btDbvtBroadphase();
	_collisionConfiguration = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
	_solver = new btSequentialImpulseConstraintSolver;
	_dynamicsWorld = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);

	//Set the gravity (-9.8, to simplify)
	_dynamicsWorld->setGravity(btVector3(0, 0, -9.8f));
}


PhysicsEngine::~PhysicsEngine()
{
	delete _broadphase;
	delete _collisionConfiguration;
	delete _dispatcher;
	delete _solver;
	delete _dynamicsWorld;
}

void PhysicsEngine::update(btScalar time)
{
	_dynamicsWorld->stepSimulation(time);
}