#pragma once

#include <osg\Node>

#include <osg\Vec3f>
#include <osg\Quat>
#include <osg\PositionAttitudeTransform>

#include "Entity.h"
#include "Coords.h"

class Player : public Entity
{
public:
	Player(osg::Group* parentNode = nullptr);
	~Player();
	
	void update(GameEngine* eng, osg::ElapsedTime &time);

	void setPosition(osg::Vec3f);
	void movePosition(const osg::Vec3f&);

	void setRotation(osg::Vec3f);
	void modRotation(osg::Vec3f);
	osg::Vec3f getRotation() { return _rotation; }

	void attach(osg::Group* group);

	osg::Node* getEyeNode();

private:
	osg::Vec3f _location;
	osg::Vec3f _rotation;

	bool _firstPerson;

	//OSG Render Data
	osg::Group* _parentNode;
	osg::PositionAttitudeTransform* _baseNode;
	osg::Switch* _bodySwitch;
	osg::PositionAttitudeTransform* _headNode;
};

