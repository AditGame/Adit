#pragma once

#include "Coords.h"
#include <queue>
#include <thread>
#include <mutex>
#include <list>

class Chunk;

class ChunkLoaderThread
{
public:
	ChunkLoaderThread();
	~ChunkLoaderThread();

	void requestLoadChunk(Coords location);
	Chunk* getLoadedChunk();

	void stopThread() { stopFlag = true; }

	void operator()();

	ChunkLoaderThread(const ChunkLoaderThread& origin) {} // add this line

	void loop();
private:
	std::queue<Coords> toLoad;
	std::queue<Chunk*> loaded;
	std::mutex toLoadMutex;
	std::mutex loadedMutex;
	bool stopFlag;

	std::list<std::thread*> loadThreads;
};
