#include "LandGenerator.h"

#include "Chunk.h"
#include "BlockGrid.h"
#include "ChunkManager.h"

#include <PolyVox\Density.h>

LandGenerator::LandGenerator(int seed)
{
	noiseModule.SetSeed(seed);
}


LandGenerator::~LandGenerator()
{
}


void LandGenerator::fillVolume(PolyVox::PagedVolume<CompositeBlock::blockDataType>::Chunk* volume, const PolyVox::Region& reg)
{
	const float sizeMod = 200.0f;
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;

	heightMapBuilder.SetSourceModule(noiseModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(reg.getWidthInVoxels(), reg.getDepthInVoxels());
	heightMapBuilder.SetBounds(
		((float)reg.getLowerCorner().getX())/ sizeMod,
		((float)reg.getUpperCorner().getX()) / sizeMod,
		((float)reg.getLowerCorner().getY()) / sizeMod,
		((float)reg.getUpperCorner().getY()) / sizeMod);
	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.Render();

	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("tutorial.bmp");
	writer.WriteDestFile();

	for (int x = 0; x < reg.getWidthInVoxels(); x++)
	{
		for (int y = 0; y < reg.getDepthInVoxels(); y++)
		{
			float perlinVal = (float)BlockGrid::gridHeight/2+heightMap.GetValue(x, y)*(float)BlockGrid::gridHeight /4;
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
				else
				{
					voxel = BlockType::BlockType_Default;
				}

				volume->setVoxel(x, y, chunkZ, voxel);
			}
		}
	}
}

