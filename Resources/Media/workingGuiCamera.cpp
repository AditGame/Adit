/* OpenSceneGraph example, osghud.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*/

#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>

#include <osgGA/TrackballManipulator>

#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/PolygonOffset>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/RenderInfo>

#include <osgDB/WriteFile>

#include <osgText/Text>

#include "Gui\GuiEngine.h"

osg::Camera* createHUD(osgViewer::Viewer &viewer)
{
	// create a camera to set up the projection and model view matrices, and the subgraph to draw in the HUD
	osg::Camera* camera = new osg::Camera;

	// set the projection matrix
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0.0, 100.0, 0.0, 100.0));

	// set the view matrix
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	//camera->setViewMatrix(osg::Matrix::identity());

	// only clear the depth buffer
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);

	// draw subgraph after main camera view.
	camera->setRenderOrder(osg::Camera::POST_RENDER);

	// we don't want the camera to grab event focus from the viewers main camera(s).
	camera->setAllowEventFocus(false);



	// add to this camera a subgraph to render
	{

		GuiEngine* gui = new GuiEngine(camera);

		viewer.addEventHandler(new MYGUIHandler(camera, gui->getManager()));

		//camera->addChild(geode);
	}

	return camera;
}

struct SnapImage : public osg::Camera::DrawCallback
{
	SnapImage(const std::string& filename) :
		_filename(filename),
		_snapImage(false)
	{
		_image = new osg::Image;
	}

	virtual void operator () (osg::RenderInfo& renderInfo) const
	{

		if (!_snapImage) return;

		osg::notify(osg::NOTICE) << "Camera callback" << std::endl;

		osg::Camera* camera = renderInfo.getCurrentCamera();
		osg::Viewport* viewport = camera ? camera->getViewport() : 0;

		osg::notify(osg::NOTICE) << "Camera callback " << camera << " " << viewport << std::endl;

		if (viewport && _image.valid())
		{
			_image->readPixels(int(viewport->x()), int(viewport->y()), int(viewport->width()), int(viewport->height()),
				GL_RGBA,
				GL_UNSIGNED_BYTE);
			osgDB::writeImageFile(*_image, _filename);

			osg::notify(osg::NOTICE) << "Taken screenshot, and written to '" << _filename << "'" << std::endl;
		}

		_snapImage = false;
	}

	std::string                         _filename;
	mutable bool                        _snapImage;
	mutable osg::ref_ptr<osg::Image>    _image;
};



struct SnapeImageHandler : public osgGA::GUIEventHandler
{

	SnapeImageHandler(int key, SnapImage* si) :
		_key(key),
		_snapImage(si) {}

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		if (ea.getHandled()) return false;

		switch (ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYUP) :
		{
			if (ea.getKey() == 'o')
			{
				osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
				osg::Node* node = view ? view->getSceneData() : 0;
				if (node)
				{
					osgDB::writeNodeFile(*node, "hud.osgt");
					osgDB::writeNodeFile(*node, "hud.osgb");
				}
				return true;
			}

			if (ea.getKey() == _key)
			{
				osg::notify(osg::NOTICE) << "event handler" << std::endl;
				_snapImage->_snapImage = true;
				return true;
			}

			break;
		}
		default:
			break;
		}

		return false;
	}

	int                     _key;
	osg::ref_ptr<SnapImage> _snapImage;
};


int main(int argc, char **argv)
{
	// use an ArgumentParser object to manage the program arguments.
	osg::ArgumentParser arguments(&argc, argv);


	// if not loaded assume no arguments passed in, try use default model instead.
	osg::ref_ptr<osg::Node> scene = osgDB::readRefNodeFile("dumptruck.osgt");


	if (!scene)
	{
		osg::notify(osg::NOTICE) << "No model loaded" << std::endl;
		return 1;
	}

	{
		// construct the viewer.
		osgViewer::Viewer viewer;

		SnapImage* postDrawCallback = new SnapImage("PostDrawCallback.png");
		viewer.getCamera()->setPostDrawCallback(postDrawCallback);
		viewer.addEventHandler(new SnapeImageHandler('p', postDrawCallback));

		SnapImage* finalDrawCallback = new SnapImage("FinalDrawCallback.png");
		viewer.getCamera()->setFinalDrawCallback(finalDrawCallback);
		viewer.addEventHandler(new SnapeImageHandler('f', finalDrawCallback));

		viewer.addEventHandler(new osgViewer::WindowSizeHandler);

		osg::ref_ptr<osg::Group> group = new osg::Group;

		// add the HUD subgraph.
		if (scene.valid()) group->addChild(scene);
		group->addChild(createHUD(viewer));

		viewer.setUpViewInWindow(20, 20, 400, 400, 0);

		// set the scene to render
		viewer.setSceneData(group);

		osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(viewer.getCamera()->getGraphicsContext());
		if (gw)
		{
			// Send window size for MyGUI to initialize
			int x, y, w, h; gw->getWindowRectangle(x, y, w, h);
			viewer.getEventQueue()->windowResize(x, y, w, h);
		}

		return viewer.run();
	}

}


/* OpenSceneGraph example, osghud.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*/

#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>

#include <osgGA/TrackballManipulator>

#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/PolygonOffset>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/RenderInfo>

#include <osgDB/WriteFile>

#include <osgText/Text>

#include "Gui\GuiEngine.h"

struct SnapImage : public osg::Camera::DrawCallback
{
	SnapImage(const std::string& filename) :
		_filename(filename),
		_snapImage(false)
	{
		_image = new osg::Image;
	}

	virtual void operator () (osg::RenderInfo& renderInfo) const
	{

		if (!_snapImage) return;

		osg::notify(osg::NOTICE) << "Camera callback" << std::endl;

		osg::Camera* camera = renderInfo.getCurrentCamera();
		osg::Viewport* viewport = camera ? camera->getViewport() : 0;

		osg::notify(osg::NOTICE) << "Camera callback " << camera << " " << viewport << std::endl;

		if (viewport && _image.valid())
		{
			_image->readPixels(int(viewport->x()), int(viewport->y()), int(viewport->width()), int(viewport->height()),
				GL_RGBA,
				GL_UNSIGNED_BYTE);
			osgDB::writeImageFile(*_image, _filename);

			osg::notify(osg::NOTICE) << "Taken screenshot, and written to '" << _filename << "'" << std::endl;
		}

		_snapImage = false;
	}

	std::string                         _filename;
	mutable bool                        _snapImage;
	mutable osg::ref_ptr<osg::Image>    _image;
};



struct SnapeImageHandler : public osgGA::GUIEventHandler
{

	SnapeImageHandler(int key, SnapImage* si) :
		_key(key),
		_snapImage(si) {}

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		if (ea.getHandled()) return false;

		switch (ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYUP) :
		{
			if (ea.getKey() == 'o')
			{
				osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
				osg::Node* node = view ? view->getSceneData() : 0;
				if (node)
				{
					osgDB::writeNodeFile(*node, "hud.osgt");
					osgDB::writeNodeFile(*node, "hud.osgb");
				}
				return true;
			}

			if (ea.getKey() == _key)
			{
				osg::notify(osg::NOTICE) << "event handler" << std::endl;
				_snapImage->_snapImage = true;
				return true;
			}

			break;
		}
		default:
			break;
		}

		return false;
	}

	int                     _key;
	osg::ref_ptr<SnapImage> _snapImage;
};


int main(int argc, char **argv)
{
	// use an ArgumentParser object to manage the program arguments.
	osg::ArgumentParser arguments(&argc, argv);


	// if not loaded assume no arguments passed in, try use default model instead.
	osg::ref_ptr<osg::Node> scene = osgDB::readRefNodeFile("dumptruck.osgt");


	if (!scene)
	{
		osg::notify(osg::NOTICE) << "No model loaded" << std::endl;
		return 1;
	}

	{
		// construct the viewer.
		osgViewer::Viewer viewer;

		SnapImage* postDrawCallback = new SnapImage("PostDrawCallback.png");
		viewer.getCamera()->setPostDrawCallback(postDrawCallback);
		viewer.addEventHandler(new SnapeImageHandler('p', postDrawCallback));

		SnapImage* finalDrawCallback = new SnapImage("FinalDrawCallback.png");
		viewer.getCamera()->setFinalDrawCallback(finalDrawCallback);
		viewer.addEventHandler(new SnapeImageHandler('f', finalDrawCallback));

		viewer.addEventHandler(new osgViewer::WindowSizeHandler);

		osg::ref_ptr<osg::Group> group = new osg::Group;

		// add the HUD subgraph.
		if (scene.valid()) group->addChild(scene);

		GuiEngine* gui = new GuiEngine(group, &viewer);

		viewer.setUpViewInWindow(20, 20, 400, 400, 0);

		// set the scene to render
		viewer.setSceneData(group);

		osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(viewer.getCamera()->getGraphicsContext());
		if (gw)
		{
			// Send window size for MyGUI to initialize
			int x, y, w, h; gw->getWindowRectangle(x, y, w, h);
			viewer.getEventQueue()->windowResize(x, y, w, h);
		}

		return viewer.run();
	}

}
