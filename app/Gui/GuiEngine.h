#include <osg/Group>
#include <osg/Geode>
#include <osg/ref_ptr>
#include <osg/Camera>

#include "GuiManager.h"

/// Provides management of gui elements
class GuiEngine
{
public:
	GuiEngine(osg::Group* root, osgViewer::Viewer* view);

	GuiManager* getManager() { return _guiManager; }

private:
	osg::Group* _parentNode;
	GuiManager* _guiManager;
	osg::Geode* _guiGeode;
	osg::Camera* _guiCamera;
};