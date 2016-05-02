#include "BlockGrid.h"

#include "CompositeBlock.h"

#include "VolumePager.h"
#include "LandGenerator.h"

#include "OSGRenderer.h"

const int BlockGrid::gridHeight = 255;

BlockGrid::BlockGrid(osg::Group* rootNode) : _parentNode(rootNode)
{
	_baseNode = new osg::Group;
	_parentNode->addChild(_baseNode);
	chunkManager = new ChunkManager(this);
	chunkManager->setCenterChunk(ChunkManager::blockToChunkCoords(Coords(1000,1000,0)));
	LandGenerator* gen = new LandGenerator();
	VolumePager* pager = new VolumePager(gen);
	_blockmap = new blockMap_type(pager);
}

BlockGrid::~BlockGrid()
{
	delete chunkManager;
	_parentNode->removeChild(_baseNode);
	_blockmap->flushAll();
	delete _blockmap;
}

CompositeBlock::blockDataType BlockGrid::getBlock(Coords location)
{
	return _blockmap->getVoxel(location);
}

void BlockGrid::setBlock(PolyVox::Vector3DInt32 location, CompositeBlock::blockDataType block, bool front)
{
	_blockmap->setVoxel(location, block);
	Coords chunk = chunkManager->blockToChunkCoords(Coords(location.getX(), location.getY(), location.getZ()));
	chunkManager->setDirty(chunk, front);

	//also update surrounding chunks if the block borders them

	// TODO: Better logic to check if the chunk actually needs rebuilding
	//X-
	if (location.getX() % Chunk::chunkWidth == 0)
	{
		Coords chunk = chunkManager->blockToChunkCoords(Coords(location.getX()-1, location.getY(), location.getZ()));
		chunkManager->setDirty(chunk, front);
	}

	//X+
	else if (location.getX() % Chunk::chunkWidth == Chunk::chunkWidth-1)
	{
		Coords chunk = chunkManager->blockToChunkCoords(Coords(location.getX() + 1, location.getY(), location.getZ()));
		chunkManager->setDirty(chunk, front);
	}

	//Y-
	if (location.getY() % Chunk::chunkWidth == 0)
	{
		Coords chunk = chunkManager->blockToChunkCoords(Coords(location.getX(), location.getY() - 1, location.getZ()));
		chunkManager->setDirty(chunk, front);
	}

	//Y+
	else if (location.getY() % Chunk::chunkWidth == Chunk::chunkWidth - 1)
	{
		Coords chunk = chunkManager->blockToChunkCoords(Coords(location.getX(), location.getY() + 1, location.getZ()));
		chunkManager->setDirty(chunk,front);
	}

	//Z-
	if (location.getZ() % Chunk::chunkHeight == 0)
	{
		Coords chunk = chunkManager->blockToChunkCoords(Coords(location.getX(), location.getY(), location.getZ() - 1));
		chunkManager->setDirty(chunk, front);
	}

	//Z+
	else if (location.getZ() % Chunk::chunkHeight == Chunk::chunkHeight - 1)
	{
		Coords chunk = chunkManager->blockToChunkCoords(Coords(location.getX(), location.getY(), location.getZ() + 1));
		chunkManager->setDirty(chunk, front);
	}
}

void BlockGrid::update()
{
	chunkManager->updateChunks();
}

