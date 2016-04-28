#pragma once

#include <noise/noise.h>
#include "noiseutils.h"


#include "CompositeBlock.h"
#include "GeneratorBase.h"

#include "WorldGenerator.h"

class Chunk;

class LandGenerator : public GeneratorBase
{
public:
	LandGenerator(int seed = 0);
	~LandGenerator();

	void fillVolume(PolyVox::ThreadedVolume<CompositeBlock::blockDataType>::Chunk* volume, const PolyVox::Region& reg);

private:
	WorldGenerator _worldGen;
	module::Perlin noiseModule;
};

