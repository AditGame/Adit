set(MAIN main.cpp)

add_adit_dir(BlockMgmt
	BlockGrid Chunk ChunkLoader ChunkManager VolumePager)

add_adit_dir(Blocks
	BlockEnum CompositeBlock)

add_adit_dir(Character
	BlockDestructionController CharacterController CharacterStats Movement Player)

add_adit_dir(Engine
	GameEngine)

add_adit_dir(Entities
	Entity)

add_adit_dir(Generation
	GeneratorBase LandGenerator noiseutils)

add_adit_dir(Gui
	GuiEngine GuiManager MYGUIManager)

add_adit_dir(Input
	InputHandler)

add_adit_dir(Physics
	PhysicsEngine SyncedMotionState)

add_adit_dir(Render
	OSGRenderer PlayerCamera RenderBins)

add_adit_dir(Utilities
	Coords Options)

# Set Properties->General->Configuration Type to Application(.exe)
# Creates app.exe with the listed sources (main.cxx)
# Adds sources to the Solution Explorer
add_executable (Adit ${ADIT_FILES} ${MAIN})

target_include_directories(Adit PRIVATE .)

target_link_libraries(Adit ${OPENSCENEGRAPH_LIBRARIES})

add_dependencies(Adit BulletCollision BulletDynamics BulletLinearMath Worldgen osgBullet)

TARGET_LINK_LIBRARIES(Adit
	osgwToolsd.lib
	osgwMxd.lib
	osgdb_osgobjectsd.lib
	osgdb_osgwToolsd.lib
	libnoised.lib
	${MYGUI_LIBRARIES}
	MyGUI.OpenGLPlatform_d.lib
	opengl32
	glu32
	)

# Move the resource directory into the worknig directory so that debugging works right
add_custom_command(TARGET Adit POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources
        .)
