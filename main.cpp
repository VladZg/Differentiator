#include "./Config.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Stack/Stack.h"
#include "./CheckFile.h"
#include "./Tree.h"
#include "./TreeDump.h"
#include "./Interactors.h"
#include "./TexTreeTranslateFunctions.h"
#include "./TreeSimplifyFunctions.h"
#include "./MathFunctions.h"
#include "./DiffDSL.h"
#include "./TexCreateFunctions.h"

// const char  DATABASE_FILENAME_DEFAULT[] = "./Database.txt";
// const char* DATABASE_FILENAME           = nullptr;

int main() //const int argc, const char** argv
{
    FILE* expression_file = fopen("./Expression.txt", "r");

    ExpressionParams params = {};
    ReadExpressionParams(expression_file, &params);
    ExpressionParamsDump(stdout, &params);

    fclose(expression_file);

    CreateTexFile("TexExpression.tex", &params);

    ExpressionParamsDtor(&params);

    return 1;
}
