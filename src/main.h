/*
	FILE : MAIN.H
	PURPOSE : Holds all constants, Data structure definitions and function prototypes in a manner accessible to all of the C files
	VERSION : 0.001
	NOTES: This could be broken down, but it is more convenient to access just one file across all
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
const static int MODIFIERPOSITION_ROUT = 0;
const static int MODIFIERPOSITION_PANICKED = 1;
const static int MODIFIERPOSITION_STUNNED = 2;
const static int MODIFIERPOSITION_PINNED = 3;
const static int MODIFIERPOSITION_STUCK = 4;
const static int MODIFIERPOSITION_IMPETOUOUS = 5;
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
	int aRRange;
	int aRAttacks;
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
	//display information
	SDL_Texture *display;
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
	SDL_Texture *display;
	SDL_Rect dimensions;
	//tile information
	int tileID;
	int civilianPopulation;
	int relativeX;
	int relativeY;
	int terrainType;
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
	int losses;
	int xObjectve;
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
	int tilesetID;
	int quoteID;
	int mapWidth;
	int mapHeight;
	char *title;
	char *description;

}mapData;
/*
	questionData: holds information on a question

*/
typedef struct
{
	char *question;
	char *answers[NO_ANSWERS];
	int answerNo;


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

*/
typedef struct
{
	int SCREEN_WIDTH, SCREEN_HEIGHT, SAMPLE_FREQUENCY, NO_CHANNELS, SAMPLE_SIZE;
	char *windowTitle;


}optionsData;
/*
	levelData: holds information on the levels
*/
typedef struct
{
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
TTF_Font *loadFont(const char *filename, int size,  int *success);// DONE
SDL_Texture *loadImage(const char *filename, SDL_Renderer *render, SDL_Rect *dimen, int *success);// DONE
//runtime objects
buttonData *loadButton(SDL_Texture *display, SDL_Rect *posAndSize, int type, int *success);// DONE
textData *createTextData(TTF_Font *font, SDL_Renderer *render, SDL_Rect *posAndSize, const char *initialData);
buttonDataText *loadButtonText(SDL_Texture *display, SDL_Rect *posAndSize, SDL_Renderer *render, const char *initialData, int type, int *success);
levelData *loadLevelData(char *levelFileData, int *success);
char *mapLevelIDToMapPath(levelData *levelDataToChoose, char *mappingFile, char id, int *success);//used to find the mapData file + directory
char *miscIDToFilePath(mapData *map, char *mappingFile, char ID, int *success);//this will be used to map tilesets and unitSide files together
mapData *loadMapData(char *filename);
sideData *loadSideData(char *filename, int *success);
unitData **loadUnitData(char *sideFile, int *success);//likely a very big function
tileData **loadTileData(char *tileFile, int *success);
quoteListData *loadQuoteListData(char *filename, int *success);
quoteData **loadQuotes(char *filename, int *success);
activityData *loadActivity(char *filename, int *success);
questionData **loadQuestions(char *filename, int *success);

//----------DEINITIALISATION-----------
void endSDL(SDL_Renderer *render, SDL_Window *screen, TTF_Font *font);
void endUnitDataArray(unitData **units);
void endSideData(sideData *side);
void endTileData(tileData **tiles);
void endQuoteListData(quoteListData *quotes);
void endQuotes(quoteData **quotes);
void endActivity(activityData *activity);
void endQuestions(questionData **questions);
void endLevels(levelData *levels);

//----------DRAWING AND GRAPHICAL-----
void drawTerrain(tileData **toDraw, int size, SDL_Renderer *render);
void drawUnits(unitData *toDraw, int size,  SDL_Renderer *render);
void drawMenuElements(buttonData **buttons,int size, SDL_Renderer *render);// DONE
void drawMenuElementsText(buttonDataText **buttons, int size, SDL_Renderer *render);
textData *renderText(TTF_Font *font, SDL_Renderer *render, const char *stringToTexture);
void drawText(textData *toDraw, int size,  SDL_Renderer *render);
void drawQuote(quoteData **quotes, int quoteNo, SDL_Renderer *render);
void drawQuestion(questionData **questions, int quoteNo, SDL_Renderer *render);


//----------INPUT---------------------
int checkButtonClicked(SDL_Rect *mouseDimensions, buttonData *button);
int checkButtonTextClicked(SDL_Rect *mouseDimensions, buttonDataText *button);
int handleMouseButtonMainMenu(SDL_Rect mouseDimensions, buttonData **buttons, buttonDataText **buttonsText);
int handleMouseButtonSelectionMenu(SDL_Rect mouseDimensions, buttonData **buttons, buttonDataText **buttonsText);
int handleKeyboardSimulation(SDL_Event *keyboardInput, unitData **units);
int handleMapClicked(unitData **applicableUnits, tileData **tiles,  buttonData **buttons, buttonDataText **textButtons);
int handleActivityClicked(questionData **questions, buttonData **buttons, buttonDataText **textButtons);

//---------SIMULATION----------------
int aStarWithTerrain(unitData **applicableUnits, int unitNo, tileData **tiles, int xPos, int yPos);
void movementMorale(unitData **applicableUnits);
void moveUnit(unitData **applicableUnits, int givenUnit);
void shootUnit(unitData **shootingSideUnits, int shootingSideNo, unitData **recievingSideUnits, int recievingSideNo);
void resolveShooting(unitData **shootingSideUnits, int shootingSideNo, unitData **recievingSideUnits, int recievingSideNo,int inflictedCasualties, int recievedCasualties);
void calculateMoraleModifier(unitData **losingSideUnits, int losingSideNo, int result);
void displaySimulationResults(sideData *sideOne, sideData *sideTwo, tileData *map);

//---------ACTIVITY------------------
void startQuiz(activityData *quiz);
int askQuestion(questionData **questions, int questionNo);
char computeGrade(activityData *quiz, int noCorrect);
void displayActivityResults(activityData *quiz, char grade);
