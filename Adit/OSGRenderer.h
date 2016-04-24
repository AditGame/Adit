#pragma once

#include "BlockGrid.h"

#include <osg/Material>
#include <osg/Geometry>

#include <PolyVox/Mesh.h>
#include <PolyVox\CubicSurfaceExtractor.h>

class OSGRenderer
{
public:
	OSGRenderer();
	~OSGRenderer();

	static void setUp();

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

	static osg::Geode* meshToGeode(PolyVox::Mesh<PolyVox::CubicVertex<CompositeBlock::blockDataType> > &mesh);

	static const float BLOCK_WIDTH;

	static bool OSGRenderer::loadShaderSource(osg::Shader* obj, const std::string& fileName);

private:

	static osg::Material* _blockMaterial;
	static osg::Program* _blockProgram;
};

