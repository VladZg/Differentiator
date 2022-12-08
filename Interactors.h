#ifndef INTERACTORS_H
#define INTERACTORS_H

#include "./Config.h"
#include <stdlib.h>
#include "./Tree.h"

struct ExpressionVar
{
    char*  name;
    double value;
};

struct ExpressionParams
{
    Node* expression;

    ExpressionVar* vars;
    size_t         n_vars;

    size_t n_differentiate;
    size_t Makloren_accuracy;
};

int ReadNodeVal(char* node_val, enum TreeDataType* val_type, enum Operators* op_val, double* num_val, const char** var_val, ExpressionParams* params);

Node* ReadExpressionToTree(FILE* expression_file, ExpressionParams* params);
int   ReadExpressionParams(FILE* expression_file, ExpressionParams* params);
int   ExpressionParamsDtor(ExpressionParams* params);
void  ExpressionParamsDump(FILE* stream, ExpressionParams* params);

int FindVarIndex(const char* var_name, const ExpressionParams* params);
int AddVar(char* var_name, double var_val, ExpressionParams* params);
int AddConstants(ExpressionParams* params);

int WriteTree(FILE* stream, Tree* tree, const char* database_name);
int WriteNode(FILE* stream, Node* node);

#endif
