#include "GuiEngine.h"

#include <osgViewer/Viewer>
#include <osg/Camera>

#include "Engine\GameEngine.h"

GuiEngine::GuiEngine(osg::Group * root, osgViewer::Viewer* view) :
	_guiGeode(new osg::Geode),
	_guiCamera(new osg::Camera),
	_parentNode(nullptr)
{

	//set up the gui manager
	_guiManager = new GuiManager();

	_guiGeode->setCullingActive(false);
	_guiGeode->addDrawable(_guiManager);
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

	view->addEventHandler(new MYGUIHandler(_guiCamera, _guiManager));

	//if (root != nullptr)
	//	attach(root);
}