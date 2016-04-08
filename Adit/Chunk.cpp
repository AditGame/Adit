#include "Chunk.h"

#include <stdexcept>
#include <cstdlib>
#include <sstream>

#include "OSGRenderer.h"
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>

#include <PolyVoxCore\Vector.h>
#include <PolyVoxCore\CubicSurfaceExtractorWithNormals.h>

#include "BlockGrid.h"

const int Chunk::chunkHeight = BlockGrid::gridHeight;
const int Chunk::chunkWidth = 8;

Chunk::Chunk(Coords chunkLocation, osg::Group* gridNode) : _parentNode(gridNode), _chunkLocation(chunkLocation), _cubeMeshNode(nullptr), _baseNode(new osg::PositionAttitudeTransform)
{
	if (gridNode != nullptr)
		attachToGrid(gridNode);
}

Chunk::~Chunk()
{
	if(_parentNode!=nullptr)
		_parentNode->removeChild(_baseNode);
}

void Chunk::attachToGrid(osg::Group * gridNode)
{
	_parentNode = gridNode;
	_parentNode->addChild(_baseNode);
	osg::Vec3d position(_chunkLocation.x()*chunkWidth*OSGRenderer::BLOCK_WIDTH, _chunkLocation.y()*chunkWidth*OSGRenderer::BLOCK_WIDTH, _chunkLocation.z()*chunkHeight*OSGRenderer::BLOCK_WIDTH);
	_baseNode->setPosition(position);
}

bool Chunk::isInBounds(Coords location)
{
	return (location.x() < chunkWidth && location.y() < chunkWidth && location.z() < chunkHeight && location.x() >= 0 && location.y() >= 0 && location.z() >= 0);
}

void Chunk::rebuild(BlockGrid* grid)
{
	int z = _chunkLocation.z()*chunkHeight + chunkHeight;
	if (z > 255) z = 255;
	PolyVox::Region reg(
		_chunkLocation.x()*chunkWidth,				_chunkLocation.y()*chunkWidth,				_chunkLocation.z()*chunkHeight,
		_chunkLocation.x()*chunkWidth + chunkWidth, _chunkLocation.y()*chunkWidth + chunkWidth, z);

	using namespace PolyVox;
	std::cout << "Rendering region: " << reg.getLowerCorner() << " -> " << reg.getUpperCorner() << std::endl;
	SurfaceMesh<PositionMaterialNormal> mesh;
	CubicSurfaceExtractorWithNormals<BlockGrid::blockMap_type> surfaceExtractor(grid->getBlockMap(), reg, &mesh);

	surfaceExtractor.execute();
	if (_cubeMeshNode != nullptr) 
		_baseNode->removeChild(_cubeMeshNode);
	_cubeMeshNode = OSGRenderer::meshToGeode(mesh);
	_baseNode->addChild(_cubeMeshNode);

}
