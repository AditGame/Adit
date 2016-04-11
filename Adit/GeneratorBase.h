#pragma once

#include <PolyVox\PagedVolume.h>

#include "CompositeBlock.h"

class GeneratorBase
{
public:
	GeneratorBase();
	~GeneratorBase();

	virtual void fillVolume(PolyVox::PagedVolume<CompositeBlock::blockDataType>::Chunk* volume, const PolyVox::Region& reg) = 0;
};

