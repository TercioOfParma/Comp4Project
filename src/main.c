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
	//CUSTOM RUNTIME STRUCTURES
	optionsData options;
	activityData *activity;
	quoteListData *quotes;
	//SDL2 + EXTENSION LIBRARIES AND JANSSON STRUCTURES
	SDL_Window *wind;
	SDL_Renderer *render;
	TTF_Font *font;
	SDL_Event eventHandle;
	//------------------------------------------------ INITIALISATION -------------------------------------------
	
	optionsFile = loadTextFile(OPTIONS_FILE, &success);
	options = initOptions(optionsFile,&success);
	wind = initSDL(&options, &success);
	render = createRenderer(wind, &success);
	font = loadFont(&options, &success);
	//use below here for testing
	activity = loadActivity("data/Hill 875/", &success);
	quotes = loadQuoteListData("data/Hill 875/", &success);

	//------------------------------------------------ MAIN LOOP ------------------------------------------------
	while(success != FAIL)
	{
	
		while(SDL_PollEvent(&eventHandle))
		{
			
		
		
		}


		quizTerminated = startQuiz(activity, render, font, &success);
		SDL_RenderPresent(render);
		if(eventHandle.type == SDL_QUIT || quizTerminated == 0)
		{
				success = FAIL;
			
		}
	}
	//------------------------------------------------ DEINITIALISATION -----------------------------------------
	endActivity(activity);
	endSDL(render, wind,font);
	return 0;
}