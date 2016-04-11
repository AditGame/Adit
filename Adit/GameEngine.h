#pragma once

#include "BlockGrid.h"

#include <osgViewer\Viewer>

class InputHandler;
class Player;
class PlayerCamera;

class GameEngine
{
public:

	static GameEngine& inst()
	{
		static GameEngine eng;
		return eng;
	}

	osgViewer::Viewer* setUpView();

	void go();

	PlayerCamera* getCamera() { return _camera; }
	osgViewer::Viewer* getViewer() { return viewer; }
	BlockGrid* getGrid() { return _grid; }
private:

	//is singleton
	GameEngine();

	//contains simple voxel data
	BlockGrid* _grid;

	//The current player
	Player* _player;

	//The Camera Manipulator
	PlayerCamera* _camera;

	//Handles keyboard input
	InputHandler* _input;

	// OSG Data
	osg::ref_ptr<osg::Group> root;

	osgViewer::Viewer* viewer;

};

