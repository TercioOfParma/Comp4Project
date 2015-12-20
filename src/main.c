#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif



int main(int argc, char *argv[])
{
	char *optionsFile;
	int success = SUCCESS;
	optionsData options;
	SDL_Window *wind;
	SDL_Renderer *render;
	
	optionsFile = loadTextFile(OPTIONS_FILE, &success);
	options = initOptions(optionsFile,&success);
	wind = initSDL(&options, &success);
	render = createRenderer(wind, &success);
	return 0;
}