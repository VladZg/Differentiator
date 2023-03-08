#include "../Include/Config.h"
#include <stdlib.h>
#include "../Include/DefineColourConsts.h"
#include "../Libs/Stack/Assert.h"
#include "../Include/Tree.h"
#include "../Include/TreeDump.h"
#include "../Include/ExpressionReader.h"

// const char* S = nullptr;

void SyntaxErrorPrint()
{
    fprintf(stderr, KYEL "Syntax error!\n" KNRM);
    exit(1);
}

Node* GetG(const char* str, const char** expression_str, ExpressionParams* params)
{
    ASSERT(str != nullptr);
    ASSERT(expression_str != nullptr);

    *expression_str = str;

    Node* node = GetE(expression_str, params);

    ASSERT(**expression_str == '\0');

    VERIFY_NODE(node);

    return node;
}

Node* GetE(const char** expression_str, ExpressionParams* params)
{
    Node* node = GetT(expression_str, params);

    while (**expression_str == '+' || **expression_str == '-')
    {
        char op = **expression_str;

        (*expression_str)++;

        Node* left_node  = node;
        Node* right_node = GetT(expression_str, params);

        if (op == '+')
            node = ADD(left_node, right_node);

        else
            node = SUB(left_node, right_node);
    }

    return node;
}

Node* GetT(const char** expression_str, ExpressionParams* params)
{
    Node* node = GetD(expression_str, params);

    while (**expression_str == '*' || **expression_str == '/')
    {
        char op = **expression_str;

        (*expression_str)++;

        Node* left_node  = node;
        Node* right_node = GetD(expression_str, params);

        if (op == '*')
            node = MUL(left_node, right_node);

        else
            node = DIV(left_node, right_node);
    }

    return node;
}

Node* GetD(const char** expression_str, ExpressionParams* params)
{
    Node* node = GetP(expression_str, params);

    while (**expression_str == '^')
    {
        (*expression_str)++;

        Node* left_node  = node;
        Node* right_node = GetP(expression_str, params);

        node = DEG(left_node, right_node);
    }

    return node;
}

Node* GetP(const char** expression_str, ExpressionParams* params)
{
    Node* node = nullptr;

    if (**expression_str == '(')
    {
        (*expression_str)++;

        node = GetE(expression_str, params);

        ASSERT(**expression_str == ')');

        (*expression_str)++;
    }

    else if ('0' <= **expression_str && **expression_str <= '9')
        node = GetN(expression_str);

    else
        node = GetW(expression_str, params);

    return node;
}

Node* GetW(const char** expression_str, ExpressionParams* params)
{
    Node* node = nullptr;
    char* name = GetV(expression_str);

    if (**expression_str == '(')
    {
        (*expression_str)++;

        Node* arg_node = GetE(expression_str, params);

        // fprintf(stdout, "function name: %s\n", name);

             if (!strcasecmp(name, "exp"   )) node = EXP   (arg_node);
        else if (!strcasecmp(name, "sqrt"  )) node = SQRT  (arg_node);
        else if (!strcasecmp(name, "ln"    )) node = LN    (arg_node);
        else if (!strcasecmp(name, "sin"   )) node = SIN   (arg_node);
        else if (!strcasecmp(name, "cos"   )) node = COS   (arg_node);
        else if (!strcasecmp(name, "tg"    )) node = TG    (arg_node);
        else if (!strcasecmp(name, "ctg"   )) node = CTG   (arg_node);
        else if (!strcasecmp(name, "ch"    )) node = CH    (arg_node);
        else if (!strcasecmp(name, "sh"    )) node = SH    (arg_node);
        else if (!strcasecmp(name, "arcsin")) node = ARCSIN(arg_node);
        else if (!strcasecmp(name, "arccos")) node = ARCCOS(arg_node);
        else if (!strcasecmp(name, "arctg" )) node = ARCTG (arg_node);
        else if (!strcasecmp(name, "arcctg")) node = ARCCTG(arg_node);

        free((void*) name);

        ASSERT(**expression_str == ')')

        (*expression_str)++;
    }

    else
    {
        if (FindVarIndex(name, params->vars, params->n_vars) == NO_VAR_NAME)
        {
            char*  new_var_name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));
            ASSERT(new_var_name != nullptr);

            memcpy(new_var_name, name, MAX_VAR_NAME_LEN);
            AddVar(new_var_name, DEFAULT_UNDEFINED_VAR_VALUE, params);
        }

        node = CREATE_VAR(name);
    }

    return node;
}

char* GetV(const char** expression_str)
{
    char* name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));

    int i = 0;

    while (('A' <= **expression_str && **expression_str <= 'Z') ||
           ('a' <= **expression_str && **expression_str <= 'z')   )
    {
        name[i++] = **expression_str;

        (*expression_str)++;
    }

    return name;
}

Node* GetN(const char** expression_str)
{
    double val = 0;

    const char* s_old = *expression_str;

    while ('0' <= **expression_str && **expression_str <= '9')
    {
        val = val * 10 + (**expression_str - '0');
        (*expression_str)++;
    }

    ASSERT(*expression_str > s_old);

    // fprintf(stderr, "%lf\n", val);

    return CREATE_NUM(val);
}
