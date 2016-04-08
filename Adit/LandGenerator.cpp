#include "LandGenerator.h"

#include "Chunk.h"
#include "ChunkManager.h"

LandGenerator::LandGenerator(int seed)
{
	noiseModule.SetSeed(seed);
}


LandGenerator::~LandGenerator()
{
}

void LandGenerator::fillChunk(Chunk * chunk)
{
	
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;

	heightMapBuilder.SetSourceModule(noiseModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(Chunk::chunkWidth, Chunk::chunkWidth);
	heightMapBuilder.SetBounds(chunk->getlocation().getX(), chunk->getlocation().getX()+1, chunk->getlocation().getY(), chunk->getlocation().getY()+1);
	heightMapBuilder.Build();

	//utils::RendererImage renderer;
	//utils::Image image;
	//renderer.SetSourceNoiseMap(heightMap);
	//renderer.SetDestImage(image);
	//renderer.Render();

	//utils::WriterBMP writer;
	//writer.SetSourceImage(image);
	//writer.SetDestFilename("tutorial.bmp");
	//writer.WriteDestFile();


	for (int x = 0; x < Chunk::chunkWidth; x++)
	{
		for (int y = 0; y < Chunk::chunkWidth; y++)
		{
			int noiseValue = ((heightMap.GetValue(x, y) + 1.0f) / 2.0f)*(float)Chunk::chunkHeight;
			int initZ = chunk->getlocation().getZ() * Chunk::chunkHeight;
			int z = initZ;
			for (; z <= noiseValue && z < initZ+Chunk::chunkHeight; z++)
			{
				Coords coords(x, y, z);

				chunk->_blockMap.setVoxelAt(PolyVox::Vector3DInt32(x, y, z - initZ), CompositeBlock(chunk->_baseNode, coords, 1));
			}
			for (; z < initZ+Chunk::chunkHeight; z++)
			{
				Coords coords(x, y, z);

				chunk->_blockMap.setVoxelAt(PolyVox::Vector3DInt32(x, y, z - initZ), CompositeBlock(chunk->_baseNode, coords, 0));
			}
		}
	}

}

void LandGenerator::fillVolume(const PolyVox::ConstVolumeProxy<CompositeBlock::blockDataType>& volume, const PolyVox::Region& reg)
{
	const float sizeMod = 80;
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
			float perlinVal = (float)ChunkManager::mapHeight/4+((heightMap.GetValue(x, y) + 1.0f) / 2.0f)*(float)ChunkManager::mapHeight/4;
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

