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
	questionData **questions;
	textData *test;
	quoteData **quotes;
	buttonDataText *testButton;
	SDL_Texture *testTex;
	SDL_Rect buttonPos;
	//------------------------------------------------ INITIALISATION -------------------------------------------
	
	optionsFile = loadTextFile(OPTIONS_FILE, &success);
	options = initOptions(optionsFile,&success);
	wind = initSDL(&options, &success);
	render = createRenderer(wind, &success);
	font = loadFont(&options, &success);
	//use below here for testing
	buttonPos.x = 500;
	buttonPos.y = 500;
	fprintf(stderr, "works\n");
	testTex = loadImage("gfx/blank_button.png",render, &buttonPos, &success);
	fprintf(stderr, "works\n");
	questions = loadQuestions("data/Hill 875/questions.json", &success);
	fprintf(stderr, "works\n");
	quotes = loadQuotes("data/Hill 875/quotes.json", &success);
	fprintf(stderr, "works\n");
	test = renderText(font, render, "Allah Ackbar Testa", &success);
	test->dimensions.x = 600;
	test->dimensions.y = 600;
	fprintf(stderr, "works\n");
	testButton = loadButtonText(testTex, &buttonPos, render, "Faggot Button", font, 1 , &success);
	//------------------------------------------------ MAIN LOOP ------------------------------------------------
	fprintf(stderr, "works\n");
	drawMenuElementsText(&testButton, 1, render);
	drawText(test, render);
	drawQuote(quotes, 1, render, font);
	drawQuestion(questions,1, render, font);
	SDL_RenderPresent(render);
	getch();
	//------------------------------------------------ DEINITIALISATION -----------------------------------------
	
	
	return 0;
}