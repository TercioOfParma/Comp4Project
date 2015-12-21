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
	//CUSTOM RUNTIME STRUCTURES
	optionsData options;
	//SDL2 + EXTENSION LIBRARIES AND JANSSON STRUCTURES
	SDL_Window *wind;
	SDL_Renderer *render;
	TTF_Font *font;
	SDL_Texture *testTex;
	SDL_Rect testRect;
	buttonDataText *test;
	//------------------------------------------------ INITIALISATION -------------------------------------------
	
	optionsFile = loadTextFile(OPTIONS_FILE, &success);
	options = initOptions(optionsFile,&success);
	wind = initSDL(&options, &success);
	render = createRenderer(wind, &success);
	font = loadFont(&options, &success);
	//use below here for testing
	testRect.x = 100;
	testRect.y = 100;
	testTex = loadImage("gfx/blank_button.png", render,&testRect, &success);
	test = loadButtonText(testTex, &testRect, render, "MEMES GUISE", font, 2, &success);
	//------------------------------------------------ MAIN LOOP ------------------------------------------------
	drawMenuElementsText(&test, 1, render);
	SDL_RenderPresent(render);
	getch();
	//------------------------------------------------ DEINITIALISATION -----------------------------------------
	
	
	return 0;
}