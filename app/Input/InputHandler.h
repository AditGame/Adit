#pragma once

#include <osgGA\GUIEventHandler>

#include <unordered_map>
#include <functional>

class BlockGrid;
class Player;
class GameEngine;

class InputHandler : public osgGA::GUIEventHandler
{
public:
	InputHandler(BlockGrid* grid, GameEngine* eng);
	~InputHandler();

	void getEulerFromQuat(osg::Quat q, double & heading, double & attitude, double & bank);

	void update();

	//handle input events
	bool handle(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);

	void setPlayer(Player* player) { _player = player; }

	void setCursorLock(bool v);

	typedef std::unordered_map<int, int> channelMap_type;
	typedef std::unordered_map<int, float> channelValues_type;

private:
	BlockGrid* _grid;
	Player* _player;
	GameEngine* _eng;

	float _oldMouseX;
	float _oldMouseY;

	bool _cursorLock;

	typedef std::function<void(int key, bool pressed)> inputHandler;

	typedef std::unordered_map<int, inputHandler> inputMap_type;

	inputMap_type _inputMap;

	channelMap_type _channelMap;

	channelValues_type _channelValues;

	enum Actions
	{
		A_Forward,
		A_Backward,
		A_Left,
		A_Right,
		A_Jump,

		A_Escape = 10, //jump to give room for more movement enums

		A_NumActions
	};
};

