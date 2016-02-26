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
	fprintf(stderr, "Calculating grade....\n");
	int percentage = ( (float) noCorrect / (float) quiz->maximumMark ) * 100;
	if( percentage >= A_GRADE )//Unfortunately, can't use a switch statement for this due to the lack of being able to compare
	{
		return 'A' ;
	}
	else if ( percentage >= B_GRADE )
	{
		return 'B' ;
	}
	else if ( percentage >= C_GRADE )
	{
		return 'C' ;
	}
	else if ( percentage >= D_GRADE )
	{
		return 'D' ;
	}
	else if ( percentage >= E_GRADE )
	{
		return 'E';
	}
	else if ( percentage >= F_GRADE )
	{
		return 'F' ;
	}
	else if ( percentage >= G_GRADE )
	{
		return 'G';
	}
	else if ( percentage >= N_GRADE )
	{
		return 'N';
	}
	else 
	{
		return 'U';
	}

}
/*
	void startQuiz(activityData *quiz, SDL_Renderer *render, TTF_Font *font):
	Starts and runs a quiz and acts as a loop.
	Nothing is malloc'd here, the data is already iniitalised prior to this being run
	If the memory isn't allocated, then it will never run this in the first place due to error
	code written in the init functions
*/
int startQuiz( activityData *quiz , SDL_Renderer *render , TTF_Font *font , SDL_Event *events ,  int *success)
{
	fprintf(stderr, "Running quiz....\n");
	int gotCorrect, i, randomQuestion, j, clicked;
	char results[ MAX_TEXT_OUTPUT ], grade;
	SDL_Rect mouseDimensions;
	mouseDimensions.w = 5;//so that the person can actually click something
	mouseDimensions.h = 5;
	srand( time(NULL) );
	randomQuestion = rand() % quiz->maximumMark ;
	gotCorrect = 0;
	i = 0;
	if( randomQuestion == 0 )//So that the quiz game doesn't select something that doesn't exist
	{
		randomQuestion = quiz->maximumMark;
	
	}
	while( i < quiz->maximumMark )
	{
		while( SDL_PollEvent( events ) )//registers input and processes clicks on them
		{
			SDL_GetMouseState( &( mouseDimensions.x ) , &( mouseDimensions.y ) );
			if( events->type == SDL_MOUSEBUTTONDOWN )
			{
				clicked = FAIL;
				for( j = 1; j <= NO_ANSWERS; j++ )
				{
					clicked = checkQuestionClicked( &mouseDimensions , quiz->questions[randomQuestion] , j );
					if( clicked == SUCCESS )
					{
						break;
					
					}
				
				}
				if(clicked == SUCCESS )//Correct or Incorrect check
				{
					if(j == quiz->questions[randomQuestion]->answerNo )
					{
						fprintf( stdout , "Correct\n" );
						gotCorrect++;
					}
					else
					{
						fprintf( stdout , "Incorrect, you chose %d, the answer was %d\n", j , quiz->questions[randomQuestion]->answerNo );
					}
					randomQuestion = rand() % quiz->maximumMark ;
					if( randomQuestion == 0 )
					{
						randomQuestion = quiz->maximumMark;
	
					}
					i++;
					fprintf( stderr , "%d / %d", i , quiz->maximumMark ); 
				}
			
			}
			if( events->type == SDL_QUIT )
			{
				*success = FAIL;
				return 0;
			
			}
		}
		SDL_RenderClear( render );//this doesnt matter since it is an enclosed game loop in its own right
		drawQuestion( quiz->questions , randomQuestion ,  render , font );
		SDL_RenderPresent( render );
	
	}
	grade = computeGrade( quiz , gotCorrect );
	snprintf( results , MAX_TEXT_OUTPUT , "YOU GOT %d / %d, THAT IS A GRADE %c, PLEASE PRESS ENTER INTO THE CONSOLE" , 
	gotCorrect , quiz->maximumMark , grade);
	displayActivityResults( quiz , results , render , font );
	return 0;
}
/*
	void displayActivityResults(activityData *quiz, char *resultsToDisplay, SDL_Renderer *render, TTF_Font *font):
	Renders, displays and destroys the results
	Deals with GPU Memory. Allocation is handled by renderText

*/
void displayActivityResults( activityData *quiz , char *resultsToDisplay , SDL_Renderer *render , TTF_Font *font )
{
	fprintf( stderr, "Displaying results of the activity....\n" );
	int wasSuccess = SUCCESS;
	textData *toDisplay = renderText( font , render, resultsToDisplay , &wasSuccess );
	if( wasSuccess == FAIL )
	{
		fprintf( stderr , "Non Fatal Error : displayActivityResults has failed \n" );
		return;

	}
	SDL_RenderClear( render );//this doesnt matter since it is an enclosed game loop in its own right
	drawText( toDisplay , render );
	SDL_RenderPresent( render );
	getch();
	SDL_DestroyTexture( toDisplay->display );
	free( toDisplay );
}
