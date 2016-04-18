#include "ChunkManager.h"

#include <list>
#include <algorithm>
#include <string>
#include <iostream>

#include <osg/Geode>

#include "BlockGrid.h"
#include "OSGRenderer.h"


ChunkManager::ChunkManager(BlockGrid* container):_gridContainer(container), _center(-100,-100,-100)
{
	_visibility = 2;
}


ChunkManager::~ChunkManager()
{
}

void ChunkManager::setVisibility(int vis)
{
	_visibility = vis;
	setCenterChunk(_center, true);
}

void ChunkManager::updateChunks()
{

	//rebuild one dirty chunk at a time
	if (_dirtyChunks.size() > 0)
	{
		_dirtyChunks.back()->rebuild(_gridContainer);
		_dirtyChunks.pop_back();
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

	_dirtyChunks.clear();
	setCenterChunk(_center, true);
}

Coords ChunkManager::blockToChunkCoords(Coords in)
{
	return Coords(in.x()/Chunk::chunkWidth, in.y()/Chunk::chunkWidth, in.z()/Chunk::chunkHeight);
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
			dirtyChunks_type::iterator find = std::find(_dirtyChunks.begin(), _dirtyChunks.end(), it->second);
			if (find != _dirtyChunks.end())
			{
				_dirtyChunks.erase(find);
			}
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
						Chunk* chunk = new Chunk(coords, _gridContainer->getBaseNode());
						_chunkMap.emplace(coords, chunk);
						_dirtyChunks.push_front(chunk);
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
	//rebuild one dirty chunk at a time
	while (_dirtyChunks.size() > 0)
	{
		_dirtyChunks.back()->rebuild(_gridContainer);
		_dirtyChunks.pop_back();
	}
}
