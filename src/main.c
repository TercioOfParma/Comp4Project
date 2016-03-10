/*
	FILE : MAIN.C
	PURPOSE : To contain the main function. This is to initialise the runtime objects, set up the libraries and to run the 
	simulations + activities
	VERSION : 0.001
	NOTES: Remove magic numbers + add comments
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
	It Does the following :
		- Declares and initialises variables and structures
		- Handles initialising libraries and windows
		- Main program loop
		- Deinitialisation

*/

int main( int argc , char *argv[] )
{
	
	//------------------------------------------------- VARIABLE DECLARATIONS -----------------------------------
	//PRIMITIVE DATA TYPES
	char *optionsFile;
	int success = SUCCESS;
	int quizTerminated = 1;
	int i;
	int buttonValuePrimary, buttonValueSecondary;
	HWND console;
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
	Mix_Music *soundtrack;
	SDL_Rect backgroundDimensions;
	SDL_Texture *background; 
	//------------------------------------------------ INITIALISATION -------------------------------------------
	//moving status window, this is done to make the status windows visible for the user
	console = GetConsoleWindow();
	MoveWindow(console, 0 , 0 , CONSOLESIZE_X , CONSOLESIZE_Y , 1); 
	
	//Handling and testing the error output
	errorRedirection = freopen( LOG_FILE , "w" , stderr );//According to the documentation I have read, it isn't an issue on windows stderr isn't a file, and so this evades the race condition issue
	fprintf( stderr , "Main function....\n" );
	
	//loading options and libraries and initialising them
	optionsFile = loadTextFile( OPTIONS_FILE , &success );
	options = initOptions( optionsFile , &success );
	wind = initSDL( &options , &success );
	render = createRenderer( wind , &success );
	font = loadFont( &options , &success );
	soundtrack = loadMusic( "audio/soundtrack.ogg" , &success );
	
	//Button initialisation and background
	startButton = loadImage( STARTBUTTON_PATH , render , &placeHolder , &success );
	quitButton = loadImage( QUITBUTTON_PATH , render , &placeHolder , &success );
	nextTurnButton = loadImage( NEXTTURNBUTTON_PATH , render , &placeHolder , &success );
	blankButton = loadImage( BLANKBUTTON_PATH , render , &placeHolder , &success );
	test = loadLevelData( render , &success );
	frontButtons = malloc( sizeof( buttonData * ) * 2 );
	frontButtons[0] = loadButton( startButton , &placeHolder , START_BUTTON_TYPE , &success );
	frontButtons[1] = loadButton( quitButton , &placeHolder , QUIT_BUTTON_TYPE , &success );
	frontButtons[0]->dimensions.x = BUTTONPOS;
	frontButtons[0]->dimensions.y = BUTTONPOS;
	frontButtons[1]->dimensions.x = BUTTONPOS;
	frontButtons[1]->dimensions.w *= 2;
	frontButtons[0]->dimensions.w *= 2;
	frontButtons[0]->dimensions.h *= 2;
	frontButtons[1]->dimensions.h *= 2;
	frontButtons[1]->dimensions.y = BUTTONPOS_OFFSET;
	placeHolder.x = 10;
	placeHolder.y = 10;
	backgroundDimensions.x = 0;
	backgroundDimensions.y = 0;
	background = loadImage("background.png", render, &backgroundDimensions, &i);
	turnButton = malloc( sizeof( buttonData * ) );
	turnButton[0] = loadButton( nextTurnButton , &placeHolder , END_TURN_BUTTON , &success );
	buttonValuePrimary = 0;
	placeHolder.x = BUTTONPOS_OFFSET * 2;
	placeHolder.y = BUTTONPOS;
	secondaryButtons = malloc( sizeof( buttonDataText * ) * test->noLevels );
	buttonValueSecondary = NO_BUTTON_SECONDARY;
	Mix_PlayMusic( soundtrack , -1 );
	
	//Level loading
	if(test->noLevels <= 0)
	{
		fprintf( stderr ,"FATAL ERROR : No Levels" );
		return 0;
	
	}
	//level button generation
	for( i = 0 ; i <  test->noLevels ; i++ )
	{
		placeHolder.y += 40 ;
		secondaryButtons[i] = loadButtonText( blankButton , &placeHolder , render , test->maps[i]->title , font , i , &success );
	}
	//------------------------------------------------ MAIN LOOP ------------------------------------------------
	while( success != FAIL )
	{
		//Drawing backgrounds
		SDL_RenderClear( render );
		SDL_RenderCopy( render , background , NULL , NULL );
		//Runs main menu
		if( buttonValuePrimary != START_BUTTON_TYPE )
		{
			buttonValuePrimary = handleMouseButtonMainMenu( frontButtons , 2 , render , &eventHandle );
		}
		//Runs secondary selection menu
		if( buttonValuePrimary == START_BUTTON_TYPE && buttonValueSecondary == NO_BUTTON_SECONDARY )
		{
			buttonValueSecondary = handleMouseButtonSelectionMenu( secondaryButtons , test->noLevels ,
			render , &eventHandle );
		}
		//Run a simulation and quiz
		else if( buttonValuePrimary == START_BUTTON_TYPE && buttonValueSecondary != NO_BUTTON_SECONDARY )
		{
			simulationMain( test->maps[ buttonValueSecondary ] , render , font , turnButton , &eventHandle , &success );
			if( success != FAIL )
			{
				quizTerminated = startQuiz( test->maps[ buttonValueSecondary ]->activity , render , font , &eventHandle , &success );
			}
		}
		if( eventHandle.type == SDL_QUIT || quizTerminated == 0 || buttonValuePrimary == QUIT_BUTTON_TYPE )
		{
				success = FAIL;
		}
		SDL_RenderPresent( render );
	}
	//------------------------------------------------ DEINITIALISATION -----------------------------------------
	endLevel( test );
	endSDL( render , wind , font );
	return 0;
}
/*

	END OF FILE


*/
