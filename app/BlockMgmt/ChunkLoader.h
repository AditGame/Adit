#pragma once

#include "Utilities/Coords.h"
#include <queue>
#include <thread>
#include <mutex>
#include <list>

class Chunk;

class ChunkLoader
{
public:
	ChunkLoader();
	~ChunkLoader();

	void start();

	void stop();

	void requestLoadChunk(Coords location, bool front=false);
	Chunk* getLoadedChunk();

	void removeChunk(Coords location);

	void stopThread() { stopFlag = true; }

	bool isRunning() { return stopFlag; }

	void waitUntilEmpty();

	void operator()();

	ChunkLoader(const ChunkLoader& origin) {} // add this line

	void loop();
	void priorityLoop();
private:
	std::list<Coords> toLoad;
	std::list<Chunk*> loaded;
	std::mutex toLoadMutex;
	std::mutex loadedMutex;
	bool stopFlag;

	std::list<std::thread*> loadThreads;
};
