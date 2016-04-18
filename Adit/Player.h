#pragma once

#include <osg\Node>

#include <osg\Vec3f>
#include <osg\Quat>
#include <osg\PositionAttitudeTransform>

#include <btBulletDynamicsCommon.h>

#include "Entity.h"
#include "Coords.h"
#include "SyncedMotionState.h"

#include "CharacterController.h"
#include "Movement.h"
#include "CharacterStats.h"

class Player : public Entity
{
public:
	Player(osg::Group* parentNode = nullptr);
	~Player();
	
	void update(GameEngine* eng, btScalar time);

	void setRotation(osg::Vec3f);

	osg::PositionAttitudeTransform* getEyeNode();

	void setFirstPerson(bool v);

	Movement& getMovement() { return _movement; }

	CharacterStats& getStats() { return _stats; }

private:

	bool _firstPerson;

	Movement _movement;

	CharacterController _controller;

	CharacterStats _stats;

	//OSG Render Data
	osg::Switch* _bodySwitch;
	osg::PositionAttitudeTransform* _headNode;

	//Bullet Data
	btCylinderShape* _physShape;
};

