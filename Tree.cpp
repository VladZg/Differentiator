#include "./Config.h"
#include <stdlib.h>
#include <math.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Stack/Stack.h"
#include "./Tree.h"

size_t NodeIndex = 0;

void WarningMessage(const char* function_name, const char* fmt_msg, ...)
{
    va_list args;
    va_start(args, fmt_msg);

    fprintf(stderr, KYEL "WARNING!!!" KNRM "(" KBLU "%45s" KNRM "): ", function_name);
    vfprintf(stderr, fmt_msg, args);
    fprintf(stderr, "\n");

    va_end(args);
}

// #define WARNING(fmt_msg, ...) \
{\
    if (sizeof((int[]){__VA_ARGS__})/sizeof(int) > 0)               \
        WarningMessage(__PRETTY_FUNCTION__, #fmt_msg, __VA_ARGS__); \
\
    else  WarningMessage(__PRETTY_FUNCTION__, #fmt_msg);\
}

int NodeVerify(const char* function_name, const Node* node)
{
    if (!node) return 1;

    if (node->val_type == NULL_TYPE)
    {
        WarningMessage(function_name, "NODE IS EMPTY");
        return 0;
    }

    if (node->val_type == NUM_TYPE)
    {
        if (node->left)
        {
            WarningMessage(function_name, "NODE WITH NUMBER HAS LEFT SUBNODE");
            return 0;
        }

        if (node->right)
        {
            WarningMessage(function_name, "NODE WITH NUMBER HAS RIGHT SUBNODE");
            return 0;
        }
    }

    if (node->val_type == VAR_TYPE)
    {
        if (!node->var_val)
        {
            WarningMessage(function_name, "NODE WITH VARIABLE TYPE IS EMPTY");
            return 0;
        }

        if (node->left)
        {
            WarningMessage(function_name, "NODE WITH VARIABLE HAS LEFT SUBNODE");
            return 0;
        }

        if (node->right)
        {
            WarningMessage(function_name, "NODE WITH VARIABLE HAS RIGHT SUBNODE");
            return 0;
        }
    }

    if (node->val_type == OP_TYPE )
    {
        if (node->op_val == NULL_OP)
        {
            WarningMessage(function_name, "NODE WITH OPERATOR %d TYPE IS EMPTY", node->op_val);
            return 0;
        }

        if (!node->left)
        {
            WarningMessage(function_name, "NODE WITH OPERATOR %d DOESN'T HAVE LEFT SUBNODE", node->op_val);
            return 0;
        }

        if (!node->right)
        {
            WarningMessage(function_name, "NODE WITH OPERATOR %d DOESN'T HAVE RIGHT SUBNODE", node->op_val);
            return 0;
        }
    }

    if (node->prev)
    {
        if (node->prev->left != node && node->prev->right != node)
        {
            WarningMessage(function_name, "NODE IS WRONGLY CONNECTED TO IT'S PARENT");
            return 0;
        }
    }

    if (node->left)
    {
        if (node != node->left->prev)
        {
            WarningMessage(function_name, "LEFT SUBNODE'S PREV IS WRONG");
            return 0;
        }

        if (!NodeVerify(function_name, node->left)) return 0;
    }

    if (node->right)
    {
        if (node != node->right->prev)
        {
            WarningMessage(function_name, "RIGHT SUBNODE'S PREV IS WRONG");
            return 0;
        }

        if (!NodeVerify(function_name, node->right)) return 0;
    }

    if ((node->left && node->right) && (node->left == node->right))
    {
        WarningMessage(function_name, "LEFT AND RIGHT SUBNODES ARE THE SAME SUBNODE");
        return 0;
    }

    return 1;
}

int NodeCtor(Node* node, enum TreeDataType val_type, double num_val, const char* var_val, enum Operators op_val)
{
    ASSERT(node     != nullptr);
    ASSERT(val_type != NULL_TYPE);

    if (val_type == OP_TYPE && op_val == NULL_OP)
    {
        WarningMessage(__PRETTY_FUNCTION__, "TRIED TO CONSTRUCT NODE WITH EMPTY OPERATOR");
        return 0;
    }

    if (val_type == VAR_TYPE && !var_val)
    {
        WarningMessage(__PRETTY_FUNCTION__, "TRIED TO CONSTRUCT NODE WITH EMPTY VARIABLE");
        return 0;
    }

    *node = {val_type, op_val, num_val, var_val, nullptr, nullptr, nullptr, NodeIndex++};

    return 1;
}

Node* CreateNode(enum TreeDataType val_type, double num_val, const char* var_val, enum Operators op_val, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    ASSERT(node != nullptr)

    NodeCtor(node, val_type, num_val, var_val, op_val);
    NodeConnect(left, right, node);

    return node;
}

Node* CopyNode(const Node* node)
{
    if (node == nullptr) return nullptr;

    Node* left  = CopyNode(node->left);
    Node* right = CopyNode(node->right);

    char* var_val = nullptr;

    if (node->val_type == VAR_TYPE)
    {
        var_val = (char*) calloc(MAX_NODE_VAL_LEN, sizeof(char));
        memcpy(var_val, node->var_val, MAX_NODE_VAL_LEN);
        // fprintf(stderr, "%s\n\n\n", node->var_val);
    }

    Node* new_node = CreateNode(node->val_type, node->num_val, var_val, node->op_val, left, right);
    new_node->prev = node->prev;

    return new_node;
}

int NodeDtor(Node** node)
{
    ASSERT(node != nullptr);

    if (!VERIFY_NODE(*node)) return 0;

    if (!(*node))
        return 1;

    if ((*node)->left)  if(!NodeDtor(&((*node)->left)))  return 0;

    if ((*node)->right) if(!NodeDtor(&((*node)->right))) return 0;

    if ((*node)->var_val) free((void*) (*node)->var_val);

    (*node)->val_type = NULL_TYPE;
    (*node)->var_val  = nullptr;
    (*node)->num_val  = 0;
    (*node)->op_val   = NULL_OP;

    (*node)->left      = nullptr;
    (*node)->right     = nullptr;
    (*node)->prev      = nullptr;
    NodeIndex--;

    free(*node);
    *node = nullptr;

    return 1;
}

int NodeConnect(Node* left, Node* right, Node* root)
{
    // ASSERT(tree != nullptr);
    ASSERT(root != nullptr);

    // if (!VERIFY_NODE(left))  return 0;
    // if (!VERIFY_NODE(right)) return 0;
    // if (!VERIFY_NODE(root))  return 0;

    if (root)
    {
        root->left = left;

        if (left)
            left->prev  = root;
            // tree->n_nodes++;

        root->right = right;

        if (right)
            right->prev  = root;
            // tree->n_nodes++;
    }

    return VERIFY_NODE(root);
}

Node* ReplaceNode(Node** replacing_node, Node* new_node)
{
    new_node->prev = (*replacing_node)->prev;

    NodeDtor(replacing_node);
    *replacing_node = new_node;

    return *replacing_node;
}

int SwapNodes(Node** node1, Node** node2)
{
    Node* temp_node = *node1;

    *node1 = *node2;
    *node2 = temp_node;

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

    return VERIFY_NODE(tree->root);
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

    return NodeDtor(&(tree->root));
}

size_t TreeDepth(const Node* node)
{
    if (node == nullptr) return 0;

    size_t left_depth  = 0;
    size_t right_depth = 0;

    if (node->left)
        left_depth = TreeDepth(node->left);

    if (node->right)
        right_depth = TreeDepth(node->right);

    // fprintf(stdout, "%ld %ld\n", left_depth, right_depth);

    return 1 + (left_depth > right_depth ? left_depth : right_depth); //max(left_depth, right_depth);
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
    if (op_code == OP_EXP   ) { fprintf(stream, "exp"   ); return; }
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

    VERIFY_NODE(node);

    if (node->val_type == VAR_TYPE)
        return 1;

    if (node->val_type == OP_TYPE)
    {
        ASSERT(node->left  != nullptr)
        ASSERT(node->right != nullptr)

        if (IsVarsInTree(node->left) || IsVarsInTree(node->right)) return 1;
    }

    return 0;
}

int IsOperatorWithOneArgument(enum Operators op_val)
{
    if (op_val == OP_EXP     ||
        op_val == OP_SQRT    ||
        // op_val == OP_R    ||
        op_val == OP_LOG     ||
        op_val == OP_LN      ||
        op_val == OP_SIN     ||
        op_val == OP_COS     ||
        op_val == OP_TG      ||
        op_val == OP_CTG     ||
        op_val == OP_SH      ||
        op_val == OP_CH      ||
        op_val == OP_ARCSIN  ||
        op_val == OP_ARCCOS  ||
        op_val == OP_ARCTG   ||
        op_val == OP_ARCCTG    )

        return 1;

    return 0;
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
