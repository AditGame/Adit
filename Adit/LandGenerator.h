#pragma once

#include <noise/noise.h>
#include "noiseutils.h"


#include "CompositeBlock.h"
#include "GeneratorBase.h"

class Chunk;

class LandGenerator : public GeneratorBase
{
public:
	LandGenerator(int seed = 0);
	~LandGenerator();

	void fillVolume(PolyVox::PagedVolume<CompositeBlock::blockDataType>::Chunk* volume, const PolyVox::Region& reg);

private:
	module::Perlin noiseModule;
};

