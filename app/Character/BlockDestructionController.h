#pragma once

#include "Utilities/Coords.h"

#include "osg\Vec3d"
#include "osg/Group"

#include <btBulletCollisionCommon.h>

#include "PolyVox\Picking.h"

class Player;

class BlockDestructionController
{
public:
	BlockDestructionController();
	~BlockDestructionController();

	void update(Player* player, double time);

	void startDestroyBlock(Player* player);
	void startCreateBlock(Player* player);

	void endDestroyBlock(Player* player);
	void endCreateBlock(Player* player);

	//Returns if a block being placed would intersect a physics object
	bool canPlaceBlock(PolyVox::Vector3DInt32 location);


	static PolyVox::PickResult preformVoxelRaycast(Player* player);

	//OSG Node Logic
	void attach(osg::Group* group);

private:
	osg::PositionAttitudeTransform* _baseNode;
	osg::Group* _parentNode;

	osg::Geode* _geode;
	osg::Geometry* _geom;

	void destroyBlock(Player* player);

	void createBlock(Player* player, int);

	void highlightBlock(Player* player);

	double _createCoolDown;
	double _destroyTime;
	bool _isDestroying;
	bool _isCreating;

	//canPlaceBlock data
	btConvexShape* boxShape;
	btCollisionObject* boxObj;

};

class contactTestCallback : public btCollisionWorld::ContactResultCallback
{
public:
	contactTestCallback::contactTestCallback(const btCollisionObject* baseObj) :
		obj(baseObj)
	{ }

	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
	{

		if (colObj0Wrap->getCollisionObject() != obj) collidedWith.push_back(colObj0Wrap->getCollisionObject());

		if (colObj1Wrap->getCollisionObject() != obj) collidedWith.push_back(colObj1Wrap->getCollisionObject());

		return 0;
	}
	
	std::list<const btCollisionObject*> collidedWith;
private:
	const btCollisionObject* obj;
};