#pragma once

#include "ThreadedVolume.h"

#include "Blocks/CompositeBlock.h"

#include "Generation/GeneratorBase.h"

class VolumePager : public PolyVox::ThreadedVolume<CompositeBlock::blockDataType>::Pager
{
public:
	VolumePager(GeneratorBase* generator) : _gen(generator) {
		_saveFolderName = "saves";
	}
	~VolumePager();

	virtual void pageIn(const PolyVox::Region& region, PolyVox::ThreadedVolume<CompositeBlock::blockDataType>::Chunk* pChunk);
	virtual void pageOut(const PolyVox::Region& region, PolyVox::ThreadedVolume<CompositeBlock::blockDataType>::Chunk* pChunk);

private:
	GeneratorBase* _gen;

	std::string _saveFolderName;
};

