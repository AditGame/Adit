#pragma once

#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>

#include "MYGUIManager.h"

class HudManager;

/// Controls all the gui elements, displaying them as needed
class GuiManager : public MYGUIManager
{
protected:
	virtual void setupResources();

	virtual void initializeControls();

	void notifyComboAccept(MyGUI::ComboBox* sender, size_t index);

	void createDemo(int index);

	void destroyDemo();

	MyGUI::Widget* _demoView;
	MyGUI::ComboBox* _comboSkins;
};