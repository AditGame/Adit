#pragma once

#include "VoxTreeNode.h"
#include "VoxTreeCoord.h"

#include "BlockGrid.h"

/**
VoxTree.h
Header for class VoxTree
*/
template <class Voxel>
class VoxTree
{
public:
	VoxTree(BlockGrid::blockMap_type* voxels, VoxTreeCoord minCoord, VoxTreeCoord maxCoord, VoxTreeCoord stride);
	virtual ~VoxTree();

	VoxTreeNode * root() { return mRoot; }
private:
	VoxTree(const VoxTree&);
	VoxTree& operator=(const VoxTree&);

	void populate(VoxTreeNode * node);
	void findBestSplit(VoxTreeNode * node, int & pos, int & quality, int axis);
	void eval(VoxTreeNode * node);
	Voxel at(int x, int y, int z) { return mVoxels->getVoxel(x,y,z); }

private:
	VoxTreeNode * mRoot;
	BlockGrid::blockMap_type* mVoxels;
	VoxTreeCoord mMinCoord;
	VoxTreeCoord mMaxCoord;
	VoxTreeCoord mStride;
};

template <class Voxel>
VoxTree<Voxel>::VoxTree(BlockGrid::blockMap_type* voxels, VoxTreeCoord minCoord, VoxTreeCoord maxCoord, VoxTreeCoord stride) :
	mRoot(0),
	mVoxels(voxels),
	mMinCoord(minCoord),
	mMaxCoord(maxCoord),
	mStride(stride)
{
	mRoot = new VoxTreeNode(minCoord, maxCoord);
	populate(mRoot);
}

template <class Voxel>
VoxTree<Voxel>::~VoxTree()
{
	delete mRoot;
}

template <class Voxel>
void VoxTree<Voxel>::populate(VoxTreeNode * node)
{
	eval(node);

	if (node->mValue == VoxTreeNode::nodeMixed)
	{
		int xpos;
		int xqual;
		int ypos;
		int yqual;
		int zpos;
		int zqual;
		findBestSplit(node, xpos, xqual, 0);
		findBestSplit(node, ypos, yqual, 1);
		findBestSplit(node, zpos, zqual, 2);

		VoxTreeCoord split(0, 0, 0);
		if (xqual > yqual && xqual > zqual)
		{
			split = node->mMaxCoord;
			split.x = xpos;
			node->mChildren[0] = new VoxTreeNode(node->mMinCoord, split);
			split = node->mMinCoord;
			split.x = xpos + 1;
			node->mChildren[1] = new VoxTreeNode(split, node->mMaxCoord);
		}
		else if (yqual > zqual)
		{
			split = node->mMaxCoord;
			split.y = ypos;
			node->mChildren[0] = new VoxTreeNode(node->mMinCoord, split);
			split = node->mMinCoord;
			split.y = ypos + 1;
			node->mChildren[1] = new VoxTreeNode(split, node->mMaxCoord);
		}
		else
		{
			split = node->mMaxCoord;
			split.z = zpos;
			node->mChildren[0] = new VoxTreeNode(node->mMinCoord, split);
			split = node->mMinCoord;
			split.z = zpos + 1;
			node->mChildren[1] = new VoxTreeNode(split, node->mMaxCoord);
		}

		populate(node->mChildren[0]);
		populate(node->mChildren[1]);
	}
}

template <class Voxel>
void VoxTree<Voxel>::findBestSplit(VoxTreeNode * node, int & pos, int & quality, int axis)
{
	quality = 0;
	pos = 0;
	int a0 = axis;
	int a1 = (axis + 1) % 3;
	int a2 = (axis + 2) % 3;
	VoxTreeCoord coord(0, 0, 0);
	for (coord.pos[a0] = node->mMinCoord.pos[a0]; coord.pos[a0] < node->mMaxCoord.pos[a0]; ++coord.pos[a0])
	{
		int qual = 0;
		for (coord.pos[a1] = node->mMinCoord.pos[a1]; coord.pos[a1] <= node->mMaxCoord.pos[a1]; ++coord.pos[a1])
		{
			for (coord.pos[a2] = node->mMinCoord.pos[a2]; coord.pos[a2] <= node->mMaxCoord.pos[a2]; ++coord.pos[a2])
			{
				Voxel v1 = at(coord.x, coord.y, coord.z);
				Voxel v2 = v1 + mStride.pos[a0];
				if ((v1 == 0) != (v2 == 0))
				{
					++qual;
				}
			}
		}
		if (qual > quality)
		{
			pos = coord.pos[a0];
			quality = qual;
		}
	}
}

template <class Voxel>
void VoxTree<Voxel>::eval(VoxTreeNode * node)
{
	bool foundSet = false;
	bool foundUnset = false;
	for (int z = node->mMinCoord.z; z <= node->mMaxCoord.z; ++z)
	{
		for (int y = node->mMinCoord.y; y <= node->mMaxCoord.y; ++y)
		{
			for (int x = node->mMinCoord.x; x <= node->mMaxCoord.x; ++x)
			{
				if (at(x, y, z))
				{
					foundSet = true;
				}
				else
				{
					foundUnset = true;
				}
				if (foundSet && foundUnset)
				{
					node->mValue = VoxTreeNode::nodeMixed;
					return;
				}
			}
		}
	}
	if (foundSet)
	{
		node->mValue = VoxTreeNode::nodeFull;
	}
	else
	{
		node->mValue = VoxTreeNode::nodeEmpty;
	}
}
