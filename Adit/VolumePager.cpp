#include "VolumePager.h"


VolumePager::~VolumePager()
{
}

void VolumePager::pageIn(const PolyVox::Region & region, PolyVox::PagedVolume<CompositeBlock::blockDataType>::Chunk * pChunk)
{
	POLYVOX_ASSERT(pChunk, "Attempting to page in NULL chunk");
	POLYVOX_ASSERT(pChunk->getData(), "Chunk must have valid data");

	std::stringstream ssFilename;
	ssFilename << _saveFolderName << "/"
		<< region.getLowerX() << "_" << region.getLowerY() << "_" << region.getLowerZ() << "_"
		<< region.getUpperX() << "_" << region.getUpperY() << "_" << region.getUpperZ();

	std::string filename = ssFilename.str();

	// FIXME - This should be replaced by C++ style IO, but currently this causes problems with
	// the gameplay-cubiquity integration. See: https://github.com/blackberry/GamePlay/issues/919

	FILE* pFile = nullptr;
	fopen_s(&pFile, filename.c_str(), "rb");
	if (pFile)
	{
		fread(pChunk->getData(), sizeof(CompositeBlock::blockDataType), pChunk->getDataSizeInBytes(), pFile);

		if (ferror(pFile))
		{
			POLYVOX_THROW(std::runtime_error, "Error reading in chunk data, even though a file exists.");
		}

		fclose(pFile);
	}
	else
	{
		_gen->fillVolume(pChunk, region);
	}
}

void VolumePager::pageOut(const PolyVox::Region & region, PolyVox::PagedVolume<CompositeBlock::blockDataType>::Chunk * pChunk)
{
	POLYVOX_ASSERT(pChunk, "Attempting to page out NULL chunk");
	POLYVOX_ASSERT(pChunk->getData(), "Chunk must have valid data");

	POLYVOX_LOG_TRACE("Paging out data for ", region);

	std::stringstream ssFilename;
	ssFilename << _saveFolderName << "/"
		<< region.getLowerX() << "_" << region.getLowerY() << "_" << region.getLowerZ() << "_"
		<< region.getUpperX() << "_" << region.getUpperY() << "_" << region.getUpperZ();

	std::string filename = ssFilename.str();

	// FIXME - This should be replaced by C++ style IO, but currently this causes problems with
	// the gameplay-cubiquity integration. See: https://github.com/blackberry/GamePlay/issues/919

	FILE* pFile = nullptr;
	fopen_s(&pFile, filename.c_str(), "wb");
	if (!pFile)
	{
		POLYVOX_THROW(std::runtime_error, "Unable to open file to write out chunk data.");
	}

	fwrite(pChunk->getData(), sizeof(uint8_t), pChunk->getDataSizeInBytes(), pFile);

	if (ferror(pFile))
	{
		POLYVOX_THROW(std::runtime_error, "Error writing out chunk data.");
	}

	fclose(pFile);
}
