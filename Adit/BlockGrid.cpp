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
	_blockmap->flushAll();
	delete _blockmap;
}

CompositeBlock::blockDataType BlockGrid::getBlock(Coords location)
{
	return _blockmap->getVoxel(location);
}

void BlockGrid::setBlock(PolyVox::Vector3DInt32 location, CompositeBlock::blockDataType block)
{
	_blockmap->setVoxel(location, block);
	Coords chunk = chunkManager->blockToChunkCoords(Coords(location.getX(), location.getY(), location.getZ()));
	chunkManager->setDirty(chunk);
}

void BlockGrid::update()
{
	chunkManager->updateChunks();
}

