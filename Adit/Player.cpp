#include "Player.h"

#include <osg\Group>
#include <osg\PositionAttitudeTransform>
#include <osg\Switch>
#include <osg\Quat>
#include <osg\Matrix>

#include <osg/ShapeDrawable>

#include <osgbCollision\Utils.h>

#include "OSGRenderer.h"
#include "GameEngine.h"
#include "PhysicsEngine.h"
#include "BlockGrid.h"
#include "SyncedMotionState.h"

Player::Player(osg::Group* parentNode) : Entity(parentNode, osg::Vec3d(0,0,135)), _headNode(new osg::PositionAttitudeTransform), _bodySwitch(new osg::Switch), _firstPerson(true), _controller(this)
{

	_headNode->setPosition(osg::Vec3d(0, ((float)OSGRenderer::BLOCK_WIDTH)*1.0f, 0));
	_baseNode->addChild(_bodySwitch);
	_bodySwitch->addChild(_headNode);

	osg::Box* foot = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.1f, 1.0f);
	osg::ShapeDrawable* footDrawable = new osg::ShapeDrawable(foot);

	osg::Box* head = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 0.5f, 0.5f, 1.0f);
	osg::ShapeDrawable* headDrawable = new osg::ShapeDrawable(head);

	_baseNode->addChild(footDrawable);
	_headNode->addChild(headDrawable);

	_physShape = new btCylinderShape(btVector3(0.4f, 1, 0.4f));
	btScalar mass = 80;
	btVector3 inertia = btVector3(0, 0, 0);
	_physShape->calculateLocalInertia(mass, inertia);

	//use osg quats as they have handy functions that let me pretend that quats don't exsit
	osg::Quat quat;
	quat.makeRotate(osg::PI_2, osg::Vec3d(1, 0, 0));
	osg::Matrix matrix;
	matrix.setRotate(quat);
	matrix.setTrans(osg::Vec3d(getPosition().x(), getPosition().y(), getPosition().z()));
	_motionState = new SyncedMotionState(osgbCollision::asBtTransform(matrix), this);

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(mass, _motionState, _physShape, inertia);
	groundRigidBodyCI.m_friction = 0;
	//groundRigidBodyCI.m_linearDamping = 10;

	_rigidBody = new btRigidBody(groundRigidBodyCI);

	_rigidBody->setAngularFactor(btVector3(0, 0, 0));

	_rigidBody->setActivationState(DISABLE_DEACTIVATION);

	GameEngine::inst().getPhysics()->getWorld()->addRigidBody(_rigidBody);
}


Player::~Player()
{
	//call base destructor
	Entity::~Entity();
}

void Player::update(GameEngine * eng, btScalar time)
{
	_controller.update();
}

void Player::setRotation(osg::Vec3f newRot)
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

osg::Node * Player::getEyeNode()
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
