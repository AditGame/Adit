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
#include "BlockDestructionController.h"

class Player : public Entity
{
public:
	Player(osg::Group* parentNode = nullptr);
	~Player();
	
	void update(GameEngine* eng, btScalar time);

	void setRotation(osg::Vec3f);

	osg::Vec3d getEyePosition();

	osg::PositionAttitudeTransform* getEyeNode();

	void setFirstPerson(bool v);

	Movement& getMovement() { return _movement; }

	CharacterStats& getStats() { return _stats; }

	void destroyBlock();

private:

	bool _firstPerson;

	Movement _movement;

	CharacterController _controller;

	CharacterStats _stats;

	BlockDestructionController _destructionController;

	//OSG Render Data
	osg::Switch* _bodySwitch;
	osg::PositionAttitudeTransform* _headNode;

	//Bullet Data
	btCapsuleShape* _physShape;


};

