#pragma once

#include "Coords.h"

#include "osg\Vec3d"
#include "osg/Group"

#include "PolyVox\Picking.h"

class Player;

class BlockDestructionController
{
public:
	BlockDestructionController();
	~BlockDestructionController();

	void highlightBlock(Player* player);

	void destroyBlock(Player* player);

	static PolyVox::PickResult preformVoxelRaycast(Player* player);

	//OSG Node Logic
	void attach(osg::Group* group);

private:
	osg::PositionAttitudeTransform* _baseNode;
	osg::Group* _parentNode;

	osg::Geode* _geode;
	osg::Geometry* _geom;

};