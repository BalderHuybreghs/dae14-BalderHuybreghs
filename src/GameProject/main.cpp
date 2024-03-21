#include "pch.h"
#include <ctime>
#include "Game.h"
#include "EditorScreen.h"
#include "GameDefines.h"
#include "PlayingScreen.h"
#include "LoadingScreen.h"

void StartHeapControl();
void DumpMemoryLeaks();

int SDL_main(int argv, char** args)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	StartHeapControl();

  GameScreen* startScreenPtr{ new LoadingScreen{ new PlayingScreen(), 0 } };
	Game* pGame{ new Game{ Window{ PROJECT_NAME, WINDOW_WIDTH , WINDOW_HEIGHT }, startScreenPtr } };
	pGame->Run();
	delete pGame;

	DumpMemoryLeaks();
	return 0;
}


void StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc( 156 );
#endif
}

void DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}


