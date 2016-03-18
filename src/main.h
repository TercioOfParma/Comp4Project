/*
	FILE : MAIN.H
	PURPOSE : Holds all constants, Data structure definitions and function prototypes in a manner accessible to all of the C files
	VERSION : 0.001
	NOTES: This could be broken down, but it is more convenient to access just one file across all as it makes compilation less complicated
	and It is not neccessary considering there are less than a hundred functions. Useless variables removed unless they show evidence of progress
	of design.
*/


//====== HEADER FILES =====
//== SDL Libraries =====
#include <SDL.h>//Graphics, Input, Misc Functions
#include <SDL_image.h>//Image handling that aren't .bmp files
#include <SDL_ttf.h>//TrueType fonts
#include <SDL_mixer.h>//Audio

//==== Jansson ========
#include <jansson/jansson.h>//JSON library

//=== Standard C functions and Windows API ======
#include <stdio.h>// I/O to status window and to log.txt
#include <stdlib.h>//malloc etc. 
#include <math.h>//abs 
#include <fcntl.h>//needed for misc functions with unistd
#include <unistd.h>//needed for advanced time and misc functions
#include <time.h>//used to get frame rate
#include <windows.h>//used for the console

//======= PROGRAM CONSTANTS =======
// #defines are used for declaring indexes and the like
#define SIZE_OF_MODIFIERS 6 //The amount of morale modifiers a unit has
#define NO_SIDES 2 //The number of sides in any scenario
#define NO_ANSWERS 3// maximum number of answers a user can give 
#define TEXT_DATA_QUESTION 4//The amount of text data objects utilized by the question data structure
#define TEXT_DATA_QUOTE 2 //The amount of text data objects utilized by the quote data structure
#define LARGE 200000//A maxiumum value for some inputs
#define START_BUTTON 0//ID for the start scenario button
#define QUIT_BUTTON 1//ID for the exit solution button
#define NO_BUTTON_SECONDARY 10000//used as a value to stop the program immediately jumping to the simulation

//The positions in the Modifiers array of various morale modifiers
const static int MODIFIERPOSITION_ROUT 		= 0;//position of rout flag
const static int MODIFIERPOSITION_PANICKED 	= 1;//Panicked flag
const static int MODIFIERPOSITION_STUNNED 	= 2;//Stunned flag
const static int MODIFIERPOSITION_PINNED 	= 3;//Pinned flag
const static int MODIFIERPOSITION_STUCK 	= 4;//Stuck flag -- Vehicles only
const static int MODIFIERPOSITION_IMPETUOUS = 5;//Impetuous flag

//side ids and whether or not functions succeed or fail, you need something you can return, as chars can be used and recognised as ints
const static char FAIL 		= 1;//Returned or success variable is set to this when a function fails
const static char SUCCESS	= 0;//Default value of the success variable or returned on function success
const static int SIDE_ONE 	= 0;//ID for the first side, Whatever is defined in the data file
const static int SIDE_TWO 	= 1;//ID for the second side

//Terrain type identifiers and objectives
const static char TERRAIN_COAST 	= 0;//Coastal terrain identifier
const static char TERRAIN_URBAN 	= 1;//Urban terrain identifier
const static char TERRAIN_JUNGLE	= 2;//Jungle terrain identifier
const static char TERRAIN_RIVER		= 3;//River terrain identifier
const static char TERRAIN_HILL 		= 4;//Hill terrain identifier
const static char TERRAIN_MOUNTAIN 	= 5;//Mountain terrain identifier

//Unit types, see design for why they are chars
const static char UNITTYPE_INFANTRY 	= 0;//Infantry unit identifier
const static char UNITTYPE_MECHANISED 	= 1;//Mechanised unit identifier
const static char UNITTYPE_TANK 		= 2;//Tank unit identifier
const static char UNITTYPE_HELICOPTER 	= 3;//Helicopter unit identifier
const static char UNITTYPE_EMPLACEMENT	= 4;//Emplacement unit identifier
const static char UNITTYPE_RECCE	 	= 5;//Reconnaissance unit identifier
const static char UNITTYPE_AA 			= 6;//Anti Air unit identifier
const static char UNITTYPE_ARTILLERY 	= 7;//Artillery unit identifier

//Default positions, these are needed as there is a requirement to access these files before using ID mapping to access the others
const static char *OPTIONS_FILE 	= "options.json";//options file name
const static char *LEVELS_FILE 		= "levels.json";//levels file name
const static char *ACTIVITY_FILE 	= "activity.json";//activity file name
const static char *QUOTELIST_FILE 	= "quoteList.json";//quotelist file name
const static char *MAPPING_FILE_MISC= "FileIDMapping.json";//module internal id system file name
const static char *SIDE_FILES[] 	= {"sideone.json", "sidetwo.json"};//side file names
const static char *UNIT_FILE 		= "unitData.json";//unit data file name
const static char *LOG_FILE 		= "log.txt";////log file name
const static char *MAPPING_FILE_MAPS= "levelMapping.json";//module independent ID system file name
const static char *MAP_FILE 		= "maps.json";//Map information file name
const static char *TILESET_FILE 	= "tileset.png";//tileset file name
const static char *TILE_FILE 		= "tileData.json";//Map tile position and info file name

//Text
const static int MAX_TEXT_OUTPUT 	= 200;//Maximum text output for SDL_TTF
const static SDL_Color DEFAULT_TEXT = {255,255,255,0}; //Default colour text values

//Positions, so that they will always be in the same place, 
const static int QUOTATION_XPOS 	= 400;//x pos for the quotation
const static int QUOTATION_YPOS 	= 300;//y pos for the quotation
const static int QUESTION_XPOS 		= 250;//x pos for the question
const static int QUESTION_YPOS 		= 200;//y pos for the question
const static int QUOTE_POS 			= 0;//Text data array position of quote
const static int ANALYSIS_POS 		= 1;//Text data array position of analysis
const static int QUESTION_POS 		= 0;//Text data array position of question
const static int ANSWERONE_POS		= 1;//Text data array position of answer 1
const static int ANSWERTWO_POS 		= 2;//Text data array position of answer 2
const static int ANSWERTHREE_POS 	= 3;//Text data array position of answer 3
const static int WINNING_MESSAGE_POSITIONX 	= 300;//x pos for the winning message
const static int WINNING_MESSAGE_POSITIONY 	= 100;//y pos for the winning message
const static int LOSSES_MESSAGE_POSITIONX 	= 400;//x pos for the soldiers killed
const static int LOSSES_MESSAGE_POSITIONY 	= 200;//y pos for the soldiers killed
const static int CONSOLESIZE_X 				= 720;//X size (width) of the console in pixels
const static int CONSOLESIZE_Y 				= 400;//Y size (height) of the console in pixels
const static int BUTTONPOS 		  = 100;//X and Y position of the buttons on the selection menu and X on the main
const static int BUTTONPOS_OFFSET = 200;//The y offset between said buttons
const static int MOUSE_X = 3;//X size (Width) of the mouse
const static int MOUSE_Y = 3;//Y size (height) of the mouse


//Grade percentages
const static int A_GRADE = 80;//Grade boundary for A
const static int B_GRADE = 70;//Grade boundary for B
const static int C_GRADE = 60;//Grade boundary for C
const static int D_GRADE = 50;//Grade boundary for D
const static int E_GRADE = 40;//Grade boundary for E
const static int F_GRADE = 30;//Grade boundary for F
const static int G_GRADE = 20;//Grade boundary for G
const static int N_GRADE = 10;//Grade boundary for N

//Tile size and map position, so the solution knows how and where to draw the map to the screen
const static int TILE_WIDTH 	= 64;//Width of a tile in pixels
const static int TILE_HEIGHT 	= 64;//Height of a tile in pixels
const static int STARTX_MAP 	= 0;//X position on map to start the map drawing
const static int STARTY_MAP 	= 0;//Y position on map to start the map drawing

//needed to see if things hit
const static int TO_HIT = 4;//Required value to meet or exceed in order to score a hit

//Cover saves, 7 so it will never pass, and others are different since units are in cover
const static int DEFAULT_COVER_SAVE = 7;//Required get lower than this to cause a hit after being hit and failed save, effectively 0 effect
const static int URBAN_COVER_SAVE	= 5;//Required get lower than this to cause a hit after being hit and failed save, strongest
const static int JUNGLE_COVER_SAVE 	= 6;//Required get lower than this to cause a hit after being hit and failed save, weakest

//Thresholds used to define whether or not a combat result is such to affect how the unit fights
const static int PINNED_THRESHOLD 	= 2;//Theshold before a unit is pinned
const static int PANICKED_THRESHOLD = 3;//Theshold before a unit is panicked
const static int ROUT_THRESHOLD 	= 3;//Theshold before a unit can be routed
const static int STUNNED_THRESHOLD 	= 3;//Theshold before a unit is stunned

const static int SQRT_TWO = 14;//Needed for A*, corresponds to default diagonal movement cost

//Return values, so the simulation knows what is clicked
const static int END_TURN_BUTTON 	= 20000;//Code returned for end turn
const static int TILE_SELECTED 		= 10;//Code returned for tile selected
const static int UNIT_SELECTED 		= 20;//Code returned for friendly unit selected
const static int UNIT_SELECTED_OTHER= 30;//Code returned for enemy unit selected
const static int NULL_INPUT 		= 12345678;//Null input

//Needed to load the invariable UI buttons
const static char *STARTBUTTON_PATH 	= "gfx/start_button.png";//Path for start button graphic
const static char *QUITBUTTON_PATH 		= "gfx/quit_button.png";//Path for quit button graphic
const static char *NEXTTURNBUTTON_PATH 	= "gfx/next_turn_button.png";//Path for next turn button graphic
const static char *BLANKBUTTON_PATH 	= "gfx/blank_button.png";//Path for blank button graphic


const static float CLICK_RATE = 35.0;//How many miliseconds needed before checking input

//----------------------------------- STRUTURE DEFINITIONS -----------------------------------

/*
	unitData : used to store all the information on a unit

*/
typedef struct
{
	int unitID;//ID of the unit
	//unit stats
	int movement;//The amount of move a unit can do
	int aPRange;//The range a unit can shoot infantry units from
	int aPAttacks;//The amount of potential wounds the unit can inflict on infantyr
	int aTRange;//range for non infantry units
	int aTAttacks;//potential wounds to non infantry units
	char unitType;//the unit type a unit is, see the constants for more details
	int wounds;//the wounds a unit can take
	int save;//The value required to get lower than in order to cause a wound sans cover save
	int morale;//the value morale tests are tested against
	//position on the map
	int relativeX;//Tile position of the unit, X position
	int relativeY;//Tile position of the unit, Y position
	//information
	char *name;//Name of the unit 
	char *description;//Brief description of the unit
	//in simulation modifiers
	char modifiers [ SIZE_OF_MODIFIERS ] ;//morale modifiers
	char alive;//whether or not the unit is alive
	int moved;//whether or not the unit has moved
	int shot;//whether or not the unit has shot
	int coverSave;//cover save
	//display information
	SDL_Rect spriteDimensions;//Dimensions of the unit tile relative to the tile sheet
	SDL_Rect dimensions;//Dimensions of the unit tile relative to the display
	char selected;
}unitData;
/*
	textData: Used to produce a display format for text

*/
typedef struct
{
	SDL_Texture *display;//Texture to display
	SDL_Rect dimensions;//Dimensions to display the texture with
}textData;
/*
	buttonData: Used to produce a display format and information on a button

*/
typedef struct
{
	SDL_Texture *display;//Texture to display
	SDL_Rect dimensions;//Dimensions to display the texture with
	int type;//The type of button the button is
}buttonData;
/*
	buttonData: Used to produce a display format and information on a button with custom text

*/
typedef struct
{
	SDL_Texture *display;//Texture to display
	SDL_Rect dimensions;//Dimensions to display the texture with
	textData *details;//text to display on top of the button
	int type;//type of button
}buttonDataText;
/*
	buttonData: Used to produce a display format and information on a tile

*/
typedef struct
{
	SDL_Rect spriteDimensions;//tile image relative to the tile sheet
	SDL_Rect dimensions;//position relative the the screen
	double angle; //used to spin the tiles so they don't look odd
	//tile information
	int tileID;// ID of the tile
	int civilianPopulation; // population of the tile
	int terrainType;// The type of terrain the tile represents, see the constants and report 
	int relativeX, relativeY;//Tile position of the tile
	int noTiles;// the number of the tiles on the map, used for looped printing etc.
	//for A* Algorithm
	int hScore;// the overall score between distance to the unit and distance to its destination
	char explored;// Whether or not the algorithm has checked this tile
	//during simulation data
	char isOccupied;//set if something is here
	char isSelected;//set if selected by the user
}tileData;
/*
	sideData: Used to hold all the data on a  side in the simulation

*/
typedef struct
{
	unitData **units;//Array of units on a side
	int sideID;//The side's ID
	int noUnits;//The number of units on that side
	int losses;//number of soldiers on that side killed
	int xObjective;// X position of the unused objective system
	int yObjective;// Y position of the unused objective system
	int turnsHeld;//Number of turns the objective would be held
}sideData;
/*
	quoteData: holds information on a quote

*/
typedef struct
{
	char *quote;//String holding the quote
	char *analysis;//String holding the analysis
	int noQuotes;//Number of quotes in the scenario, necessary for drawing etc.
	textData *display [ TEXT_DATA_QUOTE ];//Graphically renderable form of the text
}quoteData;
/*
	quoteData: holds information on quotes for a scenario

*/
typedef struct
{
	quoteData **quotes;//array of quotes
	int quoteID;//ID of the quote list

}quoteListData;
/*
	questionData: holds information on a question
*/
typedef struct
{
	char *question;//String holding the question
	char *answers[ NO_ANSWERS ];//String holding the answers
	int answerNo;//The value that is the answer
	textData *display [ TEXT_DATA_QUESTION ];//Displayable versions of the strings
}questionData;
/*
	activityData: holds information on an activity

*/
typedef struct
{
	int activityID;//ID of the activity
	char *title;//Title of the activity
	int maximumMark;//The number of questions to be ran through in the code
	questionData **questions;//The questions
}activityData;
/*
	optionsData: holds information on options
	NOTE: The names are in the snake case of the constants because they are intended to be program constants, but the nature of Structures means that
	const makes them read only members which I cannot load into 
*/
typedef struct
{
	//General options needed to run the application
	int SCREEN_WIDTH, SCREEN_HEIGHT, SAMPLE_FREQUENCY, NO_CHANNELS, SAMPLE_SIZE, FONT_SIZE;
	char *WINDOW_TITLE, *DEFAULT_FONT;
}optionsData;
/*
	mapData: holds information on a simulation scenario

*/
typedef struct
{
	//subordinate scenario componenets
	tileData **tiles;//The map tiles
	quoteListData *quoteList;//The quotes
	sideData *sides[ NO_SIDES ];//Each side and their units
	//map information
	int mapID;//The map ID
	char *title;//The title of the scenario
	char *description;//A quick synopsis of the scenario
	char *path;//The file path of the scenario
	textData *descriptionData;//Display for the description UNUSED
	textData *titleData;//Display for the title UNUSED
	activityData *activity;//The activity
	SDL_Texture *tileset;//The tileset of the simulation
}mapData;

typedef struct
{
	int noLevels;//The number of levels
	mapData **maps;//The levels
}levelData;

//====== FUNCTION PROTOTYPES ==========
/*
	The descriptions for the functions are above the functions, too verbose really to do
	here. I want to go into a decent level of detail on the functionality 

*/
//=======INITIALISATION ===========

char *loadTextFile( const char *filename , int *success );

int getFileSize( FILE *sizeToGet , int *success );

optionsData initOptions( char *fileContents , int *success );

SDL_Window *initSDL( optionsData *opt , int *success );

SDL_Renderer *createRenderer( SDL_Window *screen , int *success );

Mix_Music *loadMusic( const char *filename , int *success );

Mix_Chunk *loadEffect( const char *filename , int *success );

TTF_Font *loadFont( optionsData *opt , int *success );

SDL_Texture *loadImage( const char *filename , SDL_Renderer *render , SDL_Rect *dimen , int *success );

buttonData *loadButton( SDL_Texture *display , SDL_Rect *posAndSize , int type , int *success );

buttonDataText *loadButtonText( SDL_Texture *display , SDL_Rect *posAndSize , SDL_Renderer *render ,
 const char *initialData , TTF_Font *font , int type , int *success );

levelData *loadLevelData( SDL_Renderer *render , int *success );

char *mapLevelIDToMapPath( int id , int *success );

char *miscIDToFilePath( int ID , char *path );

mapData *loadMapData( char *levelDataFile , int mapNo , SDL_Renderer *render , int *success );

sideData *loadSideData( char *filename , int sideNumber , int *success );

unitData **loadUnitData( char *sideUnitDataFilePath , char *unitDescriptorDataFilePath , int *success );

tileData **loadTileData( char *tileFile , int *success );

quoteListData *loadQuoteListData( char *filename , int *success );

quoteData **loadQuotes( char *filename , int *success );

activityData *loadActivity( char *filename , int *success );

questionData **loadQuestions( char *filename , int *success );

unitData *loadUnit( char *unitFile , int ID , int *success );

//----------DEINITIALISATION-----------
void endSDL( SDL_Renderer *render , SDL_Window *screen , TTF_Font *font );

void endUnitDataArray( unitData **units , int size );

void endSideData( sideData *side );

void endTileData( tileData **tiles , int size );

void endQuoteListData( quoteListData *quotes );

void endQuotes( quoteData **quotes , int size );

void endActivity( activityData *activity );

void endQuestions( questionData **questions , int size );

void endLevel( levelData *level );


//----------DRAWING AND GRAPHICAL-----
void drawTerrain( tileData **toDraw , int size , SDL_Renderer *render , SDL_Texture *tileMap );

void drawUnits( unitData **toDraw , int size ,  SDL_Renderer *render , SDL_Texture *tileMap );

void drawMenuElements( buttonData **buttons , int size , SDL_Renderer *render );

void drawMenuElementsText( buttonDataText **buttons , int size , SDL_Renderer *render );

textData *renderText( TTF_Font *font , SDL_Renderer *render , const char *stringToTexture , int *success );

void drawText( textData *toDraw , SDL_Renderer *render );

void drawQuote( quoteData **quotes , int quoteNo , SDL_Renderer *render , TTF_Font *font );

void drawQuestion( questionData **questions , int questionNo , SDL_Renderer *render , TTF_Font *font );



//----------INPUT---------------------

int checkButtonClicked( SDL_Rect *mouseDimensions , buttonData *button );

int checkButtonTextClicked( SDL_Rect *mouseDimensions , buttonDataText *button );

int handleMouseButtonMainMenu( buttonData **buttons , int size , SDL_Renderer *render , SDL_Event *events );

int handleMouseButtonSelectionMenu( buttonDataText **buttonsText , int size , SDL_Renderer *render 
, SDL_Event *events );

int handleMapClicked( sideData *applicableUnits , sideData *otherSide , tileData **tiles ,  buttonData **endTurn 
, SDL_Event *events , int *turnButtonClicked);

int checkQuestionClicked( SDL_Rect *mouseDimensions , questionData *question , int answerNo );

int checkUnitClicked( SDL_Rect *mouseDimensions , unitData *unit );

int checkTileClicked( SDL_Rect *mouseDimensions , tileData *tile );

//---------SIMULATION----------------

int aStarWithTerrain( sideData *applicableUnits , int unitNo , tileData **tiles , int xPos , int yPos , int size );

void moveUnit( sideData *applicableUnits , tileData **tiles , int xPos , int yPos , int givenUnit );

int shootUnit( sideData *shootingSideUnits , int shootingSideNo , sideData *recievingSideUnits , int recievingSideNo , 
tileData **tiles );

void resolveShooting( sideData *shootingSideUnits , int shootingSideNo , sideData *recievingSideUnits , 
int recievingSideNo , int inflictedCasualties , int recievedCasualties );

void displaySimulationResults( sideData *sideOne , sideData *sideTwo , tileData **map , TTF_Font *font ,
 SDL_Renderer *render );

int findDistance( tileData *tileOne , tileData *tileTwo );

void simulationMain( mapData *map , SDL_Renderer *render , TTF_Font *font , buttonData **nextTurn , 
SDL_Event *events , int *success );

//---------ACTIVITY------------------
int startQuiz( activityData *quiz , SDL_Renderer *render , TTF_Font *font , SDL_Event *events ,  int *success );


char computeGrade( activityData *quiz , int noCorrect );

void displayActivityResults( activityData *quiz , char *resultsToDisplay , SDL_Renderer *render , TTF_Font *font );


/*
	END OF FILE


*/
