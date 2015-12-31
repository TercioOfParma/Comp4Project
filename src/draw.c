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
	fprintf(stderr, "Drawing buttons....\n");
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
	fprintf(stderr, "Rendering text....\n");
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
	fprintf(stderr, "Drawing text....\n");
	SDL_RenderCopy(render, toDraw->display, NULL, &(toDraw->dimensions));

}
/*
	void drawMenuElementsText(buttonDataText **buttons, int size, SDL_Renderer *render):
	draws a buttonData structure and associated textData

*/
void drawMenuElementsText(buttonDataText **buttons, int size, SDL_Renderer *render)
{
	fprintf(stderr, "Drawing buttons with text....\n");
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
	fprintf(stderr, "Drawing quote....\n");
	int wasSuccess = SUCCESS;
	quotes[quoteNo]->display[QUOTE_POS] = renderText(font, render, (quotes[quoteNo]->quote), &wasSuccess);
	quotes[quoteNo]->display[ANALYSIS_POS] = renderText(font, render, (quotes[quoteNo]->analysis), &wasSuccess);
	if(wasSuccess == FAIL)
	{
		fprintf(stderr, "One of the quote components has failed, please check the file and your computer\n");//NON CRITICAL FAILURE
		return;
	
	}
	quotes[quoteNo]->display[QUOTE_POS]->dimensions.x = QUOTATION_XPOS;
	quotes[quoteNo]->display[QUOTE_POS]->dimensions.y = QUOTATION_YPOS;
	quotes[quoteNo]->display[ANALYSIS_POS]->dimensions.x = QUOTATION_XPOS;
	quotes[quoteNo]->display[ANALYSIS_POS]->dimensions.y = QUOTATION_YPOS + 100;
	drawText(quotes[quoteNo]->display[QUOTE_POS], render);
	drawText(quotes[quoteNo]->display[ANALYSIS_POS], render);
}
/*
	void drawQuestion(questionData **questions, int quoteNo, SDL_Renderer *render):
	Draws a quote an its analysis in a set place, will be done at the end of a turn

*/
void drawQuestion(questionData **questions, int questionNo, SDL_Renderer *render, TTF_Font *font)
{
	fprintf(stderr, "Drawing question and answers....\n");
	int wasSuccess = SUCCESS;
	int i;
	questions[questionNo]->display[QUESTION_POS] = renderText(font, render, questions[questionNo]->question, &wasSuccess);
	questions[questionNo]->display[QUESTION_POS]->dimensions.x = QUESTION_XPOS;
	questions[questionNo]->display[QUESTION_POS]->dimensions.y = QUESTION_YPOS;
	drawText(questions[questionNo]->display[QUESTION_POS], render);
	for(i = 0; i < NO_ANSWERS; i++)
	{
		questions[questionNo]->display[i + 1]= renderText(font, render, questions[questionNo]->answers[i], &wasSuccess);
		questions[questionNo]->display[i + 1]->dimensions.x = QUESTION_XPOS;
		questions[questionNo]->display[i + 1]->dimensions.y = QUESTION_YPOS + (50 * (i + 1));
		drawText(questions[questionNo]->display[i + 1], render);
	}

}
/*
	void drawTerrain(tileData **toDraw, int size, SDL_Renderer *render, SDL_Texture *tileMap):
	Draws a map tile from the base texture that has all the textures

*/
void drawTerrain(tileData **toDraw, int size, SDL_Renderer *render, SDL_Texture *tileMap)
{
	fprintf(stderr, "Drawing terrain....\n");
	int i;
	for(i = 0; i < size; i++)
	{
		SDL_RenderCopyEx(render, tileMap, &(toDraw[i]->spriteDimensions), &(toDraw[i]->dimensions), toDraw[i]->angle, NULL, SDL_FLIP_NONE);
	}
}
/*
	void drawUnits(unitData *toDraw, int size,  SDL_Renderer *render):
	Draws a unit tile from the base texture that has all the textures

*/
void drawUnits(unitData **toDraw, int size, SDL_Renderer *render, SDL_Texture *tileMap)
{
	fprintf(stderr, "Drawing units....\n");
	int i;
	for(i = 1; i <= size; i++)
	{
		if(toDraw[i]->alive = TRUE)
		{
			SDL_RenderCopy(render, tileMap, &(toDraw[i]->spriteDimensions), &(toDraw[i]->dimensions));
		}
	}
}