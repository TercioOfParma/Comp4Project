/*
	FILE : MAIN.C
	PURPOSE : To contain the main function
	VERSION : 0.001
	NOTES: It works!
*/

//---------------------------------- C PREPROCESSOR --------------------------

#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif

//------------------------------ FUNCTIONS -----------------------------------
/*
	int main(int argc, char *argv[]):
	The big main function that ties it all together

*/

int main(int argc, char *argv[])
{
	
	//------------------------------------------------- VARIABLE DECLARATIONS -----------------------------------
	//PRIMITIVE DATA TYPES
	char *optionsFile;
	int success = SUCCESS;
	int quizTerminated = 1;
	//C Library Structures
	//FILE *errorRedirection;
	//CUSTOM RUNTIME STRUCTURES
	optionsData options;
	mapData *test;
	//SDL2 + EXTENSION LIBRARIES AND JANSSON STRUCTURES
	SDL_Window *wind;
	SDL_Renderer *render;
	TTF_Font *font;
	SDL_Event eventHandle;
	sideData *testSideOne, *testSideTwo;
	//------------------------------------------------ INITIALISATION -------------------------------------------
	//errorRedirection = freopen(LOG_FILE, "w", stderr);//According to the documentation I have read, it isn't an issue on windows stderr isn't a file, and so this evades the race condition issue
	fprintf(stderr, "Main function....\n");
	optionsFile = loadTextFile(OPTIONS_FILE, &success);
	options = initOptions(optionsFile,&success);
	wind = initSDL(&options, &success);
	render = createRenderer(wind, &success);
	font = loadFont(&options, &success);
	//use below here for testing
	test = loadMapData((char *)MAP_FILE,0,render,&success);
	//------------------------------------------------ MAIN LOOP ------------------------------------------------
	while(success != FAIL)
	{
	
		while(SDL_PollEvent(&eventHandle))
		{
			
		
		
		}
		drawTerrain(test->tiles, test->tiles[0]->noTiles, render, test->tileset);
		drawUnits(test->sides[0]->units, test->sides[0]->noUnits, render, test->tileset);
		drawUnits(test->sides[1]->units, test->sides[1]->noUnits, render, test->tileset);
		SDL_RenderPresent(render);
		getch();
		quizTerminated = startQuiz(test->activity, render, font, &success);
		SDL_RenderPresent(render);
		getch();
		if(eventHandle.type == SDL_QUIT || quizTerminated == 0)
		{
				success = FAIL;
			
		}
		
	}
	//------------------------------------------------ DEINITIALISATION -----------------------------------------
	//endActivity(activity);
	//endSideData(testSideOne);
	//endSideData(testSideTwo);
	endSDL(render, wind,font);
	return 0;
}