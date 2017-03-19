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

#include "Input/InputHandler.h"
#include "Character/Player.h"
#include "Render/PlayerCamera.h"
#include "Render/OSGRenderer.h"
#include "Physics/PhysicsEngine.h"
#include "Gui\GuiManager.h"

#include "Utilities/Options.h"

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
	_input->setPlayer(_player);

	viewer = setUpView();

	OSGRenderer::setUp();

	_gui = new GuiManager(root, viewer);
}

osgViewer::Viewer* GameEngine::setUpView()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	//viewer->getCamera()->setCullingActive(true);
	//viewer->getCamera()->setCullingMode(osg::CullSettings::VIEW_FRUSTUM_SIDES_CULLING);

	//set up screen
	int screenNum = Options::instance().getInt(Options::OPT_SCREEN_NUM);
	if(Options::instance().getBool(Options::OPT_FULLSCREEN))
		viewer->setUpViewOnSingleScreen(screenNum);
	else
		viewer->setUpViewInWindow(20, 20, Options::instance().getInt(Options::OPT_SCREEN_WIDTH), Options::instance().getInt(Options::OPT_SCREEN_HEIGHT), screenNum);

	//set up FOV (TODO: There HAS to be a better way!)
	double fov = 0; double ratio = 0; double near = 0; double far = 0;
	osg::Matrix proj;
	proj = viewer->getCamera()->getProjectionMatrix();
	proj.getPerspective(fov, ratio, near, far);

	viewer->getCamera()->setProjectionMatrix(osg::Matrix::perspective(Options::instance().getInt(Options::OPT_FOV), ratio, near, far));

	// add the state manipulator
	viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));

	// add the thread model handler
	//viewer->addEventHandler(new osgViewer::ThreadingHandler);

	viewer->setThreadingModel(osgViewer::ViewerBase::ThreadingModel::CullThreadPerCameraDrawThreadPerContext);
	

	// add the window size toggle handler
	osgViewer::WindowSizeHandler* sizeHandler = new osgViewer::WindowSizeHandler();
	sizeHandler->setKeyEventToggleFullscreen(Options::instance().getInt(Options::OPT_FULLSCREEN_KEY));
	viewer->addEventHandler(sizeHandler);

	// add the stats handler
	viewer->addEventHandler(new osgViewer::StatsHandler);

	// add the record camera path handler
	viewer->addEventHandler(new osgViewer::RecordCameraPathHandler);

	// add the LOD Scale handler
	viewer->addEventHandler(new osgViewer::LODScaleHandler);

	// add the screen capture handler
	viewer->addEventHandler(new osgViewer::ScreenCaptureHandler);

	viewer->setKeyEventSetsDone(0);

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

	osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(viewer->getCamera()->getGraphicsContext());
	if (gw)
	{
		// Send window size for MyGUI to initialize
		int x, y, w, h; gw->getWindowRectangle(x, y, w, h);
		viewer->getEventQueue()->windowResize(x, y, w, h);
	}

	_grid->chunkManager->setVisibility(4);
	_grid->chunkManager->processAllDirty();
	//_grid->chunkManager->setVisibility(8);

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

		_camera->update();

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
