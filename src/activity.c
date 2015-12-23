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
/*
	void startQuiz(activityData *quiz, SDL_Renderer *render, TTF_Font *font):
	Starts and runs a quiz

*/
int startQuiz(activityData *quiz, SDL_Renderer *render, TTF_Font *font, int *success)
{
	int gotCorrect, i, randomQuestion, j, clicked;
	char results[MAX_TEXT_OUTPUT], grade;
	SDL_Event eventHandle;
	SDL_Rect mouseDimensions;
	mouseDimensions.w = 5;//so that the person can actually click something
	mouseDimensions.h = 5;
	srand(time(NULL));
	randomQuestion = rand() % quiz->maximumMark ;
	gotCorrect = 0;
	i = 0;
	if(randomQuestion == 0)
	{
		randomQuestion = quiz->maximumMark;
	
	}
	while(i < quiz->maximumMark)
	{
		while(SDL_PollEvent(&eventHandle))
		{
			SDL_GetMouseState(&(mouseDimensions.x), &(mouseDimensions.y));
			if(eventHandle.type == SDL_MOUSEBUTTONDOWN)
			{
				clicked = FAIL;
				for(j = 0; j <= NO_ANSWERS; j++)
				{
					clicked = checkQuestionClicked(&mouseDimensions, quiz->questions[randomQuestion], j);
					if(clicked == SUCCESS)
					{
						break;
					
					}
				
				}
				if(clicked == SUCCESS)
				{
					if(j == quiz->questions[randomQuestion]->answerNo)
					{
						fprintf(stderr, "Correct\n");
						gotCorrect++;
					}
					else
					{
						fprintf(stderr, "Faggot, you chose %d, the answer was %d\n", j, quiz->questions[randomQuestion]->answerNo);
					}
					randomQuestion = rand() % quiz->maximumMark ;
					if(randomQuestion == 0)
					{
						randomQuestion = quiz->maximumMark;
	
					}
					i++;
					fprintf(stderr, "%d / %d", i, quiz->maximumMark); 
				}
			
			}
			if(eventHandle.type == SDL_QUIT)
			{
				*success = FAIL;
				return 0;
			
			}
		}
		SDL_RenderClear(render);//this doesnt matter since it is an enclosed game loop in its own right
		drawQuestion(quiz->questions,randomQuestion, render, font);
		SDL_RenderPresent(render);
	
	}
	grade = computeGrade(quiz, gotCorrect);
	snprintf(results, MAX_TEXT_OUTPUT, "YOU GOT %d / %d, THAT IS A GRADE %c, PLEASE PRESS ENTER INTO THE CONSOLE", gotCorrect, quiz->maximumMark, grade);
	displayActivityResults(quiz, results, render, font);
	getch();
	return 0;
}
/*
	void displayActivityResults(activityData *quiz, char *resultsToDisplay, SDL_Renderer *render, TTF_Font *font):
	Renders, displays and destroys the results

*/
void displayActivityResults(activityData *quiz, char *resultsToDisplay, SDL_Renderer *render, TTF_Font *font)
{
	int wasSuccess = SUCCESS;
	textData *toDisplay = renderText(font, render, resultsToDisplay, &wasSuccess);
	if(wasSuccess == FAIL)
	{
		fprintf(stderr, "Non Fatal Error : displayActivityResults has failed \n");
		return;

	}
	SDL_RenderClear(render);//this doesnt matter since it is an enclosed game loop in its own right
	drawText(toDisplay, render);
	SDL_RenderPresent(render);
	SDL_DestroyTexture(toDisplay->display);
	free(toDisplay);
}