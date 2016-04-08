#pragma once

#include <map>
#include <list>
#include "Coords.h"
#include "Chunk.h"
#include "ChunkLoaderThread.h"
#include "LandGenerator.h"

#include <PolyVoxCore\LargeVolume.h>

class BlockGrid;

class ChunkManager
{
public:
	//Default, does nothing (replace with constructed object)
	//ChunkManager() : _chunkMap(&ChunkManager::loadRegion, {}

	ChunkManager(BlockGrid*);
	~ChunkManager();

	typedef std::map<Coords, Chunk*> chunkMap_type;
	typedef std::list<Chunk*> dirtyChunks_type;

	void updateChunks();

	/// Get a reference to a chunk at the specified coordinates
	/** If a chunk exists but isn't loaded, it will attempt to load the chunk.
	/** If a chunk doesn't exist at all, it will generate the chunk and return it **/
	Chunk& getChunk(Coords coords);
	void rebuildChunks();
	Chunk& getChunk(int x, int y) { return getChunk(Coords(x, y)); }

	/// Gets the current visibility (a cube arround the player's position, visibility being the "radius")
	int getVisibility() { return _visibility; }
	int setVisibility(int i) { _visibility = i; }

	/// Sets the chunk the player is currently in. Will attempt to load the chunks within the player's visibility
	void setCenterChunk(int x, int y) { return setCenterChunk(Coords(x, y)); }
	void setCenterChunk(Coords center, bool force=false);
	void moveCenterChunk(Coords movement);

	static void loadRegion(const PolyVox::ConstVolumeProxy<CompositeBlock::blockDataType>& volume, const PolyVox::Region & reg);

	static void unloadRegion(const PolyVox::ConstVolumeProxy<CompositeBlock::blockDataType>&, const PolyVox::Region & reg);

private:

	ChunkManager(const ChunkManager &v) : _center(Coords(-10,-10,-10)) {}
	ChunkManager& operator=(const ChunkManager &v) {}

	int _visibility;

	chunkMap_type _chunkMap;

	dirtyChunks_type _dirtyChunks;

	BlockGrid* _gridContainer;

	ChunkLoaderThread chunkLoader;

	LandGenerator generator;

	Coords _center;
};
