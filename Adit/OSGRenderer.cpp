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

const int OSGRenderer::BLOCK_WIDTH = 1;

void OSGRenderer::render(BlockGrid& grid)
{
	//Render every block
	ChunkManager* chunkMan = grid.chunkManager;
	//for (ChunkManager::chunkMap_iterator it = chunkMan->chunkMap_begin();;);
}

osg::Geometry* OSGRenderer::createCube()
{
	if (cubeGeometry != nullptr) 
	{ 
		return cubeGeometry;
	}

	cubeGeometry = new osg::Geometry();

		
	//Associate the cube geometry with the cube geode
	//   Add the cube geode to the root node of the scene graph.


	osg::Vec3Array* cubeVertices = new osg::Vec3Array;
	//bottom
	cubeVertices->push_back(osg::Vec3(0,			0,				0			)); // front left
	cubeVertices->push_back(osg::Vec3(BLOCK_WIDTH,	0,				0			)); // front right
	cubeVertices->push_back(osg::Vec3(BLOCK_WIDTH,	BLOCK_WIDTH,	0			)); // back right
	cubeVertices->push_back(osg::Vec3(0,			BLOCK_WIDTH,	0			)); // back left
	//top
	cubeVertices->push_back(osg::Vec3(0,			BLOCK_WIDTH,	BLOCK_WIDTH	)); // back left
	cubeVertices->push_back(osg::Vec3(BLOCK_WIDTH,	BLOCK_WIDTH,	BLOCK_WIDTH	)); // back right
	cubeVertices->push_back(osg::Vec3(BLOCK_WIDTH,	0,				BLOCK_WIDTH	)); // front right
	cubeVertices->push_back(osg::Vec3(0,			0,				BLOCK_WIDTH	)); // front left

	cubeGeometry->setVertexArray(cubeVertices);

	//Next, create a primitive set and add it to the cube geometry.
	//Use the first four points of the cube to define the base using an
	//instance of the DrawElementsUint class. Again this class is derived
	//from the STL vector, so the push_back method will add elements in
	//sequential order. To ensure proper backface cullling, vertices
	//should be specified in counterclockwise order. The arguments for the
	//constructor are the enumerated type for the primitive
	//(same as the OpenGL primitive enumerated types), and the index in
	//the vertex array to start from.

	osg::DrawElementsUInt* cubeBase =
		new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	cubeBase->push_back(3);
	cubeBase->push_back(2);
	cubeBase->push_back(1);
	cubeBase->push_back(0);
	cubeGeometry->addPrimitiveSet(cubeBase);

	osg::DrawElementsUInt* cubeFaceOne =
		new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	cubeFaceOne->push_back(4);
	cubeFaceOne->push_back(5);
	cubeFaceOne->push_back(2);
	cubeFaceOne->push_back(3);
	cubeGeometry->addPrimitiveSet(cubeFaceOne);

	osg::DrawElementsUInt* cubeFaceTwo =
		new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	cubeFaceTwo->push_back(5);
	cubeFaceTwo->push_back(6);
	cubeFaceTwo->push_back(1);
	cubeFaceTwo->push_back(2);
	cubeGeometry->addPrimitiveSet(cubeFaceTwo);

	osg::DrawElementsUInt* cubeFaceThree =
		new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	cubeFaceThree->push_back(6);
	cubeFaceThree->push_back(7);
	cubeFaceThree->push_back(0);
	cubeFaceThree->push_back(1);
	cubeGeometry->addPrimitiveSet(cubeFaceThree);

	osg::DrawElementsUInt* cubeFaceFour =
		new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	cubeFaceFour->push_back(4);
	cubeFaceFour->push_back(3);
	cubeFaceFour->push_back(0);
	cubeFaceFour->push_back(7);
	cubeGeometry->addPrimitiveSet(cubeFaceFour);

	osg::DrawElementsUInt* cubeTop =
		new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	cubeTop->push_back(5);
	cubeTop->push_back(4);
	cubeTop->push_back(7);
	cubeTop->push_back(6);
	cubeGeometry->addPrimitiveSet(cubeTop);

	//Declare and load an array of Vec4 elements to store colors.

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.41f, 0.41f, 0.41f, 1.00f));
	colors->push_back(osg::Vec4(0.0f, 0.41f,  0.41f, 1.00f));
	colors->push_back(osg::Vec4(0.41f, 0.41f, 0.41f, 1.00f));
	colors->push_back(osg::Vec4(0.41f, 0.0f,  0.41f, 1.00f));

														  //The next step is to associate the array of colors with the geometry,
														  //assign the color indices created above to the geometry and set the
														  //binding mode to _PER_VERTEX.

	cubeGeometry->setColorArray(colors);
	cubeGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

	// Since the mapping from vertices to texture coordinates is 1:1, 
	// we don't need to use an index array to map vertices to texture
	// coordinates. We can do it directly with the 'setTexCoordArray' 
	// method of the Geometry class. 
	// This method takes a variable that is an array of two dimensional
	// vectors (osg::Vec2). This variable needs to have the same
	// number of elements as our Geometry has vertices. Each array element
	// defines the texture coordinate for the cooresponding vertex in the
	// vertex array.
	osg::Vec2Array* texcoords = new osg::Vec2Array(8);
	(*texcoords)[0].set(0.00f, 0.0f); // tex coord for vertex 0 
	(*texcoords)[1].set(0.25f, 0.0f); // tex coord for vertex 1 
	(*texcoords)[2].set(0.50f, 0.0f); // ""
	(*texcoords)[3].set(0.75f, 0.0f); // "" 
	(*texcoords)[4].set(0.50f, 1.0f); // ""
	(*texcoords)[5].set(0.50f, 0.0f); // ""
	(*texcoords)[6].set(0.75f, 0.0f); // "" 
	(*texcoords)[7].set(0.50f, 1.0f); // ""
	cubeGeometry->setTexCoordArray(0, texcoords);


	return cubeGeometry;
}

void OSGRenderer::createFace(osg::Geometry* geometry, osg::Vec3d position, OSGRenderer::FACE_SIDE facing)
{
	double x = position.x()*BLOCK_WIDTH;
	double y = position.y()*BLOCK_WIDTH;
	double z = position.z()*BLOCK_WIDTH;

	//get the vertex array
	osg::Vec3Array* cubeVertices;
	if (geometry->getVertexArray() != nullptr)
		cubeVertices = (osg::Vec3Array*)(geometry->getVertexArray());
	else
		cubeVertices = new osg::Vec3Array;

	int prevSize = cubeVertices->size();

	//Create the verticies
	switch(facing)
	{
	case FACE_SIDE::bottom:
		cubeVertices->push_back(osg::Vec3(x, y, z)); // front left
		cubeVertices->push_back(osg::Vec3(x+BLOCK_WIDTH, y, z)); // front right
		cubeVertices->push_back(osg::Vec3(x+BLOCK_WIDTH, y+BLOCK_WIDTH, z)); // back right
		cubeVertices->push_back(osg::Vec3(x, y+BLOCK_WIDTH, z)); // back left
		break;
	case FACE_SIDE::top:
		cubeVertices->push_back(osg::Vec3(x, y + BLOCK_WIDTH, z + BLOCK_WIDTH));
		cubeVertices->push_back(osg::Vec3(x + BLOCK_WIDTH, y + BLOCK_WIDTH, z + BLOCK_WIDTH));
		cubeVertices->push_back(osg::Vec3(x + BLOCK_WIDTH, y, z + BLOCK_WIDTH));
		cubeVertices->push_back(osg::Vec3(x, y, z + BLOCK_WIDTH));
		break;
	case FACE_SIDE::north:
		cubeVertices->push_back(osg::Vec3(x, y + BLOCK_WIDTH, z)); // back left
		cubeVertices->push_back(osg::Vec3(x + BLOCK_WIDTH, y + BLOCK_WIDTH, z)); // back right
		cubeVertices->push_back(osg::Vec3(x + BLOCK_WIDTH, y + BLOCK_WIDTH, z + BLOCK_WIDTH)); // front right
		cubeVertices->push_back(osg::Vec3(x, y + BLOCK_WIDTH, z + BLOCK_WIDTH)); // front left
		break;
	case FACE_SIDE::south:
		cubeVertices->push_back(osg::Vec3(x, y, z + BLOCK_WIDTH)); // front left
		cubeVertices->push_back(osg::Vec3(x + BLOCK_WIDTH, y, z + BLOCK_WIDTH)); // front right
		cubeVertices->push_back(osg::Vec3(x + BLOCK_WIDTH, y, z)); // back right
		cubeVertices->push_back(osg::Vec3(x, y, z)); // back left
		break;
	case FACE_SIDE::west:
		cubeVertices->push_back(osg::Vec3(x, y + BLOCK_WIDTH, z + BLOCK_WIDTH)); // front left
		cubeVertices->push_back(osg::Vec3(x, y, z + BLOCK_WIDTH)); // front right
		cubeVertices->push_back(osg::Vec3(x, y, z)); // back right
		cubeVertices->push_back(osg::Vec3(x, y + BLOCK_WIDTH, z)); // back left
		break;
	case FACE_SIDE::east:
		cubeVertices->push_back(osg::Vec3(x + BLOCK_WIDTH, y + BLOCK_WIDTH, z)); // front left
		cubeVertices->push_back(osg::Vec3(x + BLOCK_WIDTH, y, z)); // front right
		cubeVertices->push_back(osg::Vec3(x + BLOCK_WIDTH, y, z + BLOCK_WIDTH)); // back right
		cubeVertices->push_back(osg::Vec3(x + BLOCK_WIDTH, y + BLOCK_WIDTH, z + BLOCK_WIDTH)); // back left
		break;
	}

	geometry->setVertexArray(cubeVertices);

	//Create the primative face
	osg::DrawElementsUInt* face =
		new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_FAN, 0);
	face->push_back(prevSize+3);
	face->push_back(prevSize+2);
	face->push_back(prevSize+1);
	face->push_back(prevSize);
	geometry->addPrimitiveSet(face);

	
	//get the texture array
	osg::Vec4Array* colors;
	if (geometry->getColorArray() != nullptr)
		colors = (osg::Vec4Array*)(geometry->getColorArray());
	else
		colors = new osg::Vec4Array;

	prevSize = colors->size();

	osg::Vec4 color = osg::Vec4((rand() % 10) / 10.0f, (rand() % 10) / 10.0f, (rand() % 10) / 10.0f, 1.00f);
	colors->push_back(color);
	colors->push_back(color);
	colors->push_back(color);
	colors->push_back(color);

	//The next step is to associate the array of colors with the geometry,
	//assign the color indices created above to the geometry and set the
	//binding mode to _PER_VERTEX.

	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	//get the texture array
	osg::Vec2Array* texcoords;
	if (geometry->getTexCoordArray(0) != nullptr)
		texcoords = (osg::Vec2Array*)(geometry->getTexCoordArray(0));
	else
		texcoords = new osg::Vec2Array;

	 prevSize = texcoords->size();

	//Create the appropriate texture mapings
	texcoords->push_back(osg::Vec2f(0.00f, 0.00f));
	texcoords->push_back(osg::Vec2f(1.00f, 0.00f));
	texcoords->push_back(osg::Vec2f(1.00f, 1.00f));
	texcoords->push_back(osg::Vec2f(0.00f, 1.00f));

	geometry->setTexCoordArray(0, texcoords);
}

bool withinFloatbounds(float val, int com) { return val > (float)com - 0.5f && val < (float)com + 0.5f; }

osg::Geode* OSGRenderer::meshToGeode(PolyVox::Mesh<PolyVox::CubicVertex<CompositeBlock::blockDataType> > &mesh)
{
	using namespace PolyVox;
	using namespace std;
	using namespace osg;

	osg::Geode* geode = new osg::Geode();
	osg::Geometry* geom = new osg::Geometry();

	geom->setUseVertexBufferObjects(true);

	const PolyVox::DefaultIndexType* vecIndices = mesh.getRawIndexData();
	const PolyVox::CubicVertex<CompositeBlock::blockDataType>* vecVertices = mesh.getRawVertexData();

	if (mesh.getNoOfIndices() == 0 || mesh.getNoOfVertices() == 0) return geode;

	osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(GL_TRIANGLES, mesh.getNoOfIndices());

	for (uint i = 0; i < mesh.getNoOfIndices(); i++) {
		(*indices)[i] = vecIndices[i];
	}

	// The vertex array shared by both the polygon and the border
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(mesh.getNoOfVertices());

	for (uint i = 0; i < mesh.getNoOfVertices(); i++) {
		PolyVox::CubicVertex<CompositeBlock::blockDataType> vert0 = vecVertices[i];
		(*vertices)[i].set(vert0.encodedPosition.getX(), vert0.encodedPosition.getY(), vert0.encodedPosition.getZ());

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