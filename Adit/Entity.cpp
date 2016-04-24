#include "Entity.h"

#include "OSGRenderer.h"
#include "GameEngine.h"
#include "PhysicsEngine.h"
#include "BlockGrid.h"
#include "SyncedMotionState.h"

Entity::Entity(osg::Group* parent, osg::Vec3d initPosition) : _parentNode(parent), _baseNode(new osg::PositionAttitudeTransform()), _rotation(0, 0, 0), _rigidBody(nullptr)
{
	if (_parentNode != nullptr)
		attach(_parentNode);

	_baseNode->setPosition(initPosition);
}


Entity::~Entity()
{
	//detach the node from the parent
	attach(nullptr);
}

void Entity::setPosition(const osg::Vec3d& newPos, bool movePhysics)
{

	if (movePhysics && _motionState!=nullptr)
	{
		//throw "Moving physics not not currently implemented";
	}
	else
	{
		//Translate to world coordinates
		//osg::Vec3d position(newPos.x()*(float)OSGRenderer::BLOCK_WIDTH, newPos.y()*(float)OSGRenderer::BLOCK_WIDTH, newPos.z()*(float)OSGRenderer::BLOCK_WIDTH);
		_baseNode->setPosition(newPos);

		Coords blockLocation = Coords(newPos.x() / OSGRenderer::BLOCK_WIDTH, newPos.y() / OSGRenderer::BLOCK_WIDTH);

		Coords newChunk = ChunkManager::blockToChunkCoords(blockLocation);

		GameEngine::inst().getGrid()->chunkManager->setCenterChunk(newChunk);
	}
}

void Entity::movePosition(const osg::Vec3d& pos, bool movePhysics)
{
	setPosition(getPosition() + pos, movePhysics);
}

void Entity::setRotation(osg::Vec3f newRot)
{
	_rotation = newRot;

	osg::Quat bottomQuat;
	const osg::Vec3d headingAxis(0.0, 0.0, 1.0);
	const osg::Vec3d pitchAxis(1.0, 0.0, 0.0);
	const osg::Vec3d rollAxis(0.0, 1.0, 0.0);

	double rotation[3] = { _rotation.x(), _rotation.y(), _rotation.z() };

	bottomQuat.makeRotate(
		rotation[1] + 1.5708f, pitchAxis,
		rotation[2], rollAxis,
		-rotation[0], headingAxis);

	_baseNode->setAttitude(bottomQuat);
}

void Entity::modRotation(osg::Vec3f modRot)
{
	setRotation(modRot + _rotation);
}

void Entity::attach(osg::Group * root)
{
	if (_parentNode != nullptr)
		_parentNode->removeChild(_baseNode);
	
	_parentNode = root;

	if(_parentNode != nullptr)
		_parentNode->addChild(_baseNode);
}