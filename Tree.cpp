#include "./Config.h"
#include <stdlib.h>
#include <math.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Stack/Stack.h"
#include "./Tree.h"

size_t NodeIndex = 0;

int NodeVerify(const Node* node)
{
    if (!node) return 1;

    // if (!node->val_type) return 0;

    // if (node->val_type == NUM_TYPE)
    //     if (node->num_val == DEAD_NUM_VAL)

    // else if (node->val_type == VAR_TYPE)
    //     if (node->num_val == DEAD_VAR_VAL)

    // else if (node->val_type == OP_TYPE)
    //     if (node->num_val == DEAD_OP_VAL)

    if (node->left)
        if (!((node == node->left->prev) && NodeVerify(node->left))) return 0;

    if (node->right)
        if (!((node == node->right->prev) && NodeVerify(node->right))) return 0;

    return 1;
}

int NodeCtor(Node* node, enum TreeDataType val_type, double num_val, const char* var_val, enum Operators op_val)
{
    ASSERT(node     != nullptr);
    // ASSERT(val_type != NULL_TYPE);

    *node = {val_type, op_val, num_val, var_val, nullptr, nullptr, nullptr, NodeIndex++};

    return NodeVerify(node);
}

Node* CreateNode(enum TreeDataType val_type, double num_val, const char* var_val, enum Operators op_val, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));

    NodeCtor(node, val_type, num_val, var_val, op_val);
    NodeConnect(left, right, node);

    return node;
}

Node* CopyNode(const Node* node)
{
    ASSERT(node != nullptr);

    Node* left = nullptr;
    Node* right = nullptr;

    // HERE(START);

    if (node->left)
        left = CopyNode(node->left);

    if (node->right)
        right = CopyNode(node->right);

    char* var_val = nullptr;

    if (node->val_type == VAR_TYPE)
    {
        var_val = (char*) calloc(MAX_NODE_VAL_LEN, sizeof(char));
        memcpy(var_val, node->var_val, MAX_NODE_VAL_LEN);
        // fprintf(stderr, "%s\n\n\n", node->var_val);
    }

    return CreateNode(node->val_type, node->num_val, var_val, node->op_val, left, right);
}

int NodeDtor(Node* node)
{
    // if (!NodeVerify(node)) return 0;

    if (!node)
        return 0;

    if (node->left) NodeDtor(node->left);

    if (node->right) NodeDtor(node->right);

    if (node->var_val) free((void*) node->var_val);

    node->val_type  = NULL_TYPE;
    node->var_val   = nullptr;
    node->num_val   = 0;
    node->op_val    = NULL_OP;

    node->left      = nullptr;
    node->right     = nullptr;
    node->prev      = nullptr;
    NodeIndex--;

    free(node);
    node = nullptr;

    return 1;
}

int NodeConnect(Node* left, Node* right, Node* root)
{
    // ASSERT(tree != nullptr);
    ASSERT(root != nullptr);

    if (root)
    {
        if (left)
        {
            root->left  = left;
            left->prev  = root;
            // tree->n_nodes++;
        }

        if (right)
        {
            root->right  = right;
            right->prev  = root;
            // tree->n_nodes++;
        }
    }

    return 1;
}


int TreeVerify(const Tree* tree)
{
    if (!tree) return 1;

    // size_t n_nodes = 0;
    // if (n_nodes!= CountSubNodes(tree->root, &n_nodes))
    // {
    //     fprintf("Number of nodes is wrong. Tree is damaged!\n");
    //     return 0;
    // }

    return NodeVerify(tree->root);
}

int TreeCtor(Tree* tree)
{
    ASSERT(tree != nullptr);

    *tree = {nullptr, 0};

    return TreeVerify(tree);
}

int TreeDtor(Tree* tree)
{
    if (!TreeVerify(tree)) return 0;

    tree->n_nodes = 0;

    return NodeDtor(tree->root);
}


enum Operators IsOperator(const char* node_val)
{
    if      (!strcasecmp(node_val, "+"     )) return OP_ADD   ;
    else if (!strcasecmp(node_val, "-"     )) return OP_SUB   ;
    else if (!strcasecmp(node_val, "*"     )) return OP_MUL   ;
    else if (!strcasecmp(node_val, "/"     )) return OP_DIV   ;
    else if (!strcasecmp(node_val, "^"     )) return OP_DEG   ;
    else if (!strcasecmp(node_val, "exp"   )) return OP_EXP   ;
    else if (!strcasecmp(node_val, "sqrt"  )) return OP_SQRT  ;
    // else if (!strcasecmp(node_val, "root")) return OP_RT      ;
    else if (!strcasecmp(node_val, "log"   )) return OP_LOG   ;
    else if (!strcasecmp(node_val, "ln"    )) return OP_LN    ;
    else if (!strcasecmp(node_val, "sin"   )) return OP_SIN   ;
    else if (!strcasecmp(node_val, "cos"   )) return OP_COS   ;
    else if (!strcasecmp(node_val, "tg"    )) return OP_TG    ;
    else if (!strcasecmp(node_val, "ctg"   )) return OP_CTG   ;
    else if (!strcasecmp(node_val, "ch"    )) return OP_CH    ;
    else if (!strcasecmp(node_val, "sh"    )) return OP_SH    ;
    else if (!strcasecmp(node_val, "arcsin")) return OP_ARCSIN;
    else if (!strcasecmp(node_val, "arccos")) return OP_ARCCOS;
    else if (!strcasecmp(node_val, "arctg" )) return OP_ARCTG ;
    else if (!strcasecmp(node_val, "arcctg")) return OP_ARCCTG;

    return NULL_OP;
}

int IsVar(const char* node_val) //добавить таблицу переменных
{
    if (!strcasecmp(node_val, "x")) return 1;

    if (!strcasecmp(node_val, "y")) return 1;

    if (!strcasecmp(node_val, "z")) return 1;

    if (!strcasecmp(node_val, "e")) return 1;

    if (!strcasecmp(node_val, "pi")) return 1;

    if (!strcasecmp(node_val, "Dimas")) return 1;

    return 0;
}

void NodeValPrint(const Node* node, FILE* stream)
{
    if (node->val_type == NUM_TYPE)
        fprintf(stream, "%.3f" , node->num_val);

    else if (node->val_type == VAR_TYPE)
        fprintf(stream, "%s", node->var_val);

    else if (node->val_type == OP_TYPE)
        OperatorPrint(node->op_val, stream);
}

void OperatorPrint(enum Operators op_code, FILE* stream)
{
    if (op_code == OP_ADD   ) { fprintf(stream, "+"     ); return; }
    if (op_code == OP_SUB   ) { fprintf(stream, "-"     ); return; }
    if (op_code == OP_MUL   ) { fprintf(stream, "*"     ); return; }
    if (op_code == OP_DIV   ) { fprintf(stream, "/"     ); return; }
    if (op_code == OP_DEG   ) { fprintf(stream, "^"     ); return; }
    if (op_code == OP_SQRT  ) { fprintf(stream, "sqrt"  ); return; }
    // if (op_code == OP_RT)  { fprintf(stream, "root"); return; }
    if (op_code == OP_LOG   ) { fprintf(stream, "log"   ); return; }
    if (op_code == OP_LN    ) { fprintf(stream, "ln"    ); return; }
    if (op_code == OP_SIN   ) { fprintf(stream, "sin"   ); return; }
    if (op_code == OP_COS   ) { fprintf(stream, "cos"   ); return; }
    if (op_code == OP_TG    ) { fprintf(stream, "tg"    ); return; }
    if (op_code == OP_CTG   ) { fprintf(stream, "ctg"   ); return; }
    if (op_code == OP_CH    ) { fprintf(stream, "ch"    ); return; }
    if (op_code == OP_SH    ) { fprintf(stream, "sh"    ); return; }
    if (op_code == OP_ARCSIN) { fprintf(stream, "arcsin"); return; }
    if (op_code == OP_ARCCOS) { fprintf(stream, "arccos"); return; }
    if (op_code == OP_ARCTG ) { fprintf(stream, "arctg" ); return; }
    if (op_code == OP_ARCCTG) { fprintf(stream, "arcctg"); return; }

    fprintf(stream, "not an operator" );
}

int IsVarsInTree(Node* node)
{
    if (!node) return 0;

    NodeVerify(node);

    if (node->val_type == VAR_TYPE)
        return 1;

    if (node->val_type == OP_TYPE)
    {
        if (node->left)
            if (IsVarsInTree(node->left)) return 1;

        if (node->right)
            if (IsVarsInTree(node->right)) return 1;
    }

    return 0;
}

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

                    if (node->num_val == nan(""))
                    {
                        fprintf(stderr, "Деление на нуль!\n");
                        abort();
                        return nullptr;
                    }

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
            NodeDtor(node->left);
            NodeDtor(node->right);
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
                node = CR;
                // NodeDtor(node);
                return node;
            }

            else if (IS_NUM(node->right) && IS_ZERO(node->right))
            {
                node = CL;
                // NodeDtor(node);
                return node;
            }
        }

        if (IS_OP(OP_SUB))
        {
            if (IS_NUM(node->right) && IS_ZERO(node->right))
            {
                node = CL;
                // NodeDtor(node);
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
            }

            else if (IS_NUM(node->left) && IS_ONE(node->left))
            {
                NodeDtor(node->left);

                node = CR;
                // NodeDtor(node);
                return node;
            }

            else if (IS_NUM(node->right) && IS_ONE(node->right))
            {
                node = CL;
                // NodeDtor(node);
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
                node = CL;
                // NodeDtor(node);
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
                    node = CL;
                    // NodeDtor(node);
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
    {
        Node* node_old = *node;
        *node = DestroyNeutralTreeElements(*node);
        NodeDtor(node_old);
    }

    return *node;
}

void TreePreorderPrint(const Node* node, FILE* stream)
{
    ASSERT(node != nullptr);
    ASSERT(stream != nullptr);

    if (!node) return;

    fprintf(stream, "(");

    NodeValPrint(node, stream);

    if (node->left) TreePreorderPrint(node->left, stream);

    if (node->right) TreePreorderPrint(node->right, stream);

    fprintf(stream, ")");
}

void TreeInorderPrint(const Node* node, FILE* stream)
{
    ASSERT(node != nullptr);
    ASSERT(stream != nullptr);

    if (!node) return;

    // fprintf(stream, "(");

    if (node->left)
    {
        fprintf(stream, "(");
        TreeInorderPrint(node->left, stream);
    }

    NodeValPrint(node, stream);

    if (node->right)
    {
        TreeInorderPrint(node->right, stream);
        fprintf(stream, ")");
    }

    // fprintf(stream, ")");
}

void TreePostorderPrint(const Node* node, FILE* stream)
{
    ASSERT(node != nullptr);
    ASSERT(stream != nullptr);

    if (!node) return;

    fprintf(stream, "(");

    if (node->left) TreePostorderPrint(node->left, stream);

    if (node->right) TreePostorderPrint(node->right, stream);

    NodeValPrint(node, stream);

    fprintf(stream, ")");
}


// int IsObjectExist(const Node* node, const char* obj_name)
// {
//     ASSERT(node != nullptr);
//     ASSERT(obj_name != nullptr);
//     VERIFY_NODE(node);
//
//     if (!strcasecmp(node->data, obj_name))
//         return 1;
//
//     if (!(node->left || node->right))
//         return 0;
//
//     if (node->left)
//         if (IsObjectExist(node->left, obj_name)) return 1;
//
//     if (node->right)
//         if (IsObjectExist(node->right, obj_name)) return 1;
//
//     return 0;
// }
