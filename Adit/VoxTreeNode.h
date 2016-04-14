#pragma once

#include "VoxTreeCoord.h"

/**
VoxTreeNode.h
Header for class VoxTreeNode
*/
class VoxTreeNode
{
public:
	enum NodeValue
	{
		nodeUnknown,
		nodeEmpty,
		nodeFull,
		nodeMixed
	};
public:
	VoxTreeNode(VoxTreeCoord minCoord, VoxTreeCoord maxCoord) :
		mValue(nodeUnknown),
		mMinCoord(minCoord),
		mMaxCoord(maxCoord)
	{
		mChildren[0] = mChildren[1] = 0;
	}

	virtual ~VoxTreeNode()
	{
		delete mChildren[0];
		delete mChildren[1];
	}
public:
	VoxTreeCoord mMinCoord;
	VoxTreeCoord mMaxCoord;
	NodeValue mValue;
	VoxTreeNode * mChildren[2];
};
