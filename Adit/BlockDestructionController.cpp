#include "BlockDestructionController.h"

#include "GameEngine.h"
#include "BlockGrid.h"
#include "PhysicsEngine.h"
#include "OSGRenderer.h"

#include "Player.h"

#include "osgbCollision\Utils.h"

#include "PolyVox\Raycast.h"

#include "osg\Group"

#include "osg\ShapeDrawable"

BlockDestructionController::BlockDestructionController() :
	_parentNode(nullptr),
	_baseNode(new osg::PositionAttitudeTransform),
	_createCoolDown(0),
	_destroyTime(0),
	_isDestroying(false),
	_isCreating(false)
{
	attach(GameEngine::inst().getRoot());

	float blockSize = OSGRenderer::BLOCK_WIDTH + 0.02f; //make it slightly larger so avoid z-fighting

	osg::Box* box = new osg::Box(osg::Vec3(OSGRenderer::BLOCK_WIDTH /2, OSGRenderer::BLOCK_WIDTH /2, OSGRenderer::BLOCK_WIDTH /2), blockSize);
	osg::ShapeDrawable* unitCubeDrawable = new osg::ShapeDrawable(box);

	_baseNode->addChild(unitCubeDrawable);
}


BlockDestructionController::~BlockDestructionController()
{
	attach(nullptr);
}

void BlockDestructionController::update(Player * player, double time)
{
	highlightBlock(player);

	if (_createCoolDown <= 0.0 && _isCreating)
	{
		createBlock(player, BlockType::BlockType_Stone);
		_createCoolDown = player->getStats().getCreateBlockCooldown();
	}
	else
	{
		_createCoolDown -= time;
	}

	if (_isDestroying)
	{
		_destroyTime += time;

		if (_destroyTime >= player->getStats().getDestroyBlockTime())
		{
			destroyBlock(player);
			_destroyTime = 0;
		}
	}
	else
	{
		_destroyTime = 0;
	}
	
}

void BlockDestructionController::startDestroyBlock(Player * player)
{
	_isDestroying = true;
}

void BlockDestructionController::startCreateBlock(Player * player)
{
	_isCreating = true;
}

void BlockDestructionController::endDestroyBlock(Player * player)
{
	_isDestroying = false;
}

void BlockDestructionController::endCreateBlock(Player * player)
{
	_isCreating = false;
	_createCoolDown = 0.0;
}

void BlockDestructionController::highlightBlock(Player * player)
{
	PolyVox::PickResult res = preformVoxelRaycast(player);

	if (!res.didHit)
	{
		_baseNode->setNodeMask(0x0); //hide the box
	}
	else
	{
		_baseNode->setNodeMask(0xffffffff); //show the box
		osg::Vec3d pos(res.hitVoxel.getX() * OSGRenderer::BLOCK_WIDTH, res.hitVoxel.getY() * OSGRenderer::BLOCK_WIDTH, res.hitVoxel.getZ() * OSGRenderer::BLOCK_WIDTH);
		_baseNode->setPosition(pos);
	}
}

void BlockDestructionController::destroyBlock(Player * player)
{
	PolyVox::PickResult result = preformVoxelRaycast(player);

	if (!result.didHit) return;

	GameEngine::inst().getGrid()->setBlock(result.hitVoxel, BlockType::BlockType_Default, true);
}

void BlockDestructionController::createBlock(Player * player, int block)
{
	PolyVox::PickResult result = preformVoxelRaycast(player);

	if (!result.didHit) return;

	GameEngine::inst().getGrid()->setBlock(result.previousVoxel, block, true);
}

PolyVox::PickResult BlockDestructionController::preformVoxelRaycast(Player * player)
{

	osg::Vec3d start = player->getEyePosition();

	osg::Vec3d rot = GameEngine::inst().getPlayer()->getRotation();

	float yaw = -rot.x() - osg::PI_2;
	float pitch = -rot.y();
	float x = -cos(yaw)*cos(pitch);
	float y = -sin(yaw)*cos(pitch);
	float z = -sin(pitch);

	osg::Vec3d end(start.x() + x, start.y() + y, start.z() + z);

	using namespace PolyVox;

	Vector3DFloat startVec(start.x() / OSGRenderer::BLOCK_WIDTH, start.y() / OSGRenderer::BLOCK_WIDTH, start.z() / OSGRenderer::BLOCK_WIDTH);

	Vector3DFloat direction(x, y, z);
	direction.normalise();
	direction *= player->getStats().getManipulateDist();

	PickResult res = pickVoxel<BlockGrid::blockMap_type>(GameEngine::inst().getGrid()->getBlockMap(), startVec, direction, BlockType::BlockType_Default);


	return res;
}

void BlockDestructionController::attach(osg::Group * root)
{
	if (_parentNode != nullptr)
		_parentNode->removeChild(_baseNode);

	_parentNode = root;

	if (_parentNode != nullptr)
		_parentNode->addChild(_baseNode);
}