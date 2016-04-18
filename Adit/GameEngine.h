#pragma once

#include "BlockGrid.h"


#include <osgViewer\Viewer>

class InputHandler;
class Player;
class PlayerCamera;
class PhysicsEngine;

class GameEngine
{
public:

	static GameEngine& inst()
	{
		static GameEngine eng;
		return eng;
	}

	void setup();

	osgViewer::Viewer* setUpView();

	void go();

	PlayerCamera* getCamera() { return _camera; }
	osgViewer::Viewer* getViewer() { return viewer; }
	BlockGrid* getGrid() { return _grid; }
	PhysicsEngine* getPhysics() { return _physics; }
	Player* getPlayer() { return _player; }

	void toggleDebugDraw() { _debugDraw = !_debugDraw; }

private:

	//is singleton
	GameEngine();
	GameEngine(GameEngine const&);              // Don't Implement
	void operator=(GameEngine const&); // Don't implement

	//contains simple voxel data
	BlockGrid* _grid;

	//The current player
	Player* _player;

	//The Camera Manipulator
	PlayerCamera* _camera;

	//Handles keyboard input
	InputHandler* _input;

	//Handles/Steps the physics engine
	PhysicsEngine* _physics;

	bool _debugDraw;

	// OSG Data
	osg::ref_ptr<osg::Group> root;
	osgViewer::Viewer* viewer;

};

