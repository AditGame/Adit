#include "ChunkManager.h"

#include <list>
#include <algorithm>
#include <string>
#include <iostream>

#include "BlockGrid.h"


ChunkManager::ChunkManager(BlockGrid* container):_gridContainer(container)
{
	_visibility = 4;
	for (int x = -_visibility; x <= _visibility; x++)
	{
		for (int y = -_visibility; y <= _visibility; y++)
		{
			for (int z = -_visibility; z <= _visibility; z++)
			{
				Coords coords(x,y,z);
				chunkLoader.requestLoadChunk(coords);
			}
		}
	}
}


ChunkManager::~ChunkManager()
{
}

void ChunkManager::updateChunks()
{
	Chunk* loadedChunk = chunkLoader.getLoadedChunk();
	while (loadedChunk != nullptr)
	{
		loadedChunk->attachToGrid(_gridContainer->getBaseNode());
		_chunkMap.emplace(loadedChunk->getlocation(), loadedChunk);
		std::cout << "Chunk: " << loadedChunk->getlocation() << std::endl;
		loadedChunk = chunkLoader.getLoadedChunk();
	}
}

Chunk& ChunkManager::getChunk(Coords coords)
{
	chunkMap_iterator it = _chunkMap.find(coords);
	if (it != _chunkMap.end())
		return *(it->second);
	else
		return Chunk(coords, _gridContainer->getBaseNode());
	//TODO: Better "no chunk" logic
}

void ChunkManager::setCenterChunk(Coords center)
{
	return; //(ignore for now)
	//Delete out-of-range chunks
	for (chunkMap_iterator it = _chunkMap.begin(); it != _chunkMap.end(); it++)
	{
		if (it->first.getX() > center.getX() + _visibility && it->first.getX() < center.getX() - _visibility)
		{
			_chunkMap.erase(it);
		}
	}

	//load missing chunks
	std::list<Coords> chunksToLoad;
	for (int x = center.getX() - _visibility; x <= center.getX() + _visibility; x++)
	{
		for (int y = center.getY() - _visibility; y <= center.getY() + _visibility; y++)
		{
			Coords coords(x, y);
			chunkMap_iterator it = _chunkMap.find(coords);
			if (it==_chunkMap.end())
			{
				chunksToLoad.push_back(coords);
			}
		}
	}

	//send missing chunks to ChunkLoaderManager
}
