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

char *loadTextFile(const char *filename, int *success)
{

	int fileDescriptor;
	
	fileDescriptor = open(filename, O_RDONLY, 0600);//avoids the race condition issues
	if(fileDescriptor == -1)
	{
		fprintf(stderr, "open has failed : %s \n", strerror(errno));
		*success = FAIL;
		return NULL;
	}
	
	FILE *jsonFile = fdopen(fileDescriptor, "rb");
	int fileSize;
	char *fileContents;
	if(!jsonFile)
	{
		fprintf(stderr, "fopen has failed : %s \n", strerror(errno));
		*success = FAIL;
		return NULL;
	}
	fileSize = getFileSize(jsonFile, success);
	if(!fileSize)
	{
		return NULL;
	
	}
	fileContents = malloc(sizeof(char) * fileSize);
	if(!fileContents)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	
	}
	fread(fileContents,1,fileSize, jsonFile);
	fileContents[fileSize] = '\0';
	fclose(jsonFile);
	if(strstr(fileContents, "%x") != NULL)
	{
		fprintf(stderr, "stack change formatter detected in file provided, nice try\n");
		*success = FAIL;
		return NULL;
	
	}
	return fileContents;




}
/*
	int getFileSize(FILE *sizeToGet, int *success):
	Gets the size of a file

*/

int getFileSize(FILE *sizeToGet, int *success)
{
	int fileSize = 0;
	fseek(sizeToGet,0,SEEK_END);
	fileSize = ftell(sizeToGet);
	rewind(sizeToGet);
	if(errno > 0)
	{
		fprintf(stderr, "ftell has failed : %s", strerror(errno));
		*success = FAIL;
		return 0;
	}
	return fileSize;

}
/*
	optionsData *initOptions(char *fileContents, int *success):
	loads the options file in JSON to a optionsData structure

*/

optionsData initOptions(char *fileContents, int *success)
{
	optionsData tempOpt;
	json_t *tempJsonHandle, *optionsData;
	json_error_t errorHandle;
	
	tempJsonHandle = json_loads(fileContents,0, &errorHandle);
	if(!tempJsonHandle)
	{
		fprintf(stderr, "json_loads has failed : %s \n", errorHandle.text);
		*success = FAIL;
		return tempOpt;
	
	}
	
	optionsData = json_array_get(tempJsonHandle, 0);
	if(!json_is_object(optionsData))
	{
		fprintf(stderr,"json_object_get failed, didn't get an object\n");
		*success = FAIL;
		json_decref(tempJsonHandle);
		return tempOpt;
	
	}

	tempOpt.SCREEN_WIDTH = json_integer_value(json_object_get(optionsData,"SCREEN_WIDTH"));
	tempOpt.SCREEN_HEIGHT = json_integer_value(json_object_get(optionsData,"SCREEN_HEIGHT"));
	tempOpt.WINDOW_TITLE = json_string_value(json_object_get(optionsData, "WINDOW_TITLE"));
	tempOpt.SAMPLE_SIZE = json_integer_value(json_object_get(optionsData,"SAMPLE_SIZE"));
	tempOpt.SAMPLE_FREQUENCY = json_integer_value(json_object_get(optionsData,"SAMPLE_FREQUENCY"));
	tempOpt.NO_CHANNELS = json_integer_value(json_object_get(optionsData,"NO_CHANNELS"));
	tempOpt.FONT_SIZE = json_integer_value(json_object_get(optionsData,"FONT_SIZE"));
	tempOpt.DEFAULT_FONT = json_string_value(json_object_get(optionsData, "DEFAULT_FONT"));
	return tempOpt;
}

/*
	SDL_Window *initSDL(optionsData *opt, int *success):
	initialise SDL2 and associated library

*/

SDL_Window *initSDL(optionsData *opt, int *success)
{
	SDL_Window *temp;
	int SDL_Flags, IMG_Flags;
	SDL_Flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO;
	IMG_Flags = IMG_INIT_JPG | IMG_INIT_PNG;
	//Main SDL2 Library
	if(SDL_Init(SDL_Flags) < 0)
	{
		fprintf(stderr, "SDL_Init has failed : %s \n", SDL_GetError());
		*success = FAIL;
		return NULL;
	}
	//TTF Font Library
	if(TTF_Init() != 0)
	{
		fprintf(stderr, "TFF_Init has failed : %s \n", TTF_GetError());
		*success = FAIL;
		return NULL;
	}
	//Image library
	if(!(IMG_Init(IMG_Flags) & IMG_Flags))
	{
		fprintf(stderr, "IMG_Init has failed, %s \n", IMG_GetError());
		*success = FAIL;
		return NULL;
	
	}
	//Audio library
	if(Mix_OpenAudio(opt->SAMPLE_FREQUENCY, MIX_DEFAULT_FORMAT, opt->NO_CHANNELS, opt->SAMPLE_SIZE) < 0)//first arg : frequency of audio, second arg : format, third arg : Number of audio channels, forth arg : sample size
	{
		fprintf(stderr, "Mix_OpenAudio has failed, %s \n", Mix_GetError());
		*success = FAIL;
		return NULL;
	
	}
	//initialise window
	temp = SDL_CreateWindow(opt->WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, opt->SCREEN_WIDTH, opt->SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!temp)
	{
		fprintf(stderr, "SDL_CreateWindow has failed : %s \n", SDL_GetError());
		*success = FAIL;
		return NULL;
	
	}
	
	return temp;

}
/*
	SDL_Renderer *createRenderer(SDL_Window *screen, int *success):
	initialise hardware renderer

*/

SDL_Renderer *createRenderer(SDL_Window *screen, int *success)
{
	SDL_Renderer *temp;
	int Render_Flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	temp = SDL_CreateRenderer(screen, -1, Render_Flags);
	if(!temp)
	{
		fprintf(stderr, "SDL_CreateRenderer has failed : %s \n", SDL_GetError());
		*success = FAIL;
		return NULL;
	}

	return temp;

}
/*
	Mix_Music *loadMusic(const char *filename, int *success):
	load music 

*/

Mix_Music *loadMusic(const char *filename, int *success)
{
	Mix_Music *temp = Mix_LoadMUS(filename);
	if(!temp)
	{
		fprintf(stderr, "Mix_LoadMUS has failed : %s \n", Mix_GetError());
		*success = FAIL;
		return NULL;
	}

	return temp;

}
/*
	Mix_Chunk *loadEffect(const char *filename, int *success):
	load sound effects

*/
Mix_Chunk *loadEffect(const char *filename, int *success)
{
	Mix_Chunk *temp = Mix_LoadWAV(filename);
	if(!temp)
	{
		fprintf(stderr, "Mix_LoadWAV has failed : %s \n", Mix_GetError());
		*success = FAIL;
		return NULL;
	}

	return temp;
}
/*
	TTF_Font *loadFont(options *opt, int *success):
	load a font structure

*/
TTF_Font *loadFont(optionsData *opt, int *success)
{
	
	TTF_Font *temp = TTF_OpenFont(opt->DEFAULT_FONT, opt->FONT_SIZE);
	if(!temp)
	{
		fprintf(stderr, "TTF_OpenFont has failed : %s \n", TTF_GetError());
		*success = FAIL;
		return NULL;
	}

	return temp;

}
/*
	SDL_Texture *loadImage(const char *filename, SDL_Renderer *render, SDL_Rect *dimen, int *success):
	load an image, translates it to a hardware renderable texture and returns the original image's dimensions

*/
SDL_Texture *loadImage(const char *filename, SDL_Renderer *render, SDL_Rect *dimen, int *success)
{
	SDL_Surface *temp;
	SDL_Texture *tempTex;
	temp = IMG_Load(filename);
	if(!temp)
	{
		fprintf(stderr, "IMG_LoadBMP has failed: %s \n", IMG_GetError());
		*success = FAIL;
		return NULL;
	
	}
	SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255,0,255));
	tempTex = SDL_CreateTextureFromSurface(render, temp);
	if(!tempTex)
	{
		fprintf(stderr, "SDL_CreateTextureFromSurface has failed : %s \n", SDL_GetError());
		*success = FAIL;
		return NULL;
	}
	dimen->w = temp->w;
	dimen->h = temp->h;
	
	SDL_FreeSurface(temp);
	return tempTex;


}
/*
	buttonData *loadButton(SDL_Texture *display, SDL_Rect *posAndSize, int type, int *success):
	Loads a regular button

*/
buttonData *loadButton(SDL_Texture *display, SDL_Rect *posAndSize, int type, int *success)
{
	buttonData *temp = malloc(sizeof(buttonData));
	if(!temp)
	{
		fprintf(stderr, "malloc has failed on buttonData : %s", SDL_GetError());
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
buttonDataText *loadButtonText(SDL_Texture *display, SDL_Rect *posAndSize, SDL_Renderer *render, const char *initialData, TTF_Font *font, int type, int *success)
{
	buttonDataText *temp = malloc(sizeof(buttonDataText));
	if(!temp)
	{
		fprintf(stderr, "malloc has failed on buttonData : %s", SDL_GetError());
		*success = FAIL;
		return NULL;
	
	
	}
	temp->details = malloc(sizeof(textData));
	if(!temp->details)
	{
		fprintf(stderr, "malloc has failed on buttonData : %s", SDL_GetError());
		*success = FAIL;
		return NULL;
	
	
	}
	temp->display = display;
	temp->dimensions = *posAndSize;
	temp->type = type;
	temp->details = renderText(font, render, initialData, success);
	temp->details->dimensions.x = posAndSize->x + (posAndSize->w / 5);
	temp->details->dimensions.y = posAndSize->y + (posAndSize->h / 5);
	return temp;
}
/*
	quoteData **loadQuotes(char *filename, int *success):
	Loads the quotes from a specified data file

*/
quoteData **loadQuotes(char *filename, int *success)
{
	char *quoteDataFILE = loadTextFile(filename, success);
	quoteData **temp;
	json_t *tempJsonHandle, *quoteDataJSON;
	json_error_t errorHandle;
	int numberOfQuotes, i;
	
	tempJsonHandle = json_loads(quoteDataFILE,0, &errorHandle);
	if(!tempJsonHandle)
	{
		fprintf(stderr, "json_loads has failed : %s \n", errorHandle.text);
		*success = FAIL;
		return temp;
	
	}
	
	quoteDataJSON = json_array_get(tempJsonHandle, 0);
	if(!json_is_object(quoteDataJSON))
	{
		fprintf(stderr,"json_object_get failed, didn't get an object\n");
		*success = FAIL;
		json_decref(tempJsonHandle);
		return temp;
	
	}
	numberOfQuotes = json_integer_value(json_object_get(quoteDataJSON,"NO_QUOTES"));
	temp = malloc(sizeof(quoteData *) * numberOfQuotes);
	for(i = 0; i < numberOfQuotes; i++)
	{
		temp[i] = malloc(sizeof(quoteData));
		quoteDataJSON = json_array_get(tempJsonHandle, i);
		if(!json_is_object(quoteDataJSON))
		{
			fprintf(stderr,"json_object_get failed, didn't get an object\n");
			*success = FAIL;
			json_decref(tempJsonHandle);
			return temp;
	
		}
		temp[i]->quote = json_string_value(json_object_get(quoteDataJSON, "QUOTE"));
		temp[i]->analysis = json_string_value(json_object_get(quoteDataJSON, "ANALYSIS"));
		fprintf(stderr, "%d %s %s\n",i, temp[i]->quote, temp[i]->analysis);
	}
	return temp;
}
/*
	quoteData **loadQuotes(char *filename, int *success):
	Loads the questions from a specified data file

*/
questionData **loadQuestions(char *filename, int *success)
{
	char *questionDataFILE = loadTextFile(filename, success);
	questionData **temp;
	json_t *tempJsonHandle, *questionDataJSON;
	json_error_t errorHandle;
	int numberOfQuestions, i;
	
	tempJsonHandle = json_loads(questionDataFILE,0, &errorHandle);
	if(!tempJsonHandle)
	{
		fprintf(stderr, "json_loads has failed : %s \n", errorHandle.text);
		*success = FAIL;
		return temp;
	
	}
	
	questionDataJSON = json_array_get(tempJsonHandle, 0);
	if(!json_is_object(questionDataJSON))
	{
		fprintf(stderr,"json_object_get failed, didn't get an object\n");
		*success = FAIL;
		json_decref(tempJsonHandle);
		return temp;
	
	}
	numberOfQuestions = json_integer_value(json_object_get(questionDataJSON,"NO_QUESTIONS"));
	temp = malloc(sizeof(questionData *) * numberOfQuestions);
	for(i = 0; i < numberOfQuestions; i++)
	{
		temp[i] = malloc(sizeof(questionData));
		questionDataJSON = json_array_get(tempJsonHandle, i);
		if(!json_is_object(questionDataJSON))
		{
			fprintf(stderr,"json_object_get failed, didn't get an object\n");
			*success = FAIL;
			json_decref(tempJsonHandle);
			return temp;
	
		}
		temp[i]->question = json_string_value(json_object_get(questionDataJSON, "QUESTION"));
		temp[i]->answers[0] = json_string_value(json_object_get(questionDataJSON, "ANSWERONE"));
		temp[i]->answers[1] = json_string_value(json_object_get(questionDataJSON, "ANSWERTWO"));
		temp[i]->answers[2] = json_string_value(json_object_get(questionDataJSON, "ANSWERTHREE"));
		temp[i]->answerNo = json_integer_value(json_object_get(questionDataJSON, "CORRECT"));
		fprintf(stderr, "%d %s %s %s %s\n",i, temp[i]->question, temp[i]->answers[0], temp[i]->answers[1], temp[i]->answers[2]);
	}
	return temp;



}
/*
	char *miscIDToFilePath(int ID, char *path):
	Used to map the IDs in non Level Data files to correct file paths

*/
char *miscIDToFilePath(int ID, char *path)
{
	char filePath [MAX_TEXT_OUTPUT];
	char *mappingFileContents,*mappedPath, *stringPath;
	json_t *tempJsonHandle, *mappingDataJSON;
	json_error_t errorHandle;
	int wasSuccess = SUCCESS;
	snprintf(filePath, MAX_TEXT_OUTPUT,  "%s%s", path, MAPPING_FILE_MISC);
	mappingFileContents = loadTextFile(filePath, &wasSuccess);
	tempJsonHandle = json_loads(mappingFileContents,0, &errorHandle);
	if(!tempJsonHandle)
	{
		fprintf(stderr, "json_loads has failed : %s \n", errorHandle.text);
		return NULL;
	
	}
	
	mappingDataJSON = json_array_get(tempJsonHandle, 0);
	if(!json_is_object(mappingDataJSON))
	{
		fprintf(stderr,"json_object_get failed, didn't get an object\n");
		json_decref(tempJsonHandle);
		return NULL;
	
	}
	itoa(ID, stringPath, 10);
	mappedPath = json_string_value(json_object_get(mappingDataJSON, stringPath));
	if(!mappedPath)
	{
		fprintf(stderr, "The ID you have requested doesn't exist\n");
		return NULL;
	
	}
	return mappedPath;
}