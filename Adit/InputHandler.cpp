#include "InputHandler.h"

#include <osg/Quat>

#include "BlockGrid.h"
#include "Player.h"
#include "GameEngine.h"
#include "PhysicsEngine.h"
#include "PlayerCamera.h"

#include "Options.h"

InputHandler::InputHandler(BlockGrid* grid, GameEngine* eng) : 
	_grid(grid),
	_eng(eng),
	_cursorLock(true)
{

	Options opt = Options::instance();

	_channelMap[opt.getInt(Options::OPT_FORWARD)] = Actions::A_Forward;
	_channelMap[opt.getInt(Options::OPT_BACKWARD)] = Actions::A_Backward;
	_channelMap[opt.getInt(Options::OPT_LEFT)] = Actions::A_Left;
	_channelMap[opt.getInt(Options::OPT_RIGHT)] = Actions::A_Right;
	_channelMap[opt.getInt(Options::OPT_JUMP)] = Actions::A_Jump;


	_channelValues[Actions::A_Forward] = 0.0f;
	_channelValues[Actions::A_Backward] = 0.0f;
	_channelValues[Actions::A_Left] = 0.0f;
	_channelValues[Actions::A_Right] = 0.0f;
	_channelValues[Actions::A_Jump] = 0.0f;
	_inputMap[opt.getInt(Options::OPT_FORWARD)] = 
		[=](int key, bool pressed)
		{
			if (_player) _player->movePosition(osg::Vec3f(1, 0, 0));
		};
	_inputMap[opt.getInt(Options::OPT_LEFT)] =
		[=](int key, bool pressed)
		{
			if (_player) _player->movePosition(osg::Vec3f(0, -1, 0));
		};
	_inputMap[opt.getInt(Options::OPT_RIGHT)] =
		[=](int key, bool pressed)
		{
			if (_player) _player->movePosition(osg::Vec3f(0, 1, 0));
		};
	_inputMap[opt.getInt(Options::OPT_BACKWARD)] =
		[=](int key, bool pressed)
		{
			if (_player) _player->movePosition(osg::Vec3f(-1, 0, 0));
		};
}


InputHandler::~InputHandler()
{
}

void InputHandler::getEulerFromQuat(osg::Quat q, double& heading, double& attitude, double& bank)
{
	double limit = 0.499999;
	double sqx = q.x()*q.x();
	double sqy = q.y()*q.y();
	double sqz = q.z()*q.z();
	double t = q.x()*q.y() + q.z()*q.w();

	if (t>limit) // gimbal lock? 
	{
		heading = 2 * atan2(q.x(), q.w());
		attitude = osg::PI_2;
		bank = 0;
	}
	else if (t<-limit)
	{
		heading = -2 * atan2(q.x(), q.w());
		attitude = -osg::PI_2;
		bank = 0;
	}
	else
	{
		heading = atan2(2 * q.y()*q.w() - 2 * q.x()*q.z(), 1 - 2 * sqy - 2 * sqz);
		attitude = asin(2 * t);
		bank = atan2(2 * q.x()*q.w() - 2 * q.y()*q.z(), 1 - 2 * sqx - 2 * sqz);
	}
}

void InputHandler::update()
{
	if (_player)
	{
		float x = _channelValues[A_Forward] + -1 * _channelValues[A_Backward];
		float y = _channelValues[A_Right] + -1 * _channelValues[A_Left];
		float z = _channelValues[A_Jump];

		_player->getMovement().forwardBack = x;
		_player->getMovement().leftRight = y;
		_player->getMovement().jump = z;
	}
}

bool InputHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	switch (ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYDOWN) :
	{

		channelMap_type::iterator it = _channelMap.find(ea.getKey());
		if (it != _channelMap.end())
		{
			_channelValues[it->second] = 1.0f;
		}
		switch (ea.getKey())
		{
		case osgGA::GUIEventAdapter::KeySymbol::KEY_I:
			GameEngine::inst().toggleDebugDraw();
			return false;
			break;
		case osgGA::GUIEventAdapter::KeySymbol::KEY_BackSpace:
			setCursorLock(!_cursorLock);
			return false;
			break;
		default:
			return false;
		}
	}
	case(osgGA::GUIEventAdapter::KEYUP) :
	{
		channelMap_type::iterator it = _channelMap.find(ea.getKey());
		if (it != _channelMap.end())
		{
			_channelValues[it->second] = 0.0f;
		}
	}
	case(osgGA::GUIEventAdapter::MOVE) :
	{
		if (_player != nullptr)
		{
			float mouseX = ea.getX();
			float mouseY = ea.getY();
			_player->modRotation(osg::Vec3f((float)(ea.getX() - _oldMouseX) / 1000.0f, (float)(ea.getY() - _oldMouseY) / 1000.0f, 0));
			//aa.requestWarpPointer(200, 200);aa
			if (_cursorLock)
			{
				_oldMouseX = ea.getWindowWidth() / 2;
				_oldMouseY = ea.getWindowHeight() / 2;
				aa.requestWarpPointer(_oldMouseX, _oldMouseY);
			}
			else
			{
				_oldMouseX = mouseX;
				_oldMouseY = mouseY;
			}

		}
	}
	default:
		return false;
	}
}

void InputHandler::setCursorLock(bool v)
{
	_cursorLock = v;
	osgViewer::ViewerBase::Windows windows;
	if (GameEngine::inst().getViewer() != nullptr)
	{
		GameEngine::inst().getViewer()->getWindows(windows, true);
		if (windows.size() != 0)
		{
			if (_cursorLock)
				windows[0]->setCursor(osgViewer::GraphicsWindow::MouseCursor::NoCursor);
			else
				windows[0]->setCursor(osgViewer::GraphicsWindow::MouseCursor::InheritCursor);
		}
	}
}
