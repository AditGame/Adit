#include "BlockGrid.h"

BlockGrid::BlockGrid(osg::Group* rootNode) : _parentNode(rootNode)
{
	_baseNode = new osg::Group;
	_parentNode->addChild(_baseNode);
	chunkManager = new ChunkManager(this);
}

BlockGrid::~BlockGrid()
{
	_parentNode->removeChild(_baseNode);
}

CompositeBlock & BlockGrid::getBlock(Coords location)
{
	int chunkX = location.getX() / Chunk::chunkWidth;
	int chunkY = location.getY() / Chunk::chunkWidth;
	int blockX = location.getX() % Chunk::chunkWidth;
	int blockY = location.getY() % Chunk::chunkWidth;
	return chunkManager->getChunk(Coords(chunkX, chunkY)).getBlock(Coords(blockX, blockY));
}

void BlockGrid::update()
{
	chunkManager->updateChunks();
}

