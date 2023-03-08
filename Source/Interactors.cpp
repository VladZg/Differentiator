#include "../Include/Config.h"
#include <stdio.h>
#include <string.h>
#include "../Include/Constants.h"
#include "../Include/DefineColourConsts.h"
#include "../Libs/Stack/Include/Assert.h"
#include "../Include/Tree.h"
#include "../Include/TreeDump.h"
#include "../Include/ExpressionReader.h"
#include "../Include/Interactors.h"
#include "../Include/TreeSimplifyFunctions.h"

int ReadNodeVal(char* node_val, enum TreeDataType* val_type, enum Operators* op_val, double* num_val, const char** var_val, ExpressionParams* params)
{
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

    // fprintf(stdout, "\n\nhere: %s\n\n", node_val);

    if (FindVarIndex(node_val, params->vars, params->n_vars) == NO_VAR_NAME)
    {
        char* new_var_name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));
        ASSERT(new_var_name != nullptr);

        memcpy(new_var_name, node_val, MAX_VAR_NAME_LEN);
        AddVar(new_var_name, DEFAULT_UNDEFINED_VAR_VALUE, params);

        // fprintf(stdout, "\n\nhere: %s  %ld\n\n", new_var_name, params->n_vars);
    }

    *val_type = VAR_TYPE;
    *var_val  = node_val;

    // free(node_val);

    return 1;

    return 0;
}

Node* ReadExpressionToTree(FILE* expression_file, ExpressionParams* params)
{
    // ASSERT(tree != nullptr);
    ASSERT(expression_file != nullptr);
    // VERIFY_TREE(tree);

    char cur = fgetc(expression_file);

    if (cur == '(')
    {
        // fprintf(stdout, "(");

        Node* new_node = (Node*) calloc(1, sizeof(Node));

        Node* left = nullptr;

        cur = fgetc(expression_file);
        ungetc(cur, expression_file);

        int is_subnodes = 0;

        if (cur == '(')
        {
            is_subnodes = 1;
            left = ReadExpressionToTree(expression_file, params);
        }

        char* new_node_val = (char*) calloc(MAX_NODE_VAL_LEN, sizeof(char));
        fscanf(expression_file, "%[^()]s) ", new_node_val);
        // fprintf(stdout, "%s", new_node_val);

        enum TreeDataType val_type = NULL_TYPE;
        enum Operators op_val      = NULL_OP;
        double num_val             = 0;
        const char* var_val        = nullptr;

        ReadNodeVal(new_node_val, &val_type, &op_val, &num_val, &var_val, params);
        NodeCtor(new_node, val_type, num_val, var_val, op_val);

        Node* right = nullptr;

        if (is_subnodes)
        {
            right = ReadExpressionToTree(expression_file, params);
        }

        else if (IsOperatorWithOneArgument(op_val))
        {
            left = CREATE_NUM(0);
            right = ReadExpressionToTree(expression_file, params);
        }

        NodeConnect(left, right, new_node);

        fgetc(expression_file);
        // fprintf(stdout, ")");
        // SimplifyTree(new_node);

        return new_node;
    }

    else
    {
        fprintf(stderr, "Expression is damaged!!!");
        abort();
    }
}

int ReadExpressionParams(FILE* expression_file, ExpressionParams* params)
{
    params->vars = (ExpressionVar*) calloc(MAX_NUM_OF_VAR, sizeof(ExpressionVar));

    AddConstants(params);

    params->expression = (Node**) calloc(1, sizeof(Node*));
    ASSERT(params->expression != nullptr);

    // *(params->expression) = ReadExpressionToTree(expression_file, params);  //old version

    char  str[MAX_EXPRESSION_LEN] = {};
    const char* expression_str = nullptr;

    fscanf(expression_file, "%s", str);

    *(params->expression) = GetG(str, &expression_str, params);

    // ShowTree(*(params->expression), SIMPLE_DUMP_MODE, 1);

    fgetc(expression_file);
    fgetc(expression_file); // пропуск строки

    // fprintf(stdout, "%ld\n", params->n_vars);

    for (size_t var_i = NUM_OF_CONSTANTS; var_i < params->n_vars; var_i++)
    {
        char   var_name[MAX_VAR_NAME_LEN] = {};
        double var_val = 0;

        fscanf(expression_file, " %s = %lf", var_name, &var_val);

        int var_index = FindVarIndex(var_name, params->vars, params->n_vars);

        if (var_index != NO_VAR_NAME)
            params->vars[var_index].value = var_val;

        else
        {
            fprintf(stderr, "VARS NOT INITIALIZED!!!");
            abort();
        }
    }

    if (params->n_vars != NUM_OF_CONSTANTS) fgetc(expression_file); // пропуск строки

    params->graph_diapasone = (char*) calloc(15, sizeof(char));

    fscanf(expression_file, " n_differentiate   = %ld ", &(params->n_differentiate));
    fscanf(expression_file, " Makloren_accuracy = %ld ", &(params->Makloren_accuracy));
    fscanf(expression_file, " tangent_point     = %lf ", &(params->tangent_point));
    fscanf(expression_file, " delta_coverage    = %lf ", &(params->delta_coverage));
    fscanf(expression_file, " graph_diapasone   = %s  ", params->graph_diapasone);

    // InsertConstsInExpression(*(params->expression), params);

    return 1;
}

#define DEF_CONST(name, const_value)                                       \
{                                                                          \
    int var_index = FindVarIndex(#name, params->vars, params->n_vars);     \
                                                                           \
    if (var_index == NO_VAR_NAME)                                          \
    {                                                                      \
        char* const_name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char)); \
        memcpy(const_name, #name, strlen(#name));                          \
        AddVar(const_name, (double) const_value, params);                  \
    }                                                                      \
}

int AddConstants(ExpressionParams* params)
{
    #include "../Include/ConstsTable.h"

    return 0;
}

#undef DEF_CONST

int IsConstsInTree(const Node* node, const ExpressionVar* vars)
{
    ASSERT(vars != nullptr)

    if (!node) return 0;

    if (IsConstsInTree(node->left, vars) || IsConstsInTree(node->right, vars))
        return 1;

    if (node->val_type == VAR_TYPE && FindVarIndex(node->var_val, vars, NUM_OF_CONSTANTS) != NO_VAR_NAME)
        return 1;

    return 0;
}

void PrintParametersPoint(FILE* stream, const ExpressionParams* params)
{
    if (params->n_vars == NUM_OF_CONSTANTS) return;

    fprintf(stream, "In the point $M_0$(");

    for (size_t var_i = NUM_OF_CONSTANTS; var_i < params->n_vars - 1; var_i++)
        fprintf(stream, "$%s_0$, ", params->vars[var_i].name);

    fprintf(stream, "$%s_0$) = (", params->vars[params->n_vars-1].name);

    for (size_t var_i = NUM_OF_CONSTANTS; var_i < params->n_vars - 1; var_i++)
        fprintf(stream, "%." NUMS_PRINT_ACCURACY "lf, ", params->vars[var_i].value);

    fprintf(stream, "%." NUMS_PRINT_ACCURACY "lf)", params->vars[params->n_vars-1].value);
}

int PrintAllVarNames(FILE* stream, const ExpressionParams* params)
{
    for (int var_i = NUM_OF_CONSTANTS; var_i < (int) params->n_vars; var_i++)
        fprintf(stream, "%s, ", params->vars[var_i].name);

    return 1;
}

int PrintTextInTex(FILE* tex_file, const char* fmt_text, ...)
{
    va_list args;
    va_start(args, fmt_text);

    // fprintf(tex_file, "%s", (print_mode == USUAL_PRINT_MODE ? "" : "\\["));

    vfprintf(tex_file, fmt_text, args);

    // fprintf(tex_file, "%s", (print_mode == USUAL_PRINT_MODE ? "" : "\\]"));

    va_end(args);

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
