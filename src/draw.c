/*
	FILE : DRAW.C
	PURPOSE : Hold all the drawing and rendering related functions (aside from those that also load, which are in INIT.C)
	VERSION : 0.001
	NOTES: Some functions here are simplistic. However, they take up a long amount of time on systems with an integrated chipset (luckily our spec doesn't have such limitations). These
	will also not work on a software rendering system if you still have one of those
*/
//---------------------------------- C PREPROCESSOR --------------------------

#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif
//------------------------------ FUNCTIONS -----------------------------------
/*
	void drawMenuElements(buttonData **buttons,int size, SDL_Renderer *render):
	Draws an array of regular buttons

*/

void drawMenuElements(buttonData **buttons,int size, SDL_Renderer *render)
{
	int i;
	for(i =0; i < size; i++)
	{
		SDL_RenderCopy(render, buttons[i]->display,NULL,  &(buttons[i]->dimensions));
	}

}