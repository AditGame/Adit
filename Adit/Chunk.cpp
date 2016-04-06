#include "Chunk.h"

#include <stdexcept>
#include <cstdlib>
#include <sstream>

#include "OSGRenderer.h"
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>

#include <PolyVoxCore\Vector.h>

const int Chunk::chunkHeight = 8;
const int Chunk::chunkWidth = 8;

Chunk::Chunk(Coords chunkLocation, osg::Group* gridNode) : _parentNode(gridNode), _chunkLocation(chunkLocation), _baseNode(new osg::PositionAttitudeTransform), _blockMap(PolyVox::Region(PolyVox::Vector3DInt32(0, 0, 0), PolyVox::Vector3DInt32(chunkWidth, chunkWidth, chunkHeight)))
{
	if (gridNode != nullptr)
		attachToGrid(gridNode);

	
	LandGenerator gen = LandGenerator();
	gen.fillChunk(this);

	osg::Geode* shapeGeode = OSGRenderer::combineChunk(this);

	_baseNode->addChild(shapeGeode);
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
	osg::Vec3d position(_chunkLocation.getX()*chunkWidth*OSGRenderer::BLOCK_WIDTH, _chunkLocation.getY()*chunkWidth*OSGRenderer::BLOCK_WIDTH, _chunkLocation.getZ()*chunkHeight*OSGRenderer::BLOCK_WIDTH);
	_baseNode->setPosition(position);
}

CompositeBlock& Chunk::getBlock(Coords location, bool relativeToChunk)
{
	if (!relativeToChunk)
		location -= _chunkLocation * chunkWidth;
	if (isInBounds(location))
	{
		
		return _blockMap.getVoxelAt(location.getX(), location.getY(), location.getZ());
	}
	else
	{
		std::stringstream ss;
		ss << "Attempted to access block at coords " << location.getX() << "," << location.getY() << "," << location.getZ();
		throw std::out_of_range(ss.str());
	}
}

bool Chunk::isInBounds(Coords location)
{
	return (location.getX() < chunkWidth && location.getY() < chunkWidth && location.getZ() < chunkHeight && location.getX() >= 0 && location.getY() >= 0 && location.getZ() >= 0);
}

void Chunk::rebuild()
{

}
