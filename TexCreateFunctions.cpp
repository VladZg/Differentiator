
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
#include "./ExpressionReader.h"
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

int FindFirstDerivationTex(Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!expression) return 1;

    Node* diff1 = Differentiate(expression, tex_file, PRINT_STEPS_TEX_MODE);

    fprintf(tex_file, "Congratulations! The first derivation of the expression is:\n");
    WriteExpressionInTexFile(diff1, tex_file);

    double diff1_value = CalculateTree(diff1, params);

    PrintParametersPoint(tex_file, params);
    fprintf(tex_file, "IT'S VALUE = %." NUMS_PRINT_ACCURACY "lf !!!\n\n\n", diff1_value);

    NodeDtor(&diff1);

    return 1;
}

int SimplifyExpressionTex(Node** expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)
    ASSERT(expression != nullptr)

    if (!expression) return 1;

    InsertConstsInExpression(*expression, params);
    *expression = SimplifyTree(expression);

    if (IsVarsInTree(*expression))
    {
        fprintf(tex_file, "Firstly, let's insert all constants and simplify it:\n");
        WriteExpressionInTexFile(*expression, tex_file);
    }

    return 1;
}

int CalculateExpressionTex(Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!expression) return 1;

    fprintf(tex_file, "BRITISH SCIENTISTS WERE SHOCKED, WHEN THEY COUNT IT!!! ");
    PrintParametersPoint(tex_file, params);
    fprintf(tex_file, "IT'S VALUE = %." NUMS_PRINT_ACCURACY "lf !!!\n\n\n", CalculateTree(expression, params));

    return 1;
}

int FindNDerivationTex(Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!expression) return 1;

    fprintf(tex_file, "Let's calculate the %ld derivation of the expression:\n\n", params->n_differentiate);

    Node* n_diff = NDifferentiate(expression, params->n_differentiate, tex_file, PRINT_STEPS_TEX_MODE);

    fprintf(tex_file, "Finally... The %ld derivation of the expression:\n", params->n_differentiate);
    WriteExpressionInTexFile(n_diff, tex_file);

    fprintf(tex_file, "BRITISH SCIENTISTS WERE SHOCKED, "
                      "WHEN THEY COUNT THE %ld DERIVATION OF THIS EXPRESSION!!!\n", params->n_differentiate);
    PrintParametersPoint(tex_file, params);
    fprintf(tex_file, "IT'S VALUE = %." NUMS_PRINT_ACCURACY "lf !!!\n\n\n", CalculateTree(n_diff, params));

    NodeDtor(&n_diff);

    return 1;
}

int FindFullDerivationTex(const Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!expression) return 1;

    if (params->n_vars - NUM_OF_CONSTANTS > 0)
    {
        Node** part_diffs = (Node**) calloc(params->n_vars - NUM_OF_CONSTANTS, sizeof(Node*));
        Node* full_derivation = CREATE_NUM(0);
        Node* old_full_derivation = nullptr;

        for (int var_i = NUM_OF_CONSTANTS, part_diff_j = 0; var_i < (int) params->n_vars; var_i++, part_diff_j++)
        {
            part_diffs[part_diff_j] = FindPartialDerivation(expression, params->vars, var_i, params->n_vars, tex_file);

            fprintf(tex_file, "Partial derivation of the expression on the variable '%s':\n", params->vars[var_i].name);
            WriteExpressionInTexFile(part_diffs[part_diff_j], tex_file);

            old_full_derivation = CopyNode(full_derivation);
            NodeDtor(&full_derivation);

            // ShowTree(part_diffs[part_diff_j], SIMPLE_DUMP_MODE, 1);

            full_derivation = ADD(old_full_derivation, DEG(part_diffs[part_diff_j], CREATE_NUM(2)));

            PrintParametersPoint(tex_file, params);
            fprintf(tex_file, " \nIT'S VALUE = %lf !!!\n\n\n", CalculateTree(part_diffs[part_diff_j], params));
        }

        old_full_derivation = CopyNode(full_derivation);
        NodeDtor(&full_derivation);
        full_derivation = SQRT(old_full_derivation);

        full_derivation = SimplifyTree(&full_derivation);

        fprintf(tex_file, "Full derivation:\n");
        WriteExpressionInTexFile(full_derivation, tex_file);

        PrintParametersPoint(tex_file, params);
        fprintf(tex_file, "IT'S VALUE = %." NUMS_PRINT_ACCURACY "lf !!!\n\n\n", CalculateTree(full_derivation, params));

        NodeDtor(&full_derivation);
        free(part_diffs);

        return 1;
    }

    fprintf(tex_file, "There is no variables to count partical derivations\n\n");

    return 0;
}

int ExploreFunctionOfManyVariablesTex(const Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!expression) return 1;

    //Рассчёт значения в точке
    CalculateExpressionTex(*(params->expression), params, tex_file);

    //Первая производная
    FindFirstDerivationTex(*(params->expression), params, tex_file);

    //n-я производная в общем виде и значение в точке
    FindNDerivationTex(*(params->expression), params, tex_file);

    // Частные и полная производные
    FindFullDerivationTex(*(params->expression), params, tex_file);

    return 1;
}

Node* TranslateExpresiionToFunctionOfTheFirstVar(const Node* expression, ExpressionParams* params)
{
    ASSERT(params != nullptr)

    if (!expression) return nullptr;

    if ((int) params->n_vars - NUM_OF_CONSTANTS > 1)
        return ReplaceVarsWithNumsExceptOne(CopyNode(expression), params->vars, NUM_OF_CONSTANTS, params->n_vars);

    return CopyNode(expression);
}

int DecomposeOnMaklorensFormulaTex(Node* function_of_the_first_variable, ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!function_of_the_first_variable) return 1;

    if ((int) params->n_vars - NUM_OF_CONSTANTS >= 1)
    {
        double* Maklorens_coefficients = (double*) calloc(params->Makloren_accuracy + 1, sizeof(double));
        ASSERT(Maklorens_coefficients != nullptr)

        Node* Maklorens_formula = DecomposeByMacloren(function_of_the_first_variable, params, Maklorens_coefficients, tex_file);
        Maklorens_formula = SimplifyTree(&Maklorens_formula);

        char* var_name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));
        memcpy(var_name, params->vars[NUM_OF_CONSTANTS].name, MAX_VAR_NAME_LEN);

        Node* remaining_member_of_decomposing = DEG(SUB(CREATE_VAR(var_name), CREATE_NUM(params->vars[NUM_OF_CONSTANTS].value)),
                                                    CREATE_NUM(params->Makloren_accuracy));
        remaining_member_of_decomposing = SimplifyTree(&remaining_member_of_decomposing);

        // ShowTree(Maklorens_formula, FULL_FULL_DUMP_MODE, 1);
        fprintf(tex_file, "Maklorens formula for %s near to %lf:\n", params->vars[NUM_OF_CONSTANTS].name, params->vars[NUM_OF_CONSTANTS].value);
        WriteExpressionInTexFile(Maklorens_formula, tex_file);

        fprintf(tex_file, "And remainig member is o maloe from:\n");
        WriteExpressionInTexFile(remaining_member_of_decomposing, tex_file);

        NodeDtor(&Maklorens_formula);
        NodeDtor(&remaining_member_of_decomposing);

        free(Maklorens_coefficients);

        return 1;
    }

    fprintf(tex_file, "Maaaan... Why do you even need full derivation if it's 0?...\n");

    return 0;
}

int EquationsInThePointTex(Node* function_of_the_first_variable, ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!function_of_the_first_variable) return 1;

    double old_var_val = params->vars[NUM_OF_CONSTANTS].value;

    if ((int) params->n_vars - NUM_OF_CONSTANTS >= 1)
    {
        params->vars[NUM_OF_CONSTANTS].value = params->tangent_point;

        Node* function_of_the_first_var_diff = Differentiate(function_of_the_first_variable, tex_file, SKIP_STEPS_TEX_MODE);

        double function_of_the_first_var_value_in_point = CalculateTree(function_of_the_first_variable, params);
        double tangent_coefficient = CalculateTree(function_of_the_first_var_diff, params);

        char* var_name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));
        memcpy(var_name, params->vars[NUM_OF_CONSTANTS].name, MAX_VAR_NAME_LEN);

        Node* tangent_equation = ADD(MUL(CREATE_NUM(tangent_coefficient), SUB(CREATE_VAR(var_name), CREATE_NUM(params->tangent_point))),
                                     CREATE_NUM(function_of_the_first_var_value_in_point));
        tangent_equation = SimplifyTree(&tangent_equation);

        fprintf(tex_file, "Tangent equation in point %." NUMS_PRINT_ACCURACY "lf:\n", params->tangent_point);
        fprintf(tex_file, "f(%s) = ", params->vars[NUM_OF_CONSTANTS].name);
        WriteExpressionInTexFile(tangent_equation, tex_file);

        if (tangent_coefficient != 0)
        {
            double normal_coefficient = -1 / tangent_coefficient;

            var_name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));
            memcpy(var_name, params->vars[NUM_OF_CONSTANTS].name, MAX_VAR_NAME_LEN);

            Node* normal_equation = ADD(MUL(CREATE_NUM(normal_coefficient), SUB(CREATE_VAR(var_name), CREATE_NUM(params->tangent_point))),
                                        CREATE_NUM(function_of_the_first_var_value_in_point));

            fprintf(tex_file, "Normal equation in point %." NUMS_PRINT_ACCURACY "lf:\n", params->tangent_point);
            fprintf(tex_file, "f(%s) = ", params->vars[NUM_OF_CONSTANTS].name);
            WriteExpressionInTexFile(normal_equation, tex_file);

            NodeDtor(&normal_equation);
        }

        else fprintf(tex_file, "Normal equation in point %." NUMS_PRINT_ACCURACY "lf: %s = %." NUMS_PRINT_ACCURACY "lf\n",
                     params->tangent_point, params->vars[NUM_OF_CONSTANTS].name, function_of_the_first_var_value_in_point);

        params->vars[NUM_OF_CONSTANTS].value = old_var_val;

        NodeDtor(&tangent_equation);
        NodeDtor(&function_of_the_first_var_diff);

        return 1;
    }

    fprintf(tex_file, "Tangent equation for each point is f = 0\n"
                      "Normal equation can't be writen\n");

    return 0;
}

int TranslateTreeToGnuplotFormula(const Node* node, char* formula)
{
    ASSERT(formula != nullptr)

    if (!node) return 1;

    strcat(formula, "(");

    if (node->left)
        TranslateTreeToGnuplotFormula(node->left, formula);

    if      (node->val_type == VAR_TYPE)
        strcat(formula, "x");

    else if (node->val_type == NUM_TYPE)
    {
        char num_str[20] = {};
        sprintf(num_str, "%." NUMS_PRINT_ACCURACY "lf", node->num_val);
        strcat(formula, num_str);
    }

    else if (node->val_type == OP_TYPE)
    {
        switch(node->op_val)
        {
            case OP_ADD   : { strcat(formula, "+"           ); break; }
            case OP_SUB   : { strcat(formula, "-"           ); break; }
            case OP_MUL   : { strcat(formula, "*"           ); break; }
            case OP_DIV   : { strcat(formula, "/"           ); break; }
            case OP_DEG   : { strcat(formula, "**"          ); break; }
            case OP_EXP   : { strcat(formula, "e**"         ); break; }
            case OP_SQRT  : { strcat(formula, "sqrt"        ); break; }
            // case OP_R    : { TranslateNodeToTex(tex_file, node, "+", OP_TEX_INPRINT); break;}
            case OP_LOG   : { strcat(formula, "log"         ); break; }
            case OP_LN    : { strcat(formula, "ln"          ); break; }
            case OP_SIN   : { strcat(formula, "sin"         ); break; }
            case OP_COS   : { strcat(formula, "cos"         ); break; }
            case OP_TG    : { strcat(formula, "tan"         ); break; }
            case OP_CTG   : { strcat(formula, "cot"         ); break; }
            case OP_SH    : { strcat(formula, "sinh"        ); break; }
            case OP_CH    : { strcat(formula, "cosh"        ); break; }
            case OP_ARCSIN: { strcat(formula, "arcsin"      ); break; }
            case OP_ARCCOS: { strcat(formula, "arccos"      ); break; }
            case OP_ARCTG : { strcat(formula, "arctan"      ); break; }
            case OP_ARCCTG: { strcat(formula, "pi/2-arctan" ); break; }

            case NULL_OP  : return 0;
            // default:
        }
    }

    if (node->right)
        TranslateTreeToGnuplotFormula(node->right, formula);

    strcat(formula, ")");

    return 1;
}

int GraphOfFunction(Node* function_of_the_first_variable, ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!function_of_the_first_variable) return 0;

    fprintf(tex_file, "Graph f(%s):\n\n", params->vars[NUM_OF_CONSTANTS].name);

    char cmd[320] = "plot ";
    char function_formula[300] = {};

    TranslateTreeToGnuplotFormula(function_of_the_first_variable, function_formula);

    strcat(cmd, function_formula);

    FILE* graph_file = fopen("Graphics.txt", "w");

    fprintf(graph_file, "%s\n", cmd);

    fclose(graph_file);

    system("gnuplot Graphics.txt");

    return 1;
}

int ExploreFunctionOfTheFirstVariableTex(Node** function_of_the_first_variable, ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!(*function_of_the_first_variable)) return 1;

    *function_of_the_first_variable = SimplifyTree(function_of_the_first_variable);

    if ((int) params->n_vars - NUM_OF_CONSTANTS > 0)
    {
        fprintf(tex_file, "Let's consider the expression as a function of %s variable:\n"
                          "f(%s) = ", params->vars[NUM_OF_CONSTANTS].name, params->vars[NUM_OF_CONSTANTS].name);
        WriteExpressionInTexFile(*function_of_the_first_variable, tex_file);
    }

    // разложение по Тейлору (Маклорену)
    DecomposeOnMaklorensFormulaTex(*function_of_the_first_variable, params, tex_file);

    //Построение графика
    GraphOfFunction(*function_of_the_first_variable, params, tex_file);

    //Уравнения касательной/нормали в точке
    EquationsInThePointTex(*function_of_the_first_variable, params, tex_file);

    return 1;
}

int FillTexFile(FILE* tex_file, ExpressionParams* params)
{
    ASSERT(tex_file != nullptr)
    ASSERT(params   != nullptr)

    //Введение
    ShitSomeCringeIntroductionInTexFile(tex_file);

    //Печать начального выражения
    fprintf(tex_file, "Let's calculate smth with expression given:\n");
    WriteExpressionInTexFile(*(params->expression), tex_file);

    //Упрощение выражения
    SimplifyExpressionTex(params->expression, params, tex_file);

    //Исследование выражения как функции многих переменных
    ExploreFunctionOfManyVariablesTex(*(params->expression), params, tex_file);

    //Переход к функции от одной переменной (первой)
    Node* function_of_the_first_variable = TranslateExpresiionToFunctionOfTheFirstVar(*(params->expression), params);

    //Исследование выражения, как функции одной переменной (первой)
    ExploreFunctionOfTheFirstVariableTex(&function_of_the_first_variable, params, tex_file);

    NodeDtor(&function_of_the_first_variable);

    return 1;
}

int CompileTexFile(const char* filename)
{
    // system("cd ./TexFiles");

    char cmd1[100] = "cd ./TexFiles; pdflatex ./";
    strcat(cmd1, filename);
    strcat(cmd1, ".tex");

    system(cmd1);

    char cmd2[200] = "git add ./TexFiles/";
    strcat(cmd2, filename);
    strcat(cmd2, ".pdf; git commit ./TexFiles/");
    strcat(cmd2, filename);
    strcat(cmd2, ".pdf -m \"pdf auto-commit\"");

    system(cmd2);

    return 1;
}

int CreateTexFile(const char* filename, ExpressionParams* params)
{
    ASSERT(filename != nullptr);
    ASSERT(params   != nullptr);

    char tex_file_path[100] = "./TexFiles/";
    strcat(tex_file_path, filename);
    strcat(tex_file_path, ".tex");

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
