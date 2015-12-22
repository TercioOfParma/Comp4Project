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
/*
	textData *renderText(TTF_Font *font, SDL_Renderer *render, const char *stringToTexture):
	Creates a textData structure, and gives it a texture from the instructed text

*/
textData *renderText(TTF_Font *font, SDL_Renderer *render, const char *stringToTexture ,int *success)
{

	textData *temp = malloc(sizeof(textData));
	SDL_Surface *tempSurf;
	SDL_Texture *tempTex;
	char renderableText [MAX_TEXT_OUTPUT];
	snprintf(renderableText, MAX_TEXT_OUTPUT, "%s", stringToTexture);//no sprintf overflows here 
	tempSurf = TTF_RenderText_Solid(font, renderableText, DEFAULT_TEXT);
	if(!tempSurf)
	{
		fprintf(stderr, "TTF_RenderText_Solid has failed : %s \n", SDL_GetError());
		*success = FAIL;
		return NULL;
	}
	tempTex = SDL_CreateTextureFromSurface(render, tempSurf);
	if(!tempTex)
	{
		fprintf(stderr, "SDL_CreateTextureFromSurface has failed : %s \n", SDL_GetError());
		*success = FAIL;
		return NULL;
	
	}
	temp->display = tempTex;
	temp->dimensions.w = tempSurf->w;
	temp->dimensions.h = tempSurf->h;
	SDL_FreeSurface(tempSurf);
	return temp; //cannot simply destroy since then the address would then be hanging
}
/*
	void drawText(textData *toDraw, SDL_Renderer *render):
	draws a textData structure, (effectively a glorified wrapper function at this point

*/
void drawText(textData *toDraw, SDL_Renderer *render)
{
	SDL_RenderCopy(render, toDraw->display, NULL, &(toDraw->dimensions));

}
/*
	void drawMenuElementsText(buttonDataText **buttons, int size, SDL_Renderer *render):
	draws a buttonData structure and associated textData

*/
void drawMenuElementsText(buttonDataText **buttons, int size, SDL_Renderer *render)
{
	int i;
	for(i = 0; i < size; i++)
	{
		SDL_RenderCopy(render, buttons[i]->display,NULL,  &(buttons[i]->dimensions));
		drawText(buttons[i]->details, render);//need to give it the textData address

	}
}
/*
	void drawQuote(quoteData **quotes, int quoteNo, SDL_Renderer *render, TTF_Font *font):
	Draws a quote an its analysis in a set place, will be done at the end of a turn

*/
void drawQuote(quoteData **quotes, int quoteNo, SDL_Renderer *render, TTF_Font *font)
{
	textData *quote, *analysis;
	int wasSuccess = SUCCESS;
	quote = renderText(font, render, (quotes[quoteNo]->quote), &wasSuccess);
	analysis = renderText(font, render, (quotes[quoteNo]->analysis), &wasSuccess);
	if(wasSuccess == FAIL)
	{
		fprintf(stderr, "One of the quote components has failed, please check the file and your computer\n");//NON CRITICAL FAILURE
		return;
	
	}
	quote->dimensions.x = QUOTATION_XPOS;
	quote->dimensions.y = QUOTATION_YPOS;
	analysis->dimensions.x = QUOTATION_XPOS;
	analysis->dimensions.y = QUOTATION_YPOS + 200;
	drawText(quote, render);
	drawText(analysis, render);
	SDL_DestroyTexture(quote->display);
	SDL_DestroyTexture(analysis->display);
	free(quote);
	free(analysis);
}
/*
	void drawQuestion(questionData **questions, int quoteNo, SDL_Renderer *render):
	Draws a quote an its analysis in a set place, will be done at the end of a turn

*/
void drawQuestion(questionData **questions, int quoteNo, SDL_Renderer *render, TTF_Font *font)
{
	textData *question, *answer;
	int wasSuccess = SUCCESS;
	int i;
	question = renderText(font, render, questions[quoteNo]->question, &wasSuccess);
	question->dimensions.x = QUESTION_XPOS;
	question->dimensions.y = QUESTION_YPOS;
	drawText(question, render);
	SDL_DestroyTexture(question->display);
	free(question);
	for(i = 0; i < NO_ANSWERS; i++)
	{
		answer = renderText(font, render, questions[quoteNo]->answers[i], &wasSuccess);
		answer->dimensions.x = QUESTION_XPOS;
		answer->dimensions.y = QUESTION_YPOS + (50 * (i + 1));
		drawText(answer, render);
	}

}
