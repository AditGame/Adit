#pragma once

#include <noise/noise.h>
#include "noiseutils.h"

#include <PolyVoxCore\ConstVolumeProxy.h>

#include "CompositeBlock.h"

class Chunk;

class LandGenerator
{
public:
	LandGenerator(int seed = 0);
	~LandGenerator();

	void fillChunk(Chunk* chunk);

	void fillVolume(const PolyVox::ConstVolumeProxy<CompositeBlock::blockDataType>& volume, const PolyVox::Region& reg);

private:
	module::Perlin noiseModule;
};

