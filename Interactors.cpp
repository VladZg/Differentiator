#include "./Config.h"
#include <stdio.h>
#include <string.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Tree.h"
#include "./Interactors.h"

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

    if (FindVarIndex(node_val, params) == NO_VAR_NAME)
    {
        char* new_var_name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));
        ASSERT(new_var_name != nullptr);

        memcpy(new_var_name, node_val, MAX_VAR_NAME_LEN);
        AddVar(new_var_name, 0, params);

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
        fprintf(stderr, "Database is damaged!!!");
        // abort();
        return nullptr;
    }
}

int ReadExpressionParams(FILE* expression_file, ExpressionParams* params)
{
    params->vars = (ExpressionVar*) calloc(MAX_NUM_OF_VAR, sizeof(ExpressionVar));

    AddConstants(params);

    params->expression = ReadExpressionToTree(expression_file, params);

    fgetc(expression_file);
    fgetc(expression_file); // пропуск строки

    for (size_t var_i = NUM_OF_CONSTANTS; var_i < params->n_vars; var_i++)
    {
        char   var_name[MAX_VAR_NAME_LEN] = {};
        double var_val = 0;

        fscanf(expression_file, " %s = %lf", var_name, &var_val);

        int var_index = FindVarIndex(var_name, params);

        if (var_index != NO_VAR_NAME)
            params->vars[var_index].value = var_val;

        // else fprintf(stderr, "NO VARIABLES?");
    }

    fgetc(expression_file); // пропуск строки

    fscanf(expression_file, " n_differentiate   = %ld ", &(params->n_differentiate));
    fscanf(expression_file, " Makloren_accuracy = %ld ", &(params->Makloren_accuracy));

    // AddConstants(params);

    return 1;
}

int AddVar(char* var_name, double var_val, ExpressionParams* params)
{
    params->vars[(params->n_vars)++] = {var_name, var_val};

    return 1;
}

int FindVarIndex(const char* var_name, const ExpressionParams* params)
{
    for (size_t var_i = 0; var_i < params->n_vars; var_i++)
    {
        if (!strcasecmp(var_name, params->vars[var_i].name))
            return var_i;
    }

    return NO_VAR_NAME;
}

#define DEF_CONST(name, const_value)                            \
{                                                               \
    int var_index = FindVarIndex(#name, params);                \
                                                                \
    if (var_index == NO_VAR_NAME)                               \
    {                                                           \
        char* const_name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));\
        memcpy(const_name, #name, strlen(#name));\
        AddVar(const_name, (double) const_value, params);       \
    }                                                           \
}

int AddConstants(ExpressionParams* params)
{
    #include "./ConstsTable.h"

    return 0;
}

#undef DEF_CONST

void ExpressionParamsDump(FILE* stream, ExpressionParams* params)
{
    fprintf(stream, KBLU "\n! Expression params:\n"  KNRM);

    fprintf(stream, "! \n"
                    "!-Constants (" KYEL "%d" KNRM "):    \n", NUM_OF_CONSTANTS);

    for (size_t const_i = 0; const_i < NUM_OF_CONSTANTS; const_i++)
        fprintf(stream, "! [" KGRN "%ld" KNRM "] " KYEL "%10s" KNRM " = %lf\n", const_i, params->vars[const_i].name, params->vars[const_i].value);

    fprintf(stream, "! \n"
                    "!-Variables (" KMAG "%d" KNRM "):    \n", (int) params->n_vars - NUM_OF_CONSTANTS);

    for (size_t var_i = NUM_OF_CONSTANTS; var_i < params->n_vars; var_i++)
        fprintf(stream, "! [" KGRN "%ld" KNRM "] " KMAG "%10s" KNRM " = %lf\n", var_i, params->vars[var_i].name, params->vars[var_i].value);

    fprintf(stream, "! \n"
                    "!-Number of differentiates: %ld\n"
                    "!-Macloren's accuracy:      %ld\n\n", params->n_differentiate, params->Makloren_accuracy);
}

int ExpressionParamsDtor(ExpressionParams* params)
{
    ASSERT(params  != nullptr);

    NodeDtor(params->expression);
    params->Makloren_accuracy = 0;
    params->n_differentiate   = 0;

    for (size_t var_i = 0; var_i < params->n_vars; var_i++)
    {
        free(params->vars[var_i].name);
        params->vars[var_i].name = nullptr;
    }

    free(params->vars);
    params->vars = nullptr;

    params = nullptr;

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
