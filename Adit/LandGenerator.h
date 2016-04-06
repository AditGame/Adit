#pragma once

#include <noise/noise.h>
#include "noiseutils.h"

class Chunk;

class LandGenerator
{
public:
	LandGenerator(int seed = 0);
	~LandGenerator();

	void fillChunk(Chunk* chunk);

private:
	module::Perlin noiseModule;
};

