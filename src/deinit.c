/*
	FILE : DEINIT.C
	PURPOSE : Hold all functions related to freeing structures and deinitialising libraries unless the library is used exactly one function's scope (such as Jansson)
	VERSION : 0.001
	NOTES: likely to be quite simple with a lot of code repeated down to C's lack of templates and typeless code
*/
//---------------------------------- C PREPROCESSOR --------------------------

#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif
//------------------------------ FUNCTIONS -----------------------------------

/*
	void endQuestions(questionData **questions, int size):
	To deinit questions

*/
void endQuestions(questionData **questions, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		free(questions[i]);
	
	}
	free(questions);
}
/*
	void endActivity(activityData *activity):
	To deinit an activity

*/
void endActivity(activityData *activity)
{
	endQuestions(activity->questions, activity->maximumMark);
	free(activity);

}
/*
	void endSDL(SDL_Renderer *render, SDL_Window *screen, TTF_Font *font):
	To deinit SDL and associated libraries

*/
void endSDL(SDL_Renderer *render, SDL_Window *screen, TTF_Font *font)
{
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(screen);
	TTF_CloseFont(font);
	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();

}
/*
	void endQuotes(quoteData **quotes, int size):
	To deinit the quotes

*/
void endQuotes(quoteData **quotes, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		SDL_DestroyTexture(quotes[i]->display[0]->display);
		SDL_DestroyTexture(quotes[i]->display[1]->display);
		free(quotes[i]);
	}

	free(quotes);

}
/*
	void endQuoteListData(quoteListData *quotes):
	To deinit the quoteList
	NOTE : needs to have the quotes inside deinitialised first
	

*/
void endQuoteListData(quoteListData *quotes)
{
	free(quotes);
}