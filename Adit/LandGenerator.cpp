#include "LandGenerator.h"

#include "Chunk.h"
#include "BlockGrid.h"
#include "ChunkManager.h"

#include <PolyVoxCore\Density.h>

LandGenerator::LandGenerator(int seed)
{
	noiseModule.SetSeed(seed);
}


LandGenerator::~LandGenerator()
{
}


void LandGenerator::fillVolume(const PolyVox::ConstVolumeProxy<CompositeBlock::blockDataType>& volume, const PolyVox::Region& reg)
{
	const float sizeMod = 80.0f;
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

	for (int x = reg.getLowerCorner().getX(); x <= reg.getUpperCorner().getX(); x++)
	{
		for (int y = reg.getLowerCorner().getY(); y <= reg.getUpperCorner().getY(); y++)
		{
			float perlinVal = (float)BlockGrid::gridHeight/2+heightMap.GetValue(x - reg.getLowerCorner().getX(), y - reg.getLowerCorner().getY())*(float)BlockGrid::gridHeight /4;
			for (int z = reg.getLowerCorner().getZ(); z <= reg.getUpperCorner().getZ(); z++)
			{
				CompositeBlock::blockDataType voxel;
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

				volume.setVoxelAt(x, y, z, voxel);
			}
		}
	}
}

