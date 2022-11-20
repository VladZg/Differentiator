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
#include "./ReadAndWriteFunctions.h"
#include "./DifferentiationRules.h"
#include "./DiffDSL.h"
// #include "./CheckFile.h"

size_t N_step = 1;

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
    tree.root = ReadExpressionToTree(database_file, &tree);
    fclose(database_file);

    FILE* tex_file = fopen("./TexExpressions.tex", "w");
    WriteHeadOfTexFile(tex_file);

    // SimplifyTree(tree.root);

    fprintf(tex_file, "Let's calculate smth with expression given:\n");
    WriteExpressionInTexFile(tree.root, tex_file);
    // fprintf(stdout, "\n");

    fprintf(tex_file, "Calculating the first derivation of it:\n\n");
    N_step = 1;
    Node* diffed1_tree = Diff(tree.root, tex_file, &N_step);
    fprintf(tex_file, "Thus, the first derivation:\n");
    WriteExpressionInTexFile(diffed1_tree, tex_file);
    // fprintf(stdout, "\n");

    fprintf(tex_file, "Calculating the second derivation of it:\n\n");
    N_step = 1;
    Node* diffed2_tree = Diff(diffed1_tree, tex_file, &N_step);
    fprintf(tex_file, "Thus, the second derivation:\n");
    WriteExpressionInTexFile(diffed2_tree, tex_file);
    // fprintf(stdout, "\n");

    WriteTailOfTexFile(tex_file);
    fclose(tex_file);

    CompileTexFile("./TexExpressions.tex");

    return 1;
}
