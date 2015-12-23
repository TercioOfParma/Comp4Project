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
	if(SDL_HasIntersection(&(mouseDimensions), &(button->dimensions)) == SDL_TRUE)
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
	if(SDL_HasIntersection(&(mouseDimensions), &(question->display[answerNo]->dimensions)) == SDL_TRUE)
	{	
		
		return SUCCESS;
	
	}

	else
	{
	
		return FAIL;
	}




}