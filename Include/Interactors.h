#ifndef INTERACTORS_H
#define INTERACTORS_H

#include "./Config.h"
#include <stdlib.h>
#include "./stdarg.h"
#include "./Tree.h"
#include "./ExpressionParams.h"
#include "./ExpressionReader.h"

int ReadNodeVal(char* node_val, enum TreeDataType* val_type, enum Operators* op_val, double* num_val, const char** var_val, ExpressionParams* params);

Node* ReadExpressionToTree(FILE* expression_file, ExpressionParams* params);
int   ReadExpressionParams(FILE* expression_file, ExpressionParams* params);

void PrintParametersPoint(FILE* stream, const ExpressionParams* params);
int  AddConstants(ExpressionParams* params);
int  IsConstsInTree(const Node* node, const ExpressionVar* vars);

void PrintParametersPoint(FILE* stream, const ExpressionParams* params);
int  PrintAllVarNames    (FILE* stream, const ExpressionParams* params);
int  PrintTextInTex       (FILE* tex_file, const char* fmt_text, ...);

#define TEX_PRINT(...) PrintTextInTex(tex_file, __VA_ARGS__)

int WriteTree(FILE* stream, Tree* tree, const char* database_name);
int WriteNode(FILE* stream, Node* node);

#endif
