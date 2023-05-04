#include "../Config.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include "../Include/Constants.h"
#include "../Include/DefineColourConsts.h"
#include "../Libs/Stack/Include/Assert.h"
#include "../Libs/Stack/Include/Stack.h"
#include "../Libs/Logging/Log.h"
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

#ifndef N_OPEN_RES_PDF
    static void OpenResultPdf() {system("xdg-open ./TexExpression.pdf");}
    static int open_result_pdf = atexit(OpenResultPdf);
#endif

int main()
{
    FILE* expression_file = fopen("./Expression.txt", "r");

    ExpressionParams params = {};
    ReadExpressionParams(expression_file, &params);
    // ExpressionParamsDump(stdout, &params);
    fclose(expression_file);

    // ShowTree(*(params.expression), SIMPLE_DUMP_MODE, 0);
    // ShowTree(*(params.expression), FULL_FULL_DUMP_MODE, 1);

    CreateTexFile("TexExpression", &params);

    ExpressionParamsDtor(&params);

    system("mv ./TexFiles/TexExpression.pdf ./");

    return 1;
}
