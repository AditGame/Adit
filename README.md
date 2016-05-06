# Adit

A dwarf-fortress inspired voxel based exploration and management game

## Features

###Current:
* Rendering!
* Block Creation/Destruction
* File saving/loading
* A procedural, infinite terrain
* 4 different types of blocks!

###Future:
* Many more!

## Building

###Requires the following libraries
* OpenSceneGraph, with all extensions
* Libnoise
* Bullet
  * Due to an issue I've encountered, bullet does not actually need to be built. After cmake creates the .vsproj files, Adit only needs a link to the collision and dynamics .vsproj files.
* OsgWorks
* MyGui
  * Build with "Render System 4, OpenGL"

###Requires the following code
* AditGame/WorldGen

###CMake
After all of the above is built correctly, run CMake in the root directory of this repo. It'll generate all the file you need. Simply open the .sln and build everything, no futher work is needed