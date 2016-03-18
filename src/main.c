/*
	FILE : MAIN.C
	PURPOSE : To contain the main function. This is to initialise the runtime objects, 
	set up the libraries and to run the solution
	simulations + activities
	VERSION : 0.001
	NOTES: Done and dusted
*/

//==== C PREPROCESSOR ====

#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif

//==== FUNCTIONS ======
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
	
	//==== Variable Declarations ====
	//PRIMITIVE DATA TYPES
	char *optionsFile;
	int success = SUCCESS;
	int quizTerminated = FAIL;
	int i;
	int primaryButtonValue, secondaryButtonValue;
	HWND console;
	//==== C Standard Library Data Structures ======
	FILE *errorRedirection;
	//==== Custom Data Structures =====
	optionsData options;
	levelData *levels;
	buttonData **frontButtons;
	buttonData **turnButton;
	buttonDataText **secondaryButtons;
	//==== SDL2 Data Structures =====
	SDL_Window *wind;
	SDL_Renderer *render;
	TTF_Font *font;
	SDL_Event eventHandle;
	SDL_Texture *startButton, *quitButton, *nextTurnButton, *blankButton;
	SDL_Rect placeHolder;
	Mix_Music *soundtrack;
	SDL_Rect backgroundDimensions;
	SDL_Texture *background; 
	
	
	//=== Start up ====
	//moving status window, this is done to make the status windows visible for the user
	console = GetConsoleWindow();
	MoveWindow(console, 0 , 0 , CONSOLESIZE_X , CONSOLESIZE_Y , 1); 
	
	//Handling and testing the error output
	errorRedirection = freopen( LOG_FILE , "w" , stderr );//According to the documentation I have read, it isn't an issue on windows stderr isn't a file, and so this evades the race condition issue
	fprintf( stderr , "Main function....\n" );
	
	//loading options and libraries and initialising them
	/*
	It is important to note here that it would look like that success can be overwritten. However, 
	since, the value is deliberately never set to success outside of main, this is a non issue
	*/
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
	levels = loadLevelData( render , &success );
	frontButtons = malloc( sizeof( buttonData * ) * 2 );
	frontButtons[START_BUTTON] = loadButton( startButton , &placeHolder , START_BUTTON , &success );
	frontButtons[QUIT_BUTTON] = loadButton( quitButton , &placeHolder , QUIT_BUTTON , &success );
	
	
	frontButtons[START_BUTTON]->dimensions.x = BUTTONPOS;
	frontButtons[START_BUTTON]->dimensions.y = BUTTONPOS;
	frontButtons[QUIT_BUTTON]->dimensions.x = BUTTONPOS;
	frontButtons[QUIT_BUTTON]->dimensions.w = frontButtons[QUIT_BUTTON]->dimensions.w * 2;
	frontButtons[START_BUTTON]->dimensions.w = frontButtons[START_BUTTON]->dimensions.w * 2;
	frontButtons[START_BUTTON]->dimensions.h = frontButtons[START_BUTTON]->dimensions.h  * 2;
	frontButtons[QUIT_BUTTON]->dimensions.h = frontButtons[QUIT_BUTTON]->dimensions.h * 2;
	frontButtons[QUIT_BUTTON]->dimensions.y = BUTTONPOS_OFFSET;
	
	placeHolder.x = 10;//Nice position for the next turn button
	placeHolder.y = 10;
	backgroundDimensions.x = 0;//Only good position for a background
	backgroundDimensions.y = 0;
	
	background = loadImage("background.png", render, &backgroundDimensions, &i);
	turnButton = malloc( sizeof( buttonData * ) );
	turnButton[0] = loadButton( nextTurnButton , &placeHolder , END_TURN_BUTTON , &success );
	primaryButtonValue = NULL_INPUT;
	placeHolder.x = BUTTONPOS_OFFSET * 2;
	placeHolder.y = BUTTONPOS;
	secondaryButtons = malloc( sizeof( buttonDataText * ) * levels->noLevels );
	secondaryButtonValue = NULL_INPUT;
	Mix_PlayMusic( soundtrack , -1 );
	
	//Level loading
	if(levels->noLevels <= 0)
	{
		fprintf( stderr ,"FATAL ERROR : No Levels" );
		return 0;
	
	}
	//level button generation
	for( i = 0 ; i <  levels->noLevels ; i++ )
	{
		placeHolder.y = placeHolder.y + 40 ;//Makes it nicely spaced out
		secondaryButtons[i] = loadButtonText( blankButton , &placeHolder , render , levels->maps[i]->title , font , i , &success );
	}
	//------------------------------------------------ MAIN LOOP ------------------------------------------------
	while( success != FAIL )
	{
		//Drawing backgrounds
		SDL_RenderClear( render );
		SDL_RenderCopy( render , background , NULL , NULL );
		//Runs main menu
		if( primaryButtonValue != START_BUTTON)
		{
			primaryButtonValue = handleMouseButtonMainMenu( frontButtons , 2 , render , &eventHandle );
		}
		//Runs secondary selection menu
		if( primaryButtonValue == START_BUTTON && secondaryButtonValue == NULL_INPUT)
		{
			secondaryButtonValue = handleMouseButtonSelectionMenu( secondaryButtons , levels->noLevels ,
			render , &eventHandle );
		}
		//Run a simulation and quiz
		else if( primaryButtonValue == START_BUTTON && secondaryButtonValue != NULL_INPUT )
		{
			simulationMain( levels->maps[ secondaryButtonValue ] , render , font , turnButton , &eventHandle , &success );
			if( success != FAIL )
			{
				quizTerminated = startQuiz( levels->maps[ secondaryButtonValue ]->activity , render , font , &eventHandle , &success );
			}
		}
		if( eventHandle.type == SDL_QUIT || quizTerminated == SUCCESS || primaryButtonValue == QUIT_BUTTON )
		{
				success = FAIL;
		}
		SDL_RenderPresent( render );
	}
	//=== End ====
	endLevel( levels );
	free( frontButtons );
	free( turnButton );
	endSDL( render , wind , font );
	return SUCCESS;
}
/*

	END OF FILE


*/
