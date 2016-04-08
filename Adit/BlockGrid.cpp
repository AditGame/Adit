#include "BlockGrid.h"

const int BlockGrid::gridHeight = 255;

BlockGrid::BlockGrid(osg::Group* rootNode) : _parentNode(rootNode), _blockmap(&ChunkManager::loadRegion, &ChunkManager::unloadRegion, gridHeight + 1)
{
	_baseNode = new osg::Group;
	_parentNode->addChild(_baseNode);
	chunkManager = new ChunkManager(this);
	chunkManager->setCenterChunk(Coords(0, 0, gridHeight / 2 / Chunk::chunkHeight));
}

BlockGrid::~BlockGrid()
{
	_parentNode->removeChild(_baseNode);
}

CompositeBlock::blockDataType BlockGrid::getBlock(Coords location)
{
	return _blockmap.getVoxelAt(location);
}

void BlockGrid::update()
{
	chunkManager->updateChunks();
}

