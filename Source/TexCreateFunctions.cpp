
#include "../Config.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include "../Include/Constants.h"
#include "../Include/DefineColourConsts.h"
#include "../Libs/Stack/Include/Assert.h"
#include "../Libs/Stack/Include/Stack.h"
#include "../Include/CheckFile.h"
#include "../Include/Tree.h"
#include "../Include/TreeDump.h"
#include "../Include/ExpressionReader.h"
#include "../Include/Interactors.h"
#include "../Include/TexTreeTranslateFunctions.h"
#include "../Include/TreeSimplifyFunctions.h"
#include "../Include/MathFunctions.h"
#include "../Include/DiffDSL.h"
#include "../Include/TexCreateFunctions.h"

int Tex_page_width  = TEX_PAGE_WIDTH;
int Tex_page_height = TEX_PAGE_HEIGHT;
int N_graphs = 0;

int WriteHeadOfTexFile(FILE* tex_file)
{
    ASSERT(tex_file != nullptr)

    TEX_PRINT("\\documentclass{article}                                   \n"
              "\\usepackage[utf8]{inputenc}                               \n"
              "\\usepackage[left=15mm, top=20mm, right=15mm, bottom=20mm, papersize={%dcm,%dcm}]{geometry}\n"
              "\\usepackage{mathtools}                                    \n"
              "\\usepackage{graphicx}                                     \n"
              "\\usepackage{cmap}                                         \n"
              "\\usepackage{epstopdf}                                     \n"
              "\\usepackage[usenames]{color}                              \n"
              "\\usepackage{transparent}                                  \n"
              "\\usepackage[unicode, pdftex]{hyperref}                    \n"
              "\\title{\\textbf{\\LARGE{Expression exploration}}}         \n"
              "\\author{\\textbf{\\large{Jovanio Jorjinni (mojno verit)}}}\n"
              "\\date{\\textbf{\\today}}                                  \n"
              "                                                           \n"
              "\\begin{document}                                          \n"
              "\\maketitle                                                \n",
              Tex_page_width, (Tex_page_height = (int) (Tex_page_width * TEX_PAGE_COEFF_BETWEEN_WIDTH_HEIGTH)));

    return 1;
}

int ShitSomeCringeIntroductionInTexFile(FILE* tex_file)
{
    ASSERT(tex_file != nullptr)

    TEX_PRINT("\\section{\\LARGE{Introduction}}\n"
              "\\paragraph{\\rm{"
              "Worryingly, the importance of the derriviation is underestimated nowadays. "
              "In this extraordinary article I will show that the calculation and use of the derivative can be very interesting "
              "Our British scientists with Italian names living in America have spent about \\textbf{17 YEARS, 14 MONTHS, and 47 DAYS} "
              "studying the derivative problem and writing universal and unique differentiator. "
              "This article fully presents the results of their work!"
              "\\\\\\\\"
              "With this article, I want to restore the former greatness of mathematics and help the humanity, "
              "and what's more, most importantly, first-year students of the Moscow Institute of Physics and Technology!!! "
              "}}\n");

    return 1;
}

int ShitSomeCringeConclusionInTexFile(FILE* tex_file)
{
    ASSERT(tex_file != nullptr)

    TEX_PRINT("\\section{\\LARGE{Conclusion}}\n"
              "\\paragraph{\\rm{Thanks Ded for this amazing code experience and a lot of useful advice and care! "
              "Happy New Year!!! (Programming language is coming soon...)\\\\\\\\\\\\\\\\}}\n");

    return 1;
}

int WriteTailOfTexFile(FILE* tex_file)
{
    ASSERT(tex_file != nullptr)

    TEX_PRINT("\\href{https://www.youtube.com/watch?v=dQw4w9WgXcQ}{Repository of the author\\\\}\n"
              "\\href{https://cutt.ly/50kgC5L}{Follow for more!\\\\}\n"
              "\\href{https://inlnk.ru/me1EeG}{Or watch us on YouTube!}");

    TEX_PRINT("\\end{document}\n");

    return 1;
}

int PrintExpressionAsFunction(const Node* expression, const ExpressionParams* params, const char* function_name, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    TEX_PRINT("%s", function_name);
    if (params->n_vars - NUM_OF_CONSTANTS > 0)
    {
        TEX_PRINT("(");
        PrintAllVarNames(tex_file, params);
        fseek(tex_file, -2, SEEK_CUR);
        TEX_PRINT(")");
    }

    TEX_PRINT(" = ");

    WriteExpressionInTexFile(expression, tex_file, INPRINT_MODE);

    return 1;
}

int IntroduceParameters(const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    TEX_PRINT("\\textbf{\\large{Parameters and constants we use in this work (all data is qualified):}}\\\\\\\\");

    TEX_PRINT("\\textbf{Constants} (%d):    \\\\", NUM_OF_CONSTANTS);

    for (size_t const_i = 0; const_i < NUM_OF_CONSTANTS; const_i++)
        TEX_PRINT("\\indent{\\textcolor{black}{%10s} = %lf}\\\\", params->vars[const_i].name, params->vars[const_i].value);

    if (params->n_vars - NUM_OF_CONSTANTS)
        TEX_PRINT("\\\\\\textbf{Variables} (%d):    \\\\", (int) params->n_vars - NUM_OF_CONSTANTS);

    for (size_t var_i = NUM_OF_CONSTANTS; var_i < params->n_vars; var_i++)
        TEX_PRINT("\\indent{\\textcolor{black}{%10s} = %lf}\\\\", params->vars[var_i].name, params->vars[var_i].value);

    TEX_PRINT("\\\\"
              "\\textcolor{red}    {\\textbf{\\text{Parameters of exploration       }}}:       \\\\"
              "\\indent{\\textcolor{yellow} {\\textsl{\\text{Number of differentiates        }}} = $%ld$}\\\\"
              "\\indent{\\textcolor{green}  {\\textsl{\\text{Macloren's accuracy             }}} = $%ld$}\\\\"
              "\\indent{\\textcolor{cyan}   {\\textsl{\\text{Tanget point                    }}} = $%lf$}\\\\"
              "\\indent{\\textcolor{blue}   {\\textsl{\\text{Delta coverage of tangent point }}} = $%lf$}\\\\"
              "\\indent{\\textcolor{magenta}{\\textsl{\\text{Graph diapasone                 }}} = $%s $}\\\\"
              "\\\\",
               params->n_differentiate, params->Makloren_accuracy, params->tangent_point, params->delta_coverage, params->graph_diapasone);

    return 1;
}


int FindFirstDerivationTex(Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!expression) return 1;

    TEX_PRINT("\\paragraph{\\Large{- Finding the first derivation of function}\\\\\\\\}\n");

    TEX_PRINT("\\text{\\large{Personally, I've always thought about first derivation of something like that function... Haven't you?}}\\\\\n"
              "\\text{\\large{But now, by using informatics and math skills I feel that I'm prepared enough to calculate it!}}\\\\\\\\\n");

    Node* diff1 = Differentiate(expression, tex_file, PRINT_STEPS_TEX_MODE);

    TEX_PRINT("\\text{\\large{Congratulations! \\textbf{The first derivation of the expression} is:}}\\\\\\\\\\text{\\large{\n");

    PrintExpressionAsFunction(diff1, params, "f${ '}$", tex_file);
    double diff1_value = CalculateTree(diff1, params);
    TEX_PRINT("}}\\\\\\\\\n");

    PrintParametersPoint(tex_file, params);
    TEX_PRINT("\\textbf{ it's value = %." FUNCTION_VALUE_IN_POINT_ACCURACY "lf}\\\\\n", diff1_value);

    NodeDtor(&diff1);

    return 1;
}

int SimplifyExpressionTex(Node** expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)
    ASSERT(expression != nullptr)

    if (!expression) return 1;

    int is_constants_in_tree = IsConstsInTree(*expression, params->vars);

    if (is_constants_in_tree)
    {
        TEX_PRINT("\\\\\\\\");

        TEX_PRINT("\\textbf{\\large{Firstly, let's insert all constants:}}\\\\\\\\\\indent{\\large{\n");

        InsertConstsInExpression(*expression, params);
        PrintExpressionAsFunction(*(params->expression), params, "f", tex_file);

        TEX_PRINT("}\\\\}\n\\\\\\\\");
    }

    int old_num_of_nodes = TreeNumberOfNodes(*expression);
    *expression = SimplifyTree(expression);
    int new_number_of_nodes = TreeNumberOfNodes(*expression);

    if (old_num_of_nodes != new_number_of_nodes)
    {
        if (!is_constants_in_tree) TEX_PRINT("\\\\\\\\");

        TEX_PRINT("\\textbf{\\large{%s simplify this expression:}}\\\\\\\\\\indent{\\large{\n", (is_constants_in_tree ? "And" : "Firstly, let's"));
        PrintExpressionAsFunction(*(params->expression), params, "f", tex_file);
        TEX_PRINT("}\\\\}\n");
    }

    return 1;
}

int CalculateExpressionTex(Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    TEX_PRINT("\\paragraph{\\Large{- Calculation a value of function in the point\\\\\\\\}}\n");

    if (!expression) return 1;

    TEX_PRINT("BRITISH SCIENTISTS WERE SHOCKED, WHEN THEY COUNT IT!!!\\\\\n\\indent{\\text{\\large{\n");
    PrintParametersPoint(tex_file, params);
    TEX_PRINT(" expression's value = %." FUNCTION_VALUE_IN_POINT_ACCURACY "lf}}}\n", CalculateTree(expression, params));

    return 1;
}

int FindNDerivationTex(Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    TEX_PRINT("\\paragraph{\\Large{- Finding the %ld derivation\\\\\\\\}}\n", params->n_differentiate);

    if (!expression) return 1;

    Node* n_diff = NDifferentiate(expression, params->n_differentiate, tex_file, PRINT_STEPS_TEX_MODE);

    char n_diff_function_name[10] = {};
    sprintf(n_diff_function_name, "$f^{(%ld)}$", params->n_differentiate);

    TEX_PRINT("\\textbf{\\large{Finally... The %ld derivation of the expression:}}\\\\\\\\\\indent{\n", params->n_differentiate);
    PrintExpressionAsFunction(n_diff, params, n_diff_function_name, tex_file);
    // WriteExpressionInTexFile(n_diff, tex_file, INPRINT_MODE);
    TEX_PRINT("}\\\\\\\\\n");

    TEX_PRINT("BRITISH SCIENTISTS WERE SHOCKED AGAIN, "
              "BECAUSE THEY COUNT THE %ld DERIVATION OF THIS FUNCTION!!!\\\\\\indent{\\\\\\text{\\large{\n", params->n_differentiate);
    PrintParametersPoint(tex_file, params);
    TEX_PRINT("\\textbf{ it's value = %." FUNCTION_VALUE_IN_POINT_ACCURACY "lf}}}\\\\\n", CalculateTree(n_diff, params));

    #ifdef TEX_PAGE_AUTORESIZE
        int n_diff_tree_depth = TreeNumberOfNodes(n_diff);
        if (n_diff_tree_depth * ONE_NODE_TEX_PAGE_WIDTH > Tex_page_width) Tex_page_width = n_diff_tree_depth * ONE_NODE_TEX_PAGE_WIDTH;
    #endif

    NodeDtor(&n_diff);

    return 1;
}

int FindFullDerivationTex(const Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    TEX_PRINT("\\paragraph{\\Large{- Finding partical derivations}\\\\\\\\}\n\n");

    if (!expression) return 1;

    if (params->n_vars - NUM_OF_CONSTANTS > 0)
    {
        Node** part_diffs = (Node**) calloc(params->n_vars - NUM_OF_CONSTANTS, sizeof(Node*));
        Node* full_derivation = CREATE_NUM(0);
        Node* old_full_derivation = nullptr;

        for (int var_i = NUM_OF_CONSTANTS, part_diff_j = 0; var_i < (int) params->n_vars; var_i++, part_diff_j++)
        {
            part_diffs[part_diff_j] = FindPartialDerivation(expression, params->vars, var_i, params->n_vars, tex_file);

            TEX_PRINT("\\text{\\large{Partical derivation of the expression on the variable \\textbf{%s}:}}\\\\\\\\\n", params->vars[var_i].name);
            TEX_PRINT("\\text{\\large{$\\frac{\\partial{f}}{\\partial{%s}}$ = ", params->vars[var_i].name);
            WriteExpressionInTexFile(part_diffs[part_diff_j], tex_file, INPRINT_MODE);
            TEX_PRINT("}}\\\\\\\\\n");

            old_full_derivation = CopyNode(full_derivation);
            NodeDtor(&full_derivation);

            // ShowTree(part_diffs[part_diff_j], SIMPLE_DUMP_MODE, 1);

            full_derivation = ADD(old_full_derivation, DEG(part_diffs[part_diff_j], CREATE_NUM(2)));

            PrintParametersPoint(tex_file, params);
            TEX_PRINT(" \n\\textbf{ it's value = %." FUNCTION_VALUE_IN_POINT_ACCURACY "lf} !!!\\\\\\\\\\\\\n", CalculateTree(part_diffs[part_diff_j], params));
        }

        old_full_derivation = CopyNode(full_derivation);
        NodeDtor(&full_derivation);
        full_derivation = SQRT(old_full_derivation);

        full_derivation = SimplifyTree(&full_derivation);

        TEX_PRINT("\\paragraph{\\Large{- Finding full derivation}\\\\\\\\}\n\n");

        TEX_PRINT("\\textbf{\\large{Full derivation:}}\\\\\\\\\\indent{\n");
        WriteExpressionInTexFile(full_derivation, tex_file, INPRINT_MODE);
        TEX_PRINT("}\\\\\\\\\\indent{\n");

        PrintParametersPoint(tex_file, params);
        TEX_PRINT("\\textbf{ it's value = %." FUNCTION_VALUE_IN_POINT_ACCURACY "lf}} !!!}\n", CalculateTree(full_derivation, params));

        #ifdef TEX_PAGE_AUTORESIZE
            int full_derivation_tree_depth = TreeNumberOfNodes(full_derivation);
            if (full_derivation_tree_depth * ONE_NODE_TEX_PAGE_WIDTH > Tex_page_width) Tex_page_width = full_derivation_tree_depth * ONE_NODE_TEX_PAGE_WIDTH;
        #endif

        TEX_PRINT("\\\\\n");

        NodeDtor(&full_derivation);
        free(part_diffs);

        return 1;
    }

    TEX_PRINT("There is no variables to count partical derivations or full derivation, both of them are 0\n\n");

    return 0;
}

int ExploreFunctionOfManyVariablesTex(const Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!expression) return 1;

    int is_multiple_variables_in_function = IsVarsInTree(expression) && params->n_vars - NUM_OF_CONSTANTS > 1;

    TEX_PRINT("\\section{\\LARGE{Exploration of the expression%s}}\n", (is_multiple_variables_in_function ? " as a function of multiple variables" : ""));

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

    TEX_PRINT("\\paragraph{\\Large{- Decomposing on Macloren's formula\\\\\\\\}}\n");

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
    TEX_PRINT("\\textbf{\\large{First %ld members of Maklorens decomposition for $%s \\to %s_0 = %." NUMS_PRINT_ACCURACY "lf$}:}\\\\\\\\\\indent{\n"
            "f(%s) = ",
            params->Makloren_accuracy,
            params->vars[NUM_OF_CONSTANTS].name,  params->vars[NUM_OF_CONSTANTS].name,
            params->vars[NUM_OF_CONSTANTS].value, params->vars[NUM_OF_CONSTANTS].name);
    WriteExpressionInTexFile(Maklorens_formula, tex_file, INPRINT_MODE);

    TEX_PRINT("+o(");
    WriteExpressionInTexFile(remaining_member_of_decomposing, tex_file, INPRINT_MODE);
    TEX_PRINT(")\\\\\n");

    #ifdef TEX_PAGE_AUTORESIZE
        int Maklorens_formula_tree_depth = TreeNumberOfNodes(Maklorens_formula) + 5;
        if (Maklorens_formula_tree_depth * ONE_NODE_TEX_PAGE_WIDTH > Tex_page_width) Tex_page_width = Maklorens_formula_tree_depth * ONE_NODE_TEX_PAGE_WIDTH;
    #endif

    NodeDtor(&Maklorens_formula);
    NodeDtor(&remaining_member_of_decomposing);

    free(Maklorens_coefficients);

    return 0;
}

int EquationsInThePointTex(Node* function_of_the_first_variable, ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!function_of_the_first_variable) return 1;

    TEX_PRINT("\\paragraph{\\Large{- Equations in the point\\\\\\\\}}\n");

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

        TEX_PRINT("\\large{\\textbf{Tangent equation} in ${%s_0}$ = %." NUMS_PRINT_ACCURACY "lf: \\\\\\indent{", params->vars[NUM_OF_CONSTANTS].name, params->tangent_point);
        TEX_PRINT("f(%s) = ", params->vars[NUM_OF_CONSTANTS].name);
        WriteExpressionInTexFile(tangent_equation, tex_file, INPRINT_MODE);
        TEX_PRINT("}}\\\\\\\\\n");

        Node* normal_equation = nullptr;

        if (tangent_coefficient != 0)
        {
            double normal_coefficient = -1 / tangent_coefficient;

            var_name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));
            memcpy(var_name, params->vars[NUM_OF_CONSTANTS].name, MAX_VAR_NAME_LEN);

            normal_equation = ADD(MUL(CREATE_NUM(normal_coefficient), SUB(CREATE_VAR(var_name), CREATE_NUM(params->tangent_point))),
                                        CREATE_NUM(function_of_the_first_var_value_in_point));
            normal_equation = SimplifyTree(&normal_equation);

            TEX_PRINT("\\textbf{\\indent{Normal equation}} in ${%s_0}$ = %." FUNCTION_VALUE_IN_POINT_ACCURACY "lf: \\\\\\indent{\n", params->vars[NUM_OF_CONSTANTS].name, params->tangent_point);
            TEX_PRINT("f(%s) = ", params->vars[NUM_OF_CONSTANTS].name);
            WriteExpressionInTexFile(normal_equation, tex_file, INPRINT_MODE);
            TEX_PRINT("}\\\\\\\\\n");
        }

        else TEX_PRINT("\\textbf{Normal equation} in ${%s_0}$ = %." NUMS_PRINT_ACCURACY "lf: %s = %." NUMS_PRINT_ACCURACY "lf\n\n",
                       params->vars[NUM_OF_CONSTANTS].name, params->tangent_point, params->vars[NUM_OF_CONSTANTS].name, function_of_the_first_var_value_in_point);

        char functions_in_point_graph_diapasone[40] = {};
        sprintf(functions_in_point_graph_diapasone, "[%lf:%lf]", params->tangent_point - params->delta_coverage, params->tangent_point + params->delta_coverage);

        TEX_PRINT("\\indent{\\textbf{Their graphs} in $\\delta = %." NUMS_PRINT_ACCURACY "lf$ coverage of the point $%s_0 = %." NUMS_PRINT_ACCURACY "lf$:}\\\\\\\\\n",
                  params->delta_coverage, params->vars[NUM_OF_CONSTANTS].name, params->tangent_point);

        Node* functions_for_graph_vizualization[3] = {function_of_the_first_variable, tangent_equation, normal_equation};
        CreateGraph(functions_for_graph_vizualization, 3, functions_in_point_graph_diapasone, tex_file);

        params->vars[NUM_OF_CONSTANTS].value = old_var_val;

        NodeDtor(&tangent_equation);
        NodeDtor(&normal_equation);
        NodeDtor(&function_of_the_first_var_diff);

        return 1;
    }

    TEX_PRINT("\\textbf{Tangent equation} in each point is f = 0\\\\\\\\\n"
              "\\textbf{Normal equation} can't be writen\\\\\n");

    return 0;
}

int TranslateTreeToGnuplotFormula(const Node* node, char* formula)
{
    ASSERT(formula != nullptr)

    if (!node) return 1;

    strcat(formula, "(");

    if (node->left &&
        !(node->op_val == OP_EXP    ||
          node->op_val == OP_SQRT   ||
          node->op_val == OP_LOG    ||
          node->op_val == OP_LN     ||
          node->op_val == OP_SIN    ||
          node->op_val == OP_COS    ||
          node->op_val == OP_TG     ||
          node->op_val == OP_CTG    ||
          node->op_val == OP_SH     ||
          node->op_val == OP_CH     ||
          node->op_val == OP_ARCSIN ||
          node->op_val == OP_ARCCOS ||
          node->op_val == OP_ARCTG  ||
          node->op_val == OP_ARCCTG   ))

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
            case OP_LN    : { strcat(formula, "log"         ); break; }
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

int CreateGraph(Node** functions_of_one_variable, int n_graphs, const char* var_diapasone, FILE* tex_file)
{
    ASSERT(functions_of_one_variable!= nullptr)
    ASSERT(tex_file                 != nullptr)

    if (n_graphs <= 0) return 1;

    char filename[50] = "GnuGraph";
    char file_num[5] = {};
    sprintf(file_num, "%d", N_graphs++);
    strcat(filename, file_num);

    char full_text_filename[100] = {};
    sprintf(full_text_filename, "./TexFiles/%s.txt", filename);

    FILE* equation_graph_file = fopen(full_text_filename, "w");

    fprintf(equation_graph_file, "set terminal png\n"
                                "set output \"./TexFiles/%s.png\"\n"
                                "plot %s ", filename, var_diapasone);

    for (int graph_i = 0; graph_i < n_graphs; graph_i++)
    {
        char function_i_gnu_formula[300] = {};

        TranslateTreeToGnuplotFormula(functions_of_one_variable[graph_i], function_i_gnu_formula);

        fprintf(equation_graph_file, "%s, ", function_i_gnu_formula);
    }

    fseek(equation_graph_file, -2, SEEK_CUR);

    fprintf(equation_graph_file, "\n\n");

    fprintf(equation_graph_file, "set terminal eps\n"
                                 "set samples 1000\n"
                                 "set output \"./TexFiles/%s.eps\"\n"
                                 "replot\n", filename);

    fclose(equation_graph_file);

    char cmd[120] = {};
    sprintf(cmd, "gnuplot %s", full_text_filename);
    system(cmd);

    TEX_PRINT("\\includegraphics[scale=1.0]{%s.eps}\n\n", filename);

    return 1;
}

int GraphOfFunction(Node* function_of_the_first_variable, ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!function_of_the_first_variable) return 0;

    TEX_PRINT("\\paragraph{\\Large{- Graphics\\\\\\\\}}\n");

    int is_vars_in_function = IsVarsInTree(function_of_the_first_variable);

    TEX_PRINT("\\large{Graph of f");

    if (is_vars_in_function)
        TEX_PRINT("(%s)", params->vars[NUM_OF_CONSTANTS].name);

    TEX_PRINT(" = ");

    WriteExpressionInTexFile(function_of_the_first_variable, tex_file, INPRINT_MODE);
    TEX_PRINT(" on the diapasone ");

    if (is_vars_in_function)
        TEX_PRINT("$%s \\in $", params->vars[NUM_OF_CONSTANTS].name);

    TEX_PRINT("$%s$ :}\\\\\\\\\n", params->graph_diapasone);

//     char function_gnu_formula[300] = {};
//     TranslateTreeToGnuplotFormula(function_of_the_first_variable, function_gnu_formula);
//
//     FILE* graph_file = fopen("./TexFiles/GnuGraph.txt", "w");
//
//     fprintf(graph_file, "set terminal png\n"
//                         "set output \"./TexFiles/GnuGraph.png\"\n"
//                         "plot %s %s\n", params->graph_diapasone, function_gnu_formula);
//
//     fprintf(graph_file, "set terminal eps\n"
//                         "set output \"./TexFiles/GnuGraph.eps\"\n"
//                         "plot %s %s\n", params->graph_diapasone, function_gnu_formula);
//
//     fclose(graph_file);
//
//     system("gnuplot ./TexFiles/GnuGraph.txt");
//
//     // system("convert ./TexFiles/GnuGraph.png ./TexFiles/GnuGraph.eps");
//
//     TEX_PRINT("\\includegraphics[scale=1]{GnuGraph.eps}\n\n");

    CreateGraph(&function_of_the_first_variable, 1, params->graph_diapasone, tex_file);

    return 1;
}

int ExploreFunctionOfTheFirstVariableTex(Node** function_of_the_first_variable, ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    int is_more_than_one_variable = (int) params->n_vars - NUM_OF_CONSTANTS > 1;

    if (is_more_than_one_variable)
        TEX_PRINT("\\section{\\LARGE{Exploration of the expression as a function of the first variable}}\n");

    if (!(*function_of_the_first_variable)) return 1;

    int is_vars_in_function = IsVarsInTree(*function_of_the_first_variable);
    *function_of_the_first_variable = SimplifyTree(function_of_the_first_variable);

    if (is_more_than_one_variable)
    {
        TEX_PRINT("\\text{\\large{In this part of the article let's consider the expression as a function of the first variable %s:}}\\\\\\\\\n"
                  "\\text{\\Large{f(%s) = ", params->vars[NUM_OF_CONSTANTS].name, params->vars[NUM_OF_CONSTANTS].name);
        WriteExpressionInTexFile(*function_of_the_first_variable, tex_file, INPRINT_MODE);
        TEX_PRINT("}}\\\\\n");
    }

    // разложение по Тейлору (Маклорену)
    if (is_vars_in_function)
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

    TEX_PRINT("\\section{\\LARGE{Some basic knowledge about researching problem...}}");

    //параметры программы
    IntroduceParameters(params, tex_file);

    //Печать начального выражения
    TEX_PRINT("\\textbf{\\large{So let's calculate smth with a given function:}}\\\\\\\\\\indent{\\large{\n");
    PrintExpressionAsFunction(*(params->expression), params, "f", tex_file);
    TEX_PRINT("}\\\\}\n");

    //Упрощение выражения
    SimplifyExpressionTex(params->expression, params, tex_file);

    //Исследование выражения как функции многих переменных
    ExploreFunctionOfManyVariablesTex(*(params->expression), params, tex_file);

    //Переход к функции от одной переменной (первой)
    Node* function_of_the_first_variable = TranslateExpresiionToFunctionOfTheFirstVar(*(params->expression), params);

    //Исследование выражения, как функции одной переменной (первой)
    ExploreFunctionOfTheFirstVariableTex(&function_of_the_first_variable, params, tex_file);

    NodeDtor(&function_of_the_first_variable);

    //Заключение
    ShitSomeCringeConclusionInTexFile(tex_file);

    return 1;
}

int ChangeTexPapersize(FILE* tex_file)
{
    // fprintf(stdout, "\n\nHERE: %d\n\n", Tex_page_width);

    fseek(tex_file, 0, SEEK_SET);

    WriteHeadOfTexFile(tex_file);

    return 1;
}

int CompileTexFile(const char* filename)
{
    // system("cd ./TexFiles");

    char cmd1[100] = "cd ./TexFiles; pdflatex ./";
    strcat(cmd1, filename);
    strcat(cmd1, ".tex");

    system(cmd1);

    char full_filename[100] = {};
    // sprintf(full_filename, "./TexFiles/%s.pdf", filename);

    // if (!IsFileExist(full_filename))
    // {
        // char cmd2[200] = "git add ";
        // strcat(cmd2, full_filename);
        // strcat(cmd2, " *.png; git commit ./TexFiles/");
        // strcat(cmd2, filename);
        // strcat(cmd2, ".pdf *.png -m \"auto-commit\""); // ;git push
//
        // system(cmd2);
    // }

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

    #ifdef TEX_PAGE_AUTORESIZE
        if (Tex_page_width != TEX_PAGE_WIDTH) ChangeTexPapersize(tex_file);
    #endif

    fclose(tex_file);

    CompileTexFile(filename);

    return 1;
}
