#include "./Config.h"
#include <stdlib.h>
#include <math.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Tree.h"
#include "./TreeSimplifyFunctions.h"

Node* CalculateConstantSubtrees(Node* node)
{
    if (!node) return nullptr;

    ASSERT(node != nullptr);

    if (node->val_type == OP_TYPE)
    {
        Node* left_temp =  CalculateConstantSubtrees(node->left);
        Node* right_temp = CalculateConstantSubtrees(node->right);

        if ((left_temp->val_type == NUM_TYPE) && (right_temp->val_type == NUM_TYPE))
        {
            switch (node->op_val)
            {
                case OP_ADD:
                {
                    node->num_val = left_temp->num_val + right_temp->num_val;
                    break;
                }

                case OP_SUB:
                {
                    node->num_val = left_temp->num_val - right_temp->num_val;
                    break;
                }

                case OP_MUL:
                {
                    node->num_val = left_temp->num_val * right_temp->num_val;
                    break;
                }

                case OP_DIV:
                {
                    node->num_val = left_temp->num_val / right_temp->num_val;

                    // if (node->num_val == nan(""))
                    // {
                    //     fprintf(stderr, "Деление на нуль!\n");
                    //     abort();
                    //     return nullptr;
                    // }

                    break;
                }

                case OP_SQRT:
                {
                    node->num_val = sqrt(right_temp->num_val);
                    break;
                }

                // case OP_RT:
                // {
                //     node->num_val = pow(left_temp->num_val, right_temp->num_val);
                //     break;
                // }

                case OP_DEG:
                {
                    node->num_val = pow(left_temp->num_val, right_temp->num_val);
                    break;
                }

                case OP_EXP:
                {
                    node->num_val = exp(right_temp->num_val);
                    break;
                }

                case OP_LOG:
                {
                    node->num_val = log(right_temp->num_val);
                    break;
                }

                case OP_LN :
                {
                    node->num_val = log(right_temp->num_val);
                    break;
                }

                case OP_SIN:
                {
                    node->num_val = sin(right_temp->num_val);
                    break;
                }

                case OP_COS:
                {
                    node->num_val = cos(right_temp->num_val);
                    break;
                }

                case OP_TG :
                {
                    node->num_val = tan(right_temp->num_val);
                    break;
                }

                case OP_CTG:
                {
                    node->num_val = 1 / tan(right_temp->num_val);
                    break;
                }

                case OP_CH :
                {
                    node->num_val = cosh(right_temp->num_val);
                    break;
                }

                case OP_SH :
                {
                    node->num_val = sinh(right_temp->num_val);
                    break;
                }

                case OP_ARCSIN:
                {
                    node->num_val = asin(right_temp->num_val);
                    break;
                }

                case OP_ARCCOS:
                {
                    node->num_val = acos(right_temp->num_val);
                    break;
                }

                case OP_ARCTG:
                {
                    node->num_val = atan(right_temp->num_val);
                    break;
                }

                case OP_ARCCTG:
                {
                    node->num_val = M_PI / 2 - atan(right_temp->num_val);
                    break;
                }

                default:
                {
                    return nullptr;
                }
            }

            node->op_val = NULL_OP;
            node->val_type = NUM_TYPE;

            // free(node->left);
            // free(node->right);
            // node->left = nullptr;
            // node->right = nullptr;
            // NodeDtor(node->left);
            // NodeDtor(node->right);
        }
    }

    return node;
}

#define IS_OP(  op_name ) (node->op_val == op_name       )
#define IS_NUM( node_ptr) (node_ptr->val_type == NUM_TYPE)
#define IS_ZERO(node_ptr) (node_ptr->num_val == 0        )
#define IS_ONE( node_ptr) (node_ptr->num_val == 1        )

Node* DestroyNeutralTreeElements(Node* node)
{
    if(!node) return nullptr;

    if (node->val_type == OP_TYPE)
    {
        // Node* temp_node = (Node*) calloc(1, sizeof(Node));

        // Node* left  = nullptr;
        // Node* right = nullptr;

        if (node->left)  node->left  = DestroyNeutralTreeElements(node->left);
        if (node->right) node->right = DestroyNeutralTreeElements(node->right);

        ASSERT(node->left  != nullptr);
        ASSERT(node->right != nullptr);

        if (IS_OP(OP_ADD))
        {
            if (IS_NUM(node->left) && IS_ZERO(node->left))
            {
                Node* new_node = CR;
                NodeDtor(node);
                node = new_node;

                return node;
            }

            else if (IS_NUM(node->right) && IS_ZERO(node->right))
            {
                Node* new_node = CL;
                NodeDtor(node);
                node = new_node;

                return node;
            }
        }

        if (IS_OP(OP_SUB))
        {
            if (IS_NUM(node->right) && IS_ZERO(node->right))
            {
                Node* new_node = CL;
                NodeDtor(node);
                node = new_node;

                return node;
            }
        }

        if (IS_OP(OP_MUL))
        {
            if ((IS_NUM(node->left)  && IS_ZERO(node->left )) ||
                (IS_NUM(node->right) && IS_ZERO(node->right))   )
            {
                NodeDtor(node);
                node = CREATE_NUM(0);

                return node;
            }

            else if (IS_NUM(node->left) && IS_ONE(node->left))
            {
                Node* new_node = CR;
                NodeDtor(node);
                node = new_node;

                return node;
            }

            else if (IS_NUM(node->right) && IS_ONE(node->right))
            {
                Node* new_node = CL;
                NodeDtor(node);
                node = new_node;

                return node;
            }

            if (IS_NUM(node->right) && !IS_NUM(node->left))
            {
                Node* temp_node = node->left;

                node->left  = node->right;
                node->right = temp_node;

                return node;
            }
        }

        if (IS_OP(OP_DIV))
        {
            if (IS_NUM(node->left) && IS_ZERO(node->left))
            {
                NodeDtor(node);
                node = CREATE_NUM(0);

                return node;
            }

            if (IS_NUM(node->right) && IS_ONE(node->right))
            {
                Node* new_node = CL;
                NodeDtor(node);
                node = new_node;

                return node;
            }
        }

        if (IS_OP(OP_DEG))
        {
            if (IS_NUM(node->right))
            {
                if (IS_ZERO(node->right))
                {
                    NodeDtor(node);
                    node = CREATE_NUM(1);

                    return node;
                }

                else if (IS_ONE(node->right))
                {
                    Node* new_node = CL;
                    NodeDtor(node);
                    node = new_node;

                    return node;
                }
            }

            else if (IS_NUM(node->left) && IS_ONE(node->left))
            {
                NodeDtor(node);
                node = CREATE_NUM(1);

                return node;
            }
        }

        if (IS_OP(OP_EXP))
        {
            if (IS_NUM(node->right) &&
                IS_ZERO(node->right))
            {
                NodeDtor(node);
                node = CREATE_NUM(1);

                return node;
            }
        }

        return node;
    }

    return node;
}

#undef IS_OP
#undef IS_NUM
#undef IS_ZERO
#undef IS_ONE

Node* SimplifyTree(Node** node)
{
    if(!*node) return nullptr;

    ASSERT(*node != nullptr);

    *node = CalculateConstantSubtrees(*node);

    if (*node)
        *node = DestroyNeutralTreeElements(*node);

    return *node;
}
