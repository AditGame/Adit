#pragma once

#include "ChunkManager.h"
#include "CompositeBlock.h"

#include <PolyVoxCore\LargeVolume.h>

class BlockGrid
{
public:
	BlockGrid(osg::Group* rootNode);
	~BlockGrid();

	static const int gridHeight;

	typedef PolyVox::LargeVolume<CompositeBlock::blockDataType> blockMap_type;

	ChunkManager* chunkManager;
	CompositeBlock::blockDataType getBlock(Coords location);

	void update();

	osg::ref_ptr<osg::Group> getBaseNode() { return _baseNode; }

	blockMap_type* getBlockMap() { return &_blockmap; }

private:
	//Chunk Data
	blockMap_type _blockmap;

	//OSG Render data
	osg::ref_ptr<osg::Group> _parentNode;
	osg::ref_ptr<osg::Group> _baseNode;
};

