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
#include "./ReadAndWriteFunctions.h"
#include "./Differentiation.h"
#include "./DiffDSL.h"
// #include "./CheckFile.h"

// const char  DATABASE_FILENAME_DEFAULT[] = "./Database.txt";
// const char* DATABASE_FILENAME           = nullptr;

int main() //const int argc, const char** argv
{
    // if (!CheckFile(argc, argv, &DATABASE_FILENAME))
    //     DATABASE_FILENAME = DATABASE_FILENAME_DEFAULT;

    Tree tree = {};
    TreeCtor(&tree);

    // Node* root = (Node*) calloc(1, sizeof(Node));
    // tree.root = root;

    FILE* database_file = fopen("./Expression.txt", "r");
    Node* expression = ReadExpressionToTree(database_file, &tree);

    ExpressionParams params = {};

    ExpressionVar vars[5] = {};
    params.vars = vars;

    ReadExpressionParams(database_file, &params);

    fclose(database_file);

    FILE* tex_file = fopen("./TexExpressions.tex", "w");
    WriteHeadOfTexFile(tex_file);

    // TreeInorderPrint(expression, stderr);
    // HERE()
    ShowTree(expression, SIMPLE_DUMP_MODE);
    // ShowTree(expression, FULL_FULL_DUMP_MODE);
    SimplifyTree(&expression);
    ShowTree(expression, SIMPLE_DUMP_MODE, 1);
    // ShowTree(expression, FULL_FULL_DUMP_MODE);

    // TreeInorderPrint(expression, stderr);
    // HERE()

    // Node* expression_copy = CopyNode(expression);

    fprintf(tex_file, "Let's calculate smth with expression given:\n");
    WriteExpressionInTexFile(expression, tex_file);
    // fprintf(stdout, "\n");
//
    // fprintf(tex_file, "Calculating the first derivation of it:\n\n");
    // Node* diffed1_tree = NDiff(expression, 1, tex_file);

    // Node* diff1 = Differentiate(expression, tex_file);

    // Node* diffed1_tree = expression_copy;
    // SimplifyTree(diff1);

    // fprintf(tex_file, "The first derivation:\n");
    // WriteExpressionInTexFile(diff1, tex_file);

//     // fprintf(stdout, "\n");
//
//     root_copy = CopyNode(tree.root);
    // fprintf(tex_file, "Calculating the second derivation of it:\n\n");

    // Node* diff2 = Differentiate(diff1, tex_file);

    // fprintf(tex_file, "Thus, the fivth derivation:\n");
    // WriteExpressionInTexFile(diffed5_tree, tex_file);
    // fprintf(stdout, "\n");

    // Node* diff3 = Differentiate(diff2, tex_file);
    // Node* diff4 = Differentiate(diff3, tex_file);

    // NodeDtor(tree.root);
    // NodeDtor(diffed1_tree);
    // NodeDtor(diffed2_tree);

    // fprintf(tex_file, "Calculating the fivth derivation of it:\n\n");
    // Node* diffed5_tree = NDiff(tree.root, 5, tex_file);
    // fprintf(tex_file, "Thus, the fivth derivation:\n");
    // WriteExpressionInTexFile(diffed5_tree, tex_file);
    // // fprintf(stdout, "\n");

    WriteTailOfTexFile(tex_file);
    fclose(tex_file);

    // ShowTree(diff1, FULL_FULL_DUMP_MODE);

    NodeDtor(expression);
    TreeDtor(&tree);
    // NodeDtor(expression_copy);

    // NodeDtor(diff1);
    // NodeDtor(diff2);
    // NodeDtor(diff3);
    // NodeDtor(diff4);

    CompileTexFile("./TexExpressions.tex");

    return 1;
}
