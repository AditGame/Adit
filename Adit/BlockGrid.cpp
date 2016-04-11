#include "BlockGrid.h"

#include "CompositeBlock.h"

#include "VolumePager.h"
#include "LandGenerator.h"

const int BlockGrid::gridHeight = 255;

BlockGrid::BlockGrid(osg::Group* rootNode) : _parentNode(rootNode)
{
	_baseNode = new osg::Group;
	_parentNode->addChild(_baseNode);
	chunkManager = new ChunkManager(this);
	chunkManager->setCenterChunk(Coords(0, 0, gridHeight / 2 / Chunk::chunkHeight));
	LandGenerator* gen = new LandGenerator();
	VolumePager* pager = new VolumePager(gen);
	_blockmap = new blockMap_type(pager);
}

BlockGrid::~BlockGrid()
{
	_parentNode->removeChild(_baseNode);
}

CompositeBlock::blockDataType BlockGrid::getBlock(Coords location)
{
	return _blockmap->getVoxel(location);
}

void BlockGrid::update()
{
	chunkManager->updateChunks();
}

