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
	int i;
	int buttonValuePrimary, buttonValueSecondary;
	//C Library Structures
	FILE *errorRedirection;
	//CUSTOM RUNTIME STRUCTURES
	optionsData options;
	levelData *test;
	buttonData **frontButtons;
	buttonData **turnButton;
	buttonDataText **secondaryButtons;
	//SDL2 + EXTENSION LIBRARIES AND JANSSON STRUCTURES
	SDL_Window *wind;
	SDL_Renderer *render;
	TTF_Font *font;
	SDL_Event eventHandle;
	SDL_Texture *startButton, *quitButton, *nextTurnButton, *blankButton;
	SDL_Rect placeHolder;
	//------------------------------------------------ INITIALISATION -------------------------------------------
	errorRedirection = freopen(LOG_FILE, "w", stderr);//According to the documentation I have read, it isn't an issue on windows stderr isn't a file, and so this evades the race condition issue
	fprintf(stderr, "Main function....\n");
	optionsFile = loadTextFile(OPTIONS_FILE, &success);
	options = initOptions(optionsFile,&success);
	wind = initSDL(&options, &success);
	render = createRenderer(wind, &success);
	font = loadFont(&options, &success);
	//use below here for testing
	startButton = loadImage(STARTBUTTON_PATH, render, &placeHolder, &success);
	quitButton = loadImage(QUITBUTTON_PATH, render, &placeHolder, &success);
	nextTurnButton = loadImage(NEXTTURNBUTTON_PATH, render, &placeHolder, &success);
	blankButton = loadImage(BLANKBUTTON_PATH, render, &placeHolder, &success);
	test = loadLevelData(render, &success);//buttonData *loadButton(SDL_Texture *display, SDL_Rect *posAndSize, int type, int *success)
	frontButtons = malloc(sizeof(buttonData *) * 2);
	frontButtons[0] = loadButton(startButton, &placeHolder, START_BUTTON_TYPE, &success);
	frontButtons[1] = loadButton(quitButton, &placeHolder, QUIT_BUTTON_TYPE, &success);
	frontButtons[0]->dimensions.x = 100;
	frontButtons[0]->dimensions.y = 100;
	frontButtons[1]->dimensions.x = 100;
	frontButtons[1]->dimensions.y = 300;
	placeHolder.x = 10;
	placeHolder.y = 10;
	
	turnButton = malloc(sizeof(buttonData *));
	turnButton[0] = loadButton(nextTurnButton, &placeHolder, END_TURN_BUTTON, &success);
	buttonValuePrimary = 0;
	placeHolder.x = 500;
	placeHolder.y = 100;
	secondaryButtons = malloc(sizeof(buttonDataText *) * test->noLevels);
	buttonValueSecondary = NO_BUTTON_SECONDARY;
	for(i = 0; i <  test->noLevels; i++)
	{
		placeHolder.y += 40;
		secondaryButtons[i] = loadButtonText(blankButton, &placeHolder, render, test->maps[i]->title, font, i, &success);
	}
	//------------------------------------------------ MAIN LOOP ------------------------------------------------
	while(success != FAIL)
	{
		SDL_RenderClear(render);
		if(buttonValuePrimary != START_BUTTON_TYPE)
		{
			buttonValuePrimary = handleMouseButtonMainMenu(frontButtons, 2, render, &eventHandle);
		}
		if(buttonValuePrimary == START_BUTTON_TYPE && buttonValueSecondary == NO_BUTTON_SECONDARY )
		{
			buttonValueSecondary = handleMouseButtonSelectionMenu(secondaryButtons, test->noLevels, render, &eventHandle);
		}
		else if(buttonValuePrimary == START_BUTTON_TYPE && buttonValueSecondary != NO_BUTTON_SECONDARY)
		{
			simulationMain(test->maps[buttonValueSecondary],render, font, turnButton, &eventHandle, &success);
			if(success != FAIL)
			{
				startQuiz(test->maps[buttonValueSecondary]->activity, render, font, &eventHandle, &success);
			}
		}
		if(eventHandle.type == SDL_QUIT || quizTerminated == 0 || buttonValuePrimary == QUIT_BUTTON_TYPE)
		{
				success = FAIL;
			
		}
		SDL_RenderPresent(render);
	}
	//------------------------------------------------ DEINITIALISATION -----------------------------------------
	endLevel(test);
	endSDL(render, wind,font);
	return 0;
}