#include "GameEngine.h"

#include <osg/Timer>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/SphericalManipulator>
#include <osgGA/NodeTrackerManipulator>

#include <osgbCollision\GLDebugDrawer.h>

#include "InputHandler.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "OSGRenderer.h"
#include "PhysicsEngine.h"

#include "Options.h"

GameEngine::GameEngine() : root(new osg::Group), _debugDraw(false)
{
}

void GameEngine::setup()
{
	_grid = new BlockGrid(root);
	_input = new InputHandler(_grid, this);
	_camera = new PlayerCamera(this);
	_physics = new PhysicsEngine();
	_player = new Player(root);
	_camera->attach(_player);
	_input->setPlayer(_player);

	viewer = setUpView();

	OSGRenderer::setUp();
}

osgViewer::Viewer* GameEngine::setUpView()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	//viewer->getCamera()->setCullingActive(true);
	//viewer->getCamera()->setCullingMode(osg::CullSettings::VIEW_FRUSTUM_SIDES_CULLING);

	osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
	osg::GraphicsContext::ScreenIdentifier si;
	si.readDISPLAY();


	viewer->setUpViewOnSingleScreen(osg::GraphicsContext::ScreenIdentifier(0).screenNum);
	//viewer.setUpViewInWindow(20, 20, 400, 400, 1);

	// set up the camera manipulators.
	{
		//osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

		//keyswitchManipulator->addMatrixManipulator('1', "Trackball", new osgGA::TrackballManipulator());
		//keyswitchManipulator->addMatrixManipulator('2', "Flight", new osgGA::FlightManipulator());
		//keyswitchManipulator->addMatrixManipulator('3', "Drive", new osgGA::DriveManipulator());
		//keyswitchManipulator->addMatrixManipulator('4', "Terrain", new osgGA::TerrainManipulator());
		//keyswitchManipulator->addMatrixManipulator('5', "Orbit", new osgGA::OrbitManipulator());
		//keyswitchManipulator->addMatrixManipulator('6', "FirstPerson", new osgGA::FirstPersonManipulator());
		//keyswitchManipulator->addMatrixManipulator('7', "Spherical", new osgGA::SphericalManipulator());
		//keyswitchManipulator->addMatrixManipulator('8', "PlayerCam", _camera->getManipulator());


		//viewer->setCameraManipulator(keyswitchManipulator.get());
	}

	viewer->setCameraManipulator(_camera->getManipulator());

	// add the state manipulator
	viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));

	// add the thread model handler
	viewer->addEventHandler(new osgViewer::ThreadingHandler);

	// add the window size toggle handler
	viewer->addEventHandler(new osgViewer::WindowSizeHandler);

	// add the stats handler
	viewer->addEventHandler(new osgViewer::StatsHandler);

	// add the record camera path handler
	viewer->addEventHandler(new osgViewer::RecordCameraPathHandler);

	// add the LOD Scale handler
	viewer->addEventHandler(new osgViewer::LODScaleHandler);

	// add the screen capture handler
	viewer->addEventHandler(new osgViewer::ScreenCaptureHandler);

	return viewer;
}

void GameEngine::go()
{

	// Add the camera data
	//viewer->setCameraManipulator(_camera->getManipulator());

	// add the input handler
	viewer->addEventHandler(_input);

	//set up the physics debug draw
	osgbCollision::GLDebugDrawer dbgDraw;
	dbgDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dbgDraw.setEnabled(_debugDraw);
	if (_debugDraw)
	{
		std::cout << "osgbpp: Debug" << std::endl;

		// Enable debug drawing.
		root->addChild(dbgDraw.getSceneGraph());
		_physics->getWorld()->setDebugDrawer(&dbgDraw);
	}

	viewer->setSceneData(root);

	double currSimTime = viewer->getFrameStamp()->getSimulationTime();
	double prevSimTime = viewer->getFrameStamp()->getSimulationTime();

	viewer->realize();

	_grid->chunkManager->processAllDirty();
	while (!viewer->done()) {
		currSimTime = viewer->getFrameStamp()->getSimulationTime();

		if (_debugDraw != dbgDraw.getEnabled())
		{
			dbgDraw.setEnabled(_debugDraw);
			if (_debugDraw)
			{
				//set up debug drawer
				root->addChild(dbgDraw.getSceneGraph());
				_physics->getWorld()->setDebugDrawer(&dbgDraw);
			}
			else
			{
				//unset it up
				root->removeChild(dbgDraw.getSceneGraph());
				_physics->getWorld()->setDebugDrawer(nullptr);
			}
		}

		if (_debugDraw)
			dbgDraw.BeginDraw();

		_input->update();
		_grid->update();
		_player->update(this, currSimTime - prevSimTime);
		_physics->update(currSimTime - prevSimTime);

		if (_debugDraw)
		{
			_physics->getWorld()->debugDrawWorld();
			dbgDraw.EndDraw();
		}

		prevSimTime = currSimTime;

		viewer->frame();
	}

	delete _grid;
	delete _camera;
	//delete _player; //memory curruption? whatever, we're ending anyways. lol.
	delete _input;
	Options::instance().save(); //save any modifications to the options

}