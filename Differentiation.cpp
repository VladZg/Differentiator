#include "./Config.h"
#include <stdlib.h>
#include <cmath>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Stack/Stack.h"
#include "./Tree.h"
#include "./Differentiation.h"
#include "./DiffDSL.h"
#include "./ReadAndWriteFunctions.h"

Node* Diff(Node* node, FILE* tex_file, size_t* n_step)
{
    ASSERT(tex_file != nullptr);
    ASSERT(n_step != nullptr);

    if (!node) return nullptr;

    ASSERT(node != nullptr);

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
                    differed_node = ADD(MUL(CL, DR), MUL(CR, DL));
                    break;
                }

                case OP_DIV:
                {
                    differed_node = DIV(SUB(MUL(DL, CR), MUL(CL, DR)), DEG(CR, CREATE_NUM(2)));
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
                    differed_node = MUL(COS(node->right), DR);
                    break;
                }

                case OP_COS:
                {
                    differed_node = MUL(MUL(CREATE_NUM(-1), SIN(node->right)), DR);
                    break;
                }

                case OP_TG:
                {
                    differed_node = MUL(DIV(CREATE_NUM(1), DEG(COS(node->right), CREATE_NUM(2))), DR);
                    break;
                }

                case OP_CTG:
                {
                    differed_node = MUL(DIV(CREATE_NUM(-1), DEG(SIN(node->right), CREATE_NUM(2))), DR);
                    break;
                }

                case OP_SH:
                {
                    differed_node = MUL(CH(node->right), DR);
                    break;
                }

                case OP_CH:
                {
                    differed_node = MUL(SH(node->right), DR);
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

                default: return nullptr;
            }

            break;
        }

        default: return nullptr;
    }

    // differed_node = SimplifyTree(differed_node);

//     fprintf(tex_file, "%ld step:\n"
//                       "finding a derivation of funtion:\n", (*n_step)++);
//     WriteExpressionInTexFile(node, tex_file);
//
//     fprintf(tex_file, "here it is:\n");
//     WriteExpressionInTexFile(differed_node, tex_file);

    return differed_node;
}

Node* NDiff(Node* node, size_t n, FILE* tex_file)
{
    if (n == 0) return node;

    Node** differed_nodes = (Node**) calloc(n, sizeof(Node*));
    differed_nodes[0] = node;

    for (size_t i = 0; i < n; i++)
    {
        fprintf(tex_file, "Calculating the %ld derivation of the expression:\n\n"
                          "It's easy, just differentiating the %ld derivation\n\n", i+1, i);

        differed_nodes[i+1] = Differentiate(differed_nodes[i], tex_file);

        fprintf(tex_file, "Thus, the %ld derivation:\n", i+1);

        WriteExpressionInTexFile(differed_nodes[i+1], tex_file);
    }

    Node* n_differed_node = differed_nodes[n];

    for (size_t i = 1; i < n; i++) free(differed_nodes[i]);

    free(differed_nodes);

    return n_differed_node;
}

Node* Differentiate(Node* node, FILE* tex_file)
{
    node = SimplifyTree(node);

    size_t n_step = 1;

    Node* differed_node = Diff(node, tex_file, &n_step);

    differed_node = SimplifyTree(differed_node);

    return differed_node;
}
