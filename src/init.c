/*
	FILE : INIT.C
	PURPOSE : Hold initialisation functions for libraries, multimedia and runtime structures
	VERSION : 0.001
	NOTES: Big File, may have some unnoticed minor bugs
*/


//---------------------------------- C PREPROCESSOR --------------------------


#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif



//------------------------------ FUNCTIONS -----------------------------------
/*
	char *loadTextFile(const char *filename, int *success):
	Loads a file in a safe way and creates a string from it

*/

char *loadTextFile( const char *filename , int *success )
{

	fprintf( stderr , "Loading file %s... \n" , filename );
	int fileDescriptor, fileSize;
	char *fileContents;
	FILE *jsonFile;
	
	fileDescriptor = open( filename , O_RDONLY , 0600 );//avoids the race condition issues
	if( fileDescriptor == -1 )
	{
		fprintf( stderr , "open has failed on %s: %s \n" , filename , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	
	jsonFile = fdopen( fileDescriptor , "rb" );
	if( !jsonFile )
	{
		fprintf( stderr , "fopen has failed : %s \n" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	fileSize = getFileSize( jsonFile , success );
	if( !fileSize )
	{
		return NULL;
	
	}
	fileContents = calloc( 1 , fileSize + 1 );
	if( !fileContents )
	{
		fprintf( stderr , "malloc has failed : %s" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	
	}
	fread( fileContents , fileSize , 1 , jsonFile );
	fclose( jsonFile );
	if(strstr( fileContents , "%x" ) != NULL)
	{
		fprintf( stderr , "stack change formatter detected in file provided, nice try\n" );
		*success = FAIL;
		return NULL;
	
	}
	return fileContents;


}
/*
	int getFileSize(FILE *sizeToGet, int *success):
	Gets the size of a file

*/

int getFileSize( FILE *sizeToGet , int *success )
{
	fprintf( stderr , "Getting File size...\n" );
	int fileSize = 0;
	fseek( sizeToGet ,0 , SEEK_END );
	fileSize = ftell( sizeToGet );
	rewind( sizeToGet );//back to the start
	if(errno > 0)
	{
		fprintf( stderr , "ftell has failed : %s" , strerror( errno ) );
		*success = FAIL;
		return 0;
	}
	return fileSize;

}
/*
	optionsData *initOptions(char *fileContents, int *success):
	loads the options file in JSON to a optionsData structure

*/

optionsData initOptions( char *fileContents , int *success )
{
	fprintf( stderr , "Loading options... \n" );
	optionsData tempOpt;
	json_t *tempJsonHandle, *optionsData;
	json_error_t errorHandle;
	
	tempJsonHandle = json_loads( fileContents , 0 , &errorHandle );//loads the JSON file into Jansson 
	if( !tempJsonHandle )
	{
		fprintf( stderr , "json_loads has failed : %s \n" , errorHandle.text );
		*success = FAIL;
		return tempOpt;
	}
	
	optionsData = json_array_get( tempJsonHandle , 0 );
	if( !json_is_object(optionsData) )//makes sure that what is being opened is actually a JSON object
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		*success = FAIL;
		json_decref( tempJsonHandle );
		return tempOpt;
	
	}
	//gets the program options
	tempOpt.SCREEN_WIDTH = json_integer_value( json_object_get ( optionsData , "SCREEN_WIDTH" ) );
	tempOpt.SCREEN_HEIGHT = json_integer_value( json_object_get( optionsData , "SCREEN_HEIGHT" ) );
	tempOpt.WINDOW_TITLE = (char *)json_string_value( json_object_get( optionsData ,  "WINDOW_TITLE" ) );
	tempOpt.SAMPLE_SIZE = json_integer_value( json_object_get( optionsData , "SAMPLE_SIZE" ) );
	tempOpt.SAMPLE_FREQUENCY = json_integer_value( json_object_get( optionsData , "SAMPLE_FREQUENCY" ) );
	tempOpt.NO_CHANNELS = json_integer_value( json_object_get( optionsData , "NO_CHANNELS" ) );
	tempOpt.FONT_SIZE = json_integer_value( json_object_get( optionsData , "FONT_SIZE" ) );
	tempOpt.DEFAULT_FONT = (char *)json_string_value( json_object_get( optionsData , "DEFAULT_FONT" ) );
	return tempOpt;
}

/*
	SDL_Window *initSDL(optionsData *opt, int *success):
	initialise SDL2 and associated library

*/

SDL_Window *initSDL( optionsData *opt , int *success )
{
	fprintf( stderr , "Initialising SDL2 and SDL2 Extension Libraries....\n" );
	SDL_Window *temp;
	int SDL_Flags, IMG_Flags;
	SDL_Flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO;
	IMG_Flags = IMG_INIT_JPG | IMG_INIT_PNG;
	//Main SDL2 Library
	if( SDL_Init( SDL_Flags ) < 0 )
	{
		fprintf( stderr , "SDL_Init has failed : %s \n" , SDL_GetError() );
		*success = FAIL;
		return NULL;
	}
	//TTF Font Library
	if( TTF_Init() != 0 )
	{
		fprintf( stderr , "TFF_Init has failed : %s \n" , TTF_GetError() );
		*success = FAIL;
		return NULL;
	}
	//Image library
	if( !( IMG_Init( IMG_Flags ) & IMG_Flags ) )
	{
		fprintf( stderr , "IMG_Init has failed, %s \n" , IMG_GetError() );
		*success = FAIL;
		return NULL;
	
	}
	//Audio library
	if( Mix_OpenAudio( opt->SAMPLE_FREQUENCY , MIX_DEFAULT_FORMAT , opt->NO_CHANNELS , opt->SAMPLE_SIZE ) < 0 )//first arg : frequency of audio, second arg : format, third arg : Number of audio channels, forth arg : sample size
	{
		fprintf( stderr , "Mix_OpenAudio has failed, %s \n" , Mix_GetError() );
		*success = FAIL;
		return NULL;
	
	}
	//initialise window
	temp = SDL_CreateWindow( opt->WINDOW_TITLE , SDL_WINDOWPOS_UNDEFINED , SDL_WINDOWPOS_UNDEFINED , opt->SCREEN_WIDTH ,
	opt->SCREEN_HEIGHT , SDL_WINDOW_SHOWN );
	if( !temp )
	{
		fprintf( stderr , "SDL_CreateWindow has failed : %s \n" , SDL_GetError() );
		*success = FAIL;
		return NULL;
	}
	
	return temp;
}
/*
	SDL_Renderer *createRenderer(SDL_Window *screen, int *success):
	initialise hardware renderer

*/

SDL_Renderer *createRenderer( SDL_Window *screen , int *success )
{
	fprintf( stderr , "Creating SDL2 Renderer....\n" );
	SDL_Renderer *temp;
	int Render_Flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;//Hardware acceleration and a frame rate capped by the refresh rate of the monitor
	temp = SDL_CreateRenderer( screen , -1 , Render_Flags );
	if( !temp )
	{
		fprintf( stderr , "SDL_CreateRenderer has failed : %s \n" , SDL_GetError() );
		*success = FAIL;
		return NULL;
	}

	return temp;

}
/*
	Mix_Music *loadMusic(const char *filename, int *success):
	load music 

*/

Mix_Music *loadMusic( const char *filename , int *success )
{
	fprintf( stderr , "Loading music from %s....\n" , filename );
	Mix_Music *temp = Mix_LoadMUS( filename );
	if( !temp )
	{
		fprintf( stderr , "Mix_LoadMUS has failed : %s \n" , Mix_GetError() );
		*success = FAIL;
		return NULL;
	}

	return temp;

}
/*
	Mix_Chunk *loadEffect(const char *filename, int *success):
	load sound effects

*/
Mix_Chunk *loadEffect( const char *filename , int *success )
{
	fprintf( stderr , "Loading effect from %s....\n" , filename );
	Mix_Chunk *temp = Mix_LoadWAV( filename );
	if(!temp)
	{
		fprintf( stderr , "Mix_LoadWAV has failed : %s \n" , Mix_GetError() );
		*success = FAIL ;
		return NULL;
	}
	return temp;
}
/*
	TTF_Font *loadFont(options *opt, int *success):
	load a font structure

*/
TTF_Font *loadFont( optionsData *opt , int *success )
{
	fprintf( stderr , "Loading font from %s....\n" , opt->DEFAULT_FONT );
	TTF_Font *temp = TTF_OpenFont( opt->DEFAULT_FONT , opt->FONT_SIZE );
	if( !temp )
	{
		fprintf( stderr , "TTF_OpenFont has failed : %s \n" , TTF_GetError() );
		*success = FAIL;
		return NULL;
	}
	return temp;
}
/*
	SDL_Texture *loadImage(const char *filename, SDL_Renderer *render, SDL_Rect *dimen, int *success):
	load an image, translates it to a hardware renderable texture and returns the original image's dimensions

*/
SDL_Texture *loadImage( const char *filename , SDL_Renderer *render , SDL_Rect *dimen , int *success )
{
	fprintf( stderr , "Loading image and converting it to texture from %s....\n" , filename );
	SDL_Surface *temp;
	SDL_Texture *tempTex;
	
	temp = IMG_Load( filename );//image library used to load things other than bitmaps
	if(!temp)
	{
		fprintf( stderr , "IMG_LoadBMP has failed: %s \n" , IMG_GetError() );
		*success = FAIL;
		return NULL;
	
	}
	SDL_SetColorKey( temp , SDL_TRUE , SDL_MapRGB( temp->format , 255 , 0 ,255 ) );//makes the background colour transparent
	tempTex = SDL_CreateTextureFromSurface( render , temp );//converts to renderable type
	if( !tempTex )
	{
		fprintf( stderr , "SDL_CreateTextureFromSurface has failed : %s \n" , SDL_GetError() );
		*success = FAIL;
		return NULL;
	}
	dimen->w = temp->w;
	dimen->h = temp->h;
	SDL_FreeSurface( temp );
	return tempTex;
}
/*
	buttonData *loadButton(SDL_Texture *display, SDL_Rect *posAndSize, int type, int *success):
	Loads a regular button

*/
buttonData *loadButton( SDL_Texture *display , SDL_Rect *posAndSize , int type , int *success )
{
	fprintf( stderr , "Loading a button....\n" );
	buttonData *temp = malloc( sizeof( buttonData ) );
	if( !temp )
	{
		fprintf( stderr , "malloc has failed on buttonData : %s" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	temp->display = display; //will copy the address of the button texture to the object
	temp->dimensions = *posAndSize;
	temp->type = type;

	return temp;
}
/*
	buttonData *loadButton(SDL_Texture *display, SDL_Rect *posAndSize, int type, int *success):
	Loads a button with text

*/
buttonDataText *loadButtonText( SDL_Texture *display , SDL_Rect *posAndSize , SDL_Renderer *render , const char *initialData , TTF_Font *font , int type , int *success )
{
	fprintf( stderr , "Loading a button with text....\n" );
	buttonDataText *temp = malloc( sizeof( buttonDataText ) );
	if( !temp )
	{
		fprintf( stderr , "malloc has failed on buttonData : %s" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	temp->details = malloc( sizeof( textData ) );//text data
	if( !temp->details )
	{
		fprintf( stderr , "malloc has failed on buttonData : %s" , SDL_GetError() );
		*success = FAIL;
		return NULL;
	}
	temp->display = display;
	temp->dimensions = *posAndSize;
	temp->type = type;
	temp->details = renderText( font , render , initialData , success );
	temp->details->dimensions.x = posAndSize->x + (posAndSize->w / 5);//places the text in the centre of the button
	temp->details->dimensions.y = posAndSize->y + (posAndSize->h / 5);
	return temp;
}
/*
	quoteData **loadQuotes(char *filename, int *success):
	Loads the quotes from a specified data file

*/
quoteData **loadQuotes(char *filename, int *success)
{
	fprintf( stderr , "Loading quotes from %s....\n" , filename );
	char *quoteDataFILE = loadTextFile( filename , success );
	quoteData **temp;
	json_t *tempJsonHandle, *quoteDataJSON;
	json_error_t errorHandle;
	int numberOfQuotes, i;
	
	tempJsonHandle = json_loads( quoteDataFILE , 0 , &errorHandle );
	if( !tempJsonHandle )
	{
		fprintf( stderr , "json_loads has failed on %s: %s \n", filename , errorHandle.text );
		*success = FAIL;
		return temp;
	}
	quoteDataJSON = json_array_get( tempJsonHandle , 0 );
	if( !json_is_object( quoteDataJSON ) )
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		*success = FAIL;
		json_decref( tempJsonHandle );
		return temp;
	
	}
	numberOfQuotes = json_integer_value( json_object_get( quoteDataJSON , "NO_QUOTES" ) );
	temp = malloc( sizeof(quoteData *) * numberOfQuotes );
	for( i = 0 ; i < numberOfQuotes ; i++ )
	{
		temp[i] = malloc( sizeof( quoteData ) );
		quoteDataJSON = json_array_get( tempJsonHandle , i );
		if( !json_is_object( quoteDataJSON ) )
		{
			fprintf( stderr , "json_object_get failed, didn't get an object\n" );
			*success = FAIL;
			json_decref( tempJsonHandle );
			return temp;
		}
		temp[i]->quote = (char *) json_string_value( json_object_get( quoteDataJSON , "QUOTE" ) );
		temp[i]->analysis = (char *) json_string_value(json_object_get( quoteDataJSON , "ANALYSIS" ) );
	}
	temp[0]->noQuotes = numberOfQuotes;
	return temp;
}
/*
	quoteData **loadQuotes(char *filename, int *success):
	Loads the questions from a specified data file

*/
questionData **loadQuestions( char *filename , int *success )
{
	fprintf( stderr , "Loading questions from %s....\n", filename );
	char *questionDataFILE = loadTextFile( filename , success );
	questionData **temp;
	json_t *tempJsonHandle, *questionDataJSON;
	json_error_t errorHandle;
	int numberOfQuestions, i;
	
	tempJsonHandle = json_loads( questionDataFILE , 0 , &errorHandle );
	if( !tempJsonHandle )
	{
		fprintf( stderr , "json_loads has failed on %s: %s \n" , filename , errorHandle.text );
		*success = FAIL;
		return temp;
	}
	questionDataJSON = json_array_get( tempJsonHandle , 0 );
	if( !json_is_object( questionDataJSON ) )
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		*success = FAIL;
		json_decref( tempJsonHandle );
		return temp;
	}
	numberOfQuestions = json_integer_value( json_object_get( questionDataJSON , "NO_QUESTIONS" ));
	temp = malloc( sizeof( questionData * ) * numberOfQuestions );
	for( i = 0 ; i < numberOfQuestions ;  i++ )
	{
		temp[i] = malloc( sizeof( questionData ) );
		questionDataJSON = json_array_get( tempJsonHandle , i );
		if( !json_is_object( questionDataJSON ) )
		{
			fprintf( stderr ,"json_object_get failed, didn't get an object\n" );
			*success = FAIL;
			json_decref( tempJsonHandle );
			return temp;
		}
		temp[i]->question = (char *)json_string_value( json_object_get( questionDataJSON ,  "QUESTION" ) );
		temp[i]->answers[0] = (char *) json_string_value( json_object_get( questionDataJSON ,  "ANSWERONE" ) );
		temp[i]->answers[1] = (char *) json_string_value( json_object_get( questionDataJSON , "ANSWERTWO" ) );
		temp[i]->answers[2] = (char *) json_string_value( json_object_get( questionDataJSON , "ANSWERTHREE" ) );
		temp[i]->answerNo = json_integer_value( json_object_get( questionDataJSON , "CORRECT" ) );
	}
	return temp;
}
/*
	char *miscIDToFilePath(int ID, char *path):
	Used to map the IDs in non Level Data files to correct file paths
*/
char *miscIDToFilePath( int ID , char *path )
{
	fprintf( stderr , "Mapping ID to an actual file path within data directory %s....\n" , path );
	char filePath [ MAX_TEXT_OUTPUT ];
	char *mappingFileContents , *mappedPath , stringPath[ MAX_TEXT_OUTPUT ] ;
	json_t *tempJsonHandle ,  *mappingDataJSON ;
	json_error_t errorHandle;
	int wasSuccess = SUCCESS;
	
	snprintf( filePath , MAX_TEXT_OUTPUT ,  "%s%s", path , MAPPING_FILE_MISC );
	mappingFileContents = loadTextFile( filePath , &wasSuccess );
	
	tempJsonHandle = json_loads( mappingFileContents , 0 , &errorHandle );
	if( !tempJsonHandle )
	{
		fprintf( stderr , "%s", mappingFileContents );
		fprintf( stderr , "json_loads has failed on %s: %s \n", filePath , errorHandle.text );
		return NULL;
	}
	mappingDataJSON = json_array_get( tempJsonHandle , 0 );
	if( !json_is_object( mappingDataJSON ) )
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		json_decref( tempJsonHandle );
		return NULL;
	}
	itoa( ID , stringPath , 10 );
	mappedPath = (char *) json_string_value( json_object_get( mappingDataJSON , stringPath ) );
	if( !mappedPath )
	{
		fprintf( stderr , "The ID you have requested doesn't exist\n" );
		return NULL;
	}
	return mappedPath;
}
/*
	activityData *loadActivity(char *filename, int *success):
	Used to load ActivityData structures
*/
activityData *loadActivity( char *filename , int *success )
{
	fprintf( stderr , "Loading an activity from %s....\n" , filename );
	char *activityDataFile , pathToLoad[ MAX_TEXT_OUTPUT ] , *questionFile ;
	int wasSuccess = SUCCESS;
	json_t *tempJsonHandle , *activityDataJSON ;
	json_error_t errorHandle ;
	activityData *temp = malloc( sizeof( activityData ) );
	if( !temp )
	{
		fprintf( stderr , "malloc has failed on activityData : %s" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	snprintf( pathToLoad , MAX_TEXT_OUTPUT , "%s%s" , filename , ACTIVITY_FILE );
	activityDataFile = loadTextFile( pathToLoad , &wasSuccess );
	tempJsonHandle = json_loads( activityDataFile , 0 , &errorHandle );
	
	if( !tempJsonHandle )
	{
		fprintf( stderr , "json_loads has failed : %s \n" , errorHandle.text );
		return NULL;
	}
	activityDataJSON = json_array_get( tempJsonHandle , 0 );
	if( !json_is_object( activityDataJSON ) )
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		json_decref( tempJsonHandle );
		return NULL;
	}
	temp->activityID = json_integer_value( json_object_get( activityDataJSON , "ACTIVITY_ID" ) );
	questionFile = miscIDToFilePath( temp->activityID , filename );
	
	if(!questionFile)
	{
		fprintf( stderr , "loadActivity has failed : %s" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	temp->questions = loadQuestions( questionFile , &wasSuccess );
	temp->maximumMark = json_integer_value( json_object_get( activityDataJSON , "MAXIMUM_MARK" ) );
	temp->title = (char *) json_string_value( json_object_get( activityDataJSON ,  "TITLE" ) );
	if( wasSuccess == FAIL )
	{
		fprintf( stderr , "loadActivity has failed : %s" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	return temp;
}
/*
	activityData *loadActivity(char *filename, int *success):
	Used to load a quoteListData structures

*/
quoteListData *loadQuoteListData( char *filename , int *success )
{
	fprintf( stderr , "Loading quoteList from %s....\n", filename );
	char *quoteListDataFile, pathToLoad[ MAX_TEXT_OUTPUT ] , *quoteFile ;
	int wasSuccess = SUCCESS;
	json_t *tempJsonHandle , *quoteListDataJSON;
	json_error_t errorHandle;
	
	quoteListData *temp = malloc( sizeof( quoteListData ) );
	if( !temp )
	{
		fprintf( stderr , "malloc has failed on quoteListData : %s" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	snprintf( pathToLoad , MAX_TEXT_OUTPUT , "%s%s", filename , QUOTELIST_FILE );
	quoteListDataFile = loadTextFile( pathToLoad , &wasSuccess );
	tempJsonHandle = json_loads( quoteListDataFile , 0 , &errorHandle );
	
	if( !tempJsonHandle )
	{
		fprintf( stderr , "json_loads has failed : %s \n" , errorHandle.text );
		return NULL;
	}
	quoteListDataJSON = json_array_get( tempJsonHandle , 0 );
	if( !json_is_object(quoteListDataJSON ) )
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		json_decref( tempJsonHandle );
		return NULL;
	}
	
	temp->quoteID = json_integer_value( json_object_get( quoteListDataJSON , "QUOTELIST_ID" ) );
	quoteFile = miscIDToFilePath( temp->quoteID , filename );
	
	if( !quoteFile )
	{
		fprintf( stderr , "loadQuoteListData has failed : %s" , strerror(errno) );
		*success = FAIL;
		return NULL;
	}
	temp->quotes = loadQuotes( quoteFile , &wasSuccess );
	if( wasSuccess == FAIL )
	{
		fprintf( stderr , "loadQuoteListData has failed : %s" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	return temp;
}
/*
	tileData **loadTileData(char *tileFile, int *success):
	Used to load tiles for maps

*/
tileData **loadTileData( char *tileFile , int *success )
{
	fprintf( stderr , "Loading a tilemap from %s....\n" , tileFile );
	char *tileDataFile = loadTextFile( tileFile , success );
	tileData **temp;
	json_t *tempJsonHandle, *tileDataJSON;
	json_error_t errorHandle;
	int numberOfTiles, i;
	
	tempJsonHandle = json_loads( tileDataFile , 0 , &errorHandle );
	if( !tempJsonHandle )
	{
		fprintf( stderr , "json_loads has failed on %s: %s \n" , tileFile , errorHandle.text );
		*success = FAIL;
		return temp;
	}
	
	tileDataJSON = json_array_get( tempJsonHandle , 0 );
	if( !json_is_object( tileDataJSON ) ) 
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		*success = FAIL;
		json_decref( tempJsonHandle );
		return temp;
	}
	numberOfTiles = json_integer_value( json_object_get( tileDataJSON , "NO_TILES" ) );
	
	temp = malloc( sizeof( tileData * ) * numberOfTiles );
	for( i = 0 ; i < numberOfTiles ; i++ )
	{
		temp[i] = malloc( sizeof( tileData ) );
		tileDataJSON = json_array_get( tempJsonHandle , i );
		if( !json_is_object( tileDataJSON ) )
		{
			fprintf( stderr ,"json_object_get failed, didn't get an object\n" );
			*success = FAIL;
			json_decref( tempJsonHandle );
			return temp;
		}
		temp[i]->spriteDimensions.x = ( json_integer_value( json_object_get(tileDataJSON,"SPRITE_XPOS")) - 1) * TILE_WIDTH * 2;
		temp[i]->spriteDimensions.y = ( json_integer_value( json_object_get(tileDataJSON,"SPRITE_YPOS")) - 1 )* TILE_HEIGHT * 2;
		temp[i]->spriteDimensions.w = TILE_WIDTH * 2;//the sprites are stored in 128x128 format, but will be displayed in 64x64
		temp[i]->spriteDimensions.h = TILE_HEIGHT * 2;
		temp[i]->dimensions.h = TILE_HEIGHT;
		temp[i]->dimensions.w = TILE_WIDTH;
		temp[i]->relativeX = json_integer_value(json_object_get( tileDataJSON , "RELATIVE_XPOS" ) );//This is useful for movement and such
		temp[i]->relativeY = json_integer_value(json_object_get( tileDataJSON , "RELATIVE_YPOS" ) );
		temp[i]->dimensions.x = temp[i]->relativeX * TILE_WIDTH + STARTX_MAP;
		temp[i]->dimensions.y = temp[i]->relativeY * TILE_HEIGHT + STARTY_MAP;
		temp[i]->civilianPopulation = json_integer_value( json_object_get( tileDataJSON , "CIVILIAN_POPULATION" ) );
		temp[i]->terrainType = json_integer_value( json_object_get( tileDataJSON , "TERRAIN_TYPE" ) );
		temp[i]->tileID = json_integer_value( json_object_get( tileDataJSON , "TILE_ID" ) );
		temp[i]->angle = json_real_value( json_object_get( tileDataJSON , "ANGLE" ) );
		temp[i]->isSelected = FALSE;
	}
	temp[0]->noTiles = numberOfTiles;
	return temp;


}
/*
	tileData **loadTileData(char *tileFile, int *success):
	Used to load a unit from a unitFile based on the Unit's position in the unit file

*/
unitData *loadUnit( char *unitFile , int ID , int *success )
{
	fprintf( stderr , "Loading unit ID %d....\n" , ID );
	int i, loadedUnitID, found, numberOfUnits;
	json_t *tempJsonHandle, *unitDataJSON;
	json_error_t errorHandle;
	unitData *temp = malloc( sizeof( unitData ) );
	if( !temp )
	{
		fprintf( stderr , "Malloc has failed on loadUnit : %s \n" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	found = FAIL;
	tempJsonHandle = json_loads( unitFile , 0 , &errorHandle );
	if( !tempJsonHandle )
	{
		fprintf( stderr , "json_loads has failed on %s: %s \n" , unitFile , errorHandle.text );
		*success = FAIL;
		return temp;
	}
	unitDataJSON = json_array_get( tempJsonHandle , 0 );
	if( !json_is_object( unitDataJSON ) )
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		*success = FAIL;
		json_decref( tempJsonHandle );
		return temp;
	}
	numberOfUnits = json_integer_value( json_object_get( unitDataJSON , "NO_UNITS" ) );
	for( i = 0 ; i < numberOfUnits ; i++ )//linear search is used here as N (or size) will be quite low (<50) although a hard coded limit is illogical as it could limit teaching potential of military capabilities
	{
		unitDataJSON = json_array_get( tempJsonHandle , ID );
		if( !json_is_object( unitDataJSON ) )
		{
			fprintf( stderr , "json_object_get failed, didn't get an object\n" );
			*success = FAIL;
			json_decref( tempJsonHandle );
			return temp;
		}
		loadedUnitID = json_integer_value( json_object_get( unitDataJSON , "ID" ) );
		if( loadedUnitID == ID )
		{
			found = SUCCESS;
			break;
		}
	}
	if( found == FAIL )
	{
		fprintf( stderr , "loadUnit has failed, ID %d doesn't exist" , ID );
		*success = FAIL;
		return NULL;
	}
	
	temp->unitID = loadedUnitID;
	temp->movement = json_integer_value( json_object_get( unitDataJSON , "MOVEMENT" ) );
	temp->aPRange = json_integer_value( json_object_get( unitDataJSON , "ANTI_PERSONNEL_RANGE" ) );
	temp->aPAttacks = json_integer_value( json_object_get( unitDataJSON , "ANTI_PERSONNEL_DICE" ) );
	temp->aTRange = json_integer_value( json_object_get( unitDataJSON , "ANTI_TANK_RANGE" ) );
	temp->aTAttacks = json_integer_value( json_object_get( unitDataJSON , "ANTI_TANK_DICE" ) );
	temp->unitType = json_integer_value( json_object_get( unitDataJSON , "UNIT_TYPE" ) );
	temp->wounds = json_integer_value( json_object_get( unitDataJSON , "WOUNDS" ) );
	temp->save = json_integer_value( json_object_get( unitDataJSON , "SAVE" ) );
	temp->coverSave = DEFAULT_COVER_SAVE;
	temp->morale = json_integer_value( json_object_get( unitDataJSON , "MORALE" ) );
	temp->spriteDimensions.x = json_integer_value( json_object_get( unitDataJSON , "SPRITE_XPOS" ) ) * TILE_WIDTH * 2;//loads the positions of the sprite 
	temp->spriteDimensions.y = json_integer_value( json_object_get( unitDataJSON , "SPRITE_YPOS" ) ) * TILE_HEIGHT * 2;
	temp->spriteDimensions.w = TILE_WIDTH * 2;
	temp->spriteDimensions.h = TILE_HEIGHT * 2;
	temp->dimensions.w = TILE_WIDTH;
	temp->dimensions.h = TILE_HEIGHT;
	temp->alive = TRUE;
	temp->name = (char *) json_string_value( json_object_get( unitDataJSON , "NAME" ) );
	temp->description = (char *) json_string_value( json_object_get( unitDataJSON , "DESCRIPTION" ) );
	temp->selected = FALSE;
	fprintf( stderr , "Got a unit called %s\n" , temp->name );
	for( i = 0 ; i < SIZE_OF_MODIFIERS ; i++ )
	{
		temp->modifiers[i] = FALSE;
	}
	return temp;
}
/*
	unitData **loadUnitData(char *sideUnitDataFilePath, char *unitDescriptorDataFilePath, int *success):
	Used to load all the units with the assistance of the above helper function

*/
unitData **loadUnitData( char *sideUnitDataFilePath , char *unitDescriptorDataFilePath , int *success )
{
	fprintf( stderr , "Loading unit Data from %s using the descriptor file %s....\n" , sideUnitDataFilePath , unitDescriptorDataFilePath );
	unitData **temp;
	json_t *tempJsonHandle, *unitDataJSON;
	json_error_t errorHandle;
	int i, numberOfUnits, unitID;
	char *sideUnitDataFile = loadTextFile( sideUnitDataFilePath , success );
	char *overallUnitDataFile = loadTextFile( unitDescriptorDataFilePath , success );
	tempJsonHandle = json_loads( sideUnitDataFile , 0 , &errorHandle );
	if( !tempJsonHandle )
	{
		fprintf( stderr , "json_loads has failed on %s: %s \n" , sideUnitDataFile , errorHandle.text );
		*success = FAIL;
		return temp;
	}
	unitDataJSON = json_array_get( tempJsonHandle , 0 );
	if( !json_is_object( unitDataJSON ) ) 
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		*success = FAIL;
		json_decref( tempJsonHandle );
		return temp;//I.E. NULL	
	}
	numberOfUnits = json_integer_value( json_object_get( unitDataJSON , "NUMBER_UNITS" ) ) + 1;
	temp = malloc( sizeof( unitData * ) * numberOfUnits );
	if( !temp )
	{
		fprintf( stderr , "malloc has failed in loadUnitData : %s \n" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	for( i = 1 ; i < numberOfUnits ; i++ )
	{
		unitDataJSON = json_array_get( tempJsonHandle , i );
		if( !json_is_object( unitDataJSON ) )
		{
			fprintf( stderr , "json_object_get failed, didn't get an object\n" );
			*success = FAIL;
			json_decref( tempJsonHandle );
			return temp;
		}
		unitID = json_integer_value( json_object_get( unitDataJSON , "UNIT_ID" ) );
		temp[i] = loadUnit( overallUnitDataFile , unitID , success );
		temp[i]->relativeX = json_integer_value( json_object_get( unitDataJSON , "RELATIVE_XPOS" ) );
		temp[i]->relativeY = json_integer_value( json_object_get( unitDataJSON , "RELATIVE_YPOS" ) );
		temp[i]->dimensions.x = temp[i]->relativeX * TILE_WIDTH + STARTX_MAP;
		temp[i]->dimensions.y = temp[i]->relativeY * TILE_HEIGHT + STARTY_MAP;
	}
	
	return temp;
}
/*
	sideData *loadSideData(char *filename,int sideNumber, int *success):
	Loads a side in the scenario

*/
sideData *loadSideData( char *filename , int sideNumber , int *success )
{
	fprintf( stderr , "Loading a side from %s....\n" , filename );
	char *sideDataFile, *sideUnitPath, pathToLoad[ MAX_TEXT_OUTPUT ];
	int sideID;
	sideData *temp;
	json_t *tempJsonHandle, *sideDataJSON;
	json_error_t errorHandle;
	if( sideNumber > 1 && sideNumber < 0 )
	{
		fprintf( stderr , "sideNumber Not valid\n" );
		*success = FAIL;
		return NULL;
	}
	temp = malloc( sizeof( sideData ) );
	if( !temp )
	{
		fprintf( stderr , "malloc has failed in loadSideData, %s \n" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	snprintf( pathToLoad , MAX_TEXT_OUTPUT , "%s%s" , filename , SIDE_FILES[ sideNumber ] );
	sideDataFile = loadTextFile( pathToLoad , success );
	tempJsonHandle = json_loads( sideDataFile , 0 , &errorHandle );
	if( !tempJsonHandle )
	{
		fprintf( stderr , "json_loads has failed : %s \n" , errorHandle.text );
		*success = FAIL;
		return NULL;
	}
	sideDataJSON = json_array_get( tempJsonHandle , 0 );
	if( !json_is_object( sideDataJSON ) )
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		json_decref( tempJsonHandle );
		*success = FAIL;
		return NULL;
	}
	temp->sideID = json_integer_value( json_object_get( sideDataJSON , "SIDE_UNIT_ID" ) );//This is so we can get 
	sideUnitPath = miscIDToFilePath( temp->sideID , filename );
	if( !sideUnitPath )
	{
		fprintf( stderr , "Side unit file doesn't exist \n" );
		*success = FAIL;
		return NULL;
	}
	snprintf( pathToLoad , MAX_TEXT_OUTPUT , "%s%s" , filename , UNIT_FILE );
	temp->noUnits = json_integer_value( json_object_get( sideDataJSON , "UNIT_NUMBER" ) );
	temp->units = loadUnitData( sideUnitPath , pathToLoad , success );
	temp->xObjective = json_integer_value( json_object_get( sideDataJSON , "OBJECTIVE_XPOS" ) );
	temp->yObjective = json_integer_value( json_object_get( sideDataJSON , "OBJECTIVE_YPOS" ) );
	return temp;
}
/*
	mapData *loadMapData(char *levelDataFile ,int mapNo, int *success):
	Loads a map

*/
mapData *loadMapData( char *levelDataFile , int mapNo , SDL_Renderer *render , int *success )//This doesn't need an ID as all the maps are loaded in sequence
{
	fprintf( stderr , "Loading map.....\n" );
	mapData *temp = malloc( sizeof( mapData ) );
	json_t *tempJsonHandle, *mapDataJSON;
	json_error_t errorHandle;
	SDL_Rect placeholder;
	char tilesetPath[ MAX_TEXT_OUTPUT ], *mapFileContent, tileFilePath[ MAX_TEXT_OUTPUT ];
	if( !temp )
	{
		fprintf( stderr , "Malloc has failed in loadMapData : %s \n" , strerror( errno ) );
		*success = FAIL;
		return NULL;
	}
	mapFileContent = loadTextFile( levelDataFile , success );
	tempJsonHandle = json_loads( mapFileContent , 0 , &errorHandle );
	if( !tempJsonHandle )
	{
		fprintf( stderr , "json_loads has failed : %s \n" , errorHandle.text );
		*success = FAIL;
		return NULL;
	}
	mapDataJSON = json_array_get( tempJsonHandle , mapNo );
	if(!json_is_object( mapDataJSON ) )
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		json_decref( tempJsonHandle );
		*success = FAIL;
		return NULL;
	}
	temp->mapID = json_integer_value( json_object_get( mapDataJSON , "MAP_ID" ) );
	temp->path = mapLevelIDToMapPath( temp->mapID , success );
	temp->activity = loadActivity( temp->path , success );
	temp->quoteList = loadQuoteListData( temp->path , success );
	snprintf( tileFilePath , MAX_TEXT_OUTPUT , "%s%s" , temp->path , TILE_FILE );
	temp->tiles = loadTileData( tileFilePath , success );
	temp->sides[0] = loadSideData( temp->path , 0 , success );
	temp->sides[1] = loadSideData( temp->path , 1 , success );
	temp->title = (char *) json_string_value( json_object_get( mapDataJSON , "TITLE" ) );
	temp->description = (char *) json_string_value( json_object_get( mapDataJSON , "DESCRIPTION" ) );
	snprintf( tilesetPath , MAX_TEXT_OUTPUT , "%s%s" , temp->path , TILESET_FILE );
	temp->tileset = loadImage( tilesetPath , render , &placeholder , success );
	fprintf( stderr , "Map loading done!.....\n" );
	return temp;
}

/*
	char *mapLevelIDToMapPath(int id, int *success):
	Loads map ID to find the data


*/
char *mapLevelIDToMapPath( int id , int *success )
{
	fprintf( stderr,  "Mapping ID to an actual file path in root directory, ID : %d....\n" , id );
	char filePath [ MAX_TEXT_OUTPUT ];
	char *mappingFileContents ,*mappedPath , stringPath[ MAX_TEXT_OUTPUT ];
	json_t *tempJsonHandle , *mappingDataJSON ;
	json_error_t errorHandle;
	int wasSuccess = SUCCESS;
	
	mappingFileContents = loadTextFile( MAPPING_FILE_MAPS , &wasSuccess );
	
	tempJsonHandle = json_loads( mappingFileContents , 0 , &errorHandle );
	if( !tempJsonHandle )
	{
		fprintf( stderr , "%s" , mappingFileContents );
		fprintf( stderr , "json_loads has failed on %s: %s \n" , filePath , errorHandle.text );
		return NULL;
	}
	mappingDataJSON = json_array_get( tempJsonHandle , 0 );
	if( !json_is_object( mappingDataJSON ) )
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		json_decref( tempJsonHandle );
		return NULL;
	}
	
	itoa( id , stringPath , 10 );
	mappedPath = (char *)json_string_value( json_object_get( mappingDataJSON , stringPath ) );
	if( !mappedPath )
	{
		fprintf( stderr, "The ID you have requested doesn't exist\n" );
		return NULL;
	}
	return mappedPath;
}
/*
	levelData *loadLevelData(char *levelFileData, int *success):
	Loads the levels


*/
levelData *loadLevelData(SDL_Renderer *render, int *success)
{
	fprintf( stderr , "Loading level data.....\n" );
	levelData *temp = malloc( sizeof( levelData ) );
	char *levelFileContents;
	json_t *tempJsonHandle, *levelDataJSON;
	json_error_t errorHandle;
	int noLevels, i;
	levelFileContents = loadTextFile( LEVELS_FILE , success );
	
	tempJsonHandle = json_loads( levelFileContents , 0 , &errorHandle );
	if( !tempJsonHandle )
	{
		fprintf( stderr , "%s" , levelFileContents );
		fprintf( stderr , "json_loads has failed on %s: %s \n" , LEVELS_FILE , errorHandle.text );
		return NULL;
	
	}
	
	levelDataJSON = json_array_get( tempJsonHandle , 0 );
	if( !json_is_object( levelDataJSON ) )
	{
		fprintf( stderr , "json_object_get failed, didn't get an object\n" );
		json_decref( tempJsonHandle );
		return NULL;
	}
	noLevels = json_integer_value( json_object_get( levelDataJSON , "NO_MAPS" ) );
	temp->noLevels = noLevels;
	temp->maps = malloc( sizeof( mapData * ) * noLevels );
	for( i = 0 ; i < noLevels ; i++ )
	{
		temp->maps[i] = loadMapData( ( char * ) MAP_FILE , i , render , success );
	}
	return temp;

}
