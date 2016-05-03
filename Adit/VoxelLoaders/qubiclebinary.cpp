// ******************************************************************************
//
// Filename:	QubicleBinary.cpp
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

#include "QubicleBinary.h"

#include "Random.h"


const float QubicleBinary::BLOCK_RENDER_SIZE = 0.5f;


QubicleBinary::QubicleBinary(OpenGLRenderer* pRenderer)
{
	m_pRenderer = pRenderer;

	Reset();

	m_renderWireFrame = false;

	pRenderer->CreateMaterial(Colour(0.7f, 0.7f, 0.7f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 0.0f), 64, &m_materialID);

	float l_length = 0.5f; 
	float l_height = 0.5f;
	float l_width = 0.5f;
}

QubicleBinary::~QubicleBinary()
{
	Unload();

	Reset();
}

void QubicleBinary::Unload()
{
	if(m_loaded)
	{
		for(unsigned int i = 0; i < m_numMatrices; i++)
		{
			m_pRenderer->ClearMesh(m_pMatrices[i].m_meshID);
		}

		delete m_pMatrices;
		m_pMatrices = NULL;		
	}
}

void QubicleBinary::Reset()
{
	m_version[0] = 0;
	m_version[1] = 0;
	m_version[2] = 0;
	m_version[3] = 0;
	m_colourFormat = 0;
	m_zAxisOrientation = 0;
	m_compressed = 0;
	m_visibilityMaskEncoded = 0;
	m_numMatrices = 0;

	m_loaded = false;

	m_pMatrices = NULL;

	m_renderWireFrame = false;
}

unsigned int QubicleBinary::GetMaterial()
{
	return m_materialID;
}

Matrix4x4 QubicleBinary::GetModelMatrix(int qubicleMatrixIndex)
{
	if(m_loaded == false)
	{
		return Matrix4x4();
	}

	return m_pMatrices[qubicleMatrixIndex].m_modelMatrix;
}

int QubicleBinary::GetMatrixIndexForName(const char* matrixName)
{
	for(unsigned int i = 0; i < m_numMatrices; i++)
	{
		if(strcmp(m_pMatrices[i].m_name, matrixName) == 0)
		{
			return i;
		}
	}

	return -1;
}

void QubicleBinary::GetMatrixPosition(int index, int* aX, int* aY, int* aZ)
{
	*aX = m_pMatrices[index].m_matrixPosX;
	*aY = m_pMatrices[index].m_matrixPosY;
	*aZ = m_pMatrices[index].m_matrixPosZ;
}

bool QubicleBinary::Import(const char* fileName)
{
	char qbFilename[256];
	sprintf_s(qbFilename, fileName);;

	FILE* pQBfile = NULL;
	fopen_s(&pQBfile, qbFilename, "rb");

	const unsigned int CODEFLAG = 2;
	const unsigned int NEXTSLICEFLAG = 6;

	if (qbFilename != NULL)
	{
		int ok = 0;
		ok = fread(&m_version[0], sizeof(char)*4, 1, pQBfile) == 1;
		ok = fread(&m_colourFormat, sizeof(unsigned int), 1, pQBfile) == 1;
		ok = fread(&m_zAxisOrientation, sizeof(unsigned int), 1, pQBfile) == 1;
		ok = fread(&m_compressed, sizeof(unsigned int), 1, pQBfile) == 1;
		ok = fread(&m_visibilityMaskEncoded, sizeof(unsigned int), 1, pQBfile) == 1;
		ok = fread(&m_numMatrices, sizeof(unsigned int), 1, pQBfile) == 1;

		m_pMatrices = new QubicleMatrix[m_numMatrices];

		for(unsigned int i = 0; i < m_numMatrices; i++)
		{
			ok = fread((char *)&m_pMatrices[i].m_nameLength, sizeof(char), 1, pQBfile) == 1;
			m_pMatrices[i].m_name = new char[m_pMatrices[i].m_nameLength+1];
			ok = fread(&m_pMatrices[i].m_name[0], sizeof(char)*m_pMatrices[i].m_nameLength, 1, pQBfile) == 1;
			m_pMatrices[i].m_name[m_pMatrices[i].m_nameLength] = 0;

			ok = fread(&m_pMatrices[i].m_matrixSizeX, sizeof(unsigned int), 1, pQBfile) == 1;
			ok = fread(&m_pMatrices[i].m_matrixSizeY, sizeof(unsigned int), 1, pQBfile) == 1;
			ok = fread(&m_pMatrices[i].m_matrixSizeZ, sizeof(unsigned int), 1, pQBfile) == 1;
			
			ok = fread(&m_pMatrices[i].m_matrixPosX, sizeof(int), 1, pQBfile) == 1;
			ok = fread(&m_pMatrices[i].m_matrixPosY, sizeof(int), 1, pQBfile) == 1;
			ok = fread(&m_pMatrices[i].m_matrixPosZ, sizeof(int), 1, pQBfile) == 1;

			m_pMatrices[i].m_pColour = new unsigned int**[m_pMatrices[i].m_matrixSizeX];

			m_pMatrices[i].m_boneIndex = -1;
			m_pMatrices[i].m_meshID = -1;

			m_pMatrices[i].m_scale = 1.0f;
			m_pMatrices[i].m_offsetX = 0.0f;
			m_pMatrices[i].m_offsetY = 0.0f;
			m_pMatrices[i].m_offsetZ = 0.0f;

			for(unsigned int j = 0; j < m_pMatrices[i].m_matrixSizeX; j++)
			{
				m_pMatrices[i].m_pColour[j] = new unsigned int*[m_pMatrices[i].m_matrixSizeY];

				for(unsigned int k = 0; k < m_pMatrices[i].m_matrixSizeY; k++)
				{
					m_pMatrices[i].m_pColour[j][k] = new unsigned int[m_pMatrices[i].m_matrixSizeZ];
				}
			}

			if(m_compressed == 0)
			{
				for(unsigned int z = 0; z < m_pMatrices[i].m_matrixSizeZ; z++)
				{
					for(unsigned int y = 0; y < m_pMatrices[i].m_matrixSizeY; y++)
					{
						for(unsigned int x = 0; x < m_pMatrices[i].m_matrixSizeX; x++)
						{
							unsigned int colour = 0;
							ok = fread(&colour, sizeof(unsigned int), 1, pQBfile) == 1;

							//unsigned int alpha = (colour & 0xFF000000) >> 24;
							//unsigned int blue = (colour & 0x00FF0000) >> 16;
							//unsigned int green = (colour & 0x0000FF00) >> 8;
							//unsigned int red = (colour & 0x000000FF);

							m_pMatrices[i].m_pColour[x][y][z] = colour;
						}
					}
				}
			}
			else
			{
				unsigned int z = 0;

				while (z < m_pMatrices[i].m_matrixSizeZ) 
				{
					unsigned int index = 0;

					while(true)
					{
						unsigned int data = 0;
						ok = fread(&data, sizeof(unsigned int), 1, pQBfile) == 1;

						if (data == NEXTSLICEFLAG)
							break;
						else if (data == CODEFLAG) 
						{
							unsigned int count = 0;
							ok = fread(&count, sizeof(unsigned int), 1, pQBfile) == 1;
							ok = fread(&data, sizeof(unsigned int), 1, pQBfile) == 1;

							for(unsigned int j = 0; j < count; j++) 
							{
								unsigned int x = index % m_pMatrices[i].m_matrixSizeX;
								unsigned int y = index / m_pMatrices[i].m_matrixSizeX;

								//unsigned int alpha = (data & 0xFF000000) >> 24;
								//unsigned int blue = (data & 0x00FF0000) >> 16;
								//unsigned int green = (data & 0x0000FF00) >> 8;
								//unsigned int red = (data & 0x000000FF);

								m_pMatrices[i].m_pColour[x][y][z] = data;

								index++;
							}
						}
						else
						{
							unsigned int x = index % m_pMatrices[i].m_matrixSizeX;
							unsigned int y = index / m_pMatrices[i].m_matrixSizeX;

							//unsigned int alpha = (data & 0xFF000000) >> 24;
							//unsigned int blue = (data & 0x00FF0000) >> 16;
							//unsigned int green = (data & 0x0000FF00) >> 8;
							//unsigned int red = (data & 0x000000FF);

							m_pMatrices[i].m_pColour[x][y][z] = data;

							index++;
						}
					}

					z++;
				}
			}
		}

		fclose(pQBfile);

		CreateMesh();

		m_loaded = true;

		return true;
	}

	return false;
}

void QubicleBinary::GetColour(int matrixIndex, int x, int y, int z, float* r, float* g, float* b, float* a)
{
	unsigned colour = m_pMatrices[matrixIndex].m_pColour[x][y][z];
	unsigned int alpha = (colour & 0xFF000000) >> 24;
	unsigned int blue = (colour & 0x00FF0000) >> 16;
	unsigned int green = (colour & 0x0000FF00) >> 8;
	unsigned int red = (colour & 0x000000FF);

	*r = (float)(red / 255.0f);
	*g = (float)(green / 255.0f);
	*b = (float)(blue / 255.0f);
	*a = 1.0f;
}

unsigned int QubicleBinary::GetColourCompact(int matrixIndex, int x, int y, int z)
{
	return m_pMatrices[matrixIndex].m_pColour[x][y][z];
}

bool QubicleBinary::GetActive(int matrixIndex, int x, int y, int z)
{
	unsigned colour = m_pMatrices[matrixIndex].m_pColour[x][y][z];
	unsigned int alpha = (colour & 0xFF000000) >> 24;
	unsigned int blue = (colour & 0x00FF0000) >> 16;
	unsigned int green = (colour & 0x0000FF00) >> 8;
	unsigned int red = (colour & 0x000000FF);

	if(alpha == 0)
	{
		return false;
	}

	return true;
}

bool IsMergedXNegative(int ***merged, int x, int y, int z) { return (merged[x][y][z] & MergedSide_X_Negative) == MergedSide_X_Negative; }
bool IsMergedXPositive(int ***merged, int x, int y, int z) { return (merged[x][y][z] & MergedSide_X_Positive) == MergedSide_X_Positive; }
bool IsMergedYNegative(int ***merged, int x, int y, int z) { return (merged[x][y][z] & MergedSide_Y_Negative) == MergedSide_Y_Negative; }
bool IsMergedYPositive(int ***merged, int x, int y, int z) { return (merged[x][y][z] & MergedSide_Y_Positive) == MergedSide_Y_Positive; }
bool IsMergedZNegative(int ***merged, int x, int y, int z) { return (merged[x][y][z] & MergedSide_Z_Negative) == MergedSide_Z_Negative; }
bool IsMergedZPositive(int ***merged, int x, int y, int z) { return (merged[x][y][z] & MergedSide_Z_Positive) == MergedSide_Z_Positive; }

void QubicleBinary::CreateMesh()
{
	for(unsigned int matrixIndex = 0; matrixIndex < m_numMatrices; matrixIndex++)
	{
		int ***l_merged;

		l_merged = new int**[m_pMatrices[matrixIndex].m_matrixSizeX];

		for(unsigned int i = 0; i < m_pMatrices[matrixIndex].m_matrixSizeX; i++)
		{
			l_merged[i] = new int*[m_pMatrices[matrixIndex].m_matrixSizeY];

			for(unsigned int j = 0; j < m_pMatrices[matrixIndex].m_matrixSizeY; j++)
			{
				l_merged[i][j] = new int[m_pMatrices[matrixIndex].m_matrixSizeZ];
			}
		}

		for(unsigned int x = 0; x < m_pMatrices[matrixIndex].m_matrixSizeX; x++)
		{
			for(unsigned int y = 0; y < m_pMatrices[matrixIndex].m_matrixSizeY; y++)
			{
				for(unsigned int z = 0; z < m_pMatrices[matrixIndex].m_matrixSizeZ; z++)
				{
					l_merged[x][y][z] = MergedSide_None;
				}
			}
		}

		if(m_pMatrices[matrixIndex].m_meshID == -1)
		{
			m_pRenderer->CreateMesh(&m_pMatrices[matrixIndex].m_meshID, OGLMeshType_Textured);
		}

		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
		float a = 1.0f;	

		for(unsigned int x = 0; x < m_pMatrices[matrixIndex].m_matrixSizeX; x++)
		{
			for(unsigned int y = 0; y < m_pMatrices[matrixIndex].m_matrixSizeY; y++)
			{
				for(unsigned int z = 0; z < m_pMatrices[matrixIndex].m_matrixSizeZ; z++)
				{
					if(GetActive(matrixIndex, x, y, z) == false)
					{
						continue;
					}
					else
					{
						GetColour(matrixIndex, x, y, z, &r, &g, &b, &a);

						a = 1.0f;

						Vector3d p1(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						Vector3d p2(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						Vector3d p3(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						Vector3d p4(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						Vector3d p5(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						Vector3d p6(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						Vector3d p7(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						Vector3d p8(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);

						Vector3d n1;
						unsigned int v1, v2, v3, v4;
						unsigned int t1, t2, t3, t4;

						bool doXPositive = (IsMergedXPositive(l_merged, x, y, z) == false);
						bool doXNegative = (IsMergedXNegative(l_merged, x, y, z) == false);
						bool doYPositive = (IsMergedYPositive(l_merged, x, y, z) == false);
						bool doYNegative = (IsMergedYNegative(l_merged, x, y, z) == false);
						bool doZPositive = (IsMergedZPositive(l_merged, x, y, z) == false);
						bool doZNegative = (IsMergedZNegative(l_merged, x, y, z) == false);

						// Front
						if(doZPositive && ((z == m_pMatrices[matrixIndex].m_matrixSizeZ-1) || z < m_pMatrices[matrixIndex].m_matrixSizeZ-1 && GetActive(matrixIndex, x, y, z+1) == false))
						{
							int endX = m_pMatrices[matrixIndex].m_matrixSizeX;
							int endY = m_pMatrices[matrixIndex].m_matrixSizeY;

							UpdateMergedSide(l_merged, matrixIndex, x, y, z, &p1, &p2, &p3, &p4, x, y, endX, endY, true, true, false, false);

							n1 = Vector3d(0.0f, 0.0f, 1.0f);
							v1 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p1, n1, r, g, b, a);
							t1 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 0.0f, 0.0f);
							v2 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p2, n1, r, g, b, a);
							t2 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 1.0f, 0.0f);
							v3 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p3, n1, r, g, b, a);
							t3 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 1.0f, 1.0f);
							v4 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p4, n1, r, g, b, a);
							t4 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 0.0f, 1.0f);

							m_pRenderer->AddTriangleToMesh(m_pMatrices[matrixIndex].m_meshID, v1, v2, v3);
							m_pRenderer->AddTriangleToMesh(m_pMatrices[matrixIndex].m_meshID, v1, v3, v4);
						}

						p1 = Vector3d(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p2 = Vector3d(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p3 = Vector3d(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p4 = Vector3d(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p5 = Vector3d(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p6 = Vector3d(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p7 = Vector3d(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p8 = Vector3d(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);

						// Back
						if(doZNegative && ((z == 0) || (z > 0 && GetActive(matrixIndex, x, y, z-1) == false)))
						{
							int endX = m_pMatrices[matrixIndex].m_matrixSizeX;
							int endY = m_pMatrices[matrixIndex].m_matrixSizeY;

							UpdateMergedSide(l_merged, matrixIndex, x, y, z, &p6, &p5, &p8, &p7, x, y, endX, endY, false, true, false, false);

							n1 = Vector3d(0.0f, 0.0f, -1.0f);
							v1 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p5, n1, r, g, b, a);
							t1 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 0.0f, 0.0f);
							v2 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p6, n1, r, g, b, a);
							t2 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 1.0f, 0.0f);
							v3 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p7, n1, r, g, b, a);
							t3 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 1.0f, 1.0f);
							v4 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p8, n1, r, g, b, a);
							t4 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 0.0f, 1.0f);

							m_pRenderer->AddTriangleToMesh(m_pMatrices[matrixIndex].m_meshID, v1, v2, v3);
							m_pRenderer->AddTriangleToMesh(m_pMatrices[matrixIndex].m_meshID, v1, v3, v4);
						}

						p1 = Vector3d(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p2 = Vector3d(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p3 = Vector3d(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p4 = Vector3d(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p5 = Vector3d(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p6 = Vector3d(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p7 = Vector3d(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p8 = Vector3d(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);

						// Right
						if(doXPositive && ((x == m_pMatrices[matrixIndex].m_matrixSizeX-1) || (x < m_pMatrices[matrixIndex].m_matrixSizeX-1 && GetActive(matrixIndex, x+1, y, z) == false)))
						{
							int endX = m_pMatrices[matrixIndex].m_matrixSizeZ;
							int endY = m_pMatrices[matrixIndex].m_matrixSizeY;

							UpdateMergedSide(l_merged, matrixIndex, x, y, z, &p5, &p2, &p3, &p8, z, y, endX, endY, true, false, true, false);

							n1 = Vector3d(1.0f, 0.0f, 0.0f);
							v1 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p2, n1, r, g, b, a);
							t1 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 0.0f, 0.0f);
							v2 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p5, n1, r, g, b, a);
							t2 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 1.0f, 0.0f);
							v3 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p8, n1, r, g, b, a);
							t3 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 1.0f, 1.0f);
							v4 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p3, n1, r, g, b, a);
							t4 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 0.0f, 1.0f);

							m_pRenderer->AddTriangleToMesh(m_pMatrices[matrixIndex].m_meshID, v1, v2, v3);
							m_pRenderer->AddTriangleToMesh(m_pMatrices[matrixIndex].m_meshID, v1, v3, v4);
						}

						p1 = Vector3d(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p2 = Vector3d(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p3 = Vector3d(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p4 = Vector3d(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p5 = Vector3d(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p6 = Vector3d(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p7 = Vector3d(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p8 = Vector3d(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);

						// Left
						if(doXNegative && ((x == 0) || (x > 0 && GetActive(matrixIndex, x-1, y, z) == false)))
						{
							int endX = m_pMatrices[matrixIndex].m_matrixSizeZ;
							int endY = m_pMatrices[matrixIndex].m_matrixSizeY;

							UpdateMergedSide(l_merged, matrixIndex, x, y, z, &p6, &p1, &p4, &p7, z, y, endX, endY, false, false, true, false);

							n1 = Vector3d(-1.0f, 0.0f, 0.0f);
							v1 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p6, n1, r, g, b, a);
							t1 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 0.0f, 0.0f);
							v2 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p1, n1, r, g, b, a);
							t2 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 1.0f, 0.0f);
							v3 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p4, n1, r, g, b, a);
							t3 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 1.0f, 1.0f);
							v4 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p7, n1, r, g, b, a);
							t4 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 0.0f, 1.0f);

							m_pRenderer->AddTriangleToMesh(m_pMatrices[matrixIndex].m_meshID, v1, v2, v3);
							m_pRenderer->AddTriangleToMesh(m_pMatrices[matrixIndex].m_meshID, v1, v3, v4);
						}

						p1 = Vector3d(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p2 = Vector3d(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p3 = Vector3d(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p4 = Vector3d(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p5 = Vector3d(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p6 = Vector3d(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p7 = Vector3d(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p8 = Vector3d(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);

						// Top
						if(doYPositive && ((y == m_pMatrices[matrixIndex].m_matrixSizeY-1) || (y < m_pMatrices[matrixIndex].m_matrixSizeY-1 && GetActive(matrixIndex, x, y+1, z) == false)))
						{
							int endX = m_pMatrices[matrixIndex].m_matrixSizeX;
							int endY = m_pMatrices[matrixIndex].m_matrixSizeZ;

							UpdateMergedSide(l_merged, matrixIndex, x, y, z, &p7, &p8, &p3, &p4, x, z, endX, endY, true, false, false, true);

							n1 = Vector3d(0.0f, 1.0f, 0.0f);
							v1 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p4, n1, r, g, b, a);
							t1 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 0.0f, 0.0f);
							v2 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p3, n1, r, g, b, a);
							t2 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 1.0f, 0.0f);
							v3 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p8, n1, r, g, b, a);
							t3 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 1.0f, 1.0f);
							v4 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p7, n1, r, g, b, a);
							t4 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 0.0f, 1.0f);

							m_pRenderer->AddTriangleToMesh(m_pMatrices[matrixIndex].m_meshID, v1, v2, v3);
							m_pRenderer->AddTriangleToMesh(m_pMatrices[matrixIndex].m_meshID, v1, v3, v4);
						}

						p1 = Vector3d(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p2 = Vector3d(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p3 = Vector3d(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p4 = Vector3d(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
						p5 = Vector3d(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p6 = Vector3d(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p7 = Vector3d(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
						p8 = Vector3d(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);

						// Bottom
						if(doYNegative && ((y == 0) || (y > 0 && GetActive(matrixIndex, x, y-1, z) == false)))
						{
							int endX = m_pMatrices[matrixIndex].m_matrixSizeX;
							int endY = m_pMatrices[matrixIndex].m_matrixSizeZ;

							UpdateMergedSide(l_merged, matrixIndex, x, y, z, &p6, &p5, &p2, &p1, x, z, endX, endY, false, false, false, true);

							n1 = Vector3d(0.0f, -1.0f, 0.0f);
							v1 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p6, n1, r, g, b, a);
							t1 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 0.0f, 0.0f);
							v2 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p5, n1, r, g, b, a);
							t2 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 1.0f, 0.0f);
							v3 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p2, n1, r, g, b, a);
							t3 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 1.0f, 1.0f);
							v4 = m_pRenderer->AddVertexToMesh(m_pMatrices[matrixIndex].m_meshID, p1, n1, r, g, b, a);
							t4 = m_pRenderer->AddTextureCoordinatesToMesh(m_pMatrices[matrixIndex].m_meshID, 0.0f, 1.0f);

							m_pRenderer->AddTriangleToMesh(m_pMatrices[matrixIndex].m_meshID, v1, v2, v3);
							m_pRenderer->AddTriangleToMesh(m_pMatrices[matrixIndex].m_meshID, v1, v3, v4);
						}
					}
				}
			}
		}

		m_pRenderer->FinishMesh(m_pMatrices[matrixIndex].m_meshID, -1, m_materialID);

		// Delete the blocks
		for (unsigned int i = 0; i < m_pMatrices[matrixIndex].m_matrixSizeX; i++)
		{
			for (unsigned int j = 0; j < m_pMatrices[matrixIndex].m_matrixSizeY; j++)
			{
				delete [] l_merged[i][j];
			}

			delete [] l_merged[i];
		}
		delete [] l_merged;
	}
}

void QubicleBinary::UpdateMergedSide(int ***merged, int matrixIndex, int blockx, int blocky, int blockz, Vector3d *p1, Vector3d *p2, Vector3d *p3, Vector3d *p4, int startX, int startY, int maxX, int maxY, bool positive, bool zFace, bool xFace, bool yFace)
{
	bool doMore = true;
	unsigned int incrementX = 0;
	unsigned int incrementZ = 0;
	unsigned int incrementY = 0;

	int change = 1;
	if(positive == false)
	{
		//change = -1;
	}

	if(zFace || yFace)
	{
		incrementX = 1;
		incrementY = 1;
	}
	if(xFace)
	{
		incrementZ = 1;
		incrementY = 1;
	}

	// 1st phase
	int incrementer = 1;
	while(doMore)
	{
		if(startX + incrementer >= maxX)
		{
			doMore = false;
		}
		else
		{
			bool doPhase1Merge = true;
			float r1, r2, g1, g2, b1, b2, a1, a2;
			GetColour(matrixIndex, blockx, blocky, blockz, &r1, &g1, &b1, &a1);
			GetColour(matrixIndex, blockx + incrementX, blocky, blockz + incrementZ, &r2, &g2, &b2, &a2);
			//if(m_pBlocks[blockx][blocky][blockz].GetBlockType() != m_pBlocks[blockx + incrementX][blocky][blockz + incrementZ].GetBlockType())
			//{
				// Don't do any phase 1 merging if we don't have the same block type.
			//	doPhase1Merge = false;
			//	doMore = false;
			//}
			/*//else*/ if((r1 != r2 || g1 != g2 || b1 != b2 || a1 != a2) /*&& allMerge == false*/)
			{
				// Don't do any phase 1 merging if we don't have the same colour variation
				doPhase1Merge = false;
				doMore = false;
			}
			else
			{
				/*
				if((xFace && positive && blockx + incrementX+1 == CHUNK_SIZE) ||
				   (xFace && !positive && blockx + incrementX == 0) ||
				   (yFace && positive && blocky+1 == CHUNK_SIZE) ||
				   (yFace && !positive && blocky == 0) ||
				   (zFace && positive && blockz + incrementZ+1 == CHUNK_SIZE) ||
				   (zFace && !positive && blockz + incrementZ == 0))
				{
					doPhase1Merge = false;
					doMore = false;
				}
				// Don't do any phase 1 merging if we find an inactive block or already merged block in our path
				else */if(xFace && positive && (blockx + incrementX+1) < m_pMatrices[matrixIndex].m_matrixSizeX && GetActive(matrixIndex, blockx + incrementX+1, blocky, blockz + incrementZ) == true)
				{
					doPhase1Merge = false;
					doMore = false;
				}
				else if(xFace && !positive && (blockx + incrementX) > 0 && GetActive(matrixIndex, blockx + incrementX-1, blocky, blockz + incrementZ) == true)
				{
					doPhase1Merge = false;
					doMore = false;
				}
				else if(yFace && positive && (blocky+1) < (int)m_pMatrices[matrixIndex].m_matrixSizeY && GetActive(matrixIndex, blockx + incrementX, blocky+1, blockz + incrementZ) == true)
				{
					doPhase1Merge = false;
					doMore = false;
				}
				else if(yFace && !positive && blocky > 0 && GetActive(matrixIndex, blockx + incrementX, blocky-1, blockz + incrementZ) == true)
				{
					doPhase1Merge = false;
					doMore = false;
				}
				else if(zFace && positive && (blockz + incrementZ+1) < m_pMatrices[matrixIndex].m_matrixSizeZ && GetActive(matrixIndex, blockx + incrementX, blocky, blockz + incrementZ+1) == true)
				{
					doPhase1Merge = false;
					doMore = false;
				}
				else if(zFace && !positive && (blockz + incrementZ) > 0 && GetActive(matrixIndex, blockx + incrementX, blocky, blockz + incrementZ-1) == true)
				{
					doPhase1Merge = false;
					doMore = false;
				}
				else if(GetActive(matrixIndex, blockx + incrementX, blocky, blockz + incrementZ) == false)
				{
					doPhase1Merge = false;
					doMore = false;
				}
				else
				{
					if(xFace)
					{
						doPhase1Merge = positive ? (IsMergedXPositive(merged, blockx + incrementX, blocky, blockz + incrementZ) == false) : (IsMergedXNegative(merged, blockx + incrementX, blocky, blockz + incrementZ) == false);
					}
					if(zFace)
					{
						doPhase1Merge = positive ? (IsMergedZPositive(merged, blockx + incrementX, blocky, blockz + incrementZ) == false) : (IsMergedZNegative(merged, blockx + incrementX, blocky, blockz + incrementZ) == false);
					}
					if(yFace)
					{
						doPhase1Merge = positive ? (IsMergedYPositive(merged, blockx + incrementX, blocky, blockz + incrementZ) == false) : (IsMergedYNegative(merged, blockx + incrementX, blocky, blockz + incrementZ) == false);
					}
				}

				if(doPhase1Merge)
				{
					if(zFace || yFace)
					{
						(*p2).x += change * (BLOCK_RENDER_SIZE * 2.0f);
						(*p3).x += change * (BLOCK_RENDER_SIZE * 2.0f);
					}
					if(xFace)
					{
						(*p2).z += change * (BLOCK_RENDER_SIZE * 2.0f);
						(*p3).z += change * (BLOCK_RENDER_SIZE * 2.0f);
					}

					if(positive)
					{
						if(zFace)
						{
							merged[blockx + incrementX][blocky][blockz + incrementZ] |= MergedSide_Z_Positive;
						}
						if(xFace)
						{
							merged[blockx + incrementX][blocky][blockz + incrementZ] |= MergedSide_X_Positive;
						}
						if(yFace)
						{
							merged[blockx + incrementX][blocky][blockz + incrementZ] |= MergedSide_Y_Positive;
						}
					}
					else
					{
						if(zFace)
						{
							merged[blockx + incrementX][blocky][blockz + incrementZ] |= MergedSide_Z_Negative;
						}
						if(xFace)
						{
							merged[blockx + incrementX][blocky][blockz + incrementZ] |= MergedSide_X_Negative;
						}
						if(yFace)
						{
							merged[blockx + incrementX][blocky][blockz + incrementZ] |= MergedSide_Y_Negative;
						}
					}
				}
				else
				{
					doMore = false;
				}
			}
		}

		if(zFace || yFace)
		{
			incrementX += change;
		}
		if(xFace)
		{
			incrementZ += change;
		}

		incrementer += change;
	}


	// 2nd phase
	int loop = incrementer;
	incrementer = 0;
	incrementer = incrementY;

	doMore = true;
	while(doMore)
	{
		if(startY + incrementer >= maxY)
		{
			doMore = false;
		}
		else
		{
			for(int i = 0; i < loop-1; i++)
			{
				// Don't do any phase 2 merging is we have any inactive blocks or already merged blocks on the row
				if(zFace)
				{
					float r1, r2, g1, g2, b1, b2, a1, a2;
					GetColour(matrixIndex, blockx, blocky, blockz, &r1, &g1, &b1, &a1);
					GetColour(matrixIndex, blockx + i, blocky + incrementY, blockz, &r2, &g2, &b2, &a2);

					if(positive && (blockz+1) < (int)m_pMatrices[matrixIndex].m_matrixSizeZ && GetActive(matrixIndex, blockx + i, blocky + incrementY, blockz+1) == true)
					{
						doMore = false;
					}
					else if(!positive && blockz > 0 && GetActive(matrixIndex, blockx + i, blocky + incrementY, blockz-1) == true)
					{
						doMore = false;
					}
					else if(GetActive(matrixIndex, blockx + i, blocky + incrementY, blockz) == false || (positive ? (IsMergedZPositive(merged, blockx + i, blocky + incrementY, blockz) == true) : (IsMergedZNegative(merged, blockx + i, blocky + incrementY, blockz) == true)))
					{
						// Failed active or already merged check
						doMore = false;
					}
					/*else if(m_pBlocks[blockx][blocky][blockz].GetBlockType() != m_pBlocks[blockx + i][blocky + incrementY][blockz].GetBlockType())
					{
						// Failed block type check
						doMore = false;
					}
					*/
					else if((r1 != r2 || g1 != g2 || b1 != b2 || a1 != a2) /*&& allMerge == false*/)
					{
						// Failed colour check
						doMore = false;
					}
				}
				if(xFace)
				{
					float r1, r2, g1, g2, b1, b2, a1, a2;
					GetColour(matrixIndex, blockx, blocky, blockz, &r1, &g1, &b1, &a1);
					GetColour(matrixIndex, blockx, blocky + incrementY, blockz + i, &r2, &g2, &b2, &a2);

					if(positive && (blockx+1) < (int)m_pMatrices[matrixIndex].m_matrixSizeX && GetActive(matrixIndex, blockx+1, blocky + incrementY, blockz + i) == true)
					{
						doMore = false;
					}
					else if(!positive && (blockx) > 0 && GetActive(matrixIndex, blockx-1, blocky + incrementY, blockz + i) == true)
					{
						doMore = false;
					}
					else if(GetActive(matrixIndex, blockx, blocky + incrementY, blockz + i) == false || (positive ? (IsMergedXPositive(merged, blockx, blocky + incrementY, blockz + i) == true) : (IsMergedXNegative(merged, blockx, blocky + incrementY, blockz + i) == true)))
					{
						// Failed active or already merged check
						doMore = false;
					}
					/*else if(m_pBlocks[blockx][blocky][blockz].GetBlockType() != m_pBlocks[blockx][blocky + incrementY][blockz + i].GetBlockType())
					{
						// Failed block type check
						doMore = false;
					}
					*/
					else if((r1 != r2 || g1 != g2 || b1 != b2 || a1 != a2) /*&& allMerge == false*/)
					{
						// Failed colour check
						doMore = false;
					}
				}
				if(yFace)
				{
					float r1, r2, g1, g2, b1, b2, a1, a2;
					GetColour(matrixIndex, blockx, blocky, blockz, &r1, &g1, &b1, &a1);
					GetColour(matrixIndex, blockx + i, blocky, blockz + incrementY, &r2, &g2, &b2, &a2);

					if(positive && (blocky+1) < (int)m_pMatrices[matrixIndex].m_matrixSizeY && GetActive(matrixIndex, blockx + i, blocky+1, blockz + incrementY) == true)
					{
						doMore = false;
					}
					else if(!positive && blocky > 0 && GetActive(matrixIndex, blockx + i, blocky-1, blockz + incrementY) == true)
					{
						doMore = false;
					}
					else if(GetActive(matrixIndex, blockx + i, blocky, blockz + incrementY) == false || (positive ? (IsMergedYPositive(merged, blockx + i, blocky, blockz + incrementY) == true) : (IsMergedYNegative(merged, blockx + i, blocky, blockz + incrementY) == true)))
					{
						// Failed active or already merged check
						doMore = false;
					}
					/*else if(m_pBlocks[blockx][blocky][blockz].GetBlockType() != m_pBlocks[blockx + i][blocky][blockz + incrementY].GetBlockType())
					{
						// Failed block type check
						doMore = false;
					}
					*/
					else if((r1 != r2 || g1 != g2 || b1 != b2 || a1 != a2) /*&& allMerge == false*/)
					{
						// Failed colour check
						doMore = false;
					}
				}
			}

			if(doMore == true)
			{
				if(zFace || xFace)
				{
					(*p3).y += change * (BLOCK_RENDER_SIZE * 2.0f);
					(*p4).y += change * (BLOCK_RENDER_SIZE * 2.0f);
				}
				if(yFace)
				{
					(*p3).z += change * (BLOCK_RENDER_SIZE * 2.0f);
					(*p4).z += change * (BLOCK_RENDER_SIZE * 2.0f);
				}

				for(int i = 0; i < loop-1; i++)
				{
					if(positive)
					{
						if(zFace)
						{
							merged[blockx + i][blocky + incrementY][blockz] |= MergedSide_Z_Positive;
						}
						if(xFace)
						{
							merged[blockx][blocky + incrementY][blockz + i] |= MergedSide_X_Positive;
						}
						if(yFace)
						{
							merged[blockx + i][blocky][blockz + incrementY] |= MergedSide_Y_Positive;
						}
					}
					else
					{
						if(zFace)
						{
							merged[blockx + i][blocky + incrementY][blockz] |= MergedSide_Z_Negative;
						}
						if(xFace)
						{
							merged[blockx][blocky + incrementY][blockz + i] |= MergedSide_X_Negative;
						}
						if(yFace)
						{
							merged[blockx + i][blocky][blockz + incrementY] |= MergedSide_Y_Negative;
						}
					}
				}
			}
		}

		incrementY += change;
		incrementer += change;
	}
}

int QubicleBinary::GetNumMatrices()
{
	return m_numMatrices;
}

QubicleMatrix* QubicleBinary::GetQubicleMatrix(int index)
{
	return &m_pMatrices[index];
}

const char* QubicleBinary::GetMatrixName(int index)
{
	return m_pMatrices[index].m_name;
}

float QubicleBinary::GetMatrixScale(int index)
{
	return m_pMatrices[index].m_scale;
}

Vector3d QubicleBinary::GetMatrixOffset(int index)
{
	return Vector3d(m_pMatrices[index].m_offsetX, m_pMatrices[index].m_offsetY, m_pMatrices[index].m_offsetZ);
}

void QubicleBinary::SetupMatrixBones(MS3DAnimator* pSkeleton)
{
	for(unsigned int i = 0; i < m_numMatrices; i++)
	{
		int boneIndex = pSkeleton->GetModel()->GetBoneIndex(m_pMatrices[i].m_name);

		if(boneIndex != -1)
		{
			m_pMatrices[i].m_boneIndex = boneIndex;
		}
	}
}

void QubicleBinary::SetScaleAndOffsetForMatrix(const char* matrixName, float scale, float xOffset, float yOffset, float zOffset)
{
	for(unsigned int i = 0; i < m_numMatrices; i++)
	{
		if(strcmp(m_pMatrices[i].m_name, matrixName) == 0)
		{
			m_pMatrices[i].m_scale = scale;
			m_pMatrices[i].m_offsetX = xOffset;
			m_pMatrices[i].m_offsetY = yOffset;
			m_pMatrices[i].m_offsetZ = zOffset;
		}
	}
}

float QubicleBinary::GetScale(const char* matrixName)
{
	for(unsigned int i = 0; i < m_numMatrices; i++)
	{
		if(strcmp(m_pMatrices[i].m_name, matrixName) == 0)
		{
			return m_pMatrices[i].m_scale;
		}
	}

	return 1.0f;
}

Vector3d QubicleBinary::GetOffset(const char* matrixName)
{
	for(unsigned int i = 0; i < m_numMatrices; i++)
	{
		if(strcmp(m_pMatrices[i].m_name, matrixName) == 0)
		{
			return Vector3d(m_pMatrices[i].m_offsetX, m_pMatrices[i].m_offsetY, m_pMatrices[i].m_offsetZ);
		}
	}

	return Vector3d(0.0f, 0.0f, 0.0f);
}

void QubicleBinary::SetWireFrameRender(bool wireframe)
{
	m_renderWireFrame = wireframe;
}

void QubicleBinary::Update(float dt)
{

}

void QubicleBinary::Render(bool renderOutline, bool refelction)
{
	m_pRenderer->PushMatrix();
		for(unsigned int i = 0; i < m_numMatrices; i++)
		{
			m_pRenderer->PushMatrix();
				// Scale for external matrix scale value
				m_pRenderer->ScaleWorldMatrix(m_pMatrices[i].m_scale, m_pMatrices[i].m_scale, m_pMatrices[i].m_scale);

				// Translate for initial block offset
				m_pRenderer->TranslateWorldMatrix(0.5f, 0.5f, 0.5f);

				// Translate to center of model
				m_pRenderer->TranslateWorldMatrix(-(float)m_pMatrices[i].m_matrixSizeX*0.5f, -(float)m_pMatrices[i].m_matrixSizeY*0.5f, -(float)m_pMatrices[i].m_matrixSizeZ*0.5f);
				
				// Translate for external matrix offset value
				m_pRenderer->TranslateWorldMatrix(m_pMatrices[i].m_offsetX, m_pMatrices[i].m_offsetY, m_pMatrices[i].m_offsetZ);

				if(renderOutline)
				{
					m_pRenderer->DisableDepthTest();
					m_pRenderer->SetLineWidth(5.0f);
					m_pRenderer->SetCullMode(CM_FRONT);
					m_pRenderer->SetRenderMode(RM_WIREFRAME);
					m_pRenderer->ImmediateColourAlpha(1.0f, 1.0f, 0.0f, 1.0f);
				}
				else if(m_renderWireFrame)
				{
					m_pRenderer->SetLineWidth(1.0f);
					m_pRenderer->SetRenderMode(RM_WIREFRAME);
				}
				else
				{
					m_pRenderer->SetRenderMode(RM_SOLID);
				}

				// Store the model matrix
				if(refelction == false)
				{
					m_pRenderer->GetModelMatrix(&m_pMatrices[i].m_modelMatrix);
				}

				m_pRenderer->PushMatrix();
					m_pRenderer->StartMeshRender();

					// Texture manipulation (for shadow rendering)
					{
						Matrix4x4 worldMatrix;
						m_pRenderer->GetModelMatrix(&worldMatrix);

						m_pRenderer->PushTextureMatrix();
						m_pRenderer->MultiplyWorldMatrix(worldMatrix);
					}

					if(renderOutline)
					{
						m_pRenderer->EndMeshRender();
						m_pRenderer->RenderMesh_NoColour(m_pMatrices[i].m_meshID);
					}
					else
					{
						m_pRenderer->MeshStaticBufferRender(m_pMatrices[i].m_meshID);
					}

					// Texture manipulation (for shadow rendering)
					{
						m_pRenderer->PopTextureMatrix();
					}
					m_pRenderer->EndMeshRender();
				m_pRenderer->PopMatrix();

				if(renderOutline)
				{
					m_pRenderer->SetCullMode(CM_BACK);
					m_pRenderer->EnableDepthTest(DT_LESS);
				}
			m_pRenderer->PopMatrix();
		}
	m_pRenderer->PopMatrix();
}