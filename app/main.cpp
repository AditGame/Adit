
#include <iostream>

#include "Engine/GameEngine.h"

int go(int argc, char** argv)
{
	GameEngine* eng = &GameEngine::inst();
	eng->setup();
	eng->go();
	return 0;
}

int main(int argc, char** argv)
{
	int returnCode = go(argc,argv);
	//system("pause");
	return returnCode;
}