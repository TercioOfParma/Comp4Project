/*
	FILE : SIMULATION.C
	PURPOSE : Hold all functions related to computing the simulation (movement, shooting, morale etc)
	VERSION : 0.001
	NOTES: Could get far more complex in later non coursework versions
*/
//---------------------------------- C PREPROCESSOR --------------------------

#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif
//------------------------------ FUNCTIONS -----------------------------------
/*
	int aStarWithTerrain(sideData *applicableUnits, int unitNo, tileData **tiles, int xPos, int yPos):
	used to see if a unit can move or shoot a tile

*/
int aStarWithTerrain(sideData *applicableUnits, int unitNo, tileData **tiles, int xPos, int yPos, int size)
{
	fprintf(stderr, "A* Algorithm to %d %d\n", xPos, yPos);
	int posBeginX, posBeginY, posEndX, posEndY, posArrayStart, posArrayEnd, posCurrentX, posCurrentY, posArrayCurrent, i, favouredPositionArray, favouredPositionLength, totalLength, favouredPositionToEnd;
	posArrayStart = 0;
	posArrayEnd = 0;
	posBeginX = 0;
	posEndX = 0;
	posBeginY = 0;
	posEndY = 0;
	while(posBeginX != applicableUnits->units[unitNo]->relativeX || posBeginY != applicableUnits->units[unitNo]->relativeY )
	{
			posArrayStart++;	
			posBeginX = tiles[posArrayStart]->relativeX;
			posBeginY = tiles[posArrayStart]->relativeY;
	}
	while(posEndX != xPos || posEndY != yPos)
	{
			posArrayEnd++;	
			posEndX = tiles[posArrayEnd]->relativeX;
			posEndY = tiles[posArrayEnd]->relativeY;
			
	}
	if(posArrayStart == posArrayEnd)
	{
		return 0;
	
	}
	tiles[posArrayStart]->hScore = 0;
	posCurrentX = posBeginX;
	posCurrentY = posBeginY;
	posArrayCurrent = posArrayStart;
	totalLength = 0;
	for(i = 0; i < size; i++)
	{
		tiles[i]->explored = FALSE;
	
	}
	while(TRUE)
	{
		tiles[posArrayCurrent]->explored = TRUE;
		favouredPositionLength = LARGE;
		favouredPositionToEnd = LARGE;
		for(i = 0; i < size; i++)
		{
			if(abs(tiles[i]->relativeX - tiles[posArrayCurrent]->relativeX) == 1 || abs(tiles[i]->relativeY - tiles[posArrayCurrent]->relativeY) == 1 && tiles[i]->explored == FALSE)
			{
				
				if(tiles[i]->terrainType != TERRAIN_RIVER || tiles[i]->terrainType != TERRAIN_MOUNTAIN || applicableUnits->units[unitNo]->unitType == UNITTYPE_HELICOPTER)
				{
					if(tiles[i]->terrainType == TERRAIN_URBAN || tiles[i]->terrainType == TERRAIN_JUNGLE && applicableUnits->units[unitNo]->unitType != UNITTYPE_INFANTRY)
					{
						tiles[i]->hScore = findDistance(tiles[i], tiles[posArrayStart]) + findDistance(tiles[i], tiles[posArrayEnd]) + 5;//rough terrain movement and shooting modifier for non infantry in jungle and cities
						
					}
					else
					{
						tiles[i]->hScore = findDistance(tiles[i], tiles[posArrayStart]) + findDistance(tiles[i], tiles[posArrayEnd]);
					}
					if(favouredPositionLength > tiles[i]->hScore)
					{
						
						favouredPositionLength = tiles[i]->hScore;
						favouredPositionArray = i;
						favouredPositionToEnd = findDistance(tiles[i], tiles[posArrayEnd]);
					}
					else if(favouredPositionLength == tiles[i]->hScore && favouredPositionToEnd > findDistance(tiles[i], tiles[posArrayEnd]))
					{
						
						favouredPositionLength = tiles[i]->hScore;
						favouredPositionArray = i;
						favouredPositionToEnd = findDistance(tiles[i], tiles[posArrayEnd]);
					
					}
					if(findDistance(tiles[i], tiles[posArrayEnd]) == 0)
					{
						favouredPositionLength = tiles[i]->hScore;
						favouredPositionArray = i;
						break;
					}
				}
			}
		}
		if(favouredPositionLength != LARGE)
		{
			totalLength += favouredPositionLength;
			posArrayCurrent = favouredPositionArray;
			posCurrentY = tiles[favouredPositionArray]->relativeY;
			posCurrentX = tiles[favouredPositionArray]->relativeX;
		}
		if(posCurrentY == yPos && posCurrentX == xPos)
		{
			break;
		
		}
	}
	fprintf(stderr, "works\n");
	if(totalLength <= applicableUnits->units[unitNo]->movement)
	{
		fprintf(stdout, "Legal Move!\n");
		return totalLength;
	
	}
	else
	{
		fprintf(stdout, "illegal Move!\n");
		return LARGE;
	
	}
	fprintf(stderr, "works\n");
	return LARGE;
}
/*
	int aStarWithTerrain(sideData *applicableUnits, int unitNo, tileData **tiles, int xPos, int yPos):
	used to see if a unit can move or shoot a tile

*/
int findDistance(tileData *tileOne, tileData *tileTwo)
{
	int totalDistance, xDistance, yDistance;
	totalDistance = 0;
	xDistance = abs(tileOne->relativeX - tileTwo->relativeX);
	yDistance = abs(tileOne->relativeY - tileTwo->relativeY);
	while(xDistance != 0 && yDistance != 0)
	{
		xDistance--;
		yDistance--;
		totalDistance += SQRT_TWO;
	
	}
	totalDistance = totalDistance + (xDistance * 10) + (yDistance * 10);
	
	return totalDistance;
}
/*
	void moveUnit(sideData *applicableUnits, int givenUnit):
	moves a unit

*/
void moveUnit(sideData *applicableUnits, tileData **tiles, int xPos, int yPos, int givenUnit)
{
	fprintf(stderr, "Moving unit %d, %s\n", applicableUnits->units[givenUnit]->unitID, applicableUnits->units[givenUnit]->name);
	applicableUnits->units[givenUnit]->moved = FALSE;
	int aStarResult;
	int i, j, k, stuckMove;
	k = 0;
	i = 0;
	j = 0;
	if(applicableUnits->units[givenUnit]->modifiers[MODIFIERPOSITION_STUCK] == TRUE)
	{
		stuckMove = (applicableUnits->units[givenUnit]->movement / 3);
		applicableUnits->units[givenUnit]->movement -= stuckMove;
		aStarResult = aStarWithTerrain(applicableUnits, givenUnit, tiles, xPos, yPos, tiles[0]->noTiles);
		applicableUnits->units[givenUnit]->movement += stuckMove;
	}
	else
	{
		aStarResult = aStarWithTerrain(applicableUnits, givenUnit, tiles, xPos, yPos, tiles[0]->noTiles);
	}
	if(aStarResult <= applicableUnits->units[givenUnit]->movement && applicableUnits->units[givenUnit]->modifiers[MODIFIERPOSITION_STUNNED] == FALSE && applicableUnits->units[givenUnit]->modifiers[MODIFIERPOSITION_PANICKED] == FALSE && applicableUnits->units[givenUnit]->modifiers[MODIFIERPOSITION_PINNED] == FALSE)
	{
	
		applicableUnits->units[givenUnit]->relativeX = xPos;
		applicableUnits->units[givenUnit]->relativeY = yPos;
		applicableUnits->units[givenUnit]->dimensions.x = applicableUnits->units[givenUnit]->relativeX * TILE_WIDTH + STARTX_MAP;
		applicableUnits->units[givenUnit]->dimensions.y = applicableUnits->units[givenUnit]->relativeY * TILE_HEIGHT + STARTY_MAP;
		applicableUnits->units[givenUnit]->moved = TRUE;
		//Cover save stuff
		while(i != xPos && j != yPos)
		{
			i = tiles[k]->relativeX;
			j = tiles[k]->relativeY;
			k++;	
		}
		if(tiles[k]->terrainType == TERRAIN_URBAN)
		{
			applicableUnits->units[givenUnit]->coverSave = URBAN_COVER_SAVE;
		
		}
		else if(tiles[k]->terrainType == TERRAIN_JUNGLE)
		{
			applicableUnits->units[givenUnit]->coverSave = JUNGLE_COVER_SAVE;
		
		}
		
	}
	else
	{
		fprintf(stdout, "Invalid Move!\n");
	
	}
}


/*
	void shootUnit(sideData *shootingSideUnits, int shootingSideNo, sideData *recievingSideUnits, int recievingSideNo):
	shoots a unit

*/
int shootUnit(sideData *shootingSideUnits, int shootingSideNo, sideData *recievingSideUnits, int recievingSideNo, tileData **tiles)
{
	srand(time(NULL));
	fprintf(stderr, "Shooting unit %d, %s\n", shootingSideUnits->units[shootingSideNo]->unitID, shootingSideUnits->units[shootingSideNo]->name);
	shootingSideUnits->units[shootingSideNo]->shot = FALSE;
	fprintf(stderr, "%d\n", tiles[0]->noTiles);
	int aStarResult = aStarWithTerrain(shootingSideUnits,shootingSideNo, tiles, recievingSideUnits->units[recievingSideNo]->relativeX, recievingSideUnits->units[recievingSideNo]->relativeY, tiles[0]->noTiles - 1);
	int i, totalHits, currentHit;
	totalHits = 0;
	if(aStarResult <= shootingSideUnits->units[shootingSideNo]->aPRange &&  recievingSideUnits->units[recievingSideNo]->unitType == UNITTYPE_INFANTRY)
	{
		shootingSideUnits->units[shootingSideNo]->shot = TRUE;
		for(i = 0; i < shootingSideUnits->units[shootingSideNo]->aPAttacks; i++)
		{
			currentHit = rand() % 6 + 1;
			if(currentHit >= TO_HIT)
			{
				currentHit = rand() % 6 + 1;
				if(currentHit < recievingSideUnits->units[recievingSideNo]->save)
				{
					currentHit = rand() % 6 + 1;
					if(currentHit < recievingSideUnits->units[recievingSideNo]->coverSave)
					{
						totalHits += 1;
					}
				
				}
			
			}
		
		}
		recievingSideUnits->units[recievingSideNo]->wounds -= totalHits;
	
	}
	else
	{
		fprintf(stdout, "Invalid Shoot!\n");
		return 0;
	}
	if(recievingSideUnits->units[recievingSideNo]->wounds <= 0)
	{
		recievingSideUnits->units[recievingSideNo]->alive = FALSE;
		fprintf(stdout, "%s has been taken out !\n", recievingSideUnits->units[recievingSideNo]->name);
	}
	
	fprintf(stdout, "The %s delived %d damage to the %s\n", shootingSideUnits->units[shootingSideNo]->name, totalHits, recievingSideUnits->units[recievingSideNo]->name);
	return totalHits;
}
/*
	void resolveShooting(sideData *shootingSideUnits, int shootingSideNo, sideData *recievingSideUnits, int recievingSideNo,int inflictedCasualties, int recievedCasualties):
	shoots a unit

*/

void resolveShooting(sideData *shootingSideUnits, int shootingSideNo, sideData *recievingSideUnits, int recievingSideNo,int inflictedCasualties, int recievedCasualties)
{
	fprintf(stderr, "Resolving shooting.......\n");
	int combatRes = inflictedCasualties - recievedCasualties;
	int moraleTest = (rand() % 6 + 1) * 2;
	if(combatRes == 0)
	{
		return;
	}
	else if(combatRes > 0)
	{
		fprintf(stdout, "Shooting side won!");
		recievingSideUnits->units[recievingSideNo]->modifiers[MODIFIERPOSITION_IMPETUOUS] = FALSE;
		if(combatRes > PINNED_THRESHOLD)
		{
			fprintf(stdout, "%s gained Pinned!\n", recievingSideUnits->units[recievingSideNo]->name);
			recievingSideUnits->units[recievingSideNo]->modifiers[MODIFIERPOSITION_PINNED] = TRUE;
		
		}
		if(combatRes > PANICKED_THRESHOLD)
		{
			fprintf(stdout, "%s gained Panicked!\n", recievingSideUnits->units[recievingSideNo]->name);
			recievingSideUnits->units[recievingSideNo]->modifiers[MODIFIERPOSITION_PANICKED] = TRUE;
		
		}
		if(combatRes > ROUT_THRESHOLD && recievingSideUnits->units[recievingSideNo]->morale < moraleTest )
		{
			fprintf(stdout, "%s gained Rout!\n", recievingSideUnits->units[recievingSideNo]->name);
			recievingSideUnits->units[recievingSideNo]->modifiers[MODIFIERPOSITION_PINNED] = TRUE;
		
		}
		if(combatRes > STUNNED_THRESHOLD)
		{
			fprintf(stdout, "%s gained Stunned!\n", recievingSideUnits->units[recievingSideNo]->name);
			recievingSideUnits->units[recievingSideNo]->modifiers[MODIFIERPOSITION_STUNNED] = TRUE;
		
		}
		fprintf(stdout, "%s gained Impetuous!\n", shootingSideUnits->units[shootingSideNo]->name);
		shootingSideUnits->units[shootingSideNo]->modifiers[MODIFIERPOSITION_IMPETUOUS] = TRUE;
	}
	else
	{
		fprintf(stdout, "Recieving side won!");
		moraleTest = abs(moraleTest);
		shootingSideUnits->units[shootingSideNo]->modifiers[MODIFIERPOSITION_IMPETUOUS] = FALSE;
		if(combatRes > PINNED_THRESHOLD)
		{
			fprintf(stdout, "%s gained Pinned!\n", shootingSideUnits->units[shootingSideNo]->name);
			shootingSideUnits->units[shootingSideNo]->modifiers[MODIFIERPOSITION_PINNED] = TRUE;
		
		}
		if(combatRes > PANICKED_THRESHOLD)
		{
			fprintf(stdout, "%s gained Panicked!\n", shootingSideUnits->units[shootingSideNo]->name);
			shootingSideUnits->units[shootingSideNo]->modifiers[MODIFIERPOSITION_PANICKED] = TRUE;
		
		}
		if(combatRes > ROUT_THRESHOLD && shootingSideUnits->units[shootingSideNo]->morale < moraleTest )
		{
			fprintf(stdout, "%s gained Rout!\n", shootingSideUnits->units[shootingSideNo]->name);
			shootingSideUnits->units[shootingSideNo]->modifiers[MODIFIERPOSITION_PINNED] = TRUE;
		
		}
		if(combatRes > STUNNED_THRESHOLD)
		{
			fprintf(stdout, "%s gained Stunned!\n", shootingSideUnits->units[shootingSideNo]->name);
			shootingSideUnits->units[shootingSideNo]->modifiers[MODIFIERPOSITION_STUNNED] = TRUE;
		
		}
		fprintf(stdout, "%s gained Impetuous!\n", recievingSideUnits->units[recievingSideNo]->name);
		recievingSideUnits->units[recievingSideNo]->modifiers[MODIFIERPOSITION_IMPETUOUS] = TRUE;
	}
	
	


}
/*
	void displaySimulationResults(sideData *sideOne, sideData *sideTwo, tileData *map):
	displays the result of a simulation

*/

void displaySimulationResults(sideData *sideOne, sideData *sideTwo, tileData *map, TTF_Font *font, SDL_Renderer *render)
{
	fprintf(stderr, "Displaying Simulation Results.......\n");
	int noAliveOne, noAliveTwo, i, wasSuccess;
	char soldiersKilledStr [MAX_TEXT_OUTPUT];
	textData *winningMessage, *numberSoldiersDead;
	wasSuccess = SUCCESS;
	for(i = 0; i < sideOne->noUnits; i++)
	{
		if(sideOne->units[i]->alive == TRUE)
		{
			noAliveOne = 1;
		
		}
	
	}
	for(i = 0; i < sideTwo->noUnits; i++)
	{
		if(sideTwo->units[i]->alive == TRUE)
		{
			noAliveTwo = 1;
		
		}
	
	}
	snprintf(soldiersKilledStr, MAX_TEXT_OUTPUT, "Number dead : %d soldiers", sideTwo->losses + sideOne->losses);
	if(noAliveOne == 1)
	{
		winningMessage = renderText(font, render, "Side One Wins!", &wasSuccess);
		numberSoldiersDead = renderText(font, render, soldiersKilledStr, &wasSuccess);
		winningMessage->dimensions.x = WINNING_MESSAGE_POSITIONX;
		winningMessage->dimensions.y = WINNING_MESSAGE_POSITIONY;
		numberSoldiersDead->dimensions.x = LOSSES_MESSAGE_POSITIONX;
		numberSoldiersDead->dimensions.y = LOSSES_MESSAGE_POSITIONY;
		drawText(winningMessage, render);
		drawText(numberSoldiersDead, render);
	}
	else if(noAliveTwo == 1)
	{
		winningMessage = renderText(font, render, "Side Two Wins!", &wasSuccess);
		numberSoldiersDead = renderText(font, render, soldiersKilledStr, &wasSuccess);
		winningMessage->dimensions.x = WINNING_MESSAGE_POSITIONX;
		winningMessage->dimensions.y = WINNING_MESSAGE_POSITIONY;
		numberSoldiersDead->dimensions.x = LOSSES_MESSAGE_POSITIONX;
		numberSoldiersDead->dimensions.y = LOSSES_MESSAGE_POSITIONY;
		drawText(winningMessage, render);
		drawText(numberSoldiersDead, render);
	
	}


}