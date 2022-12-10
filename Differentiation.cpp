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
#include "./TexFunctions.h"
#include "./DiffDSL.h"
#include "./Differentiation.h"
#include "./TreeSimplifyFunctions.h"

Node* Diff(Node* node, FILE* tex_file, size_t* n_step, enum TexModes tex_mode)
{
    // return CopyNode(node);

    // if (!VERIFY_NODE(node)) return nullptr;

    ASSERT(tex_file != nullptr);
    ASSERT(n_step   != nullptr);

    if (!node) return nullptr;

    ASSERT(node != nullptr);

    if (tex_mode == PRINT_STEPS_TEX_MODE)
    {
        fprintf(tex_file, "%ld step:\n"
                        "finding a derivation of function:\n", *n_step);
        WriteExpressionInTexFile(node, tex_file);
    }

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

        default: return node;
    }

    // ShowTree(differed_node, FULL_FULL_DUMP_MODE, 0);

    // VERIFY_NODE(differed_node);
    differed_node = SimplifyTree(&differed_node);

    if (tex_mode == PRINT_STEPS_TEX_MODE)
    {
        fprintf(tex_file, "here it is:\n");
        WriteExpressionInTexFile(differed_node, tex_file);
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
            fprintf(tex_file, "Calculating the %ld derivation of the expression:\n\n", i+1);

        differed_nodes[i+1] = Differentiate(differed_nodes[i], tex_file, tex_mode);
        // differed_nodes[i+1] = CopyNode(differed_nodes[i]);

        // ShowTree(differed_nodes[i+1], SIMPLE_DUMP_MODE, 0);

        // if (is_tex_print)
        // {
        //     fprintf(tex_file, "Thus, the %ld derivation:\n", i+1);
        //     WriteExpressionInTexFile(differed_nodes[i+1], tex_file);
        // }
    }

    // TreeInorderPrint(differed_nodes[3], stdout);
    fprintf(stdout, "\n");

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

    // differed_node = SimplifyTree(&differed_node);

    return differed_node;
}
