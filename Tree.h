#ifndef TREE_H
#define TREE_H

#include "./Config.h"
#include <stdlib.h>
#include <stdio.h>

struct Node
{
    enum TreeDataType val_type;

    enum   Operators  op_val;
    double            num_val;
    const  char*      var_val;

    Node*  prev;
    Node*  left;
    Node*  right;

    size_t index;
};

struct Tree
{
    Node*  root;
    size_t n_nodes;
};

int NodeVerify (const Node* node);
int NodeCtor   (Node* node, enum TreeDataType val_type, double num_val, const char* var_val, enum Operators op_val);
int NodeDtor   (Node* node);
int NodeConnect(Node* left, Node* right, Node* root);

Node* CreateNode(enum TreeDataType val_type, double num_val, const char* var_val, enum Operators op_val, Node* left, Node* right);
Node* CopyNode(const Node* node);
int   IsVarsInTree(Node* node);
int   IsOperatorWithOneArgument(enum Operators op_val);

int TreeVerify(const Tree* tree);
int TreeCtor  (Tree* tree);
int TreeDtor  (Tree* tree);

enum Operators IsOperator(const char* node_val);

void NodeValPrint      (const Node* node, FILE* stream);
void OperatorPrint     (enum Operators op_code, FILE* stream);
void TreePreorderPrint (const Node* node, FILE* stream);
void TreeInorderPrint  (const Node* node, FILE* stream);
void TreePostorderPrint(const Node* node, FILE* stream);

// int  IsObjectExist(const Node* node, const char* obj_name);
// int  CountSubNodes(Node* node, int* counter);

#ifndef NDEBUG

#define VERIFY_TREE( tree_ptr )     \
    if (!TreeVerify(tree_ptr)) return 0;

#define VERIFY_NODE( node_ptr )     \
    if (!NodeVerify(node_ptr)) return 0;

#else

#define VERIFY_TREE( tree_ptr )
#define VERIFY_NODE( node_ptr )

#endif

#include "./DiffDSL.h"

#endif
