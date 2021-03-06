#pragma once

#include "ThreadedVolume.h"

#include "Blocks/CompositeBlock.h"

class GeneratorBase
{
public:
	GeneratorBase();
	~GeneratorBase();

	virtual void fillVolume(PolyVox::ThreadedVolume<CompositeBlock::blockDataType>::Chunk* volume, const PolyVox::Region& reg) = 0;
};

