#include "headers/io.h"

void cprint(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	fprintf(yyout, "%3d: ", code_lineno++);
	vfprintf(yyout, format, args);
	va_end(args);

	fflush(yyout);
}


