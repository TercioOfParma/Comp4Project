/*
	FILE : INPUT.C
	PURPOSE : Hold all functions related to input from a keyboard or mouse (and later perhaps client side code)
	VERSION : 0.001
	NOTES: It will be interesting to see how this code turns out
*/
//---------------------------------- C PREPROCESSOR --------------------------

#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif
//------------------------------ FUNCTIONS -----------------------------------
/*
	int checkButtonClicked(SDL_Rect *mouseDimensions, buttonData *button):
	Checks if a mouse clicked a button
*/

int checkButtonClicked( SDL_Rect *mouseDimensions , buttonData *button )
{
	fprintf( stderr , "Checking if a button has been clicked....\n" );
	if(SDL_HasIntersection( mouseDimensions , &( button->dimensions ) ) == SDL_TRUE )
	{	
		return SUCCESS;
	}
	else
	{
		return FAIL;
	}            
	return NULL;
}
/*
	int checkButtonTextClicked(SDL_Rect *mouseDimensions, buttonDataText *button):
	Checks if a mouse clicked a button
*/
int checkButtonTextClicked( SDL_Rect *mouseDimensions , buttonDataText *button )
{
	fprintf( stderr , "Checking if a text button has been clicked....\n" );
	if( SDL_HasIntersection( mouseDimensions , &( button->dimensions ) ) == SDL_TRUE )
	{	
		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
	return NULL;
}
/*
	int checkQuestionClicked(SDL_Rect *mouseDimensions, questionData *button):
	Checks if a mouse clicked a question
*/
int checkQuestionClicked( SDL_Rect *mouseDimensions , questionData *question , int answerNo )
{
	fprintf( stderr , "Checking if an answer has been clicked....\n" );
	if( SDL_HasIntersection( mouseDimensions , &( question->display[ answerNo ]->dimensions ) ) == SDL_TRUE )
	{	
		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
	return NULL;
}
/*
	int checkUnitClicked(SDL_Rect *mouseDimensions, unitData *unit):
	Checks if a mouse clicked a unit
*/
int checkUnitClicked( SDL_Rect *mouseDimensions , unitData *unit )
{
	fprintf( stderr , "Checking if a unit has been clicked: x : %d , y : %d, w : %d, h : %d....\n" , 
	unit->dimensions.x , unit->dimensions.y , unit->dimensions.w , unit->dimensions.h );
	if( SDL_HasIntersection( mouseDimensions , &( unit->dimensions ) ) == SDL_TRUE )
	{	
		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
	return NULL;
}
/*
	int checkUnitClicked(SDL_Rect *mouseDimensions, unitData *unit):
	Checks if a mouse clicked a unit
*/
int checkTileClicked( SDL_Rect *mouseDimensions , tileData *tile )
{
	fprintf( stderr , "Checking if a tile has been clicked : x : %d , y : %d, w : %d, h : %d....\n" , tile->dimensions.x ,
	tile->dimensions.y , tile->dimensions.w , tile->dimensions.h );
	fprintf( stderr , "Mouse Coordinates : x : %d , y : %d, w : %d, h : %d....\n" , mouseDimensions->x , mouseDimensions->y ,
	mouseDimensions->w , mouseDimensions->h );
	if( SDL_HasIntersection( mouseDimensions , &( tile->dimensions ) ) == SDL_TRUE )
	{	
		return SUCCESS;
	}
	else
	{
		fprintf( stderr , "FALSE!\n" );
		return FAIL;
	}
	return NULL;
}
/*
	int handleMouseButtonMainMenu(buttonData **buttons, int size, SDL_Renderer *render):
	Checks the mouse buttons on the main menu and draws them etc.
*/
int handleMouseButtonMainMenu( buttonData **buttons , int size , SDL_Renderer *render , SDL_Event *events )
{
	fprintf( stderr , "Checking main menu....\n" );
	SDL_Rect mouseCoords;
	int i, buttonClickResult;
	mouseCoords.w = 3;
	mouseCoords.h = 3;
	buttonClickResult = 0;
	drawMenuElements( buttons , size , render );
	while( SDL_PollEvent( events ) )
	{
		SDL_GetMouseState( &( mouseCoords.x ) , &( mouseCoords.y ) );
		if( events->type == SDL_MOUSEBUTTONDOWN )
		{
			for( i = 0 ; i < size ; i++ )
			{
				buttonClickResult = checkButtonClicked( &mouseCoords , buttons[i] );
				if( buttonClickResult == SUCCESS )
				{
					break;
				}
			}
			if( buttonClickResult == SUCCESS )
			{
				break;
			}
		}
	}
	if( buttonClickResult == SUCCESS )
	{
		return buttons[i]->type;//this'll be either START_BUTTON or QUIT_BUTTON
	}
	return NO_BUTTON_MAIN;
}
/*
	int handleMouseButtonSelectionMenu(SDL_Rect mouseDimensions,buttonDataText **buttonsText, int size, SDL_Renderer *render):
	Checks the mouse buttons on the secondary menu and draws them etc.


*/
int handleMouseButtonSelectionMenu( buttonDataText **buttonsText , int size , SDL_Renderer *render , SDL_Event *events )
{
	fprintf( stderr , "Checking secondary menu....\n" );
	SDL_Rect mouseCoords;
	int i, buttonClickResult;
	mouseCoords.w = 3;
	mouseCoords.h = 3;
	drawMenuElementsText( buttonsText , size , render );
	buttonClickResult = FAIL;
	while( SDL_PollEvent( events ) )
	{
		SDL_GetMouseState( &( mouseCoords.x ), &( mouseCoords.y ) );
		if( events->type == SDL_MOUSEBUTTONDOWN )
		{
			for( i = 0 ; i < size ; i++ )
			{
				buttonClickResult = checkButtonTextClicked( &mouseCoords , buttonsText[i] );
				buttonClickResult = checkButtonTextClicked( &mouseCoords , buttonsText[i] );
				if( buttonClickResult == SUCCESS )
				{
					return buttonsText[i]->type;
				}
			}
			if( buttonClickResult == SUCCESS )
			{
				break;
			}
		}
	}
	if( buttonClickResult == SUCCESS )
	{
		fprintf( stderr , "Result : %d\n" , buttonClickResult );
		return buttonsText[i]->type;//this'll be either START_BUTTON or QUIT_BUTTON
	}
	return NO_BUTTON_SECONDARY;
}
/*
	int handleMouseButtonMainMenu(SDL_Rect mouseDimensions, buttonData **buttons):
	Checks the keyboard during the simulation
*/
int handleKeyboardSimulation( SDL_Event *keyboardInput , unitData **units )
{
	if( keyboardInput->type == SDL_KEYDOWN )//checks to make sure the key has been pressed during the given event queue
	{
		switch( keyboardInput->key.keysym.sym )//This accesses the key state from inside SDL_Event
		{
			case SDLK_q:
				return QUIT_KEY;
				break;
			default:
				break;
		}
	}

}
/*
	int handleMapClicked(sideData *applicableUnits, sideData *otherSide, tileData **tiles,  buttonData **endTurn, SDL_Event *events):
	handles the units and the map being clicked


*/
int handleMapClicked( sideData *applicableUnits , sideData *otherSide , tileData **tiles ,  buttonData **endTurn , SDL_Event *events )
{
	SDL_Rect mouseCoords;
	int i, buttonClickResultTile, buttonClickResultUnit, buttonClickResultButton;
	mouseCoords.w = 3;
	mouseCoords.h = 3;
	SDL_GetMouseState( &( mouseCoords.x ) , &( mouseCoords.y ) );
	if( events->type == SDL_MOUSEBUTTONDOWN )
	{
		for( i = 1 ; i <= applicableUnits->noUnits ; i++ )
		{	
			buttonClickResultUnit = checkUnitClicked( &mouseCoords , applicableUnits->units[i] );
			if( buttonClickResultUnit == SUCCESS && applicableUnits->units[i]->alive == TRUE )
			{	
				fprintf( stdout , "Unit of %s selected \n" ,  applicableUnits->units[i]->name );
				applicableUnits->units[i]->selected = TRUE;
				return UNIT_SELECTED;
			}
			if( applicableUnits->units[i]->alive == FALSE )
			{
				applicableUnits->units[i]->selected = FALSE;
			}
			buttonClickResultUnit = FAIL;
		}
		for( i = 1 ; i <= otherSide->noUnits ; i++ )
		{	
			buttonClickResultUnit = checkUnitClicked( &mouseCoords , otherSide->units[i] );
			if( buttonClickResultUnit == SUCCESS && otherSide->units[i]->alive == TRUE )
			{	
				fprintf( stdout , "Unit of %s selected for attack\n" ,  otherSide->units[i]->name );
				otherSide->units[i]->selected = TRUE;
				return UNIT_SELECTED_OTHER;
			}
			buttonClickResultUnit = FAIL;
		}
		for( i = 0 ; i < tiles[0]->noTiles ; i++ )
		{	
			fprintf( stderr , "Mouse Coordinates : x : %d , y : %d, w : %d, h : %d \n" , mouseCoords.x , mouseCoords.y , 
			mouseCoords.w , mouseCoords.h );
			buttonClickResultTile = checkTileClicked( &mouseCoords , tiles[i] );
			if( buttonClickResultTile == SUCCESS )
			{
				fprintf( stdout , "Tile clicked! %d %d \n" , tiles[i]->relativeX , tiles[i]->relativeY );
				tiles[i]->isSelected = TRUE;
				return TILE_SELECTED;
			}
			buttonClickResultTile = FAIL;
		}
		buttonClickResultButton = checkButtonClicked( &mouseCoords , endTurn[0] );
		if( buttonClickResultButton == SUCCESS )
		{
			fprintf( stderr , "End of turn button Clicked ! \n" );
			return END_TURN_BUTTON;
		}
	}
	return 0;
}
