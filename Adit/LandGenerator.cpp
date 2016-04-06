#include "LandGenerator.h"

#include "Chunk.h"


LandGenerator::LandGenerator(int seed)
{
	//noiseModule.SetSeed(seed);
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

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.Render();

	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("tutorial.bmp");
	writer.WriteDestFile();


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

