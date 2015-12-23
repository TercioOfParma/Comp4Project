/*
	FILE : ACTIVITY.C
	PURPOSE : Hold all functions related to the activity section of the solution
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
	char computeGrade(activityData *quiz, int noCorrect):
	Generates a grade from answers

*/
char computeGrade(activityData *quiz, int noCorrect)
{
	int percentage = ((float)noCorrect / (float)quiz->maximumMark) * 100;
	if(percentage >= A_GRADE)
	{
		return 'A';
	}
	else if (percentage >= B_GRADE)
	{
		return 'B';
	}
	else if (percentage >= C_GRADE)
	{
		return 'C';
	}
	else if (percentage >= D_GRADE)
	{
		return 'D';
	}
	else if (percentage >= E_GRADE)
	{
		return 'E';
	}
	else if (percentage >= F_GRADE)
	{
		return 'F';
	}
	else if (percentage >= G_GRADE)
	{
		return 'G';
	}
	else if (percentage >= N_GRADE)
	{
		return 'N';
	}
	else 
	{
		return 'U';
	}

}