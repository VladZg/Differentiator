#include "../Include/Config.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include "../Include/Constants.h"
#include "../Include/DefineColourConsts.h"
#include "../Libs/Stack/Include/Assert.h"
#include "../Libs/Stack/Include/Stack.h"
#include "../Include/CheckFile.h"
#include "../Include/Tree.h"
#include "../Include/TreeDump.h"
#include "../Include/ExpressionReader.h"
#include "../Include/Interactors.h"
#include "../Include/TexTreeTranslateFunctions.h"
#include "../Include/TreeSimplifyFunctions.h"
#include "../Include/MathFunctions.h"
#include "../Include/DiffDSL.h"
#include "../Include/TexCreateFunctions.h"

// const char  DATABASE_FILENAME_DEFAULT[] = "./Database.txt";
// const char* DATABASE_FILENAME           = nullptr;

int main() //const int argc, const char** argv
{
    FILE* expression_file = fopen("./Expression.txt", "r");

    ExpressionParams params = {};
    ReadExpressionParams(expression_file, &params);
    ExpressionParamsDump(stdout, &params);

    fclose(expression_file);

    CreateTexFile("TexExpression", &params);

    ExpressionParamsDtor(&params);

    return 1;
}
