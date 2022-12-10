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

    // Node* exp_copy1 = CopyNode(*(params.expression));

    // fprintf(stdout, "ШОК!!! ЗНАЧЕНИЕ ДЕРЕВА РАВНО... %lf !!!\n", CalculateTree(params.expression, &params));
    // fprintf(stdout, "ГЛУБИНА: %ld\n", TreeDepth(params.expression));

    // ShowTree(*(params.expression), SIMPLE_DUMP_MODE, 1);
    // ShowTree(*(params.expression), SIMPLE_DUMP_MODE);

    *(params.expression) = SimplifyTree(params.expression);
    // Node* exp_copy2 = CopyNode(*(params.expression));

    // ShowTree(*(params.expression), SIMPLE_DUMP_MODE);
    // ShowTree(exp_copy1, SIMPLE_DUMP_MODE);
    // ShowTree(exp_copy2, SIMPLE_DUMP_MODE);

    fprintf(tex_file, "Let's calculate smth with expression given:\n");
    WriteExpressionInTexFile(*(params.expression), tex_file);

    fprintf(tex_file, "BRITISH SCIENTISTS WERE SHOCKED, "
                      "WHEN THEY COUNT THIS EXPRESSION ");
    PrintParametersPoint(tex_file, &params);
    fprintf(tex_file, "...\n\n"
                      "IT'S VALUE = %lf !!!\n\n\n", CalculateTree(*(params.expression), &params));

    // Node* diff1 = Differentiate(*(params.expression), tex_file, SKIP_STEPS_TEX_MODE);

    Node* n_diff = NDifferentiate(*(params.expression), params.n_differentiate, tex_file, PRINT_STEPS_TEX_MODE);

    fprintf(tex_file, "Finally... The %ld derivation of the expression:\n", params.n_differentiate);
    WriteExpressionInTexFile(n_diff, tex_file);

    fprintf(tex_file, "BRITISH SCIENTISTS WERE SHOCKED, "
                      "WHEN THEY COUNT THE %ld DERIVATION OF THIS EXPRESSION ", params.n_differentiate);
    PrintParametersPoint(tex_file, &params);
    fprintf(tex_file, "...\n\n"
                      "IT'S VALUE = %lf !!!\n\n\n", CalculateTree(n_diff, &params));

    WriteTailOfTexFile(tex_file);
    fclose(tex_file);

    // ShowTree(n_diff, SIMPLE_DUMP_MODE, 0);
    // NodeDtor(diff1);
//
    // ShowTree(n_diff, SIMPLE_DUMP_MODE, 0);
    // ShowTree(n_diff, FULL_FULL_DUMP_MODE, 1);
//
    // NodeDtor(&n_diff);

//     Node* exp_copy1 = CopyNode(params.expression);
//     Node* exp_copy2 = CopyNode(exp_copy1);
//
    // ShowTree(exp_copy1, SIMPLE_DUMP_MODE);
    // ShowTree(exp_copy2, SIMPLE_DUMP_MODE);
//
//     // NodeDtor(&exp_copy1);
//     NodeDtor(&exp_copy1);
// //
//     ShowTree(exp_copy2, SIMPLE_DUMP_MODE, 1);
    // ShowTree(diff1, SIMPLE_DUMP_MODE, 1);
// //
    NodeDtor(&n_diff);

    ExpressionParamsDtor(&params);

    CompileTexFile("./TexExpressions.tex");

    return 1;
}
