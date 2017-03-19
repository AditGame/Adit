#include "ChunkLoader.h"

#include <algorithm>

#include <windows.h>

#include "Chunk.h"

#include <iostream>

#include "Engine/GameEngine.h"

void ChunkLoader::operator()()
{
	loop();
}

ChunkLoader::ChunkLoader()
{
	stopFlag = true;
}


ChunkLoader::~ChunkLoader()
{
	stop();
}

void ChunkLoader::start()
{
	if (isRunning())
	{
		stopFlag = false;
		for (int i = 0; i < 3; i++)
		{
			loadThreads.push_back(new std::thread([=] { loop(); }));
		}
	}
}

void ChunkLoader::stop()
{
	stopFlag = true;
	while (!loadThreads.empty())
	{
		loadThreads.front()->join();
		loadThreads.pop_front();
	}
	std::cout << "chunkloading done" << std::endl;
}

void ChunkLoader::requestLoadChunk(Coords location, bool front)
{
	toLoadMutex.lock();
	if (std::find(toLoad.begin(), toLoad.end(), location) == toLoad.end())
	{
		if (front)
		{
			toLoad.push_front(location);
		}
		else
		{
			toLoad.push_back(location);
		}
	}
	toLoadMutex.unlock();
}

void ChunkLoader::waitUntilEmpty()
{
	while (true)
	{
		toLoadMutex.lock();
		if (toLoad.empty())
		{
			toLoadMutex.unlock();
			return;
		}
		toLoadMutex.unlock();
		Sleep(500);
	}
}

Chunk * ChunkLoader::getLoadedChunk()
{
	std::lock_guard<std::mutex> mutexlock(loadedMutex);
	if (loaded.empty())
		return nullptr;
	else
	{
		Chunk* returnChunk = loaded.front();
		loaded.pop_front();
		return returnChunk;
	}
}

void ChunkLoader::removeChunk(Coords location)
{
}

void ChunkLoader::loop()
{
	stopFlag = false;
	Sleep(10); //give the rest of the program a wee bit to set up
	while (!stopFlag)
	{
		toLoadMutex.lock();
		if (toLoad.size() == 0)
		{
			toLoadMutex.unlock();
			std::this_thread::yield(); //yield to prevent spinlocking
		}
		else
		{
			//Retrieve oldest load request
			Coords coords = toLoad.front();
			toLoad.pop_front();
			toLoadMutex.unlock();

			Chunk* chunk = new Chunk(coords);
			chunk->rebuild(GameEngine::inst().getGrid());

			loadedMutex.lock();
			loaded.push_back(chunk);
			loadedMutex.unlock();
		}
	}
}