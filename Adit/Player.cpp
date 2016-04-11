#include "Player.h"

#include <osg\Group>
#include <osg\PositionAttitudeTransform>
#include <osg\Switch>

#include <osg/ShapeDrawable>

#include "OSGRenderer.h"
#include "GameEngine.h"
#include "BlockGrid.h"

Player::Player(osg::Group* parentNode) : _parentNode(parentNode), _baseNode(new osg::PositionAttitudeTransform()), _headNode(new osg::PositionAttitudeTransform), _bodySwitch(new osg::Switch), _location(0,0,0), _rotation(0,0,0), _firstPerson(true)
{
	if (_parentNode != nullptr)
		attach(_parentNode);

	_headNode->setPosition(osg::Vec3d(0, ((float)OSGRenderer::BLOCK_WIDTH)*1.0f, 0));
	_baseNode->addChild(_bodySwitch);
	_bodySwitch->addChild(_headNode);

	osg::Box* foot = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.1f, 1.0f);
	osg::ShapeDrawable* footDrawable = new osg::ShapeDrawable(foot);

	osg::Box* head = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 0.5f, 0.5f, 1.0f);
	osg::ShapeDrawable* headDrawable = new osg::ShapeDrawable(head);

	_baseNode->addChild(footDrawable);
	_headNode->addChild(headDrawable);
	//if (_firstPerson)
	//	_bodySwitch->setAllChildrenOff();
	//else
	//	_bodySwitch->setAllChildrenOn();

}


Player::~Player()
{
	if (_parentNode != nullptr)
		_parentNode->removeChild(_baseNode);
}

void Player::update(GameEngine * eng, osg::ElapsedTime & time)
{
	CompositeBlock::blockDataType block = eng->getGrid()->getBlock(Coords(_location.x(), _location.y(), _location.z()));
	CompositeBlock::blockDataType blockUnder = eng->getGrid()->getBlock(Coords(_location.x(), _location.y(), _location.z()-1));
	double _;
	double percentage = std::modf(_location.z(), &_);
	if (blockUnder == BlockType::BlockType_Default)
		movePosition(osg::Vec3f(0, 0, -0.1f));
	else if (block != BlockType::BlockType_Default)
		movePosition(osg::Vec3f(0, 0, 1.0f));
	else if (percentage > .01f)
		movePosition(osg::Vec3f(0, 0, -(percentage - .01f)));
}

void Player::setPosition(osg::Vec3f newPos)
{
	_location = newPos;
	osg::Vec3d position(_location.x()*OSGRenderer::BLOCK_WIDTH, _location.y()*OSGRenderer::BLOCK_WIDTH, _location.z()*OSGRenderer::BLOCK_WIDTH);
	_baseNode->setPosition(position);

	GameEngine::inst().getGrid()->chunkManager->setCenterChunk(ChunkManager::blockToChunkCoords(Coords(_location.x(), _location.y())));
}

void Player::movePosition(const osg::Vec3f &pos)
{
	setPosition(_location + pos);
}

void Player::setRotation(osg::Vec3f newRot)
{
	std::cout << _rotation.x() << " " << newRot.x() << std::endl;
	_rotation = newRot;

	osg::Quat bottomQuat;
	osg::Quat headQuat;
	const osg::Vec3d headingAxis(0.0, 0.0, 1.0);
	const osg::Vec3d pitchAxis(1.0, 0.0, 0.0);
	const osg::Vec3d rollAxis(0.0, 1.0, 0.0);

	double rotation[3] = { _rotation.x(), _rotation.y(), _rotation.z() };

	bottomQuat.makeRotate(
		0 + 1.5708f, pitchAxis,
		rotation[2], rollAxis,
		-rotation[0], headingAxis);
	headQuat.makeRotate(
		rotation[1] + 1.5708f, pitchAxis,
		rotation[2], rollAxis,
		0, headingAxis);

	_baseNode->setAttitude(bottomQuat);
	_headNode->setAttitude(headQuat);
}

void Player::modRotation(osg::Vec3f modRot)
{
	setRotation(modRot + _rotation);
}

void Player::attach(osg::Group * root)
{
	_parentNode = root;
	_parentNode->addChild(_baseNode);
	osg::Vec3d position(_location.x()*OSGRenderer::BLOCK_WIDTH, _location.y()*OSGRenderer::BLOCK_WIDTH, _location.z()*OSGRenderer::BLOCK_WIDTH);
	_baseNode->setPosition(position);
}

osg::Node * Player::getEyeNode()
{
	return _headNode;
}
