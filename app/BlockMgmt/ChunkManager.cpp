#include "ChunkManager.h"

#include <list>
#include <algorithm>
#include <string>
#include <iostream>

#include <osg/Geode>

#include "BlockGrid.h"
#include "Render/OSGRenderer.h"


ChunkManager::ChunkManager(BlockGrid* container):_gridContainer(container), _center(-100,-100,-100)
{
	_visibility = 4;
}


ChunkManager::~ChunkManager()
{
}

void ChunkManager::setVisibility(int vis)
{
	_visibility = vis;
	setCenterChunk(_center, true);
}

void ChunkManager::setDirty(Coords coords, bool front)
{
	chunkMap_type::iterator it = _chunkMap.find(coords);

	//exit if chunk isn't loaded (it's rebuilt anyways when loaded)
	if (it == _chunkMap.end())
		return;

	//delete it->second;
	//_chunkMap.erase(it);

	chunkLoader.requestLoadChunk(coords, front);
}

void ChunkManager::updateChunks()
{
	Chunk* chunk = chunkLoader.getLoadedChunk();
	if (chunk != nullptr && chunk->getlocation().dist_squared_2D(_center) < _visibility * _visibility)
	{
		chunkMap_type::iterator it = _chunkMap.find(chunk->getlocation());
		if (it != _chunkMap.end())
		{
			delete it->second;
			_chunkMap.erase(it);
		}

		_chunkMap.emplace(chunk->getlocation(), chunk);
		chunk->attachToGrid(_gridContainer->getBaseNode());
	}
}

Chunk& ChunkManager::getChunk(Coords coords)
{
	return Chunk(coords, _gridContainer->getBaseNode());
}

void ChunkManager::rebuildChunks()
{
	for (chunkMap_type::iterator it = _chunkMap.begin(); it != _chunkMap.end(); it++)
	{
		delete it->second;
	}
	_chunkMap.clear();

	setCenterChunk(_center, true);
}

Coords ChunkManager::blockToChunkCoords(Coords in)
{
	int newX = std::floor((float)in.x() / (float)Chunk::chunkWidth);
	int newY = std::floor((float)in.y() / (float)Chunk::chunkWidth);
	int newZ = std::floor((float)in.z() / (float)Chunk::chunkHeight);
	Coords result = Coords(newX, newY, newZ);
	return result;
}

void ChunkManager::setCenterChunk(Coords center, bool force)
{
	if (center == _center && !force) return;
	_center = center;
	//Delete out-of-range chunks
	for (chunkMap_type::iterator it = _chunkMap.begin(); it != _chunkMap.end(); /* No increment */)
	{
		if (it->first.dist_squared_2D(center) > _visibility * _visibility)
		{
			delete it->second;
			it = _chunkMap.erase(it); //returns next element
		}
		else
		{
			it++;
		}
	}

	//load missing chunks
	for (int x = center.x() - _visibility; x <= center.x() + _visibility; x++)
	{
		for (int y = center.y() - _visibility; y <= center.y() + _visibility; y++)
		{
			for (int z = 0; z <= BlockGrid::gridHeight / Chunk::chunkHeight; z++)
			{
				if (z < 0) z = 0; //ensure we don't go too low
				Coords coords(x, y, z);
				if (coords.dist_squared_2D(center) < _visibility * _visibility)
				{
					Coords coords(x, y, z);
					chunkMap_type::iterator it = _chunkMap.find(coords);
					if (it == _chunkMap.end())
					{
						chunkLoader.requestLoadChunk(coords);
						//_dirtyChunks.push_front(chunk);
					}
				}
			}
		}
	}
}

void ChunkManager::moveCenterChunk(Coords movement)
{
	movement = movement + _center;
	std::cout << _center << "=>" << movement << std::endl;
	setCenterChunk(movement);
}

void ChunkManager::processAllDirty()
{
	chunkLoader.waitUntilEmpty();
	
	Chunk* chunk = chunkLoader.getLoadedChunk();
	while (chunk != nullptr && chunk->getlocation().dist_squared_2D(_center) < _visibility * _visibility)
	{
		_chunkMap.emplace(chunk->getlocation(), chunk);
		chunk->attachToGrid(_gridContainer->getBaseNode());
		chunk = chunkLoader.getLoadedChunk();
	}
}
