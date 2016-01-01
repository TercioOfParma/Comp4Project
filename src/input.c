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

int checkButtonClicked(SDL_Rect *mouseDimensions, buttonData *button)
{
	fprintf(stderr, "Checking if a button has been clicked....\n");
	if(SDL_HasIntersection(mouseDimensions, &(button->dimensions)) == SDL_TRUE)
	{	
		return SUCCESS;
	}
	else
	{
		return FAIL;
	}

}
/*
	int checkButtonTextClicked(SDL_Rect *mouseDimensions, buttonDataText *button):
	Checks if a mouse clicked a button


*/
int checkButtonTextClicked(SDL_Rect *mouseDimensions, buttonDataText *button)
{
	fprintf(stderr, "Checking if a text button has been clicked....\n");
	if(SDL_HasIntersection(mouseDimensions, &(button->dimensions)) == SDL_TRUE)
	{	
		return SUCCESS;
	}

	else
	{
		return FAIL;
	}

}
/*
	int checkQuestionClicked(SDL_Rect *mouseDimensions, questionData *button):
	Checks if a mouse clicked a question


*/

int checkQuestionClicked(SDL_Rect *mouseDimensions, questionData *question, int answerNo)
{
	fprintf(stderr, "Checking if an answer has been clicked....\n");
	if(SDL_HasIntersection(mouseDimensions, &(question->display[answerNo]->dimensions)) == SDL_TRUE)
	{	
		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
}
/*
	int handleMouseButtonMainMenu(SDL_Rect mouseDimensions, buttonData **buttons):
	Checks the mouse buttons on the main menu and draws them etc.


*/
int handleMouseButtonMainMenu(SDL_Rect mouseDimensions, buttonData **buttons, int size, SDL_Renderer *render)
{
	fprintf(stderr, "Checking main menu....\n");
	SDL_Event eventHandle;
	SDL_Rect mouseCoords;
	int i, buttonClickResult;
	mouseCoords.x = 3;
	mouseCoords.y = 3;
	drawMenuElements(buttons, size, render);
	while(SDL_PollEvent(&eventHandle))
	{
		SDL_GetMouseState(&(mouseCoords.x), &(mouseCoords.y));
		for(i = 0; i < size; i++)
		{
			buttonClickResult = checkButtonClicked(&mouseCoords, buttons[i]);
			if(buttonClickResult == SUCCESS)
			{
				break;
			}
		}
		if(buttonClickResult == SUCCESS)
		{
			return buttons[i]->type;//this'll be either START_BUTTON or QUIT_BUTTON
		}
	}

	return NO_BUTTON_MAIN;
}
/*
	int handleMouseButtonSelectionMenu(SDL_Rect mouseDimensions,buttonDataText **buttonsText, int size, SDL_Renderer *render):
	Checks the mouse buttons on the secondary menu and draws them etc.


*/
int handleMouseButtonSelectionMenu(SDL_Rect mouseDimensions,buttonDataText **buttonsText, int size, SDL_Renderer *render)
{
	fprintf(stderr, "Checking secondary menu....\n");
	SDL_Event eventHandle;
	SDL_Rect mouseCoords;
	int i, buttonClickResult;
	mouseCoords.x = 3;
	mouseCoords.y = 3;
	drawMenuElementsText(buttonsText, size, render);
	while(SDL_PollEvent(&eventHandle))
	{
		SDL_GetMouseState(&(mouseCoords.x), &(mouseCoords.y));
		for(i = 0; i < size; i++)
		{
			buttonClickResult = checkButtonTextClicked(&mouseCoords, buttonsText[i]);
			if(buttonClickResult == SUCCESS)
			{
				break;
			}
		}
		if(buttonClickResult == SUCCESS)
		{
			return buttonsText[i]->type;//this'll be either START_BUTTON or QUIT_BUTTON
		}
	}

	return NO_BUTTON_SECONDARY;


}
/*
	int handleMouseButtonMainMenu(SDL_Rect mouseDimensions, buttonData **buttons):
	Checks the keyboard during the simulation


*/
int handleKeyboardSimulation(SDL_Event *keyboardInput, unitData **units)
{
	if(keyboardInput->type == SDL_KEYDOWN)
	{
		switch(keyboardInput->key.keysym.sym)
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
	int handleMapClicked(unitData **applicableUnits, tileData **tiles,  buttonData **buttons, buttonDataText **textButtons):
	handles the units and the map being clicked


*/
int handleMapClicked(unitData **applicableUnits, tileData **tiles,  buttonData *endTurn)
{




}