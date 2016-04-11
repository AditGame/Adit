#pragma once

#include <osg/Timer>

class GameEngine;

class Entity
{
public:
	Entity();
	~Entity();

	virtual void update(GameEngine* eng, osg::ElapsedTime &time) = 0;
};

