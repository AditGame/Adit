#include "Chunk.h"

#include <stdexcept>
#include <cstdlib>
#include <sstream>

#include "OSGRenderer.h"
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>

#include <PolyVox\Vector.h>
#include <PolyVox\CubicSurfaceExtractor.h>

#include <osgbCollision\CollisionShapes.h>

#include "BlockGrid.h"

#include "GameEngine.h"
#include "PhysicsEngine.h"

const int Chunk::chunkHeight = 16;
const int Chunk::chunkWidth = 16;

Chunk::Chunk(Coords chunkLocation, osg::Group* gridNode) : 
	_parentNode(gridNode), 
	_chunkLocation(chunkLocation), 
	_cubeMeshNode(nullptr), 
	_baseNode(new osg::PositionAttitudeTransform), 
	_physShape(nullptr),
	_rigidBody(nullptr),
	_motionState(nullptr)
{
	if (gridNode != nullptr)
		attachToGrid(gridNode);

	osg::Vec3d position(_chunkLocation.x()*chunkWidth*OSGRenderer::BLOCK_WIDTH, _chunkLocation.y()*chunkWidth*OSGRenderer::BLOCK_WIDTH, _chunkLocation.z()*chunkHeight*OSGRenderer::BLOCK_WIDTH);
	_baseNode->setPosition(position);
}

Chunk::~Chunk()
{
	if (_parentNode != nullptr && _cubeMeshNode != nullptr)
		_parentNode->removeChild(_baseNode);

	if (_rigidBody != nullptr)
	{
		GameEngine::inst().getPhysics()->getWorld()->removeRigidBody(_rigidBody);
		delete _physShape;
		delete _rigidBody;
		delete _motionState;
	}
}

void Chunk::attachToGrid(osg::Group * gridNode)
{
	_parentNode = gridNode;
	if (_cubeMeshNode != nullptr)
	{
		_parentNode->addChild(_baseNode);
	}
	attachToWorld();
}

void Chunk::attachToWorld()
{
	if(_rigidBody!=nullptr)
		GameEngine::inst().getPhysics()->getWorld()->addRigidBody(_rigidBody);
}

bool Chunk::isInBounds(Coords location)
{
	return (location.x() < chunkWidth && location.y() < chunkWidth && location.z() < chunkHeight && location.x() >= 0 && location.y() >= 0 && location.z() >= 0);
}

bool Chunk::isEmpty(BlockGrid* volume)
{
	for (int x = _chunkLocation.x()*chunkWidth; x < _chunkLocation.x()*chunkWidth + chunkWidth - 1; x++)
	{
		for (int y = _chunkLocation.y()*chunkWidth; y < _chunkLocation.y()*chunkWidth + chunkWidth - 1; y++)
		{
			for (int z = _chunkLocation.z()*chunkHeight; z < _chunkLocation.z()*chunkHeight + chunkHeight - 1; z++)
			{
				if (volume->getBlockMap()->getVoxel(x, y, z) != BlockType_Default)
					return false;
			}
		}
	}
	return true;
}

void Chunk::rebuild(BlockGrid* grid)
{
	int z = _chunkLocation.z()*chunkHeight + chunkHeight;
	if (z > 255) z = 255;
	PolyVox::Region reg(
		_chunkLocation.x()*chunkWidth,				_chunkLocation.y()*chunkWidth,				_chunkLocation.z()*chunkHeight,
		_chunkLocation.x()*chunkWidth + chunkWidth-1, _chunkLocation.y()*chunkWidth + chunkWidth-1, z);

	using namespace PolyVox;
	std::cout << "Rendering region: " << reg.getLowerCorner() << " -> " << reg.getUpperCorner() << std::endl;

	PolyVox::Mesh<PolyVox::CubicVertex<CompositeBlock::blockDataType>> mesh = extractCubicMesh(grid->getBlockMap(), reg);

	// cleanup meshes and whatnot

	//clean up old cube mesh
	if (_cubeMeshNode != nullptr)
	{
		_baseNode->removeChild(_cubeMeshNode);
		_cubeMeshNode = nullptr;
	}

	//clean up old physics mesh
	if (_physShape != nullptr)
	{
		delete _physShape;
		_physShape = nullptr;
	}

	if (_rigidBody != nullptr)
	{
		GameEngine::inst().getPhysics()->getWorld()->removeRigidBody(_rigidBody);
	}

	//exit early if the chunk is empty
	if (isEmpty(grid))
	{
		return;
	}

	_cubeMeshNode = OSGRenderer::meshToGeode(mesh);

	//exit if there's no mesh
	if (_cubeMeshNode == nullptr)
	{
		return;
	}

	if (_parentNode != nullptr)
	{
		attachToGrid(_parentNode);
	}

	//add the mesh to the world
	_baseNode->addChild(_cubeMeshNode);

	//generate a collision shape from the simplified mesh
	_physShape = osgbCollision::btTriMeshCollisionShapeFromOSG(_baseNode);

	//set up the rigid body
	if (_rigidBody == nullptr)
	{
		//_motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(_chunkLocation.x()*Chunk::chunkWidth, _chunkLocation.y()*Chunk::chunkWidth, _chunkLocation.z()*Chunk::chunkHeight)));
		
		_motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0,0,0)));

		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, _motionState, _physShape, btVector3(0, 0, 0));

		_rigidBody = new btRigidBody(groundRigidBodyCI);
	}
	else
	{
		_rigidBody->setCollisionShape(_physShape);
	}
}
