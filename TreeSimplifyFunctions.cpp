#include "./Config.h"
#include <stdlib.h>
#include <math.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Tree.h"
#include "./TreeSimplifyFunctions.h"

double CalculateOperatorNode(enum Operators op_val, double left_num_val, double right_num_val)
{
    switch (op_val)
    {
        case OP_ADD:
            return left_num_val + right_num_val;

        case OP_SUB:
            return left_num_val - right_num_val;

        case OP_MUL:
            return left_num_val * right_num_val;

        case OP_DIV:
            return  left_num_val / right_num_val;

            // if (node->num_val == nan(""))
            // {
            //     fprintf(stderr, "Деление на нуль!\n");
            //     abort();
            //     return nullptr;
            // }

        case OP_SQRT:
            return sqrt(right_num_val);

        // case OP_RT:
        // {
        //     node->num_val = pow(left_num_val, right_num_val);
        //     break;
        // }

        case OP_DEG:
            return pow(left_num_val, right_num_val);

        case OP_EXP:
            return exp(right_num_val);

        // case OP_LOG:
        //     return log(right_num_val);

        case OP_LN :
            return log(right_num_val);

        case OP_SIN:
            return sin(right_num_val);

        case OP_COS:
            return cos(right_num_val);

        case OP_TG :
            return tan(right_num_val);

        case OP_CTG:
            return 1 / tan(right_num_val);

        case OP_CH :
            return cosh(right_num_val);

        case OP_SH :
            return sinh(right_num_val);

        case OP_ARCSIN:
            return asin(right_num_val);

        case OP_ARCCOS:
            return acos(right_num_val);

        case OP_ARCTG:
            return atan(right_num_val);

        case OP_ARCCTG:
            return  M_PI / 2 - atan(right_num_val);

        default:
            return WRONG_CALCULATED_NODE;
    }
}

Node* CalculateConstantSubtrees(Node** node)
{
    ASSERT(node != nullptr);

    if (*node == nullptr) return nullptr;

    // VERIFY_NODE(*node);

    ASSERT((*node) != nullptr);

    if ((*node)->val_type == OP_TYPE)
    {
        Node* left_temp  = CalculateConstantSubtrees(&((*node)->left));
        Node* right_temp = CalculateConstantSubtrees(&((*node)->right));

        if (left_temp->val_type == NUM_TYPE)
        {
            if (right_temp->val_type == NUM_TYPE)
            {
                (*node)->num_val  = CalculateOperatorNode((*node)->op_val, left_temp->num_val, right_temp->num_val);
                (*node)->val_type = NUM_TYPE;
                (*node)->op_val   = NULL_OP;

                // free((*node)->left);
                // free((*node)->right);
                // (*node)->left = nullptr;
                // (*node)->right = nullptr;
                NodeDtor(&((*node)->left));
                NodeDtor(&((*node)->right));
            }

//             else if ((*node)->op_val == OP_MUL &&
//                       right_temp->val_type == OP_TYPE && right_temp->op_val == OP_MUL)
//             {
//                 if (right_temp->left->val_type == NUM_TYPE)
//                 {
//                     (*node)->left->num_val *= right_temp->left->num_val;
//                 }
//
//                 else if (right_temp->right->val_type == NUM_TYPE)
//                 {
//                     (*node)->left->num_val *= right_temp->right->num_val;
//                 }
//             }
        }
    }

    // VERIFY_NODE(*node);

    return *node;
}

int InsertConstsInExpression(Node* node, const ExpressionParams* params)
{
    if (node == nullptr) return 1;

    if (node->left) InsertConstsInExpression(node->left, params);

    if (node->right) InsertConstsInExpression(node->right, params);

    if (node->val_type == VAR_TYPE)
    {
        int const_index = FindVarIndex(node->var_val, params->vars, NUM_OF_CONSTANTS);

        if (const_index != NO_VAR_NAME)
        {
            free((void*) node->var_val);
            node->var_val = nullptr;

            node->val_type = NUM_TYPE;
            node->num_val  = params->vars[const_index].value;
        }
    }

    return 1;
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

#define IS_OP(  op_name ) ((*node)->op_val == op_name    )
#define IS_NUM( node_ptr) (node_ptr->val_type == NUM_TYPE)
#define IS_ZERO(node_ptr) (node_ptr->num_val == 0        )
#define IS_ONE( node_ptr) (node_ptr->num_val == 1        )

Node* DestroyNeutralTreeElements(Node** node)
{
    ASSERT(node != nullptr)

    if (*node == nullptr) return nullptr;

    // VERIFY_NODE(*node);

    if ((*node)->val_type == OP_TYPE)
    {
        // Node* temp_node = (Node*) calloc(1, sizeof(Node));

        // Node* left  = nullptr;
        // Node* right = nullptr;

        if ((*node)->left)  (*node)->left  = DestroyNeutralTreeElements(&((*node)->left));
        if ((*node)->right) (*node)->right = DestroyNeutralTreeElements(&((*node)->right));

        ASSERT((*node)->left  != nullptr);
        ASSERT((*node)->right != nullptr);

        if (IS_OP(OP_ADD))
        {
            if (IS_NUM((*node)->left) && IS_ZERO((*node)->left))
            {
//                 Node* new_node = CR_PTR;
//                 new_node->prev = (*node)->prev;
//
//                 NodeDtor(node);
//                 *node = new_node;
//                 return *node;

                return ReplaceNode(node, CR_PTR);
            }

            if (IS_NUM((*node)->right) && IS_ZERO((*node)->right))
            {
//                 Node* new_node = CL_PTR;
//                 new_node->prev = (*node)->prev;
//
//                 NodeDtor(node);
//                 *node = new_node;
//
//                 return *node;

                return ReplaceNode(node, CL_PTR);
            }
        }

        if (IS_OP(OP_SUB))
        {
            if (IS_NUM((*node)->right) && IS_ZERO((*node)->right))
            {
//                 Node* new_node = CL_PTR;
//                 new_node->prev = (*node)->prev;
//
//                 NodeDtor(node);
//                 *node = new_node;
//
//                 return *node;

                return ReplaceNode(node, CL_PTR);
            }

            if (IS_NUM((*node)->left) && IS_ZERO((*node)->left))
            {
//                 Node* new_node = MUL(CREATE_NUM(-1), CR_PTR);
//                 new_node->prev = (*node)->prev;
//
//                 NodeDtor(node);
//                 *node = new_node;
//
//                 return *node;

                return ReplaceNode(node, MUL(CREATE_NUM(-1), CR_PTR));
            }
        }

        if (IS_OP(OP_MUL))
        {
            if ((IS_NUM((*node)->left)  && IS_ZERO((*node)->left )) ||
                (IS_NUM((*node)->right) && IS_ZERO((*node)->right))   )
            {
//                 Node* new_node = CREATE_NUM(0);
//                 new_node->prev = (*node)->prev;
//
//                 NodeDtor(node);
//                 *node = new_node;
//
//                 return *node;

                return ReplaceNode(node, CREATE_NUM(0));
            }

            if (IS_NUM((*node)->left) && IS_ONE((*node)->left))
            {
//                 Node* new_node = CR_PTR;
//                 new_node->prev = (*node)->prev;
//
//                 NodeDtor(node);
//                 *node = new_node;
//
//                 return *node;

                return ReplaceNode(node, CR_PTR);
            }

            if (IS_NUM((*node)->right) && IS_ONE((*node)->right))
            {
//                 Node* new_node = CL_PTR;
//                 new_node->prev = (*node)->prev;
//
//                 NodeDtor(node);
//                 *node = new_node;
//
//                 return *node;

                return ReplaceNode(node, CL_PTR);
            }

            if (IS_NUM((*node)->right) && !IS_NUM((*node)->left))
            {
//                 Node* temp_node = (*node)->left;
//
//                 (*node)->left  = (*node)->right;
//                 (*node)->right = temp_node;
//
//                 return *node;

                SwapNodes(&((*node)->left), &((*node)->right));

                return *node;
            }

//             if ((*node)->right->val_type == OP_TYPE
//                 && ((*node)->right->op_val == OP_MUL || (*node)->right->op_val == OP_DIV)
//                 && IS_NUM((*node)->left))
//             {
//                 if (IS_NUM((*node)->right->left))
//                 {
//                     (*node)->left->num_val *= (*node)->right->left->num_val;
//
//                     Node* new_right_node = CopyNode((*node)->right->right);
//
//                     // fprintf(stdout, "\nHERE: %lf\n\n", (*node)->left->num_val);
//
//                     ReplaceNode(&((*node)->right), new_right_node);
//                 }
//
//                 if (IS_NUM((*node)->right->right) && (*node)->right->op_val == OP_MUL)
//                 {
//                     (*node)->left->num_val *= (*node)->right->right->num_val;
//
//                     Node* new_right_node = CopyNode((*node)->right->right);
//
//                     ReplaceNode(&((*node)->right), new_right_node);
//                 }
//
//                 return *node;
//             }
        }

        if (IS_OP(OP_DIV))
        {
            if (IS_NUM((*node)->left) && IS_ZERO((*node)->left))
            {
//                 Node* new_node = CREATE_NUM(0);
//                 new_node->prev = (*node)->prev;
//
//                 NodeDtor(node);
//                 *node = new_node;
//
//                 return *node;

                return ReplaceNode(node, CREATE_NUM(0));
            }

            if (IS_NUM((*node)->right) && IS_ONE((*node)->right))
            {
//                 Node* new_node = CL_PTR;
//                 new_node->prev = (*node)->prev;
//
//                 NodeDtor(node);
//                 *node = new_node;
//
//                 return *node;

                return ReplaceNode(node, CL_PTR);
            }

//             if (IS_NUM(node->left) && IS_ONE(node->left) &&
//                 (node->prev != nullptr && node->prev->val_type == OP_TYPE && node->prev->op_val == OP_MUL))
//             {
//                 if (node == node->prev->left)
//                 {
//
//                 }
//
//                 else if (node == node->prev->right)
//                 {
//
//                 }
//
//                 return node;
//             }
        }

        if (IS_OP(OP_DEG))
        {
            if (IS_NUM((*node)->right))
            {
                if (IS_ZERO((*node)->right))
                {
//                     Node* new_node = CREATE_NUM(1);
//                     new_node->prev = (*node)->prev;
//
//                     NodeDtor(node);
//                     *node = new_node;
//
//                     return *node;

                    return ReplaceNode(node, CREATE_NUM(1));
                }

                if (IS_ONE((*node)->right))
                {
//                     Node* new_node = CL_PTR;
//                     new_node->prev = (*node)->prev;
//
//                     NodeDtor(node);
//                     *node = new_node;
//
//                     return *node;

                    return ReplaceNode(node, CL_PTR);
                }
            }

            else if (IS_NUM((*node)->left) && IS_ONE((*node)->left))
            {
//                 Node* new_node = CREATE_NUM(1);
//                 new_node->prev = (*node)->prev;
//
//                 NodeDtor(node);
//                 *node = new_node;
//
//                 return *node;

                return ReplaceNode(node, CREATE_NUM(1));
            }
        }

        if (IS_OP(OP_EXP))
        {
            if (IS_NUM((*node)->right) &&
                IS_ZERO((*node)->right))
            {
//                 Node* new_node = CREATE_NUM(1);
//                 new_node->prev = (*node)->prev;
//
//                 NodeDtor(node);
//                 *node = new_node;
//
//                 return *node;

                return ReplaceNode(node, CREATE_NUM(1));
            }
        }
    }

    // VERIFY_NODE(*node);

    return *node;
}

#undef IS_OP
#undef IS_NUM
#undef IS_ZERO
#undef IS_ONE

Node* SimplifyTree(Node** node)
{
    ASSERT(node != nullptr);

    VERIFY_NODE(*node);

    if(!(*node)) return nullptr;

    *node = CalculateConstantSubtrees(node);

    if (*node)
        *node = DestroyNeutralTreeElements(node);

    VERIFY_NODE(*node);

    // TreeInorderPrint(*node, stdout);

    if (*node) return *node;

    WarningMessage(__PRETTY_FUNCTION__, "NODE SIMPLIFIED WRONGLY");
    abort();

    return nullptr;
}
