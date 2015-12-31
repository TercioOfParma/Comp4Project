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
int aStarWithTerrain(sideData *applicableUnits, int unitNo, tileData **tiles, int xPos, int yPos)
{
	fprintf(stderr, "A* Algorithm to %d %d\n", xPos, yPos);
	return 2;

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
		aStarResult = aStarWithTerrain(applicableUnits, givenUnit, tiles, xPos, yPos);
		applicableUnits->units[givenUnit]->movement += stuckMove;
	}
	else
	{
		aStarResult = aStarWithTerrain(applicableUnits, givenUnit, tiles, xPos, yPos);
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
	int aStarResult = aStarWithTerrain(shootingSideUnits,shootingSideNo, tiles, recievingSideUnits->units[recievingSideNo]->relativeX, recievingSideUnits->units[recievingSideNo]->relativeY);
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
	fprintf(stdout, "%s delived %d damage to %s\n", shootingSideUnits->units[shootingSideNo]->name, totalHits, recievingSideUnits->units[recievingSideNo]->name);
	return totalHits;
}
/*
	void resolveShooting(sideData *shootingSideUnits, int shootingSideNo, sideData *recievingSideUnits, int recievingSideNo,int inflictedCasualties, int recievedCasualties):
	shoots a unit

*/

void resolveShooting(sideData *shootingSideUnits, int shootingSideNo, sideData *recievingSideUnits, int recievingSideNo,int inflictedCasualties, int recievedCasualties)
{
	
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