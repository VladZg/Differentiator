
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

int Tex_page_width  = TEX_PAGE_WIDTH;
int Tex_page_height = TEX_PAGE_HEIGHT;
int N_graphs = 0;

int WriteHeadOfTexFile(FILE* tex_file)
{
    ASSERT(tex_file != nullptr)

    TEX_PRINT("\\documentclass{article}\n"
              "\\usepackage[left=15mm, top=20mm, right=15mm, bottom=20mm, papersize={%dcm,%dcm}]{geometry}\n"
              "\\usepackage{mathtools}\n"
              "\\usepackage{graphicx} \n"
              "\\usepackage{cmap}     \n"
              "\\usepackage{epstopdf} \n"
              "\\usepackage[usenames]{color}\n"
              "\\usepackage{transparent}\n"
              "\n"
              "\\begin{document}              \n"
              "\\author{Zagorodniuk Vladislav}\n",
              Tex_page_width, (Tex_page_height = (int) (Tex_page_width * TEX_PAGE_COEFF_BETWEEN_WIDTH_HEIGTH)));

    return 1;
}

int ShitSomeCringeIntroductionInTexFile(FILE* tex_file)
{
    ASSERT(tex_file != nullptr)

    TEX_PRINT("\\section{Introduction}\n"
              "\\[CrInGeCrInGe Production. Super cringe introduction here:\\]\n");

    return 1;
}

int ShitSomeCringeConclusionInTexFile(FILE* tex_file)
{
    ASSERT(tex_file != nullptr)

    TEX_PRINT("\\section{Conclusion}\n"
              "\\[Ultrar cringe conclusion here:\\]\n");

    return 1;
}

int WriteTailOfTexFile(FILE* tex_file)
{
    ASSERT(tex_file != nullptr)

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

    TEX_PRINT("\\textbf{Constants} (%d):    \n\n", NUM_OF_CONSTANTS);

    for (size_t const_i = 0; const_i < NUM_OF_CONSTANTS; const_i++)
        TEX_PRINT("\\textcolor{black}{%10s} = %lf\n\n", params->vars[const_i].name, params->vars[const_i].value);

    if (params->n_vars - NUM_OF_CONSTANTS)
        TEX_PRINT("\\textbf{Variables} (%d):    \n\n", (int) params->n_vars - NUM_OF_CONSTANTS);

    for (size_t var_i = NUM_OF_CONSTANTS; var_i < params->n_vars; var_i++)
        TEX_PRINT("\\textcolor{black}{%10s} = %lf\n\n", params->vars[var_i].name, params->vars[var_i].value);

    TEX_PRINT("\\textcolor{red}    {\\textbf{\\text{Parameters of exploration      }}}:      \n\n"
              "\\textcolor{yellow} {\\textsl{\\text{Number of differentiates       }}}: $%ld$\n\n"
              "\\textcolor{green}  {\\textsl{\\text{Macloren's accuracy            }}}: $%ld$\n\n"
              "\\textcolor{cyan}   {\\textsl{\\text{Tanget point                   }}}: $%lf$\n\n"
              "\\textcolor{blue}   {\\textsl{\\text{Delta coverage of tangent point}}}: $%lf$\n\n"
              "\\textcolor{magenta}{\\textsl{\\text{Graph diapasone                }}}: $%s $\n\n", params->n_differentiate, params->Makloren_accuracy, params->tangent_point, params->delta_coverage, params->graph_diapasone);

    return 1;
}


int FindFirstDerivationTex(Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    if (!expression) return 1;

    TEX_PRINT("Personally, I've always thought about first derivation of something like that function... Haven't you?\n\n"
              "But now, by using informatics and math skills I feel that I'm prepared enough to calculate it!\n\n");

    Node* diff1 = Differentiate(expression, tex_file, PRINT_STEPS_TEX_MODE);

    TEX_PRINT("Congratulations! \\textbf{The first derivation of the expression} is:\n\n");

    PrintExpressionAsFunction(diff1, params, "f${ '}$", tex_file);
    double diff1_value = CalculateTree(diff1, params);
    TEX_PRINT("\n\n");

    PrintParametersPoint(tex_file, params);
    TEX_PRINT(" it's value = %." FUNCTION_VALUE_IN_POINT_ACCURACY "lf\n\n", diff1_value);

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

    TEX_PRINT("\n\n");

    if (is_constants_in_tree)
    {
        TEX_PRINT("Firstly, let's insert all constants: ");

        InsertConstsInExpression(*expression, params);
        PrintExpressionAsFunction(*(params->expression), params, "f", tex_file);

        TEX_PRINT("\n\n");
    }

    TEX_PRINT("%s simplify this expression (if possible): ", (is_constants_in_tree ? "And" : "Firstly, let's"));

    *expression = SimplifyTree(expression);
    PrintExpressionAsFunction(*(params->expression), params, "f", tex_file);

    TEX_PRINT("\n\n");

    return 1;
}

int CalculateExpressionTex(Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    TEX_PRINT("\\paragraph{Calculation value of function in the point}\n");

    if (!expression) return 1;

    TEX_PRINT("BRITISH SCIENTISTS WERE SHOCKED, WHEN THEY COUNT IT!!!\n\n");
    PrintParametersPoint(tex_file, params);
    TEX_PRINT(" \\textbf{it's value} = %." FUNCTION_VALUE_IN_POINT_ACCURACY "lf\n\n", CalculateTree(expression, params));

    return 1;
}

int FindNDerivationTex(Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    TEX_PRINT("\\paragraph{Finding the %ld derivation}\n\n", params->n_differentiate);

    if (!expression) return 1;

    Node* n_diff = NDifferentiate(expression, params->n_differentiate, tex_file, PRINT_STEPS_TEX_MODE);

    char n_diff_function_name[10] = {};
    sprintf(n_diff_function_name, "$f^{(%ld)}$", params->n_differentiate);

    TEX_PRINT("\\textbf{Finally... The %ld derivation of the expression:}\n\n", params->n_differentiate);
    PrintExpressionAsFunction(n_diff, params, n_diff_function_name, tex_file);
    // WriteExpressionInTexFile(n_diff, tex_file, INPRINT_MODE);
    TEX_PRINT("\n\n");

    TEX_PRINT("BRITISH SCIENTISTS WERE SHOCKED AGAIN, "
                      "WHEN THEY COUNT THE %ld DERIVATION OF THIS EXPRESSION!!!\n\n", params->n_differentiate);
    PrintParametersPoint(tex_file, params);
    TEX_PRINT(" it's value = %." FUNCTION_VALUE_IN_POINT_ACCURACY "lf\n\n", CalculateTree(n_diff, params));

    int n_diff_tree_depth = TreeNumberOfNodes(n_diff);
    if (n_diff_tree_depth * ONE_NODE_TEX_PAGE_WIDTH > Tex_page_width) Tex_page_width = n_diff_tree_depth * ONE_NODE_TEX_PAGE_WIDTH;

    NodeDtor(&n_diff);

    return 1;
}

int FindFullDerivationTex(const Node* expression, const ExpressionParams* params, FILE* tex_file)
{
    ASSERT(params     != nullptr)
    ASSERT(tex_file   != nullptr)

    TEX_PRINT("\\paragraph{Finding partical derivations}\n\n");

    if (!expression) return 1;

    if (params->n_vars - NUM_OF_CONSTANTS > 0)
    {
        Node** part_diffs = (Node**) calloc(params->n_vars - NUM_OF_CONSTANTS, sizeof(Node*));
        Node* full_derivation = CREATE_NUM(0);
        Node* old_full_derivation = nullptr;

        for (int var_i = NUM_OF_CONSTANTS, part_diff_j = 0; var_i < (int) params->n_vars; var_i++, part_diff_j++)
        {
            part_diffs[part_diff_j] = FindPartialDerivation(expression, params->vars, var_i, params->n_vars, tex_file);

            TEX_PRINT("Partial derivation of the expression on the variable %s:\n\n", params->vars[var_i].name);
            TEX_PRINT("$\\frac{\\partial{f}}{\\partial{%s}}$ = ", params->vars[var_i].name);
            WriteExpressionInTexFile(part_diffs[part_diff_j], tex_file, INPRINT_MODE);
            TEX_PRINT("\n\n");

            old_full_derivation = CopyNode(full_derivation);
            NodeDtor(&full_derivation);

            // ShowTree(part_diffs[part_diff_j], SIMPLE_DUMP_MODE, 1);

            full_derivation = ADD(old_full_derivation, DEG(part_diffs[part_diff_j], CREATE_NUM(2)));

            PrintParametersPoint(tex_file, params);
            TEX_PRINT(" \n it's value = %." FUNCTION_VALUE_IN_POINT_ACCURACY "lf !!!\n\n\n", CalculateTree(part_diffs[part_diff_j], params));
        }

        old_full_derivation = CopyNode(full_derivation);
        NodeDtor(&full_derivation);
        full_derivation = SQRT(old_full_derivation);

        full_derivation = SimplifyTree(&full_derivation);

        TEX_PRINT("\\paragraph{Finding full derivation}\n\n");

        TEX_PRINT("Full derivation:\n\n");
        WriteExpressionInTexFile(full_derivation, tex_file, INPRINT_MODE);
        TEX_PRINT("\n\n");

        PrintParametersPoint(tex_file, params);
        TEX_PRINT(" it's value = %." FUNCTION_VALUE_IN_POINT_ACCURACY "lf !!!\n\n\n", CalculateTree(full_derivation, params));

        int full_derivation_tree_depth = TreeNumberOfNodes(full_derivation);
        if (full_derivation_tree_depth * ONE_NODE_TEX_PAGE_WIDTH > Tex_page_width) Tex_page_width = full_derivation_tree_depth * ONE_NODE_TEX_PAGE_WIDTH;

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

    TEX_PRINT("\\section{Exploration the expression%s}\n", (is_multiple_variables_in_function ? " as a function of multiple variables" : ""));

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

    TEX_PRINT("\\paragraph{Decomposing on Macloren's formula}\n\n");

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
    TEX_PRINT("\\textbf{Maklorens formula for $%s \\to %s_0 = %." NUMS_PRINT_ACCURACY "lf$}:\n\n"
            "f(%s) = ",
            params->vars[NUM_OF_CONSTANTS].name,  params->vars[NUM_OF_CONSTANTS].name,
            params->vars[NUM_OF_CONSTANTS].value, params->vars[NUM_OF_CONSTANTS].name);
    WriteExpressionInTexFile(Maklorens_formula, tex_file, INPRINT_MODE);

    TEX_PRINT("+o(");
    WriteExpressionInTexFile(remaining_member_of_decomposing, tex_file, INPRINT_MODE);
    TEX_PRINT(")\n\n");

    int Maklorens_formula_tree_depth = TreeNumberOfNodes(Maklorens_formula) + 5;
    if (Maklorens_formula_tree_depth * ONE_NODE_TEX_PAGE_WIDTH > Tex_page_width) Tex_page_width = Maklorens_formula_tree_depth * ONE_NODE_TEX_PAGE_WIDTH;

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

    TEX_PRINT("\\paragraph{Equations in the point}\n\n");

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

        TEX_PRINT("\\textbf{Tangent equation} in the point ${%s_0}$ = %." NUMS_PRINT_ACCURACY "lf:\n\n", params->vars[NUM_OF_CONSTANTS].name, params->tangent_point);
        TEX_PRINT("f(%s) = ", params->vars[NUM_OF_CONSTANTS].name);
        WriteExpressionInTexFile(tangent_equation, tex_file, INPRINT_MODE);
        TEX_PRINT("\n\n");

        Node* normal_equation = nullptr;

        if (tangent_coefficient != 0)
        {
            double normal_coefficient = -1 / tangent_coefficient;

            var_name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));
            memcpy(var_name, params->vars[NUM_OF_CONSTANTS].name, MAX_VAR_NAME_LEN);

            normal_equation = ADD(MUL(CREATE_NUM(normal_coefficient), SUB(CREATE_VAR(var_name), CREATE_NUM(params->tangent_point))),
                                        CREATE_NUM(function_of_the_first_var_value_in_point));
            normal_equation = SimplifyTree(&normal_equation);

            TEX_PRINT("\\textbf{Normal equation} in the point ${%s_0}$ = %." NUMS_PRINT_ACCURACY "lf: \n\n", params->vars[NUM_OF_CONSTANTS].name, params->tangent_point);
            TEX_PRINT("f(%s) = ", params->vars[NUM_OF_CONSTANTS].name);
            WriteExpressionInTexFile(normal_equation, tex_file, INPRINT_MODE);
            TEX_PRINT("\n\n");
        }

        else TEX_PRINT("\\textbf{Normal equation} in the point ${%s_0}$ = %." NUMS_PRINT_ACCURACY "lf: %s = %." NUMS_PRINT_ACCURACY "lf\n\n",
                       params->vars[NUM_OF_CONSTANTS].name, params->tangent_point, params->vars[NUM_OF_CONSTANTS].name, function_of_the_first_var_value_in_point);

        char functions_in_point_graph_diapasone[40] = {};
        sprintf(functions_in_point_graph_diapasone, "[%lf:%lf]", params->tangent_point - params->delta_coverage, params->tangent_point + params->delta_coverage);

        TEX_PRINT("Their graphs in $\\delta = %." FUNCTION_VALUE_IN_POINT_ACCURACY "lf$ coverage of the point $%s_0 = %lf$\n\n",
                  params->delta_coverage, params->vars[NUM_OF_CONSTANTS].name, params->tangent_point);

        Node* functions_for_graph_vizualization[3] = {function_of_the_first_variable, tangent_equation, normal_equation};
        CreateGraph(functions_for_graph_vizualization, 3, functions_in_point_graph_diapasone, tex_file);

        params->vars[NUM_OF_CONSTANTS].value = old_var_val;

        NodeDtor(&tangent_equation);
        NodeDtor(&normal_equation);
        NodeDtor(&function_of_the_first_var_diff);

        return 1;
    }

    TEX_PRINT("\\textbf{Tangent equation} in each point is f = 0\n\n"
              "\\textbf{Normal equation} can't be writen\n");

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

    TEX_PRINT("\\paragraph{Graphics}\n\n");

    int is_vars_in_function = IsVarsInTree(function_of_the_first_variable);

    TEX_PRINT("\\textbf{Graph} f");

    if (is_vars_in_function)
        TEX_PRINT("(%s)", params->vars[NUM_OF_CONSTANTS].name);

    TEX_PRINT(" = ");

    WriteExpressionInTexFile(function_of_the_first_variable, tex_file, INPRINT_MODE);
    TEX_PRINT(" on the diapasone ");

    if (is_vars_in_function)
        TEX_PRINT("$%s \\in $", params->vars[NUM_OF_CONSTANTS].name);

    TEX_PRINT("$%s$ :\n\n", params->graph_diapasone);

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
        TEX_PRINT("\\section{Exploration the expression as a function of the first variable}\n");

    if (!(*function_of_the_first_variable)) return 1;

    int is_vars_in_function = IsVarsInTree(*function_of_the_first_variable);
    *function_of_the_first_variable = SimplifyTree(function_of_the_first_variable);

    if (is_more_than_one_variable)
    {
        TEX_PRINT("Now let's consider the expression as a function of the first variable %s:\n"
                          "f(%s) = ", params->vars[NUM_OF_CONSTANTS].name, params->vars[NUM_OF_CONSTANTS].name);
        WriteExpressionInTexFile(*function_of_the_first_variable, tex_file, INPRINT_MODE);
        TEX_PRINT("\n\n");
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

    TEX_PRINT("\\section{Some basic knowledge about researching problem...}\n\n");
    TEX_PRINT("\n\nParameters and constants we use in this work:\n\n");
    IntroduceParameters(params, tex_file);

    //Печать начального выражения
    TEX_PRINT("So let's calculate smth with a given function: ");
    PrintExpressionAsFunction(*(params->expression), params, "f", tex_file);

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

    char cmd2[200] = "git add ./TexFiles/";
    strcat(cmd2, filename);
    strcat(cmd2, ".pdf *.png; git commit ./TexFiles/");
    strcat(cmd2, filename);
    strcat(cmd2, ".pdf *.png -m \"auto-commit\""); // ;git push

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

    if (Tex_page_width != TEX_PAGE_WIDTH) ChangeTexPapersize(tex_file);

    fclose(tex_file);

    CompileTexFile(filename);

    return 1;
}
