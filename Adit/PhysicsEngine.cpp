#include "PhysicsEngine.h"

#include "VoxTree.h"

#include "BlockGrid.h"

#include "GameEngine.h"

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

// Usage example for bullet:
void PhysicsEngine::addVoxelTree(PolyVox::Region &reg)
{
	VoxTreeCoord minCoord(reg.getLowerX(), reg.getLowerY(), reg.getLowerZ());
	VoxTreeCoord maxCoord(reg.getUpperX(), reg.getUpperY(), reg.getUpperZ());

	VoxTreeCoord stride(1, reg.getWidthInVoxels(), reg.getHeightInVoxels() * reg.getWidthInVoxels());

	// This is the bit that does all the magic:
	VoxTree<CompositeBlock::blockDataType> tree(GameEngine::inst().getGrid()->getBlockMap(), minCoord, maxCoord, stride);

	// This is the bit that makes it work in bullet:
	addVoxelNode(tree.root());
}

void PhysicsEngine::addVoxelNode(VoxTreeNode * node)
{
	switch (node->mValue)
	{
	case VoxTreeNode::nodeMixed:
		addVoxelNode(node->mChildren[0]);
		addVoxelNode(node->mChildren[1]);
		break;
	case VoxTreeNode::nodeFull:
	{
		btVector3 low(btScalar(node->mMinCoord.x), btScalar(node->mMinCoord.y), btScalar(node->mMinCoord.z));
		low -= btVector3(0.5f, 0.5f, 0.5f);
		btVector3 high(btScalar(node->mMaxCoord.x), btScalar(node->mMaxCoord.y), btScalar(node->mMaxCoord.z));
		high += btVector3(0.5f, 0.5f, 0.5f);
		addStaticBox(0.5f * (low + high), 0.5f * (high - low));
	}
	break;
	default:
		break;
	}
}

void PhysicsEngine::addStaticBox(const btVector3 & pos, const btVector3 & halfSize)
{
	btCollisionShape * colShape = new btBoxShape(halfSize);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, 0, colShape);
	btRigidBody * body = new btRigidBody(rbInfo);
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(pos);
	body->setWorldTransform(trans);
	_dynamicsWorld->addRigidBody(body);
}