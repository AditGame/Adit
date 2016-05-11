#pragma once

#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osg/Group>
#include <osg/Geode>
#include <osg/ref_ptr>
#include <osg/Camera>

#include "MYGUIManager.h"
#include "GuiManager.h"
#include "Modes.h"

class EscapeMenu;
class SettingsMenu;

/// Controls all the gui elements, displaying them as needed
class GuiManager : public MYGUIManager
{
public:
	GuiManager(osg::Group* root, osgViewer::Viewer* view);

	//adds a new gui mode (effectly opens up a new gui)
	void addGuiMode(GuiMode newMode);

	//Removes this gui mode if it exists (typically the topmost)
	void removeGuiMode(GuiMode oldMode);

	//removes the top gui
	void popGuiMode();

	//clears all guis, returning the user to the game
	void clearGuiModes();

	bool isAnyGuiMode() { return !_guiModes.empty(); }

	GuiMode getTopGui();

protected:
	virtual void setupResources();

	virtual void initializeControls();

private:
	//if the top of guiModes has been changed, update the visibility of all widgets
	void updateVisibility();

	//Holds the list of opened windows
	std::list<GuiMode> _guiModes;

	//the GUIs
	EscapeMenu* _escMenu;
	SettingsMenu* _settingsMenu;

	//OSG Render Data
	osg::Group* _parentNode;
	osg::Geode* _guiGeode;
	osg::Camera* _guiCamera;

};