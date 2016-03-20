/*
	FILE : INPUT.C
	PURPOSE : Hold all functions related to input from a keyboard or mouse (and later perhaps client side code)
	VERSION : 0.001
	NOTES: It will be interesting to see how this code turns out
*/
//==== Preprocessor ========

#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif
//==== Functions =======
/*	===================================================================================
	int checkButtonClicked(SDL_Rect *mouseDimensions, buttonData *button):
	Checks if a mouse clicked a button. 
	
	Returns success if it has
	Returns fail if it doesn't
	Otherwise its NULL_INPUT
	===================================================================================	
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
	return NULL_INPUT;
}
/*	===================================================================================
	int checkButtonTextClicked(SDL_Rect *mouseDimensions, buttonDataText *button):
	Checks if a mouse clicked a button
	
	Returns success if it has
	Returns fail if it doesn't
	Otherwise its NULL_INPUT
	===================================================================================
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
	return NULL_INPUT;
}
/*	===================================================================================
	int checkQuestionClicked(SDL_Rect *mouseDimensions, questionData *button):
	Checks if a mouse clicked a question
	
	Returns success if it has
	Returns fail if it doesn't
	Otherwise its NULL_INPUT
	===================================================================================
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
	return NULL_INPUT;
}
/*
	===================================================================================
	int checkUnitClicked(SDL_Rect *mouseDimensions, unitData *unit):
	Checks if a mouse clicked a unit
	
	Returns success if it has
	Returns fail if it doesn't
	Otherwise its NULL_INPUT
	===================================================================================
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
	return NULL_INPUT;
}
/*	====================================================================================
	int checkTileClicked( SDL_Rect *mouseDimensions , tileData *tile ):
	Checks if a mouse clicked a unit
	
	Returns success if it has
	Returns fail if it doesn't
	Otherwise its NULL_INPUT
	====================================================================================
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
	return NULL_INPUT;
}
/*	====================================================================================
	int handleMouseButtonMainMenu(buttonData **buttons, int size, SDL_Renderer *render):
	Checks the mouse buttons on the main menu and draws them etc.

	Will return the button's type on success
	Otherwise, it will return a NULL_INPUT
	====================================================================================
*/
int handleMouseButtonMainMenu( buttonData **buttons , int size , SDL_Renderer *render , SDL_Event *events )
{
	fprintf( stderr , "Checking main menu....\n" );
	SDL_Rect mouseCoords;
	int i, buttonClickResult;
	//This is big enough to click, but small enough that the user doesn't click anything by accident
	mouseCoords.w = MOUSE_X;
	mouseCoords.h = MOUSE_Y;
	buttonClickResult = FAIL;
	drawMenuElements( buttons , size , render );
	while( SDL_PollEvent( events ) )//For each event, check the mouse coordinates, and see if a button has been clicked
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
	return NULL_INPUT;
}
/*	=========================================================================================
	int handleMouseButtonSelectionMenu(SDL_Rect mouseDimensions,buttonDataText **buttonsText,
	int size, SDL_Renderer *render):
	Checks the mouse buttons on the secondary menu and draws them etc.

	On succrss, it will return the type of the selection button
	Otherwise, it will return NULL_INPUT
	=========================================================================================
*/
int handleMouseButtonSelectionMenu( buttonDataText **buttonsText , int size , SDL_Renderer *render , SDL_Event *events )
{
	fprintf( stderr , "Checking secondary menu....\n" );
	SDL_Rect mouseCoords;
	int i, buttonClickResult;
	mouseCoords.w = MOUSE_X;
	mouseCoords.h = MOUSE_Y;
	drawMenuElementsText( buttonsText , size , render );
	buttonClickResult = FAIL;
	while( SDL_PollEvent( events ) )//For each event, check the mouse coordinates, and see if a button has been clicked
	{
		SDL_GetMouseState( &( mouseCoords.x ), &( mouseCoords.y ) );
		if( events->type == SDL_MOUSEBUTTONDOWN )
		{
			for( i = 0 ; i < size ; i++ )
			{
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
	return NULL_INPUT;
}
/*
	========================================================================================
	int handleMapClicked(sideData *applicableUnits, sideData *otherSide, tileData **tiles,  
	buttonData **endTurn, SDL_Event *events):
	handles the units and the map being clicked

	UNIT_SELECTED will be returned if a friendly unit has been selected
	UNIT_SELECTED_OTHER will be returned if an enemy unit has been selected
	TILE_SELECTED will be returned if a map tile is selected
	END_TURN_BUTTON will be returned if the end turn button is selected
	Otherwise, NULL_INPUT will be returned
	========================================================================================
*/
int handleMapClicked( sideData *applicableUnits , sideData *otherSide , tileData **tiles ,  buttonData **endTurn , SDL_Event *events , int *turnButtonClicked)
{
	SDL_Rect mouseCoords;
	int i, buttonClickResultTile, buttonClickResultUnit, buttonClickResultButton;
	//This is big enough to click, but small enough that the user doesn't click anything by accident
	mouseCoords.w = MOUSE_X;
	mouseCoords.h = MOUSE_Y;
	SDL_GetMouseState( &( mouseCoords.x ) , &( mouseCoords.y ) );
	while(	SDL_PollEvent( events ) )//checks all the input. since this is only run once every 35 miliseconds, 
	//it is neccessary to check everything that may have happened in the last 35 miliseconds and do that rather than processing
	//each time as other functions do
	{
	
	}
	if( events->type == SDL_MOUSEBUTTONDOWN )
	{
		//====================== checks friendly units======================================================
		for( i = 1 ; i <= applicableUnits->noUnits ; i++ )
		{	
			buttonClickResultUnit = checkUnitClicked( &mouseCoords , applicableUnits->units[i] );
			if( buttonClickResultUnit == SUCCESS && applicableUnits->units[i]->alive == TRUE )
			{	
				fprintf( stdout , "Unit of %s selected \nDescription : %s \n" ,  applicableUnits->units[i]->name, applicableUnits->units[i]->description );
				applicableUnits->units[i]->selected = TRUE;
				return UNIT_SELECTED;
			}
			if( applicableUnits->units[i]->alive == FALSE )
			{
				applicableUnits->units[i]->selected = FALSE;
			}
			buttonClickResultUnit = FAIL;
		}
		//======================================checks enemy units=============================
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
		//==========================checks map===========================
		for( i = 1 ; i < tiles[0]->noTiles ; i++ )
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
		buttonClickResultButton = checkButtonClicked( &mouseCoords , endTurn[0] );//checks the next turn button
		if( buttonClickResultButton == SUCCESS && *turnButtonClicked == FALSE )
		{
			fprintf( stderr , "End of turn button Clicked ! \n" );
			*turnButtonClicked = TRUE;
			return END_TURN_BUTTON;
		}
	}
	return NULL_INPUT;
}

/*

	END OF FILE


*/