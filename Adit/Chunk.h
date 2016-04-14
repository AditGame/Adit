#pragma once

#include <vector>

#include "CompositeBlock.h"

#include "Coords.h"

#include <PolyVox\Region.h>
#include <PolyVox\Vector.h>
#include <PolyVox\CubicSurfaceExtractor.h>

#include <btBulletDynamicsCommon.h>

class BlockGrid;


class Chunk
{
public:
	friend class OSGRenderer;
	friend class LandGenerator;

	Chunk(Coords chunkLocation, osg::Group* gridNode = nullptr);
	~Chunk();

	static const int chunkHeight;
	static const int chunkWidth;

	void attachToGrid(osg::Group* gridNode);

	Coords getlocation() { return _chunkLocation;  }

	static bool isInBounds(Coords location);

	void rebuild(BlockGrid* grid);

	void createPhysData(PolyVox::Mesh<PolyVox::CubicVertex<CompositeBlock::blockDataType> > &mesh);

private:
	Coords _chunkLocation;

	//OSG Render data
	osg::ref_ptr<osg::Group> _parentNode;
	osg::ref_ptr<osg::PositionAttitudeTransform> _baseNode;
	osg::ref_ptr<osg::Geode> _cubeMeshNode;

	//Bullet Physics Data
	btConvexHullShape* _physShape;
	btRigidBody* _rigidBody;
	btDefaultMotionState* _motionState;
};

