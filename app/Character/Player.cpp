#include "Player.h"

#include <osg\Group>
#include <osg\PositionAttitudeTransform>
#include <osg\Switch>
#include <osg\Quat>
#include <osg\Matrix>

#include <osg/ShapeDrawable>

#include <osgbCollision\Utils.h>

#include "Render/OSGRenderer.h"
#include "Engine/GameEngine.h"
#include "Physics/PhysicsEngine.h"
#include "BlockMgmt/BlockGrid.h"
#include "Physics/SyncedMotionState.h"

Player::Player(osg::Group* parentNode) : Entity(parentNode, osg::Vec3d(1000 * OSGRenderer::BLOCK_WIDTH,1000 * OSGRenderer::BLOCK_WIDTH,200 * OSGRenderer::BLOCK_WIDTH)), _headNode(new osg::PositionAttitudeTransform), _bodySwitch(new osg::Switch), _firstPerson(true), _controller(this)
{
	_baseNode->addChild(_bodySwitch);
	_bodySwitch->addChild(_headNode);

	osg::Box* foot = new osg::Box(osg::Vec3(0.0f, 0.05f, 0.0f), 0.8f, 0.1f, 0.4f);
	osg::ShapeDrawable* footDrawable = new osg::ShapeDrawable(foot);

	osg::Box* head = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 0.5f, 0.5f, 0.5f);
	osg::ShapeDrawable* headDrawable = new osg::ShapeDrawable(head);
	_headNode->setPosition(osg::Vec3d(0, 1.75f, 0));

	_baseNode->addChild(footDrawable);
	_headNode->addChild(headDrawable);

	//_physShape = new btCylinderShape(btVector3(0.4f, 2.0f * 0.5f, 0.4f));
	_physShape = new btCapsuleShape(0.4f, 2.0f - 0.4f*2); //height is height + radius*2, so undo that
	btScalar mass = 80;
	btVector3 inertia = btVector3(0, 0, 0);
	_physShape->calculateLocalInertia(mass, inertia);

	//use osg quats as they have handy functions that let me pretend that quats don't exsit
	osg::Quat quat;
	quat.makeRotate(osg::PI_2, osg::Vec3d(1, 0, 0));
	osg::Matrix matrix;
	matrix.setRotate(quat);
	matrix.setTrans(osg::Vec3d(getPosition().x(), getPosition().y(), getPosition().z()));
	_motionState = new SyncedMotionState(osgbCollision::asBtTransform(matrix), this, osg::Vec3d(0,0,1), false);

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(mass, _motionState, _physShape, inertia);
	groundRigidBodyCI.m_friction = 0;
	//groundRigidBodyCI.m_linearDamping = 10;

	_rigidBody = new btRigidBody(groundRigidBodyCI);

	_rigidBody->setAngularFactor(btVector3(0, 0, 0));

	_rigidBody->setActivationState(DISABLE_DEACTIVATION);

	_rigidBody->setRestitution(0.0);

	GameEngine::inst().getPhysics()->getWorld()->addRigidBody(_rigidBody);
}


Player::~Player()
{
	//call base destructor
	Entity::~Entity();
}

void Player::update(GameEngine * eng, btScalar time)
{
	_controller.update(time);
	_destructionController.update(this, time);
}



void Player::setRotation(osg::Vec3d newRot)
{
	if (newRot.y() > osg::PI_2) newRot.y() = osg::PI_2;
	if (newRot.y() < -osg::PI_2) newRot.y() = -osg::PI_2;

	//std::cout << _rotation.y() << " " << newRot.y() << std::endl;
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

osg::Vec3d Player::getEyePosition()
{
	return osg::computeLocalToWorld(getEyeNode()->getParentalNodePaths().at(0)).getTrans();
}

osg::PositionAttitudeTransform * Player::getEyeNode()
{
	return _headNode;
}

void Player::setFirstPerson(bool v)
{
	_firstPerson = v;
	if (_firstPerson)
		_bodySwitch->setAllChildrenOff();
	else
		_bodySwitch->setAllChildrenOn();
}
