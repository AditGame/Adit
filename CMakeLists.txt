cmake_minimum_required(VERSION 2.8)

project(Adit)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

# Macros
include(AditMacros)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#sets universal flags
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")

#User Options
set(ADIT_WORLDGEN_INCLUDE "" CACHE FILEPATH "location of the worldgen source")
set(ADIT_WORLDGEN_VSPROJ "" CACHE FILEPATH "Location of the worldgen project file")

set(BULLET_SOURCE $ENV{BULLET_ROOT}/src CACHE PATH "location of the bullet source code")
set(BULLET_BUILD "$ENV{BULLET_ROOT}/build" CACHE PATH "location of bullet build folder")
find_path(BULLET_COLLISION_VSPROJ_PATH NAMES BulletCollision.vcxproj PATHS "${BULLET_BUILD}/src/BulletCollision/")
find_path(BULLET_DYNAMICS_VSPROJ_PATH NAMES BulletDynamics.vcxproj PATHS "${BULLET_BUILD}/src/BulletDynamics/")
find_path(BULLET_LINEARMATH_VSPROJ_PATH NAMES LinearMath.vcxproj PATHS "${BULLET_BUILD}/src/LinearMath/")

set(POLYVOX_INCLUDE_DIR CACHE PATH "Location of Polyvox include files")

find_package(LibNoise REQUIRED)
if(LIBNOISE_INCLUDE_DIR MATCHES LIBNOISE_INCLUDE_DIR-NOTFOUND)
	message(FATAL_ERROR "Adit requires libnoise")
endif()

find_package(OpenSceneGraph REQUIRED osgGA osgDB osgViewer osgText osgUtil OpenThreads)
if(NOT OPENSCENEGRAPH_FOUND)
	message(FATAL_ERROR "Adit requires OpenSceneGraph")
endif()

FIND_PACKAGE( osgWorks REQUIRED )
if( osgWorks_FOUND )
    include( ${osgWorks_USE_FILE} )
    message( "osgWorks found: ${osgWorks_DIR}" )
endif()

#allow absolute paths
cmake_policy(SET CMP0015 OLD)
include_directories(${LIBNOISE_INCLUDE_DIR}/..) #for some reason find_package looks for include/noise, which is not normal convention
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
include_directories(${BULLET_SOURCE})
include_directories(${POLYVOX_INCLUDE_DIR})
link_directories(${LIBNOISE_LIBRARIES})
link_directories(${osgWorks_DIR})
cmake_policy(SET CMP0015 NEW)

#add main project
add_subdirectory(app)

#add all the required bullet libraries
#add_subdirectory($(BULLET_ROOT)/src/BulletCollision)
#add_subdirectory($(BULLET_ROOT)/src/BulletDynamics)

#add osgbullet PATH
add_subdirectory(osgbullet)

#as polyvox multithreaded is header only, just add the src file
target_include_directories(Adit PRIVATE Polyvox-Multithread/src)

target_include_directories(Adit PRIVATE ${ADIT_WORLDGEN_INCLUDE})

target_include_directories(Adit PRIVATE osgBullet)

#add the worldgen project
INCLUDE_EXTERNAL_MSPROJECT(Worldgen ${ADIT_WORLDGEN_VSPROJ})

#Bullet Projects
INCLUDE_EXTERNAL_MSPROJECT(BulletCollision "${BULLET_COLLISION_VSPROJ_PATH}/BulletCollision.vcxproj")
INCLUDE_EXTERNAL_MSPROJECT(BulletDynamics "${BULLET_DYNAMICS_VSPROJ_PATH}/BulletDynamics.vcxproj")
INCLUDE_EXTERNAL_MSPROJECT(BulletLinearMath "${BULLET_LINEARMATH_VSPROJ_PATH}/LinearMath.vcxproj")
