#pragma once

#include "ChunkManager.h"
#include "CompositeBlock.h"

#include <PolyVox\PagedVolume.h>

class BlockGrid
{
public:
	BlockGrid(osg::Group* rootNode);
	~BlockGrid();

	static const int gridHeight;

	typedef PolyVox::PagedVolume<CompositeBlock::blockDataType> blockMap_type;

	ChunkManager* chunkManager;
	CompositeBlock::blockDataType getBlock(Coords location);

	void setBlock(PolyVox::Vector3DInt32 location, CompositeBlock::blockDataType block, bool front);

	void update();

	osg::ref_ptr<osg::Group> getBaseNode() { return _baseNode; }

	blockMap_type* getBlockMap() { return _blockmap; }

private:
	//Chunk Data
	blockMap_type* _blockmap;

	//OSG Render data
	osg::ref_ptr<osg::Group> _parentNode;
	osg::ref_ptr<osg::Group> _baseNode;
};

