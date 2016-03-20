/*
	FILE : SIMULATION.C
	PURPOSE : Hold all functions related to computing the simulation (movement, shooting, morale etc)
	VERSION : 0.001
	NOTES: Could get far more complex in later non coursework versions
*/
//============ Preprocessor ======================

#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif
//=========== Functions ==========================
/*
	============================================================================
	int aStarWithTerrain(sideData *applicableUnits, int unitNo, tileData **tiles, int xPos, int yPos):
	used to see if a unit can move or shoot a tile
	
	When the program works normally, it will return the minimum length
	The other will return a zero, as this is when somehow the unit attempts to use the
	algorithm to get to their own tile
	============================================================================

*/
int aStarWithTerrain( sideData *applicableUnits , int unitNo , tileData **tiles , int xPos , int yPos , int size )
{
	fprintf( stderr , "A* Algorithm to X : %d  y : %d from : X : %d, Y : %d\n" , xPos , yPos , 
	applicableUnits->units[ unitNo ]->relativeX , applicableUnits->units[ unitNo ]->relativeY );
	int posBeginX, posBeginY, posEndX, posEndY, posArrayStart, posArrayEnd, posCurrentX, posCurrentY, posArrayCurrent, i, favouredPositionArray, favouredPositionLength, totalLength, favouredPositionToEnd, isSearched;
	posArrayStart = 0;
	posArrayEnd = 0;
	posBeginX = 0;
	posEndX = 0;
	posBeginY = 0;
	posEndY = 0;
	//=================================== INIT ============================
	while( posBeginX != applicableUnits->units[ unitNo ]->relativeX || posBeginY != applicableUnits->units[ unitNo ]->relativeY )
	{
			posArrayStart++;	
			posBeginX = tiles[ posArrayStart ]->relativeX;
			posBeginY = tiles[ posArrayStart ]->relativeY;
	}
	while( posEndX != xPos || posEndY != yPos )
	{
			posArrayEnd++;	
			posEndX = tiles[ posArrayEnd ]->relativeX;
			posEndY = tiles[ posArrayEnd ]->relativeY;
	}
	if( posArrayStart == posArrayEnd )
	{
		return 0;
	}
	tiles[ posArrayStart ]->hScore = 0;
	posCurrentX = posBeginX;
	posCurrentY = posBeginY;
	posArrayCurrent = posArrayStart;
	totalLength = 0;
	for( i = 0 ; i < size ; i++ )
	{
		tiles[i]->explored = FALSE;
		tiles[i]->isSelected = FALSE;
	}
	tiles[ posArrayStart ]->explored = TRUE;
	//================================================= FULL A* IMPLEMENTATION =======================================
	while( TRUE )
	{
		tiles[ posArrayCurrent ]->explored = TRUE;
		favouredPositionLength = LARGE;
		favouredPositionToEnd = LARGE;
		for( i = 0 ; i < size ; i++ )
		{
			tiles[i]->hScore = 0;
			if( tiles[i]->explored == TRUE )
			{
				isSearched++;
			}
			if(  tiles[i]->explored == FALSE && (abs ( posCurrentX - tiles[i]->relativeX ) < 2 && abs( posCurrentY - tiles[i]->relativeY ) < 2) )
			{
				if( tiles[i]->terrainType != TERRAIN_RIVER || tiles[i]->terrainType != TERRAIN_MOUNTAIN || applicableUnits->units[ unitNo ]->unitType == UNITTYPE_HELICOPTER )
				{
					tiles[i]->hScore = findDistance( tiles[i] , tiles[ posArrayStart ] ) + findDistance( tiles[i] , tiles[ posArrayEnd ] );
					if( applicableUnits->units[ unitNo ]->unitType != UNITTYPE_INFANTRY && ( tiles[i]->terrainType == TERRAIN_URBAN || tiles[i]->terrainType == TERRAIN_JUNGLE ) )
					{
						tiles[i]->hScore += 5;//rough terrain movement and shooting modifier for non infantry in jungle and cities
					}
					if( favouredPositionLength > tiles[i]->hScore )//checks to see if the new h score is lower
					{
						favouredPositionLength = tiles[i]->hScore;
						tiles[i]->explored = TRUE;
						favouredPositionArray = i;
						favouredPositionToEnd = findDistance( tiles[i] , tiles[ posArrayEnd ] );
					}
					if( favouredPositionLength == tiles[i]->hScore && favouredPositionToEnd > findDistance( tiles[i] , tiles[ posArrayEnd ] ) )
					{
						favouredPositionLength = tiles[i]->hScore;
						tiles[i]->explored = TRUE;
						favouredPositionArray = i;
						favouredPositionToEnd = findDistance( tiles[i] , tiles[ posArrayEnd ] );
					}
					if( i == posArrayEnd )//since all tiles have a unique position this works
					{
						favouredPositionLength = tiles[i]->hScore;
						tiles[i]->explored = TRUE;
						favouredPositionArray = i;
						break;
					}
				}
			}
		}
		if( favouredPositionLength != LARGE )
		{
			totalLength += findDistance( tiles[ favouredPositionArray ], tiles[ posArrayCurrent ]);
			if( applicableUnits->units[ unitNo ]->unitType != UNITTYPE_INFANTRY && ( tiles[ favouredPositionArray ]->terrainType == TERRAIN_URBAN || tiles[ favouredPositionArray ]->terrainType == TERRAIN_JUNGLE ) )
			{
				totalLength += 5;
			}
			posArrayCurrent = favouredPositionArray;
			posCurrentY = tiles[ favouredPositionArray ]->relativeY;
			posCurrentX = tiles[ favouredPositionArray ]->relativeX;
			
		}
		if( i == posArrayEnd || ( posCurrentY == yPos && posCurrentX == xPos ) || isSearched ==  size - 1 )
		{
			break;
		}
	}
	return totalLength;
}
/*
	====================================================================
	int findDistance( tileData *tileOne , tileData *tileTwo ):
	Finds the distance, factoring diagonals where applicable
	
	returns the total distance
	====================================================================

*/
int findDistance( tileData *tileOne , tileData *tileTwo )
{
	int totalDistance, xDistance, yDistance;
	totalDistance = 0;
	xDistance = abs( tileOne->relativeX - tileTwo->relativeX );
	yDistance = abs( tileOne->relativeY - tileTwo->relativeY );
	while( xDistance > 0 && yDistance > 0 )
	{
		xDistance--;
		yDistance--;
		totalDistance += SQRT_TWO;
	}
	totalDistance += ( xDistance * 10 ) + ( yDistance * 10 );
	return totalDistance;
}
/*
	===============================================================================
	void moveUnit( sideData *applicableUnits , tileData **tiles , int xPos , 
	int yPos , int givenUnit ):
	moves a unit
	
	===============================================================================
*/
void moveUnit( sideData *applicableUnits , tileData **tiles , int xPos , int yPos , int givenUnit )
{
	fprintf( stderr , "Moving unit %d, %s\n" , applicableUnits->units[ givenUnit ]->unitID , applicableUnits->units[ givenUnit ]->name );
	int aStarResult;
	int i, j, k, stuckMove, morale;
	k = 0;
	i = 0;
	j = 0;
	if(applicableUnits->units[ givenUnit ]->moved == TRUE)//checks to see if the unit has moved
	{
		fprintf( stdout , "%s has Already moved!\n", applicableUnits->units[ givenUnit ]->name);
		applicableUnits->units[ givenUnit ]->selected = FALSE;
		return;
	
	}
	applicableUnits->units[ givenUnit ]->moved = TRUE;
	//============================= Morale Checks =============================================
	//no sense including impetouous here because they will have lost it if they are any of these
	if(applicableUnits->units[ givenUnit ]->modifiers[ MODIFIERPOSITION_ROUT ] == TRUE)
	{
		morale = rand() % 12 + 1;
		if(morale <= applicableUnits->units[ givenUnit ]->morale)
		{
			applicableUnits->units[ givenUnit ]->modifiers[ MODIFIERPOSITION_ROUT ] = FALSE;
		
		}
		else
		{
			fprintf(stdout, "ROUTING!\n");
			xPos = applicableUnits->units[ givenUnit ]->relativeX + 1;
			yPos = applicableUnits->units[ givenUnit ]->relativeY + 1;
		
		}
	
	}
	if( applicableUnits->units[ givenUnit ]->modifiers[ MODIFIERPOSITION_STUNNED ] == TRUE || 
	applicableUnits->units[ givenUnit ]->modifiers[ MODIFIERPOSITION_PANICKED ] == TRUE )
	
	{
		morale = rand() % 12 + 1;
		if(morale <= applicableUnits->units[ givenUnit ]->morale)
		{
			applicableUnits->units[ givenUnit ]->modifiers[ MODIFIERPOSITION_STUNNED ] = FALSE;
			applicableUnits->units[ givenUnit ]->modifiers[ MODIFIERPOSITION_PANICKED ] = FALSE;
		}
		else
		{
			fprintf(stdout, "Unable to move: Stunned or Panicked!\n");
			return;
		
		}
	
	}
	if( applicableUnits->units[ givenUnit ]->modifiers[ MODIFIERPOSITION_PINNED ] == TRUE )
	{
		fprintf(stderr, "Can't move: We're Pinned down!\n");
		return;
	
	}
	//====================== Movement ======================================
	if( applicableUnits->units[ givenUnit ]->modifiers[ MODIFIERPOSITION_STUCK ] == TRUE )
	{
		stuckMove = ( applicableUnits->units[ givenUnit ]->movement / 3 );
		applicableUnits->units[ givenUnit ]->movement -= stuckMove;
		aStarResult = aStarWithTerrain( applicableUnits , givenUnit , tiles , xPos , yPos , tiles[0]->noTiles );
		fprintf( stdout , " Action cost : %d ,Movement value : %d\n" , aStarResult ,  applicableUnits->units[ givenUnit ]->movement );
		applicableUnits->units[ givenUnit ]->movement += stuckMove;
	}
	else
	{
		aStarResult = aStarWithTerrain( applicableUnits , givenUnit , tiles , xPos , yPos , tiles[0]->noTiles );
		fprintf( stdout , " Action cost : %d ,Movement value : %d\n" , aStarResult ,  applicableUnits->units[ givenUnit ]->movement );
	}
	if( aStarResult <= applicableUnits->units[ givenUnit ]->movement && 
	applicableUnits->units[ givenUnit ]->modifiers[ MODIFIERPOSITION_STUNNED ] == FALSE &&//Where the unit actually moves
	applicableUnits->units[ givenUnit ]->modifiers[ MODIFIERPOSITION_PANICKED ] == FALSE &&
	applicableUnits->units[ givenUnit ]->modifiers[ MODIFIERPOSITION_PINNED ] == FALSE )
	{
		
		applicableUnits->units[ givenUnit ]->relativeX = xPos;
		applicableUnits->units[ givenUnit ]->relativeY = yPos;
		applicableUnits->units[ givenUnit ]->selected = FALSE;
		applicableUnits->units[ givenUnit ]->dimensions.x = applicableUnits->units[ givenUnit ]->relativeX * TILE_WIDTH + STARTX_MAP;
		applicableUnits->units[ givenUnit ]->dimensions.y = applicableUnits->units[ givenUnit ]->relativeY * TILE_HEIGHT + STARTY_MAP;
		applicableUnits->units[ givenUnit ]->moved = TRUE;
		
		//Cover save stuff
		while( i != xPos && j != yPos )
		{
			i = tiles[k]->relativeX;
			j = tiles[k]->relativeY;
			k++;	
		}
		tiles[k]->isSelected = FALSE;
		if( tiles[k]->terrainType == TERRAIN_URBAN )
		{
			applicableUnits->units[ givenUnit ]->coverSave = URBAN_COVER_SAVE;
		}
		else if( tiles[k]->terrainType == TERRAIN_JUNGLE )
		{
			applicableUnits->units[ givenUnit ]->coverSave = JUNGLE_COVER_SAVE;
		}
	}
	else
	{
		fprintf( stdout , "Invalid Move!\n" );
		applicableUnits->units[ givenUnit ]->moved = FALSE;
	}
	applicableUnits->units[ givenUnit ]->selected = FALSE;
}


/*
	===============================================================================================================
	int shootUnit( sideData *shootingSideUnits , int shootingSideNo , sideData *recievingSideUnits , 
	int recievingSideNo , tileData **tiles ):
	shoots a unit
	
	Returns the number of wounds inflicted
	===============================================================================================================
*/
int shootUnit( sideData *shootingSideUnits , int shootingSideNo , sideData *recievingSideUnits , int recievingSideNo , tileData **tiles )
{
	srand( time( NULL ) );
	fprintf( stderr , "Shooting unit %d, %s\n" , shootingSideUnits->units[ shootingSideNo ]->unitID , shootingSideUnits->units[ shootingSideNo ]->name );
	if(shootingSideUnits->units[ shootingSideNo ]->shot == TRUE)
	{
		fprintf(stdout, " %s has Already shot !\n", shootingSideUnits->units[ shootingSideNo ]->name);
		recievingSideUnits->units[ recievingSideNo ]->selected = FALSE;
		shootingSideUnits->units[ shootingSideNo ]->selected = FALSE;
		return 0;
	}
	//================================== Morale =====================================================
	if( shootingSideUnits->units[ shootingSideNo ]->modifiers[ MODIFIERPOSITION_ROUT ] == TRUE )
	{
		fprintf(stderr, "Can't shoot! Routing!\n" );
		return 0;
	}
	shootingSideUnits->units[ shootingSideNo ]->modifiers[ MODIFIERPOSITION_PINNED ] = FALSE;
	shootingSideUnits->units[ shootingSideNo ]->shot = TRUE;
	fprintf( stderr , "%d\n" , tiles[0]->noTiles );
	//====================== Path finding and more morale ==========================================
	int aStarResult = aStarWithTerrain( shootingSideUnits , shootingSideNo , tiles , recievingSideUnits->units[ recievingSideNo ]->relativeX ,
	recievingSideUnits->units[ recievingSideNo ]->relativeY, tiles[0]->noTiles - 1 );
	int i, totalHits, currentHit, morale;
	totalHits = 0;
	if( shootingSideUnits->units[ shootingSideNo ]->modifiers[ MODIFIERPOSITION_STUNNED ] == TRUE )
	{
		morale = rand() % 12 + 1;
		if(morale <= shootingSideUnits->units[ shootingSideNo ]->morale)
		{
			shootingSideUnits->units[ shootingSideNo ]->modifiers[ MODIFIERPOSITION_STUNNED ] = FALSE;
		}
		else
		{
			fprintf(stdout, "Unable to shoot: Stunned or Panicked!\n");
			return;
		
		}
	
	}
	//============================= Shooting =======================================
	if( aStarResult <= shootingSideUnits->units[ shootingSideNo ]->aPRange && 
	recievingSideUnits->units[ recievingSideNo ]->unitType == UNITTYPE_INFANTRY)
	
	{
		fprintf( stdout , " Action cost : %d ,Shooting range : %d , Shooting attacks : %d\n" , aStarResult ,  
		shootingSideUnits->units[ shootingSideNo ]->aPRange,  shootingSideUnits->units[ shootingSideNo ]->aPAttacks  );
		
		shootingSideUnits->units[ shootingSideNo ]->shot = TRUE;
		for( i = 0 ; i < shootingSideUnits->units[ shootingSideNo ]->aPAttacks ; i++ )
		{
			currentHit = rand() % 6 + 1;
			fprintf(stderr, "SHOOT!\n");
			if( currentHit >= TO_HIT )
			{
				fprintf(stderr, "Save!\n");
				currentHit = rand() % 6 + 1;
				if( currentHit < recievingSideUnits->units[ recievingSideNo ]->save )
				{
					currentHit = rand() % 6 + 1;
					fprintf(stderr, "Cover save!\n");
					if( currentHit < recievingSideUnits->units[ recievingSideNo ]->coverSave )
					{
						totalHits += 1;
						fprintf(stderr, "HIT!\n");
						recievingSideUnits->units[ recievingSideNo ]->wounds -= 1;
					} 
				}
			}
		}
	}
	else if( aStarResult <= shootingSideUnits->units[ shootingSideNo ]->aTRange &&
	recievingSideUnits->units[ recievingSideNo ]->unitType != UNITTYPE_INFANTRY)
	
	{
		fprintf( stdout , " Action cost : %d ,Shooting value : %d , Shooting attacks : %d\n" , aStarResult ,  shootingSideUnits->units[ shootingSideNo ]->aTRange,  shootingSideUnits->units[ shootingSideNo ]->aTAttacks  );
		shootingSideUnits->units[ shootingSideNo ]->shot = TRUE;
		for( i = 0 ; i < shootingSideUnits->units[ shootingSideNo ]->aTAttacks ; i++ )
		{
			fprintf(stderr, "SHOOT!\n");
			currentHit = rand() % 6 + 1;
			if( currentHit >= TO_HIT )
			{
				currentHit = rand() % 6 + 1;
				fprintf(stderr, "Save!\n");
				if( currentHit < recievingSideUnits->units[ recievingSideNo ]->save )
				{
					currentHit = rand() % 6 + 1;
					fprintf(stderr, "Cover save!\n");
					if( currentHit < recievingSideUnits->units[ recievingSideNo ]->coverSave )
					{
						totalHits += 1;
						fprintf(stderr, "HIT!\n");
						recievingSideUnits->units[ recievingSideNo ]->wounds -= 1;
										
					} 
				}
			}
		}
	}
	else
	{
		fprintf( stdout , "Invalid Shoot!\n" );
		shootingSideUnits->units[ shootingSideNo ]->shot = FALSE;
		recievingSideUnits->units[ recievingSideNo ]->selected = FALSE;
		shootingSideUnits->units[ shootingSideNo ]->selected = FALSE;
		return 0;
	}
	if( recievingSideUnits->units[ recievingSideNo ]->wounds <= 0 )
	{
		recievingSideUnits->units[ recievingSideNo ]->alive = FALSE;
		recievingSideUnits->units[ recievingSideNo ]->selected = FALSE;
		fprintf( stdout , "%s has been taken out !\n" , recievingSideUnits->units[ recievingSideNo ]->name);
	}	
	
	
	
	fprintf( stdout , "The %s delived %d damage to the %s, leaving %d wounds\n" , shootingSideUnits->units[ shootingSideNo ]->name , totalHits ,
	recievingSideUnits->units[ recievingSideNo ]->name , recievingSideUnits->units[ recievingSideNo ]->wounds );

	shootingSideUnits->units[ shootingSideNo ]->selected = FALSE;
	shootingSideUnits->units[ shootingSideNo ]->shot = TRUE;
	return totalHits;
}
/*
	==================================================================================================
	void resolveShooting(sideData *shootingSideUnits, int shootingSideNo, sideData *recievingSideUnits, 
	int recievingSideNo,int inflictedCasualties, int recievedCasualties):
	shoots a unit

	==================================================================================================
*/

void resolveShooting( sideData *shootingSideUnits , int shootingSideNo , sideData *recievingSideUnits , int recievingSideNo , int inflictedCasualties , int recievedCasualties )
{
	fprintf( stderr , "Resolving shooting.......\n" );
	int combatRes = inflictedCasualties - recievedCasualties ;
	int moraleTest;
	moraleTest = rand() % 12 + 1;
	if( combatRes == 0 )
	{
		return;
	}
	//============================ Actual resolving ============================================
	else if( combatRes > 0 )
	{
		fprintf( stdout , "Shooting side won!" );
		recievingSideUnits->units[ recievingSideNo ]->modifiers[ MODIFIERPOSITION_IMPETUOUS ] = FALSE;
		recievingSideUnits->units[ recievingSideNo ]->morale -= 1;
		if( recievingSideUnits->units[ recievingSideNo ]->morale < moraleTest )
		{
			if( combatRes > PINNED_THRESHOLD )
			{
				fprintf( stdout , "%s gained Pinned!\n" , recievingSideUnits->units[ recievingSideNo ]->name );
				recievingSideUnits->units[ recievingSideNo ]->modifiers[ MODIFIERPOSITION_PINNED ] = TRUE;
			}
			if( combatRes > PANICKED_THRESHOLD )
			{
				fprintf( stdout , "%s gained Panicked!\n" , recievingSideUnits->units[ recievingSideNo ]->name );
				recievingSideUnits->units[ recievingSideNo ]->modifiers[ MODIFIERPOSITION_PANICKED ] = TRUE;
			}
			if( combatRes > ROUT_THRESHOLD )
			{
				fprintf( stdout , "%s gained Rout!\n" , recievingSideUnits->units[ recievingSideNo ]->name );
				recievingSideUnits->units[ recievingSideNo ]->modifiers[ MODIFIERPOSITION_PINNED ] = TRUE;
			}
			if( combatRes > STUNNED_THRESHOLD )
			{
				fprintf( stdout , "%s gained Stunned!\n" , recievingSideUnits->units[ recievingSideNo ]->name );
				recievingSideUnits->units[ recievingSideNo ]->modifiers[ MODIFIERPOSITION_STUNNED ] = TRUE;
			}
		}
		fprintf( stdout , "%s gained Impetuous!\n" , shootingSideUnits->units[ shootingSideNo ]->name );
		shootingSideUnits->units[ shootingSideNo ]->modifiers[ MODIFIERPOSITION_IMPETUOUS ] = TRUE;
		shootingSideUnits->units[ shootingSideNo ]->morale += 1;
	}
	else
	{
		fprintf( stdout , "Recieving side won!" );
		shootingSideUnits->units[ shootingSideNo ]->modifiers[ MODIFIERPOSITION_IMPETUOUS ] = FALSE;
		shootingSideUnits->units[ shootingSideNo ]->morale -= 1;
		if( shootingSideUnits->units[ shootingSideNo ]->morale < moraleTest )
		{
			if( combatRes > PINNED_THRESHOLD )
			{
				fprintf( stdout , "%s gained Pinned!\n" , shootingSideUnits->units[ shootingSideNo ]->name );
				shootingSideUnits->units[ shootingSideNo ]->modifiers[ MODIFIERPOSITION_PINNED ] = TRUE;
			}
			if( combatRes > PANICKED_THRESHOLD )
			{
				fprintf( stdout , "%s gained Panicked!\n" , shootingSideUnits->units[ shootingSideNo ]->name );
				shootingSideUnits->units[ shootingSideNo ]->modifiers[ MODIFIERPOSITION_PANICKED ] = TRUE;
			}
			if( combatRes > ROUT_THRESHOLD )
			{
				fprintf( stdout , "%s gained Rout!\n" , shootingSideUnits->units[ shootingSideNo ]->name );
				shootingSideUnits->units[ shootingSideNo ]->modifiers[ MODIFIERPOSITION_PINNED ] = TRUE;
			}
			if( combatRes > STUNNED_THRESHOLD )
			{
				fprintf( stdout , "%s gained Stunned!\n" , shootingSideUnits->units[ shootingSideNo ]->name );
				shootingSideUnits->units[ shootingSideNo ]->modifiers[ MODIFIERPOSITION_STUNNED ] = TRUE;
			}
		}
		fprintf( stdout , "%s gained Impetuous!\n" , recievingSideUnits->units[ recievingSideNo ]->name );
		recievingSideUnits->units[ recievingSideNo ]->modifiers[ MODIFIERPOSITION_IMPETUOUS ] = TRUE;
		recievingSideUnits->units[ recievingSideNo ]->morale += 1;
	}
}
/*
	============================================================================================
	void displaySimulationResults(sideData *sideOne, sideData *sideTwo, tileData **map, 
	TTF_Font *font, SDL_Renderer *render):
	
	displays the result of a simulation
	============================================================================================
*/

void displaySimulationResults( sideData *sideOne , sideData *sideTwo , tileData **map , TTF_Font *font , SDL_Renderer *render )
{
	fprintf( stderr , "Displaying Simulation Results.......\n" );
	SDL_RenderClear( render );
	int noAliveOne, noAliveTwo, i, wasSuccess;
	char soldiersKilledStr [ MAX_TEXT_OUTPUT ];
	textData *winningMessage, *numberSoldiersDead;
	wasSuccess = SUCCESS;
	for( i = 1 ; i < sideOne->noUnits ; i++ )
	{
		if( sideOne->units[i]->alive == TRUE )
		{
			noAliveOne = 1;
		}
	}
	for( i = 1 ; i < sideTwo->noUnits ; i++ )
	{
		if( sideTwo->units[i]->alive == TRUE )
		{
			noAliveTwo = 1;
		}
	}
	snprintf( soldiersKilledStr ,  MAX_TEXT_OUTPUT , "Number dead : %d soldiers" , sideTwo->losses + sideOne->losses );
	if( noAliveOne == 1 )
	{
		winningMessage = renderText( font , render , "Side One Wins!Please press enter into the console window" , &wasSuccess );
		numberSoldiersDead = renderText( font , render , soldiersKilledStr , &wasSuccess );
		winningMessage->dimensions.x = WINNING_MESSAGE_POSITIONX;
		winningMessage->dimensions.y = WINNING_MESSAGE_POSITIONY;
		numberSoldiersDead->dimensions.x = LOSSES_MESSAGE_POSITIONX;
		numberSoldiersDead->dimensions.y = LOSSES_MESSAGE_POSITIONY;
		drawText( winningMessage , render );
		drawText( numberSoldiersDead , render );
	}
	else if( noAliveTwo == 1 )
	{
		winningMessage = renderText( font , render , "Side Two Wins! Please press enter into the console window" , &wasSuccess );
		numberSoldiersDead = renderText( font , render , soldiersKilledStr , &wasSuccess );
		winningMessage->dimensions.x = WINNING_MESSAGE_POSITIONX;
		winningMessage->dimensions.y = WINNING_MESSAGE_POSITIONY;
		numberSoldiersDead->dimensions.x = LOSSES_MESSAGE_POSITIONX;
		numberSoldiersDead->dimensions.y = LOSSES_MESSAGE_POSITIONY;
		drawText( winningMessage , render );
		drawText( numberSoldiersDead , render );
	}
	SDL_RenderPresent( render );
	getch();
}
/*
	=========================================================================================
	void simulationMain(mapData *map, SDL_Renderer *render, TTF_Font *font, buttonData *nextTurn,
	SDL_Event *events, int *success):
	Handles a simulation like a subordinate main function
	=========================================================================================
*/
void simulationMain( mapData *map , SDL_Renderer *render , TTF_Font *font , buttonData **nextTurn , SDL_Event *events , int *success )
{
	fprintf( stderr , "Running the simulation....\n" );
	int oneSideDead = FALSE;
	int i, turnNumber, whichSide, turnButtonClicked, turnChanged,  j, oldSelected, otherSide, k, attackingSideCasualties, 
	defendingSideCasualties, clickedState, quoteNo, time;
	SDL_Rect backgroundDimensions;
	clock_t start, end;
	SDL_Texture *background;
	Mix_Chunk *march, *shoot;
	//========================== INIT ==================================
	backgroundDimensions.x = 0;
	backgroundDimensions.y = 0;
	background = loadImage("background.png", render, &backgroundDimensions, &i);
	turnNumber = 0;//turn value
	whichSide = SIDE_ONE;//starting side
	otherSide = SIDE_TWO;
	turnChanged = 0;
	clickedState = FALSE;
	turnButtonClicked = FALSE;
	time = 0;
	shoot = loadEffect( "audio/attack.wav" , &i );
	march = loadEffect( "audio/march.wav" , &i );
	start = clock();
	//================================= Main functionality ===============
	while( oneSideDead != TRUE )
	{	
		if( turnChanged == TRUE && turnButtonClicked == TRUE)//Changing turn
		{
			turnChanged = 0;
			turnNumber = turnNumber + 1;
			fprintf( stdout , "Turn : %d\n" , turnNumber );
			SDL_RenderClear(render);
			SDL_RenderCopy(render, background, NULL, NULL);
			turnButtonClicked = FALSE;
			quoteNo = rand() % map->quoteList->quotes[0]->noQuotes;
			if( quoteNo == 0 )
			{
				quoteNo = 1;
			
			}
			drawQuote( map->quoteList->quotes , quoteNo , render ,  font );
			SDL_RenderPresent( render );
			fprintf( stdout , "Press enter here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
			getch();
		}
		for( i = 1 ; i <= map->sides[ whichSide ]->noUnits ; i++ )//removes selection
		{
			map->sides[ whichSide ]->units[i]->selected = FALSE;
			map->sides[ whichSide ]->units[i]->shot = FALSE;
			map->sides[ whichSide ]->units[i]->moved = FALSE;
		}
		for( i = 1 ; i <= map->sides[ otherSide ]->noUnits ; i++ )
		{
			map->sides[ otherSide ]->units[i]->selected = FALSE;
			map->sides[ otherSide ]->units[i]->shot = FALSE;
			map->sides[ otherSide ]->units[i]->moved = FALSE;
		}
		for( j = 0 ; j < map->tiles[0]->noTiles ; j++ )
		{
			map->tiles[j]->isSelected = FALSE;
		}
		if( turnNumber % 2 == 1 )
		{
			whichSide = SIDE_ONE;
			otherSide = SIDE_TWO;
		}
		else
		{
			whichSide = SIDE_TWO;
			otherSide = SIDE_ONE;
		
		}
		while( turnButtonClicked != TRUE )
		{
			if( clickedState  != FALSE )
			{
				oldSelected = clickedState ;
			}
			SDL_RenderClear( render );
			SDL_RenderCopy(render, background, NULL, NULL);
			end = clock();
			if( ( end - start ) / ( CLOCKS_PER_SEC/1000 ) > CLICK_RATE )
			{
				clickedState = handleMapClicked( map->sides[ whichSide ] , map->sides[ otherSide ] , map->tiles , nextTurn , events, &turnButtonClicked );
				start = clock();
				if( clickedState == NULL_INPUT )
				{
					clickedState = oldSelected;
				
				}
			}
			if( ( oldSelected == UNIT_SELECTED &&
			clickedState == TILE_SELECTED ) ||
			( oldSelected == UNIT_SELECTED &&
			clickedState == UNIT_SELECTED_OTHER ) )
			{
				for( i = 1 ; i <= map->sides[ whichSide ]->noUnits ; i++ )
				{
					if( map->sides[ whichSide ]->units[i]->selected == TRUE )
					{
						break;
					}
				}
				for( k = 1 ; k <= map->sides[ otherSide ]->noUnits ; k++ )
				{
					if( map->sides[ otherSide ]->units[k]->selected == TRUE )
					{
						break;
					}
				}
				for( j = 0 ; j < map->tiles[0]->noTiles ; j++ )
				{
					if( map->tiles[j]->isSelected == TRUE )
					{
						break;
					}
				}
				fprintf(stdout, "Old Selected : %d\n New Selected : %d\n", oldSelected, clickedState);
				
				if( i != map->sides[ whichSide ]->noUnits + 1 &&
				!( j ==  map->tiles[0]->noTiles ) && 
				map->sides[ whichSide ]->units[i]->selected == TRUE &&
				map->tiles[j]->isSelected == TRUE &&
				oldSelected == UNIT_SELECTED &&
				clickedState == TILE_SELECTED  )//Movement
				
				{
					Mix_PlayChannel( -1 , march , 0 );
					moveUnit( map->sides[ whichSide ], map->tiles , map->tiles[j]->relativeX , map->tiles[j]->relativeY , i );
				}
				
				if(i != map->sides[ whichSide ]->noUnits + 1 &&
				map->sides[ whichSide ]->units[i]->selected == TRUE &&
				k != map->sides[ otherSide ]->noUnits + 1 &&
				map->sides[ otherSide ]->units[k]->selected == TRUE &&
				oldSelected == UNIT_SELECTED &&
				clickedState == UNIT_SELECTED_OTHER )//Shooting
				
				{
					Mix_PlayChannel( -1 , shoot , 0 );
					
					defendingSideCasualties = shootUnit( map->sides[ whichSide ] , i , map->sides[ otherSide ] , k , map->tiles );
					
					attackingSideCasualties = shootUnit( map->sides[ otherSide ] , k , map->sides[ whichSide ] , i , map->tiles );
					
					resolveShooting( map->sides[ whichSide ] , i ,  map->sides[ otherSide ] , k , defendingSideCasualties , attackingSideCasualties );
					
					map->sides[ whichSide ]->units[i]->selected = FALSE;
					
					map->sides[ otherSide ]->units[k]->selected = FALSE;
				}
			}
			
			if( events->type == SDL_QUIT )//Clicking the red cross in the top right
			{
				*success = FAIL;
				return;
			}
			//Drawing
			drawTerrain( map->tiles , map->tiles[0]->noTiles , render , map->tileset );
			drawUnits( map->sides[0]->units , map->sides[0]->noUnits , render , map->tileset );
			drawUnits( map->sides[1]->units , map->sides[1]->noUnits , render , map->tileset );
			drawMenuElements( nextTurn , 1 , render );
			for( i = 1 ; i <= map->sides[0]->noUnits ; i++ )
			{
				oneSideDead = TRUE;
				if( map->sides[0]->units[i]->alive == TRUE )
				{
					oneSideDead = FALSE;
					break;
				}
			}
			if(oneSideDead != TRUE)
			{
				for( i = 1 ; i <= map->sides[1]->noUnits ; i++ )
				{
					oneSideDead = TRUE;
					if( map->sides[1]->units[i]->alive == TRUE )
					{
						oneSideDead = FALSE;
						break;
					}
				}
			}
			if( turnButtonClicked == TRUE )
			{
				turnChanged = TRUE;
				break;
			}
			end = clock();
			time += end - start;
			SDL_RenderPresent( render );
		}
	}
	displaySimulationResults( map->sides[0] , map->sides[1] , map->tiles , font , render );
}
