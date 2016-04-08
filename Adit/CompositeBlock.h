#pragma once

#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/PositionAttitudeTransform>
#include "Coords.h"
#include "BlockEnums.h"

class CompositeBlock
{
public:
	CompositeBlock() : CompositeBlock(nullptr, Coords(0, 0, 0)) {}

	CompositeBlock(osg::Group* chunkNode, Coords absPos, int body = 0, int topFace = 0, int northFace = 0, int southFace = 0, int westFace = 0, int eastFace = 0);

	~CompositeBlock();

	int isSolid() { return body != BlockType::BlockType_Default; }

	typedef uint32_t blockDataType;

	operator uint8_t() const { return body; }

	int body;
	int topFace;
	int northFace;
	int southFace;
	int eastFace;
	int westFace;

	typedef int value_type;
private:

	//OSG Render data
	osg::ref_ptr<osg::Group> _parentNode;
	osg::ref_ptr<osg::PositionAttitudeTransform> _baseNode;
};

