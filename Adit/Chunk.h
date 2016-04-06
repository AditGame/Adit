#pragma once

#include <vector>

#include "CompositeBlock.h"

#include "Coords.h"

#include "PolyVoxCore\SimpleVolume.h"


class Chunk
{
public:
	friend class OSGRenderer;
	friend class LandGenerator;

	Chunk(Coords chunkLocation, osg::Group* gridNode = nullptr);
	~Chunk();

	void attachToGrid(osg::Group* gridNode);

	Coords getlocation() { return _chunkLocation;  }

	static const int chunkHeight;
	static const int chunkWidth;

	typedef std::vector<std::vector<std::vector<CompositeBlock> > > blockMap_type;

	CompositeBlock& getBlock(Coords location, bool relativeToChunk = true);

	static bool isInBounds(Coords location);

	void rebuild();

private:
	PolyVox::SimpleVolume<CompositeBlock> _blockMap;
	Coords _chunkLocation;

	//OSG Render data
	osg::ref_ptr<osg::Group> _parentNode;
	osg::ref_ptr<osg::PositionAttitudeTransform> _baseNode;
};

