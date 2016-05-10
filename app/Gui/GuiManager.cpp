#include "GuiManager.h"

#include <osgViewer/Viewer>
#include <osg/Camera>

#include "Engine\GameEngine.h"
#include "Input\InputHandler.h"

#include "EscapeMenu.h"

GuiManager::GuiManager(osg::Group * root, osgViewer::Viewer* view) :
	MYGUIManager(),
	_guiGeode(new osg::Geode),
	_guiCamera(new osg::Camera),
	_parentNode(nullptr),
	_escMenu(nullptr)
{

	//set up the geode
	_guiGeode->setCullingActive(false);
	_guiGeode->addDrawable(this);
	_guiGeode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	_guiGeode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);


	// set the projection matrix
	_guiCamera->setProjectionMatrix(osg::Matrix::ortho2D(0.0, 100.0, 0.0, 100.0));

	// set the view matrix
	_guiCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	//camera->setViewMatrix(osg::Matrix::identity());

	// only clear the depth buffer
	_guiCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

	// draw subgraph after main camera view.
	_guiCamera->setRenderOrder(osg::Camera::POST_RENDER);

	// we don't want the camera to grab event focus from the viewers main camera(s).
	_guiCamera->setAllowEventFocus(false);

	_guiCamera->addChild(_guiGeode);

	root->addChild(_guiCamera);

	view->addEventHandler(new MYGUIHandler(_guiCamera, this));

}

void GuiManager::setupResources()
{
	MYGUIManager::setupResources();
	_platform->getDataManagerPtr()->addResourceLocation(_rootMedia + "/Demos/Demo_Themes", false);
	_platform->getDataManagerPtr()->addResourceLocation(_rootMedia + "/Common/Demos", false);
	_platform->getDataManagerPtr()->addResourceLocation(_rootMedia + "/Common/Themes", false);
	_platform->getDataManagerPtr()->addResourceLocation(_rootMedia + "/Layouts", true);
}

void GuiManager::initializeControls()
{
	//init all the guis
	_escMenu = new EscapeMenu();

	updateVisibility();
}

void GuiManager::addGuiMode(GuiMode newMode)
{
	_guiModes.push_front(newMode);

	updateVisibility();
}

void GuiManager::removeGuiMode(GuiMode oldMode)
{
	//deletek all instances of the gui mode
	std::list<GuiMode>::iterator it = _guiModes.begin();
	while (it != _guiModes.end())
	{
		if (*it == oldMode)
			it = _guiModes.erase(it);
		else
			++it;
	}

	updateVisibility();
}

void GuiManager::popGuiMode()
{
	_guiModes.pop_front();

	updateVisibility();
}

void GuiManager::clearGuiModes()
{
	_guiModes.clear();

	updateVisibility();
}

GuiMode GuiManager::getTopGui()
{
	if (!_guiModes.empty())
		return _guiModes.front();
	else
		return GuiMode::GUI_None;
}


void GuiManager::updateVisibility()
{

	// To begin with, hide all GUIs
	_escMenu->setVisible(false);

	// update mouse trap/visibility
	bool gameMode = !isAnyGuiMode();
	GameEngine::inst().getInput()->setCursorLock(gameMode);

	// Unhide the relevent GUI
	if (!_guiModes.empty())
	{
		switch (_guiModes.front())
		{
		case GUI_Escape_Menu:
			_escMenu->setVisible(true);
			break;
		}
	}
}
