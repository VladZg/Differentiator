
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

int WriteHeadOfTexFile(FILE* tex_file)
{
    ASSERT(tex_file != nullptr)

    fprintf(tex_file, "\\documentclass{article}\n"
                      "\\begin{document}\n\n"); //"\\userpackage[papersize={13cm,8cm}\n\n");

    return 1;
}

int ShitSomeCringeIntroductionInTexFile(FILE* tex_file)
{
    ASSERT(tex_file != nullptr)

    fprintf(tex_file, "CrInGeCrInGe Production. Super cringe introduction here:\n\n\n");

    return 1;
}

int WriteTailOfTexFile(FILE* tex_file)
{
    ASSERT(tex_file != nullptr)

    fprintf(tex_file, "\\end{document}\n");

    return 1;
}

int FillTexFile(FILE* tex_file, ExpressionParams* params)
{
    ASSERT(tex_file != nullptr)
    ASSERT(params   != nullptr)

    ShitSomeCringeIntroductionInTexFile(tex_file);

    fprintf(tex_file, "Let's calculate smth with expression given:\n");
    WriteExpressionInTexFile(*(params->expression), tex_file);

    InsertConstsInExpression(*(params->expression), params);
    SimplifyTree(params->expression);

    fprintf(tex_file, "Firstly, let's insert all constants and simplify it:\n");
    WriteExpressionInTexFile(*(params->expression), tex_file);

    fprintf(tex_file, "BRITISH SCIENTISTS WERE SHOCKED, "
                      "WHEN THEY COUNT THIS EXPRESSION ");
    PrintParametersPoint(tex_file, params);
    fprintf(tex_file, "...\n\n"
                      "IT'S VALUE = %lf !!!\n\n\n", CalculateTree(*(params->expression), params));

    // Node* diff1 = Differentiate(*(params.expression), tex_file, SKIP_STEPS_TEX_MODE);

    Node* n_diff = NDifferentiate(*(params->expression), params->n_differentiate, tex_file, PRINT_STEPS_TEX_MODE);

    fprintf(tex_file, "Finally... The %ld derivation of the expression:\n", params->n_differentiate);
    WriteExpressionInTexFile(n_diff, tex_file);

    fprintf(tex_file, "BRITISH SCIENTISTS WERE SHOCKED, "
                      "WHEN THEY COUNT THE %ld DERIVATION OF THIS EXPRESSION ", params->n_differentiate);
    PrintParametersPoint(tex_file, params);
    fprintf(tex_file, "...\n\n"
                      "IT'S VALUE = %lf !!!\n\n\n", CalculateTree(n_diff, params));

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

    return 1;
}

int CompileTexFile(const char* filename)
{
    // system("cd ./TexFiles");

    char cmd[100] = "cd ./TexFiles; pdflatex ./";
    strcat(cmd, filename);
    // strcat(cmd, " -output-directory=./TexFiles");

    return system(cmd);
}

int CreateTexFile(const char* filename, ExpressionParams* params)
{
    ASSERT(filename != nullptr);
    ASSERT(params   != nullptr);

    char tex_file_path[100] = "./TexFiles/";
    strcat(tex_file_path, filename);

    // fprintf(stderr, "%s", tex_file_path);

    // if (!IsFileExist(tex_file_path)) return 0;

    FILE* tex_file = fopen(tex_file_path, "w");
    ASSERT(tex_file != nullptr);

    WriteHeadOfTexFile(tex_file);
    FillTexFile       (tex_file, params);
    WriteTailOfTexFile(tex_file);

    fclose(tex_file);

    CompileTexFile(filename);

    return 1;
}
