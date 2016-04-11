#pragma once

#include <vector>

#include "CompositeBlock.h"

#include "Coords.h"

#include <PolyVox\Region.h>

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

private:
	Coords _chunkLocation;

	//OSG Render data
	osg::ref_ptr<osg::Group> _parentNode;
	osg::ref_ptr<osg::PositionAttitudeTransform> _baseNode;

	osg::ref_ptr<osg::Geode> _cubeMeshNode;
};

