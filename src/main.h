/*
	FILE : MAIN.H
	PURPOSE : Holds all constants, Data structure definitions and function prototypes in a manner accessible to all of the C files
	VERSION : 0.001
	NOTES: This could be broken down, but it is more convenient to access just one file across all
	
	FUNCTIONS COMPLETED : 42/56
*/


//----------------------------------- HEADER FILES AND C PREPROCESSOR (MINUS #define)------------------------
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <jansson/jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>

//----------------------------------- PROGRAM CONSTANTS --------------------------------------
#define SIZE_OF_MODIFIERS 6
#define NO_SIDES 2
#define NO_ANSWERS 3
#define TEXT_DATA_QUESTION 4
#define TEXT_DATA_QUOTE 2
const static int MODIFIERPOSITION_ROUT = 0;
const static int MODIFIERPOSITION_PANICKED = 1;
const static int MODIFIERPOSITION_STUNNED = 2;
const static int MODIFIERPOSITION_PINNED = 3;
const static int MODIFIERPOSITION_STUCK = 4;
const static int MODIFIERPOSITION_IMPETUOUS = 5;
const static char FALSE = 0;
const static char TRUE = 1;
const static char FAIL = 0;
const static char SUCCESS = 1;
const static int SIDE_ONE = 0;
const static int SIDE_TWO = 1;
const static int TERRAIN_COAST = 0;
const static int TERRAIN_URBAN = 1;
const static int TERRAIN_JUNGLE = 2;
const static int TERRAIN_RIVER = 3;
const static int TERRAIN_HILL = 4;
const static int TERRAIN_MOUNTAIN = 5;
const static int OBJECTIVE_SEARCHANDDESTROY = 0;
const static int OBJECTIVE_TAKEANDHOLD = 1;
const static int UNITTYPE_INFANTRY = 0;
const static int UNITTYPE_MECHANISED = 1;
const static int UNITTYPE_TANK = 2;
const static int UNITTYPE_HELICOPTER = 3;
const static int UNITTYPE_EMPLACEMENT = 4;
const static int UNITTYPE_RECCE = 5;
const static int UNITTYPE_AA = 6;
const static int UNITTYPE_ARTILLERY = 7;
const static char *OPTIONS_FILE = "options.json";
const static char *LEVELS_FILE = "levels.json";
const static int MAX_TEXT_OUTPUT = 200;
const static SDL_Color DEFAULT_TEXT = {255,255,255,0}; 
const static int QUOTATION_XPOS = 400;
const static int QUOTATION_YPOS = 300;
const static int QUESTION_XPOS = 250;
const static int QUESTION_YPOS = 200;
const static char *MAPPING_FILE_MISC = "FileIDMapping.json";
const static int QUOTE_POS = 0;
const static int ANALYSIS_POS = 1;
const static int QUESTION_POS = 0;
const static int ANSWERONE_POS = 1;
const static int ANSWERTWO_POS = 2;
const static int ANSWERTHREE_POS = 3;
const static int A_GRADE = 80;
const static int B_GRADE = 70;
const static int C_GRADE = 60;
const static int D_GRADE = 50;
const static int E_GRADE = 40;
const static int F_GRADE = 30;
const static int G_GRADE = 20;
const static int N_GRADE = 10;
const static char *ACTIVITY_FILE = "activity.json";
const static char *QUOTELIST_FILE = "quoteList.json";
const static int TILE_WIDTH = 64;
const static int TILE_HEIGHT = 64;
const static int STARTX_MAP = 100;
const static int STARTY_MAP = 100;
const static char *SIDE_FILES[] = {"sideone.json", "sidetwo.json"};
const static char *UNIT_FILE = "unitData.json";
const static char *LOG_FILE = "log.txt";
const static char *MAPPING_FILE_MAPS = "levelMapping.json";
const static char *MAP_FILE = "maps.json";
const static char *TILESET_FILE = "tileset.png";
const static char *TILE_FILE = "tileData.json";
const static int TO_HIT = 4;
const static int DEFAULT_COVER_SAVE = 7;
const static int URBAN_COVER_SAVE = 5;
const static int JUNGLE_COVER_SAVE = 6;
const static int PINNED_THRESHOLD = 2;
const static int PANICKED_THRESHOLD = 3;
const static int ROUT_THRESHOLD = 3;
const static int STUNNED_THRESHOLD = 3;
#define LARGE_TEXT_FILE 20000000
//----------------------------------- STRUTURE DEFINITIONS -----------------------------------

/*
	unitData : used to store all the information of a unit

*/
typedef struct
{
	int unitID;
	//unit stats
	int movement;
	int aPRange;
	int aPAttacks;
	int aTRange;
	int aTAttacks;
	int unitType;
	int wounds;
	int save;
	int morale;
	//position on the map
	int relativeX;
	int relativeY;
	//information
	char *name;
	char *description;
	//in simulation modifiers
	char modifiers [SIZE_OF_MODIFIERS] ;
	char alive;
	int moved;
	int shot;
	int coverSave;
	//display information
	SDL_Rect spriteDimensions;
	SDL_Rect dimensions;

}unitData;
/*
	textData: Used to produce a display format for text

*/
typedef struct
{
	SDL_Texture *display;
	SDL_Rect dimensions;

}textData;
/*
	buttonData: Used to produce a display format and information on a button

*/
typedef struct
{
	SDL_Texture *display;
	SDL_Rect dimensions;
	int type;

}buttonData;
/*
	buttonData: Used to produce a display format and information on a button with custom text

*/
typedef struct
{
	SDL_Texture *display;
	SDL_Rect dimensions;
	textData *details;
	int type;

}buttonDataText;
/*
	buttonData: Used to produce a display format and information on a tile

*/
typedef struct
{
	SDL_Rect spriteDimensions;
	SDL_Rect dimensions;
	double angle; //used to spin the tiles so they don't look odd
	//tile information
	int tileID;
	int civilianPopulation;
	int terrainType;
	int relativeX, relativeY;
	int noTiles;
	//for A* Algorithm
	int gScore;
	int fScore;
	//during simulation data
	char isOccupied;
}tileData;
/*
	sideData: Used to hold all the data on a  side in the simulation

*/
typedef struct
{
	unitData **units;
	int sideID;
	int noUnits;
	int losses;
	int xObjective;
	int yObjective;
	int turnsHeld;

}sideData;
/*
	quoteData: holds information on a quote

*/
typedef struct
{
	char *quote;
	char *analysis;
	textData *display [TEXT_DATA_QUOTE];

}quoteData;
/*
	quoteData: holds information on quotes for a scenario

*/
typedef struct
{
	quoteData **quotes;
	int quoteID;


}quoteListData;
/*
	questionData: holds information on a question

*/
typedef struct
{
	char *question;
	char *answers[NO_ANSWERS];
	int answerNo;
	textData *display [TEXT_DATA_QUESTION];

}questionData;
/*
	activityData: holds information on an activity

*/
typedef struct
{
	int activityID;
	char *title;
	int maximumMark;
	questionData **questions;

}activityData;
/*
	optionsData: holds information on options
	NOTE: The names are in the snake case of the constants because they are intended to be program constants, but the nature of Structures means that
	const makes them read only members which I cannot load into 
*/
typedef struct
{
	int SCREEN_WIDTH, SCREEN_HEIGHT, SAMPLE_FREQUENCY, NO_CHANNELS, SAMPLE_SIZE, FONT_SIZE;
	char *WINDOW_TITLE, *DEFAULT_FONT;


}optionsData;
/*
	mapData: holds information on a simulation scenario

*/
typedef struct
{
	//subordinate scenario componenets
	tileData **tiles;
	quoteListData *quoteList;
	sideData *sides[NO_SIDES];
	//map information
	int mapID;
	char *title;
	char *description;
	char *path;
	textData *descriptionData;
	textData *titleData;
	activityData *activity;
	SDL_Texture *tileset;
}mapData;

typedef struct
{
	int noLevels;
	mapData **maps;

}levelData;

//----------------------------------- FUNCTION PROTOTYPES ------------------------------------
//-----------INITIALISATION-----------
//loading libraries and the files themselves
char *loadTextFile(const char *filename, int *success);// DONE
int getFileSize(FILE *sizeToGet, int *success);// DONE
optionsData initOptions(char *fileContents, int *success);// DONE
SDL_Window *initSDL(optionsData *opt, int *success);// DONE
SDL_Renderer *createRenderer(SDL_Window *screen, int *success);// DONE
Mix_Music *loadMusic(const char *filename, int *success);// DONE
Mix_Chunk *loadEffect(const char *filename, int *success);// DONE
TTF_Font *loadFont(optionsData *opt, int *success);// DONE
SDL_Texture *loadImage(const char *filename, SDL_Renderer *render, SDL_Rect *dimen, int *success);// DONE
//runtime objects
buttonData *loadButton(SDL_Texture *display, SDL_Rect *posAndSize, int type, int *success);// DONE
//there was createTextData here, but i felt it unneccessary, Can always roll back changes need be
buttonDataText *loadButtonText(SDL_Texture *display, SDL_Rect *posAndSize, SDL_Renderer *render, const char *initialData, TTF_Font *font, int type, int *success);//DONE
levelData *loadLevelData(SDL_Renderer *render, int *success);//DONE
char *mapLevelIDToMapPath(int id, int *success);//DONE
char *miscIDToFilePath(int ID, char *path);//DONE
mapData *loadMapData(char *levelDataFile ,int mapNo, SDL_Renderer *render,int *success);//DONE
sideData *loadSideData(char *filename,int sideNumber, int *success);//DONE
unitData **loadUnitData(char *sideUnitDataFilePath, char *unitDescriptorDataFilePath, int *success);//DONE
tileData **loadTileData(char *tileFile, int *success);//DONE
quoteListData *loadQuoteListData(char *filename, int *success);//DONE
quoteData **loadQuotes(char *filename, int *success);//DONE
activityData *loadActivity(char *filename, int *success);//DONE
questionData **loadQuestions(char *filename, int *success);//DONE
unitData *loadUnit(char *unitFile, int ID, int *success);//DONE
//----------DEINITIALISATION-----------
void endSDL(SDL_Renderer *render, SDL_Window *screen, TTF_Font *font);//DONE
void endUnitDataArray(unitData **units, int size);//DONE
void endSideData(sideData *side);//DONE
void endTileData(tileData **tiles, int size);//DONE
void endQuoteListData(quoteListData *quotes);//DONE
void endQuotes(quoteData **quotes, int size);//DONE
void endActivity(activityData *activity);//DONE
void endQuestions(questionData **questions, int size);//DONE
void endLevel(levelData *level);//DONE

//----------DRAWING AND GRAPHICAL-----
void drawTerrain(tileData **toDraw, int size, SDL_Renderer *render, SDL_Texture *tileMap);//DONE
void drawUnits(unitData **toDraw, int size,  SDL_Renderer *render, SDL_Texture *tileMap);//DONE
void drawMenuElements(buttonData **buttons,int size, SDL_Renderer *render);// DONE
void drawMenuElementsText(buttonDataText **buttons, int size, SDL_Renderer *render);//DONE
textData *renderText(TTF_Font *font, SDL_Renderer *render, const char *stringToTexture,int *success);// DONE
void drawText(textData *toDraw,SDL_Renderer *render);// DONE
void drawQuote(quoteData **quotes, int quoteNo, SDL_Renderer *render, TTF_Font *font);//DONE
void drawQuestion(questionData **questions, int questionNo, SDL_Renderer *render, TTF_Font *font);//DONE


//----------INPUT---------------------
int checkButtonClicked(SDL_Rect *mouseDimensions, buttonData *button);//DONE
int checkButtonTextClicked(SDL_Rect *mouseDimensions, buttonDataText *button);
int handleMouseButtonMainMenu(SDL_Rect mouseDimensions, buttonData **buttons, buttonDataText **buttonsText);
int handleMouseButtonSelectionMenu(SDL_Rect mouseDimensions, buttonData **buttons, buttonDataText **buttonsText);
int handleKeyboardSimulation(SDL_Event *keyboardInput, unitData **units);
int handleMapClicked(unitData **applicableUnits, tileData **tiles,  buttonData **buttons, buttonDataText **textButtons);
int checkQuestionClicked(SDL_Rect *mouseDimensions, questionData *question, int answerNo);//DONE
//---------SIMULATION----------------
int aStarWithTerrain(sideData *applicableUnits, int unitNo, tileData **tiles, int xPos, int yPos);
void moveUnit(sideData *applicableUnits, tileData **tiles, int xPos, int yPos, int givenUnit);//DONE
int shootUnit(sideData *shootingSideUnits, int shootingSideNo, sideData *recievingSideUnits, int recievingSideNo, tileData **tiles);//DONE
void resolveShooting(sideData *shootingSideUnits, int shootingSideNo, sideData *recievingSideUnits, int recievingSideNo,int inflictedCasualties, int recievedCasualties);//DONE
void displaySimulationResults(sideData *sideOne, sideData *sideTwo, tileData *map);

//---------ACTIVITY------------------
int startQuiz(activityData *quiz, SDL_Renderer *render, TTF_Font *font, int *success);//DONE
//there was askQuestion here but it seemed unneccessary, so Its been folded into startQuiz
char computeGrade(activityData *quiz, int noCorrect);//DONE
void displayActivityResults(activityData *quiz, char *resultsToDisplay, SDL_Renderer *render, TTF_Font *font);//DONE