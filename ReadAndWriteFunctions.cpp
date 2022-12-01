#include "./Config.h"
#include <stdio.h>
#include <string.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Tree.h"
#include "./ReadAndWriteFunctions.h"

size_t Database_format_shift = 0;

// static void FprintfNSymb(FILE* stream, char symb, size_t n_symb)
// {
//     ASSERT(stream != nullptr);
//
//     for (size_t i = 1; i <= n_symb; i++)
//         fprintf(stream, "%c", symb);
// }
//
// static void SkipNSpaces(FILE* stream, size_t n)
// {
//     ASSERT(stream != nullptr);
//
//     for (size_t i = 0; i < n; i++)
//         fgetc(stream);
// }

// int ReadDatabaseName(FILE* database_file, char* database_name)
// {
//     ASSERT(database_file != nullptr);
//     ASSERT(database_name != nullptr);
//
//     fscanf(database_file, " \"%[^\"]s", database_name);
//     fscanf(database_file, "%*[^\n]s");
//     fgetc(database_file);
//     fscanf(database_file, "%*[^\n]s");
//     fgetc(database_file);
//
//     return 1;
// }

int ReadNodeVal(char* node_val, enum TreeDataType* val_type, enum Operators* op_val, double* num_val, const char** var_val)
{
    if (IsVar(node_val))
    {
        *val_type = VAR_TYPE;
        *var_val = node_val;

        // free(node_val);

        return 1;
    }

    enum Operators op_val_temp = IsOperator(node_val);

    if (op_val_temp)
    {
        *val_type = OP_TYPE;
        *op_val = op_val_temp;

        free(node_val);

        return 1;
    }

    double num_val_temp = 0;

    if (sscanf(node_val, " %lf ", &num_val_temp))
    {
        *val_type = NUM_TYPE;
        *num_val = num_val_temp;

        free(node_val);

        return 1;
    }

    return 0;
}

int IsOperatorWithOneArgument(enum Operators op_val)
{
    if (op_val == OP_EXP     ||
        op_val == OP_SQRT    ||
        // op_val == OP_R    ||
        op_val == OP_LOG     ||
        op_val == OP_LN      ||
        op_val == OP_SIN     ||
        op_val == OP_COS     ||
        op_val == OP_TG      ||
        op_val == OP_CTG     ||
        op_val == OP_SH      ||
        op_val == OP_CH      ||
        op_val == OP_ARCSIN  ||
        op_val == OP_ARCCOS  ||
        op_val == OP_ARCTG   ||
        op_val == OP_ARCCTG    )

        return 1;

    return 0;
}

Node* ReadExpressionToTree(FILE* database_file, Tree* tree)
{
    ASSERT(tree != nullptr);
    ASSERT(database_file != nullptr);
    VERIFY_TREE(tree);

    char cur = fgetc(database_file);

    if (cur == '(')
    {
        // fprintf(stdout, "(");

        Node* new_node = (Node*) calloc(1, sizeof(Node));

        Node* left = nullptr;

        cur = fgetc(database_file);
        ungetc(cur, database_file);

        int is_subnodes = 0;

        if (cur == '(')
        {
            is_subnodes = 1;
            left = ReadExpressionToTree(database_file, tree);
        }

        char* new_node_val = (char*) calloc(MAX_NODE_VAL_LEN, sizeof(char));
        fscanf(database_file, "%[^()]s) ", new_node_val);
        // fprintf(stdout, "%s", new_node_val);

        enum TreeDataType val_type = NULL_TYPE;
        enum Operators op_val      = NULL_OP;
        double num_val             = 0;
        const char* var_val        = nullptr;

        ReadNodeVal(new_node_val, &val_type, &op_val, &num_val, &var_val);
        NodeCtor(new_node, val_type, num_val, var_val, op_val);

        Node* right = nullptr;

        if (is_subnodes)
        {
            right = ReadExpressionToTree(database_file, tree);
        }

        else if (IsOperatorWithOneArgument(op_val))
        {
            left = CREATE_NUM(0);
            right = ReadExpressionToTree(database_file, tree);
        }

        NodeConnect(left, right, new_node);

        fgetc(database_file);
        // fprintf(stdout, ")");
        // SimplifyTree(new_node);

        return new_node;
    }

    else
    {
        fprintf(stderr, "Database is damaged!!!");
        // abort();
        return nullptr;
    }
}

int ReadExpressionParams(FILE* expression_file, ExpressionParams* params)
{
    size_t i = 0;

    while (fgetc(expression_file) == '\n')
    {
        char cur = fgetc(expression_file);

        if (cur != EOF)
        {
            ungetc(cur, expression_file);

            fscanf(expression_file, "%lf", &(params->vars[i].value));

            // fprintf(stdout, "\n\n%s = %lf\n\n", params->vars[i].name, params->vars[i].value);
        }

        i++;
    }

    return 1;
}

int WriteNode(FILE* stream, Node* node)
{
    ASSERT(stream != nullptr);
    ASSERT(node != nullptr);
    VERIFY_NODE(node);

    TreeInorderPrint(node, stream);

    return 1;
}

int WriteTree(FILE* stream, Tree* tree, const char* database_name)
{
    ASSERT(stream != nullptr);
    ASSERT(tree != nullptr);
    ASSERT(database_name != nullptr);
    VERIFY_TREE(tree);

    WriteNode(stream, tree->root);

    return 1;
}

int WriteHeadOfTexFile(FILE* tex_file)
{
    fprintf(tex_file, "\\documentclass{article}\n"
                      "\\begin{document}\n\n");

    return 1;
}

int WriteTailOfTexFile(FILE* tex_file)
{
    fprintf(tex_file, "\\end{document}\n");

    return 1;
}

int WriteExpressionInTexFile(const Node* node, FILE* tex_file)
{
    ASSERT(node     != nullptr);
    ASSERT(tex_file != nullptr);

    fprintf(tex_file, "\\[");

    TranslateTreeToTex(node, tex_file);

    fprintf(tex_file, "\\]\n");

    return 1;
}

int TranslateNodeToTex(FILE* tex_file, const Node* node, const char* op_text, enum OpTexPrintModes mode, int is_brackets)
{
    ASSERT(node        != nullptr);
    ASSERT(node->left  != nullptr);
    ASSERT(node->right != nullptr);

    if (is_brackets) fprintf(tex_file, "(");

    if (mode == OP_TEX_PREPRINT_ONE_ARG)
    {
        fprintf(tex_file, "%s", op_text);
        TranslateTreeToTex(node->right, tex_file);
    }

    else if (mode == OP_TEX_INPRINT)
    {
        TranslateTreeToTex(node->left, tex_file);
        fprintf(tex_file, "%s", op_text);
        TranslateTreeToTex(node->right, tex_file);
    }

    else if (mode == OP_TEX_PREPRINT_TWO_ARGS)
    {
        fprintf(tex_file, "%s", op_text);
        TranslateTreeToTex(node->left, tex_file);
        TranslateTreeToTex(node->right, tex_file);
    }

    else return 0;

    if (is_brackets) fprintf(tex_file, ")");

    return 1;
}

int IsPrintableNode(const Node* node)
{
    if (node->val_type == NUM_TYPE && node->num_val == 0)
        return 0;

    return 1;
}

int TranslateTreeToTex(const Node* node, FILE* tex_file)
{
    ASSERT(node != nullptr);
    ASSERT(tex_file != nullptr);

    if (!node) return 1;

    if (node->val_type == OP_TYPE)
    {
        if (!((node->left) && (node->right))) return 0;

        ASSERT(node->left  != nullptr);
        ASSERT(node->right != nullptr);

        fprintf(tex_file, "{");

        switch(node->op_val)
        {
            case OP_ADD   : { TranslateNodeToTex(tex_file, node, "+"                , OP_TEX_INPRINT          , 1); break; }
            case OP_SUB   : { TranslateNodeToTex(tex_file, node, "-"                , OP_TEX_INPRINT          , 1); break; }
            case OP_MUL   : { TranslateNodeToTex(tex_file, node, "\\cdot"           , OP_TEX_INPRINT          , 0); break; }
            case OP_DIV   : { TranslateNodeToTex(tex_file, node, "\\frac"           , OP_TEX_PREPRINT_TWO_ARGS, 0); break; }
            case OP_DEG   : { TranslateNodeToTex(tex_file, node, "^"                , OP_TEX_INPRINT          , 0); break; }
            case OP_EXP   : { TranslateNodeToTex(tex_file, node, "e^"               , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_SQRT  : { TranslateNodeToTex(tex_file, node, "\\sqrt"           , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            // case OP_R    : { TranslateNodeToTex(tex_file, node, "+", OP_TEX_INPRINT); break;}
            case OP_LOG   : { TranslateNodeToTex(tex_file, node, "\\log"            , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_LN    : { TranslateNodeToTex(tex_file, node, "\\ln"             , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_SIN   : { TranslateNodeToTex(tex_file, node, "\\sin"            , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_COS   : { TranslateNodeToTex(tex_file, node, "\\cos"            , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_TG    : { TranslateNodeToTex(tex_file, node, "\\tan"            , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_CTG   : { TranslateNodeToTex(tex_file, node, "\\cot"            , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_SH    : { TranslateNodeToTex(tex_file, node, "\\sinh"           , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_CH    : { TranslateNodeToTex(tex_file, node, "\\cosh"           , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_ARCSIN: { TranslateNodeToTex(tex_file, node, "\\arcsin"         , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_ARCCOS: { TranslateNodeToTex(tex_file, node, "\\arccos"         , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_ARCTG : { TranslateNodeToTex(tex_file, node, "\\arctan"         , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_ARCCTG: { TranslateNodeToTex(tex_file, node, "\\pi/2-\\arctan"  , OP_TEX_PREPRINT_ONE_ARG , 1); break; }

            default      : return 0;
        }

        fprintf(tex_file, "}");

        return 1;
    }

    fprintf(tex_file, "{");

    // if (IsPrintableNode(node)) NodeValPrint(node, tex_file);

    NodeValPrint(node, tex_file);

    fprintf(tex_file, "}");

    return 1;
}

int CompileTexFile(const char* filename)
{
    char cmd[100] = "pdflatex ";
    strcat(cmd, filename);

    return system(cmd);
}
