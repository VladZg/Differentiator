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
#include "./TexFunctions.h"
#include "./TreeSimplifyFunctions.h"
#include "./Differentiation.h"
#include "./DiffDSL.h"
// #include "./CheckFile.h"

// const char  DATABASE_FILENAME_DEFAULT[] = "./Database.txt";
// const char* DATABASE_FILENAME           = nullptr;

int main() //const int argc, const char** argv
{
    FILE* expression_file = fopen("./Expression.txt", "r");

    ExpressionParams params = {};

    ReadExpressionParams(expression_file, &params);

    ExpressionParamsDump(stdout, &params);

    fclose(expression_file);

    FILE* tex_file = fopen("./TexFiles/TexExpressions.tex", "w");
    WriteHeadOfTexFile(tex_file);

    // Node* exp_copy = CopyNode(params.expression);

    fprintf(stdout, "ШОК!!! ЗНАЧЕНИЕ ДЕРЕВА РАВНО... %lf !!!\n", CalculateTree(params.expression, &params));

    ShowTree(params.expression, SIMPLE_DUMP_MODE);
    // SimplifyTree(&params.expression);
    ShowTree(params.expression, FULL_FULL_DUMP_MODE, 1);

    fprintf(tex_file, "Let's calculate smth with expression given:\n");
    WriteExpressionInTexFile(params.expression, tex_file);

//     Node* diff1 = Differentiate(params.expression, tex_file);
//     WriteExpressionInTexFile(diff1, tex_file);
//
//     Node* diff2 = Differentiate(diff1, tex_file);
//     WriteExpressionInTexFile(diff2, tex_file);
//
//     Node* diff3 = Differentiate(diff2, tex_file);
//     WriteExpressionInTexFile(diff3, tex_file);
//
//     Node* diff4 = Differentiate(diff3, tex_file);
//     WriteExpressionInTexFile(diff4, tex_file);

    WriteTailOfTexFile(tex_file);
    fclose(tex_file);

    // NodeDtor(diff1);
    // NodeDtor(diff2);
    // NodeDtor(diff3);
    // NodeDtor(diff4);

    // NodeDtor(exp_copy);

    ExpressionParamsDtor(&params);

    // CompileTexFile("./TexExpressions.tex");

    return 1;
}
