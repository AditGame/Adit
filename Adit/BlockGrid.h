#pragma once

#include "ChunkManager.h"
#include "CompositeBlock.h"
class BlockGrid
{
public:
	BlockGrid(osg::Group* rootNode);
	~BlockGrid();
	ChunkManager* chunkManager;
	CompositeBlock& getBlock(Coords location);

	void update();

	osg::ref_ptr<osg::Group> getBaseNode() { return _baseNode; }

private:
	//OSG Render data
	osg::ref_ptr<osg::Group> _parentNode;
	osg::ref_ptr<osg::Group> _baseNode;
};

