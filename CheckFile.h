#ifndef CHECKFILE_H
#define CHECKFILE_H

#include "./Config.h"
#include <stdio.h>
#include "./Stack/Assert.h"

size_t IsFileExist(const char* filename);
size_t CheckFile(const int argc, const char** argv, const char** filename_input);

#endif
