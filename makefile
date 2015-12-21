all:
	gcc	-c	src/main.h
	gcc	-c	src/input.c
	gcc	-c	src/init.c
	gcc	-c	src/deinit.c
	gcc	-c	src/simulation.c
	gcc	-c	src/activity.c
	gcc	-c	src/draw.c
	gcc	-c	src/main.c
	gcc	-o	CWC	main.o	draw.o	activity.o	simulation.o	deinit.o	init.o	input.o	-lmingw32	-lSDL2Main	-lSDL2	-lSDL2_image	-lSDL2_ttf	-lSDL2_mixer	-ljansson
	rm	"main.o"
	rm	"draw.o"
	rm	"activity.o"
	rm	"simulation.o"
	rm	"deinit.o"
	rm	"init.o"
	rm	"input.o"
debug:
	gcc	-c	src/main.h	-Wall
	gcc	-c	src/input.c	-Wall
	gcc	-c	src/init.c	-Wall
	gcc	-c	src/deinit.c	-Wall
	gcc	-c	src/simulation.c	-Wall
	gcc	-c	src/activity.c	-Wall
	gcc	-c	src/draw.c	-Wall
	gcc	-c	src/main.c	-Wall
	gcc	-o	CWCDEBUG	main.o	draw.o	activity.o	simulation.o	deinit.o	init.o	input.o	-lmingw32	-lSDL2Main	-lSDL2	-lSDL2_image	-lSDL2_ttf	-lSDL2_mixer	-ljansson	-Wall
	rm	"main.o"
	rm	"draw.o"
	rm	"activity.o"
	rm	"simulation.o"
	rm	"deinit.o"
	rm	"init.o"
	rm	"input.o"