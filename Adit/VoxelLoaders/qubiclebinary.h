// ******************************************************************************
//
// Filename:	QubicleBinary.h
// Project:	Vox
// Author:	Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 10/07/14
//
// Copyright (c) 2005-2014, Steven Ball
//
// ******************************************************************************

#pragma once

#include "engine.h"

#include "MS3DModel.h"
#include "MS3DAnimator.h"

class VoxelCharacter;

enum MergedSide
{
	MergedSide_None = 0,

	MergedSide_X_Positive = 1,
	MergedSide_X_Negative = 2,
	MergedSide_Y_Positive = 4,
	MergedSide_Y_Negative = 8,
	MergedSide_Z_Positive = 16,
	MergedSide_Z_Negative = 32,
};

bool IsMergedXNegative(int ***merged, int x, int y, int z);
bool IsMergedXPositive(int ***merged, int x, int y, int z);
bool IsMergedYNegative(int ***merged, int x, int y, int z);
bool IsMergedYPositive(int ***merged, int x, int y, int z);
bool IsMergedZNegative(int ***merged, int x, int y, int z);
bool IsMergedZPositive(int ***merged, int x, int y, int z);

class QubicleMatrix
{
public:
	char m_nameLength;
	char* m_name;

	unsigned int m_matrixSizeX;
	unsigned int m_matrixSizeY;
	unsigned int m_matrixSizeZ;

	int m_matrixPosX;
	int m_matrixPosY;
	int m_matrixPosZ;

	unsigned int ***m_pColour;

	int m_boneIndex;

	Matrix4x4 m_modelMatrix;

	float m_scale;
	float m_offsetX;
	float m_offsetY;
	float m_offsetZ;

	unsigned int m_meshID;
};

class QubicleBinary
{
public:
	/* Public methods */
	QubicleBinary(OpenGLRenderer* pRenderer);
	~QubicleBinary();

	void Unload();

	void Reset();

	unsigned int GetMaterial();

	Matrix4x4 GetModelMatrix(int qubicleMatrixIndex);

	int GetMatrixIndexForName(const char* matrixName);
	void GetMatrixPosition(int index, int* aX, int* aY, int* aZ);

	bool Import(const char* fileName);

	void GetColour(int matrixIndex, int x, int y, int z, float* r, float* g, float* b, float* a);
	unsigned int GetColourCompact(int matrixIndex, int x, int y, int z);
	bool GetActive(int matrixIndex, int x, int y, int z);

	void CreateMesh();
	void UpdateMergedSide(int ***merged, int matrixIndex, int blockx, int blocky, int blockz, Vector3d *p1, Vector3d *p2, Vector3d *p3, Vector3d *p4, int startX, int startY, int maxX, int maxY, bool positive, bool zFace, bool xFace, bool yFace);

	int GetNumMatrices();
	QubicleMatrix* GetQubicleMatrix(int index);
	const char* GetMatrixName(int index);
	float GetMatrixScale(int index);
	Vector3d GetMatrixOffset(int index);

	void SetupMatrixBones(MS3DAnimator* pSkeleton);
	
	void SetScaleAndOffsetForMatrix(const char* matrixName, float scale, float xOffset, float yOffset, float zOffset);
	float GetScale(const char* matrixName);
	Vector3d GetOffset(const char* matrixName);

	void SetWireFrameRender(bool wireframe);

	void Update(float dt);

	void Render(bool renderOutline, bool refelction);

	static const float BLOCK_RENDER_SIZE;

protected:


private:

	OpenGLRenderer* m_pRenderer;

	// Loaded flag
	bool m_loaded;

	// Qubicle binary file information
	char m_version[4];
	unsigned int m_colourFormat;
	unsigned int m_zAxisOrientation;
	unsigned int m_compressed;
	unsigned int m_visibilityMaskEncoded;
	unsigned int m_numMatrices;

	// Matrix data for file
	QubicleMatrix* m_pMatrices;

	// Render modes
	bool m_renderWireFrame;

	// Material
	unsigned int m_materialID;
};