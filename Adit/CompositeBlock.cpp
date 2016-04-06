#include "CompositeBlock.h"

#include "OSGRenderer.h"
#include <osg/ref_ptr>
#include <osg/Geode>
#include <osg/Geometry>

CompositeBlock::CompositeBlock(osg::Group* chunkNode, Coords absPos, int body, int topFace, int northFace, int southFace, int westFace, int eastFace)
	: _parentNode(chunkNode), body(body), topFace(topFace), northFace(northFace), southFace(southFace), eastFace(eastFace), westFace(westFace) 
{
	//_baseNode = new osg::PositionAttitudeTransform;
	//_parentNode->addChild(_baseNode);
	//_baseNode->setPosition(osg::Vec3d(absPos.getX()*OSGRenderer::BLOCK_WIDTH, absPos.getY()*OSGRenderer::BLOCK_WIDTH, absPos.getZ()*OSGRenderer::BLOCK_WIDTH));

	if (body != 0)
	{
		//osg::Geometry* geo = OSGRenderer::createCube();
		//osg::ref_ptr<osg::Geode> shapeGeode = new osg::Geode();
		//shapeGeode->addDrawable(geo);
		//_baseNode->addChild(shapeGeode);
	}
}

CompositeBlock::~CompositeBlock()
{
	//_parentNode->removeChild(_baseNode);
}
