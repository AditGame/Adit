#pragma once

#include <PolyVox\PagedVolume.h>

#include "CompositeBlock.h"

#include "GeneratorBase.h"

class VolumePager : public PolyVox::PagedVolume<CompositeBlock::blockDataType>::Pager
{
public:
	VolumePager(GeneratorBase* generator) : _gen(generator) {
		_saveFolderName = "saves";
	}
	~VolumePager();

	virtual void pageIn(const PolyVox::Region& region, PolyVox::PagedVolume<CompositeBlock::blockDataType>::Chunk* pChunk);
	virtual void pageOut(const PolyVox::Region& region, PolyVox::PagedVolume<CompositeBlock::blockDataType>::Chunk* pChunk);

private:
	GeneratorBase* _gen;

	std::string _saveFolderName;
};

