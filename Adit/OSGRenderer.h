#pragma once

#include "BlockGrid.h"

class OSGRenderer
{
public:
	OSGRenderer();
	~OSGRenderer();

	void render(BlockGrid& grid);

	static osg::Geometry* createCube();

	enum FACE_SIDE
	{
		top,
		bottom,
		north,
		south,
		east,
		west
	};
	static void createFace(osg::Geometry* geometry, osg::Vec3d position, OSGRenderer::FACE_SIDE facing);

	static osg::Geometry* cubeGeometry;

	static osg::Geode* combineChunk(Chunk* chunk);

	static const int BLOCK_WIDTH;

	static bool OSGRenderer::loadShaderSource(osg::Shader* obj, const std::string& fileName);

private:
};

