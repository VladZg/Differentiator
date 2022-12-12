#include "./Config.h"
#include <stdlib.h>
#include <cmath>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Stack/Stack.h"
#include "./Tree.h"
#include "./TreeDump.h"
#include "./Interactors.h"
#include "./TexTreeTranslateFunctions.h"
#include "./DiffDSL.h"
#include "./MathFunctions.h"
#include "./TreeSimplifyFunctions.h"

static int factorial(int num)
{
    if (num == 0)
        return 1;

    return num * factorial(num-1);
}

const char* PickRandomTransition()
{
    return transitions[rand() % NUM_OF_TRANSITION_PHRASES];
}

double CalculateTree(Node* node, const ExpressionParams* params)
{
    ASSERT(node != nullptr)
    VERIFY_NODE(node);

    double ret_val = WRONG_CALCULATED_NODE;

    if (node->val_type == NUM_TYPE)
    {
        ret_val = node->num_val;
    }

    else if (node->val_type == OP_TYPE)
    {
        ASSERT(node->left  != nullptr)
        ASSERT(node->right != nullptr)

        ret_val = CalculateOperatorNode(node->op_val, CalculateTree(node->left, params), CalculateTree(node->right, params));
    }

    else if (node->val_type == VAR_TYPE)
    {
        int var_index = FindVarIndex(node->var_val, params->vars, params->n_vars);

        if(var_index != NO_VAR_NAME)
            ret_val = params->vars[var_index].value;
    }

    return ret_val;
}

Node* Diff(Node* node, FILE* tex_file, size_t* n_step, enum TexModes tex_mode)
{
    // return CopyNode(node);

    // if (!VERIFY_NODE(node)) return nullptr;

    ASSERT(tex_file != nullptr);
    ASSERT(n_step   != nullptr);

    if (!node) return nullptr;

    ASSERT(node != nullptr)

    // ShowTree(node, FULL_FULL_DUMP_MODE, 0);

    Node* differed_node = nullptr;

    switch(node->val_type)
    {
        case NUM_TYPE:
        {
            differed_node = CREATE_NUM(0);
            break;
        }

        case VAR_TYPE:
        {
            differed_node = CREATE_NUM(1);
            break;
        }

        case OP_TYPE:
        {
            switch(node->op_val)
            {
                case OP_ADD:
                {
                    differed_node = ADD(DL, DR);
                    break;
                }

                case OP_SUB:
                {
                    differed_node = SUB(DL, DR);
                    break;
                }

                case OP_MUL:
                {
                    differed_node = ADD(MUL(DL, CR), MUL(DR, CL));
                    break;
                }

                case OP_DIV:
                {
                    differed_node = DIV(SUB(MUL(DL, CR), MUL(DR, CL)), DEG(CR, CREATE_NUM(2)));
                    break;
                }

                case OP_EXP:
                {
                    differed_node = MUL(CN, DR);
                    break;
                }

                case OP_DEG:
                {
                    int is_vars_in_base   = IsVarsInTree(node->left);
                    int is_vars_in_degree = IsVarsInTree(node->right);

                    if (is_vars_in_base)
                    {
                        if (is_vars_in_degree)
                            differed_node = MUL(CN, ADD(MUL(DL, DIV(CR, CL)), MUL(DR, LN(CL))));

                        else
                            differed_node = MUL(MUL(CREATE_NUM(node->right->num_val), DEG(CL, CREATE_NUM(node->right->num_val - 1.0))), DL);
                    }

                    else
                    {
                        if (is_vars_in_degree)
                            differed_node = MUL(MUL(LN(CREATE_NUM(node->left->num_val)), CN), DR);

                        else
                            differed_node = CREATE_NUM(0);
                    }

                    break;
                }

                case OP_SQRT:
                {
                    differed_node = MUL(DIV(CREATE_NUM(0.5), SQRT(CR)), DR);
                    break;
                }

                // case OP_RT:
                // {
                //     differed_node = MUL(DIV(CREATE_NUM(0.5)), DR);
                //     break;
                // }

                // case OP_LOG:
                // {
                //     differed_node = DIV(DR, MUL(CREATE_NUM(log(node->left->num_val)), CR));
                //     break;
                // }

                case OP_LN:
                {
                    differed_node = MUL(DIV(CREATE_NUM(1), CR), DR);
                    break;
                }

                case OP_SIN:
                {
                    differed_node = MUL(COS(CR), DR);
                    break;
                }

                case OP_COS:
                {
                    differed_node = MUL(MUL(CREATE_NUM(-1), SIN(CR)), DR);
                    break;
                }

                case OP_TG:
                {
                    differed_node = MUL(DIV(CREATE_NUM(1), DEG(COS(CR), CREATE_NUM(2))), DR);
                    break;
                }

                case OP_CTG:
                {
                    differed_node = MUL(DIV(CREATE_NUM(-1), DEG(SIN(CR), CREATE_NUM(2))), DR);
                    break;
                }

                case OP_SH:
                {
                    differed_node = MUL(CH(CR), DR);
                    break;
                }

                case OP_CH:
                {
                    differed_node = MUL(SH(CR), DR);
                    break;
                }

                case OP_ARCSIN:
                {
                    differed_node = MUL(DIV(CREATE_NUM(1), SQRT(SUB(CREATE_NUM(1), DEG(CR, CREATE_NUM(2))))), DR);
                    // differed_node = CREATE_NUM(1337);
                    break;
                }

                case OP_ARCCOS:
                {
                    differed_node = MUL(DIV(CREATE_NUM(-1), SQRT(SUB(CREATE_NUM(1), DEG(CR, CREATE_NUM(2))))), DR);
                    // differed_node = CREATE_NUM(1337);
                    break;
                }

                case OP_ARCTG:
                {
                    differed_node = MUL(DIV(CREATE_NUM(1), ADD(CREATE_NUM(1), DEG(CR, CREATE_NUM(2)))), DR);
                    // differed_node = CREATE_NUM(1337);
                    break;
                }

                case OP_ARCCTG:
                {
                    differed_node = MUL(DIV(CREATE_NUM(-1), ADD(CREATE_NUM(1), DEG(CR, CREATE_NUM(2)))), DR);
                    // differed_node = CREATE_NUM(1337);
                    break;
                }

                default:
                {
                    fprintf(stderr, "NO SUCH OPERATION");
                    abort();
                }
            }

            break;
        }

        default:
        {
            WarningMessage(__PRETTY_FUNCTION__, "WRONG TYPE OF NODE");
            return nullptr;
        }
    }

    // ShowTree(differed_node, FULL_FULL_DUMP_MODE, 0);

    // VERIFY_NODE(differed_node);
    differed_node = SimplifyTree(&differed_node);

    if (tex_mode == PRINT_STEPS_TEX_MODE)
    {
        TEX_PRINT("%ld step.\n"
                        "finding a derivation of:\n\n", *n_step);
        WriteExpressionInTexFile(node, tex_file, INPRINT_MODE);
        TEX_PRINT("\n\n");

        TEX_PRINT("%s:\n", PickRandomTransition());
        TEX_PRINT("\n\n${(");
        TranslateTreeToTex(node, tex_file);
        TEX_PRINT(")^\\prime}$ {= ... = [top secret] = ... = \\\\ = }");
        WriteExpressionInTexFile(differed_node, tex_file, INPRINT_MODE);
        TEX_PRINT("\n\n");
    }

//     ShowTree(node, SIMPLE_DUMP_MODE, 0);
//     ShowTree(node, FULL_FULL_DUMP_MODE, 0);
//
//     ShowTree(differed_node, SIMPLE_DUMP_MODE, 0);
//     ShowTree(differed_node, FULL_FULL_DUMP_MODE, 0);

    (*n_step)++;

    return differed_node;
}

Node* NDifferentiate(Node* node, size_t n_diff, FILE* tex_file, enum TexModes tex_mode)
{
    Node* diff0 = CopyNode(node);

    if (n_diff == 0) return diff0;

    Node** differed_nodes = (Node**) calloc(n_diff + 1, sizeof(Node*));
    differed_nodes[0] = diff0;

    int is_tex_print = tex_mode == PRINT_STEPS_TEX_MODE;

    for (size_t i = 0; i < n_diff; i++)
    {
        if (is_tex_print)
            TEX_PRINT("Calculating the %ld derivation of the expression:\n\n", i+1);

        differed_nodes[i+1] = Differentiate(differed_nodes[i], tex_file, tex_mode);
        // differed_nodes[i+1] = CopyNode(differed_nodes[i]);

        // ShowTree(differed_nodes[i+1], SIMPLE_DUMP_MODE, 0);

        // if (is_tex_print)
        // {
        //     TEX_PRINT("Thus, the %ld derivation:\n", i+1);
        //     WriteExpressionInTexFile(differed_nodes[i+1], tex_file);
        // }
    }

    // TreeInorderPrint(differed_nodes[3], stdout);
    // fprintf(stdout, "\n");

    // ShowTree(differed_nodes[n_diff], FULL_FULL_DUMP_MODE, 0);

    Node* n_differed_node = CopyNode(differed_nodes[n_diff]);

    // ShowTree(differed_nodes[n_diff], FULL_FULL_DUMP_MODE, 0);
    // ShowTree(n_differed_node, FULL_FULL_DUMP_MODE, 0);

    // HERE(5)

    for (size_t i = 0; i <= n_diff; i++) NodeDtor(&differed_nodes[i]);

    free(differed_nodes);

    // ShowTree(n_differed_node, SIMPLE_DUMP_MODE, 1);

    return n_differed_node;
}

Node* Differentiate(Node* node, FILE* tex_file, enum TexModes tex_mode)
{
    node = SimplifyTree(&node);

    size_t n_step = 1;

    Node* differed_node = Diff(node, tex_file, &n_step, tex_mode);

    differed_node = SimplifyTree(&differed_node);

    return differed_node;
}

Node* FindPartialDerivation(const Node* node, const ExpressionVar* vars, int var_index, size_t n_vars, FILE* tex_file)
{
    ASSERT(node     != nullptr)
    ASSERT(vars     != nullptr)
    ASSERT(tex_file != nullptr)

    VERIFY_NODE(node);

    Node* one_var_expression = ReplaceVarsWithNumsExceptOne(CN, vars, var_index, n_vars);
    one_var_expression = SimplifyTree(&one_var_expression);

    // ShowTree(one_var_expression, SIMPLE_DUMP_MODE, 0);

    Node* part_diff = Differentiate(one_var_expression, tex_file, SKIP_STEPS_TEX_MODE);

    // ShowTree(part_diff, SIMPLE_DUMP_MODE, 1);

    NodeDtor(&one_var_expression);

    return part_diff;
}

Node* DecomposeByMacloren(Node* node, const ExpressionParams* params, double Maklorens_coefficients[], FILE* tex_file)
{
    ASSERT(node                   != nullptr)
    ASSERT(params                 != nullptr)
    ASSERT(Maklorens_coefficients != nullptr)

    VERIFY_NODE(node);

    Node* Maklorens_formula = CREATE_NUM(0);

    // Node** diff_members = (Node**) calloc(params->Makloren_accuracy, sizeof(Node*));
    // ASSERT(diff_members != nullptr)

    // diff_members[0] = CopyNode(node);
//     // double i_coefficient = 1;
//     // fprintf(stderr, "\ncoeff: %lf\n", i_coefficient);
//
//     for (int i_member = 1; i_member <= (int) params->Makloren_accuracy; i_member++)
//     {
//         diff_members[i_member] = Differentiate(diff_members[i_member - 1], tex_file, SKIP_STEPS_TEX_MODE);
//         // fprintf(stderr, "\ncoeff: %d\n", i_member);
// //         i_coefficient = CalculateTree(diff_members[i_member], params) / (double) factorial(i_member);
// //         fprintf(stderr, "\ncoeff: %lf\n", i_coefficient);
// //
// //         // Maclorens_formula = ADD(CREATE_NUM(i_coefficient), CopyNode(Maclorens_formula));
//     }
//
//     for (int i_member = 0; i_member <= (int) params->Makloren_accuracy; i_member++)
//     {
//         // fprintf(stderr, "\ncoeff: %d\n", i_member);
//         NodeDtor(&(diff_members[i_member]));
//     }

    for (int i_member = 0; i_member <= (int) params->Makloren_accuracy; i_member++)
    {
        Node* diff_i_member = NDifferentiate(node, i_member, tex_file, SKIP_STEPS_TEX_MODE);

        // ShowTree(diff_i_member, SIMPLE_DUMP_MODE, 1);

        Maklorens_coefficients[i_member] = CalculateTree(diff_i_member, params) / (double) factorial(i_member);
        // fprintf(stderr, "\ncoeff %d: %lf\n", i_member, Maklorens_coefficients[i_member]);

        NodeDtor(&diff_i_member);

        // if (Maklorens_coefficients[i_member] == 0) continue;

        VERIFY_NODE(Maklorens_formula);

        Node* old_formula = CopyNode(Maklorens_formula);

        NodeDtor(&Maklorens_formula);

        char* var_name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));
        memcpy(var_name, params->vars[NUM_OF_CONSTANTS].name, MAX_VAR_NAME_LEN);

        Node* var_node = DEG(SUB(CREATE_VAR(var_name), CREATE_NUM(params->vars[NUM_OF_CONSTANTS].value)), CREATE_NUM((double) i_member));

        Maklorens_formula = ADD(old_formula, MUL(CREATE_NUM(Maklorens_coefficients[i_member]), var_node));

        // Maklorens_formula = ADD(old_formula, var_node);
    }

    // free(diff_members);
    Maklorens_formula = SimplifyTree(&Maklorens_formula);

    return Maklorens_formula;
}
