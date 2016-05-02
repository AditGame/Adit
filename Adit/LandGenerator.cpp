#include "LandGenerator.h"

#include "Chunk.h"
#include "BlockGrid.h"
#include "ChunkManager.h"

#include <PolyVox\Density.h>

LandGenerator::LandGenerator(int seed) :
	_worldGen(seed)
{
	noiseModule.SetSeed(seed);
}


LandGenerator::~LandGenerator()
{
}


void LandGenerator::fillVolume(PolyVox::ThreadedVolume<CompositeBlock::blockDataType>::Chunk* volume, const PolyVox::Region& reg)
{
	const float sizeMod = 200000.0f;

	utils::NoiseMap heightMap = _worldGen.generateChunk(reg.getLowerCorner().getX(),
		(float)reg.getUpperCorner().getX(),
		(float)reg.getLowerCorner().getY(),
		(float)reg.getUpperCorner().getY(),
		sizeMod);

	for (int x = 0; x < reg.getWidthInVoxels(); x++)
	{
		for (int y = 0; y < reg.getDepthInVoxels(); y++)
		{
			float perlinVal = (float)BlockGrid::gridHeight/2+(heightMap.GetValue(x, y)/ 8192.0)*(float)BlockGrid::gridHeight /2;
			for (int chunkZ = 0; chunkZ < reg.getHeightInVoxels(); chunkZ++)
			{
				CompositeBlock::blockDataType voxel;
				int z = chunkZ + reg.getLowerZ();
				if (z < perlinVal - 3)
				{
					voxel = BlockType::BlockType_Stone;
				}
				else if (z < perlinVal - 1)
				{
					voxel = BlockType::BlockType_Dirt;
				}
				else if (z < perlinVal)
				{
					voxel = BlockType::BlockType_Grass;
				}
				else if (z < (float)BlockGrid::gridHeight / 2)
				{
					voxel = BlockType::BlockType_Water;
				}
				else
				{
					voxel = BlockType::BlockType_Default;
				}

				volume->setVoxel(x, y, chunkZ, voxel);
			}
		}
	}
}

