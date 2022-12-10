#ifndef TEXCREATEFUNCTIONS_H
#define TEXCREATEFUNCTIONS_H

#include "./Config.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Stack/Stack.h"
#include "./Tree.h"
#include "./TreeDump.h"
#include "./Interactors.h"
#include "./TexTreeTranslateFunctions.h"
#include "./TreeSimplifyFunctions.h"
#include "./MathFunctions.h"
#include "./DiffDSL.h"

int WriteHeadOfTexFile(FILE* tex_file);
int ShitSomeCringeIntroductionInTexFile(FILE* tex_file);
int WriteTailOfTexFile(FILE* tex_file);
int FillTexFile       (FILE* tex_file,   ExpressionParams* params);
int CreateTexFile (const char* filename, ExpressionParams* params);
int CompileTexFile(const char* filename);

#endif
