#include "OSGRenderer.h"

#include <osg/Geometry>
#include <osg/Geode>
#include <osgUtil\Simplifier>
#include <osg/Material>
#include <osg/Shader>
#include <osg/ShadeModel>
#include <osgDB/FileUtils>
#include <osg/ValueObject>

#include <cstdlib>

#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/MarchingCubesSurfaceExtractor.h"

OSGRenderer::OSGRenderer()
{
}

osg::Geometry* OSGRenderer::cubeGeometry = nullptr;
osg::Material* OSGRenderer::_blockMaterial = nullptr;
osg::Program* OSGRenderer::_blockProgram = nullptr;

OSGRenderer::~OSGRenderer()
{
}

void OSGRenderer::setUp()
{
	// create white material
	_blockMaterial = new osg::Material();
	_blockMaterial->setDiffuse(osg::Material::FRONT, osg::Vec4(0.41, 0.41, 0.41, 1.0));
	_blockMaterial->setSpecular(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
	_blockMaterial->setAmbient(osg::Material::FRONT, osg::Vec4(0.1, 0.1, 0.1, 1.0));
	_blockMaterial->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
	_blockMaterial->setShininess(osg::Material::FRONT, 25.0);

	_blockProgram = new osg::Program;
	osg::Shader* brickVertexObject =
		new osg::Shader(osg::Shader::VERTEX);
	osg::Shader* brickFragmentObject =
		new osg::Shader(osg::Shader::FRAGMENT);

	_blockProgram->addShader(brickFragmentObject);
	_blockProgram->addShader(brickVertexObject);
	loadShaderSource(brickVertexObject, "shaders/brick.vert");
	loadShaderSource(brickFragmentObject, "shaders/brick.frag");
}

const float OSGRenderer::BLOCK_WIDTH = 0.5f;

void OSGRenderer::render(BlockGrid& grid)
{
	//Render every block
	ChunkManager* chunkMan = grid.chunkManager;
	//for (ChunkManager::chunkMap_iterator it = chunkMan->chunkMap_begin();;);
}

bool withinFloatbounds(float val, int com) { return val > (float)com - 0.5f && val < (float)com + 0.5f; }

osg::Geode* OSGRenderer::meshToGeode(PolyVox::Mesh<PolyVox::CubicVertex<CompositeBlock::blockDataType> > &mesh)
{
	using namespace PolyVox;
	using namespace std;
	using namespace osg;

	const PolyVox::DefaultIndexType* vecIndices = mesh.getRawIndexData();
	const PolyVox::CubicVertex<CompositeBlock::blockDataType>* vecVertices = mesh.getRawVertexData();

	if (mesh.getNoOfIndices() == 0 || mesh.getNoOfVertices() == 0) 
		return nullptr;

	osg::Geode* geode = new osg::Geode();
	osg::Geometry* geom = new osg::Geometry();

	geom->setUseVertexBufferObjects(true);

	osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(GL_TRIANGLES, mesh.getNoOfIndices());

	for (uint i = 0; i < mesh.getNoOfIndices(); i++) {
		(*indices)[i] = vecIndices[i];
	}

	// The vertex array shared by both the polygon and the border
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(mesh.getNoOfVertices());

	for (uint i = 0; i < mesh.getNoOfVertices(); i++) {
		PolyVox::CubicVertex<CompositeBlock::blockDataType> vert0 = vecVertices[i];
		(*vertices)[i].set(vert0.encodedPosition.getX()*BLOCK_WIDTH, vert0.encodedPosition.getY()*BLOCK_WIDTH, vert0.encodedPosition.getZ()*BLOCK_WIDTH);

	}

	// The color array
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(mesh.getNoOfVertices());

	for (uint i = 0; i < mesh.getNoOfVertices(); i++) {
		PolyVox::CubicVertex<CompositeBlock::blockDataType> vert0 = vecVertices[i];
		if(withinFloatbounds(vert0.data,BlockType::BlockType_Stone))
			(*colors)[i].set(0.41f, 0.41f, 0.41f, 0.0f);
		else if (withinFloatbounds(vert0.data, BlockType::BlockType_Grass))
			(*colors)[i].set(0.22f, 0.36f, 0.20f, 0.0f);
		else if (withinFloatbounds(vert0.data, BlockType::BlockType_Dirt))
			(*colors)[i].set(0.53f, 0.26f, 0.12f, 0.0f);
		else
			(*colors)[i].set(0.0f, 0.0f, 1.0f, 0.0f);
		//std::cout << vert0.getMaterial() << std::endl;
	}

	osg::StateSet* brickState = geom->getOrCreateStateSet();
	brickState->setAttributeAndModes(_blockProgram, osg::StateAttribute::ON);


	// Construct the polygon geometry

	geom->setStateSet(brickState);
	geom->setDataVariance(osg::Object::DYNAMIC);
	geom->setUseDisplayList(false);
	geom->setUseVertexBufferObjects(true);
	geom->setVertexArray(vertices.get());

	geom->addPrimitiveSet(indices.get());

	geom->setColorArray(colors);
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	//for (int x = 0; x < Chunk::chunkWidth; x++)
	//{
	//	for (int y = 0; y < Chunk::chunkWidth; y++)
	//	{
	//		for (int z = 0; z < Chunk::chunkHeight; z++)
	//		{
	//			osg::Vec3d currPosition(x, y, z);
	//			if (!chunk->getBlock(Coords(x, y, z)).isSolid())
	//				continue; //ignore unsolid blocks

	//			if (!Chunk::isInBounds(Coords(x + 1, y, z)) || !chunk->getBlock(Coords(x + 1, y, z)).isSolid())
	//				createFace(geo, currPosition, FACE_SIDE::east);
	//			if (!Chunk::isInBounds(Coords(x - 1, y, z)) || !chunk->getBlock(Coords(x - 1, y, z)).isSolid())
	//				createFace(geo, currPosition, FACE_SIDE::west);
	//			if (!Chunk::isInBounds(Coords(x, y + 1, z)) || !chunk->getBlock(Coords(x, y + 1, z)).isSolid())
	//				createFace(geo, currPosition, FACE_SIDE::north);
	//			if (!Chunk::isInBounds(Coords(x, y - 1, z)) || !chunk->getBlock(Coords(x, y - 1, z)).isSolid())
	//				createFace(geo, currPosition, FACE_SIDE::south);
	//			if (!Chunk::isInBounds(Coords(x, y, z + 1)) || !chunk->getBlock(Coords(x, y, z + 1)).isSolid())
	//				createFace(geo, currPosition, FACE_SIDE::top);
	//			if (!Chunk::isInBounds(Coords(x, y, z - 1)) || !chunk->getBlock(Coords(x, y, z - 1)).isSolid())
	//				createFace(geo, currPosition, FACE_SIDE::bottom);
	//		}
	//	}
	//}

	////don't render empty chunks
	if (geom->getVertexArray() != nullptr)
	{
		//osgUtil::Simplifier sim;
		//sim.simplify(*geo);
		geode->addDrawable(geom);
	}

	return geode;
}

bool OSGRenderer::loadShaderSource(osg::Shader* obj, const std::string& fileName)
{
	std::string fqFileName = osgDB::findDataFile(fileName);
	if (fqFileName.length() == 0)
	{
		std::cout << "File \"" << fileName << "\" not found." << std::endl;
		return false;
	}
	bool success = obj->loadShaderSourceFromFile(fqFileName.c_str());
	if (!success)
	{
		std::cout << "Couldn't load file: " << fileName << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}