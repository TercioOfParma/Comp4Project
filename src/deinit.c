/*
	FILE : DEINIT.C
	PURPOSE : Hold all functions related to freeing structures and deinitialising libraries unless the library is used exactly one function's scope (such as Jansson)
	VERSION : 0.001
	NOTES: likely to be quite simple with a lot of code repeated down to C's lack of templates and typeless code
*/
//---------------------------------- C PREPROCESSOR --------------------------

#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif
//------------------------------ FUNCTIONS -----------------------------------