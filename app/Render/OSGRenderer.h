#pragma once


#include <osg/Material>
#include <osg/Geometry>

#include <PolyVox/Mesh.h>
#include <PolyVox\CubicSurfaceExtractor.h>

#include "Blocks\CompositeBlock.h"

class OSGRenderer
{
public:
	OSGRenderer();
	~OSGRenderer();

	static void setUp();

	static osg::Geode* meshToGeode(PolyVox::Mesh<PolyVox::CubicVertex<CompositeBlock::blockDataType> > &mesh);

	static const float BLOCK_WIDTH;

	static bool OSGRenderer::loadShaderSource(osg::Shader* obj, const std::string& fileName);

private:

	static osg::Material* _blockMaterial;
	static osg::Program* _blockProgram;
};

