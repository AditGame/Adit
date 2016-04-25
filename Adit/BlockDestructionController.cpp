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

BlockDestructionController::BlockDestructionController() : _parentNode(nullptr), _baseNode(new osg::PositionAttitudeTransform)
{
	attach(GameEngine::inst().getRoot());

	float blockSize = OSGRenderer::BLOCK_WIDTH + 0.02f;

	osg::Box* box = new osg::Box(osg::Vec3(blockSize/2, blockSize/2, blockSize/2), blockSize); //make it slightly larger so avoid z-fighting
	osg::ShapeDrawable* unitCubeDrawable = new osg::ShapeDrawable(box);

	_baseNode->addChild(unitCubeDrawable);
}


BlockDestructionController::~BlockDestructionController()
{
	attach(nullptr);
}

void BlockDestructionController::highlightBlock(Player * player)
{
	PolyVox::PickResult res = preformVoxelRaycast(player);

	if (!res.didHit) return;

	osg::Vec3d pos(res.hitVoxel.getX() * OSGRenderer::BLOCK_WIDTH, res.hitVoxel.getY() * OSGRenderer::BLOCK_WIDTH, res.hitVoxel.getZ() * OSGRenderer::BLOCK_WIDTH);


	_baseNode->setPosition(pos);

}

void BlockDestructionController::destroyBlock(Player * player)
{
	PolyVox::PickResult result = preformVoxelRaycast(player);

	if (!result.didHit) return;

	GameEngine::inst().getGrid()->setBlock(result.hitVoxel, BlockType::BlockType_Default);
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