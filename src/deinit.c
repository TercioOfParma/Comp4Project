/*
	FILE : DEINIT.C
	PURPOSE : Hold all functions related to freeing structures and deinitialising libraries unless the library is used exactly one function's scope (such as Jansson)
	VERSION : 0.001
	NOTES: likely to be quite simple with a lot of code repeated down to C's lack of templates and typeless code. Strong use of memory management throughout
	all void so no return types
*/
//============ Preprocessor =================

#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif
//============ Functions ================

/*
	==================================================================================
	void endQuestions(questionData **questions, int size):
	To deinit questions
	FOR ALL THOSE THAT DEINITIALISE ARRAYS:
	questions[i] or whatever it's called within these functions refers to the memory
	pointed to in the pointer array, questions is simply the 2d pointer pointing to the now
	hanging pointers
	==================================================================================
*/
void endQuestions( questionData **questions , int size )
{
	fprintf( stderr , "Deinitialising questions....\n" );
	int i;
	for( i = 0 ; i < size ; i++ )
	{
		free( questions[i] );
	
	}
	free( questions );
}
/*
	=====================================================================
	void endActivity(activityData *activity):
	To deinit an activity and questions subordinate.
	
	=====================================================================

*/
void endActivity( activityData *activity )
{
	fprintf( stderr , "Deinitialising Activity....\n" );
	endQuestions( activity->questions , activity->maximumMark );
	free( activity );

}
/*
	======================================================================
	void endSDL(SDL_Renderer *render, SDL_Window *screen, TTF_Font *font):
	To deinit SDL and associated libraries
	
	======================================================================
*/
void endSDL( SDL_Renderer *render , SDL_Window *screen , TTF_Font *font )
{
	fprintf( stderr , "Deinitialising SDL2, extensions libraries, hardware rendering and window....\n" );
	//runtime errors
	SDL_DestroyRenderer( render );//Hardware renderer
	SDL_DestroyWindow( screen );//Main window
	TTF_CloseFont( font );//close the TTF font
	//other bits
	IMG_Quit();//Deinit SDL_Image
	Mix_Quit();//SDL_Mixer
	TTF_Quit();//SDL_TTF
	SDL_Quit();//SDL2 in general

}
/*
	===============================================
	void endQuotes(quoteData **quotes, int size):
	To deinit the quotes
	===============================================
*/
void endQuotes( quoteData **quotes , int size )
{
	fprintf(stderr, "Deinitialising quotes....\n");
	int i;
	for( i = 0 ; i < size ; i++ )
	{
		SDL_DestroyTexture( quotes[i]->display[0]->display );//Destroy texture
		SDL_DestroyTexture( quotes[i]->display[1]->display );
		free( quotes[i] );
	}

	free( quotes );

}
/*
	==========================================================
	void endQuoteListData(quoteListData *quotes):
	To deinit the quoteList
	NOTE : needs to have the quotes inside deinitialised first
	==========================================================
*/
void endQuoteListData(quoteListData *quotes)
{
	fprintf( stderr , "Deinitialising quoteList....\n" );
	free( quotes );
}
/*
	================================================
	void endTileData(tileData **tiles, int size):
	To deinit the map
	================================================
*/
void endTileData( tileData **tiles , int size )
{
	fprintf( stderr , "Deinitialising map tiles....\n" );
	int i;
	for(i = 0 ; i < size ; i++ )
	{
		free( tiles[i] );//each map tile
	
	}
	free( tiles );
}
/*
	====================================================
	void endUnitDataArray(unitData **units, int size):
	To deinit the units
	====================================================
*/
void endUnitDataArray( unitData **units , int size )
{
	fprintf( stderr , "Deinitialising units....\n" );
	int i;
	for(i = 1 ; i <= size ; i++ )
	{
		free( units[i] );
	
	}
	free( units );
}
/*
	======================================
	void endSideData(sideData *side):
	To deinit the side
	======================================
*/
void endSideData( sideData *side )
{
	fprintf( stderr , "Deinitialising a side....\n" );
	endUnitDataArray( side->units , side->noUnits );
	free( side );

}
/*
	=======================================================
	void endLevel(levelData *level):
	To deinit everything, all modules, all sub structures
	=======================================================
*/
void endLevel(levelData *level)
{
	int i;
	for( i = 0 ; i < level->noLevels ; i++ )
	{
		endTileData( level->maps[i]->tiles , level->maps[i]->tiles[0]->noTiles );//map tiles
		endSideData( level->maps[i]->sides[0] );//side 1 data
		endSideData( level->maps[i]->sides[1] );//side two data
		endQuoteListData( level->maps[i]->quoteList );//quotes
		endActivity( level->maps[i]->activity );//activity
	}
	free( level );
}
/*
	End of File

*/
