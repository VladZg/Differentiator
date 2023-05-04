#ifndef LOG_H
#define LOG_H

#include <stdio.h>

FILE* OpenLog  ();
void  WriteLog (const char* format_data, ...);
void  CloseLog ();

#endif
