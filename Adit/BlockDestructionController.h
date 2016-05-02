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

	void update(Player* player, double time);

	void startDestroyBlock(Player* player);
	void startCreateBlock(Player* player);

	void endDestroyBlock(Player* player);
	void endCreateBlock(Player* player);


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

};