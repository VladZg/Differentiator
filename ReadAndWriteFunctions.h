#ifndef READANDWRITEFUNCTIONS_H
#define READANDWRITEFUNCTIONS_H

struct ExpressionParams
{
    ExpressionVar* vars;
};

int ReadNodeVal(char* node_val, enum TreeDataType* val_type, enum Operators* op_val, double* num_val, const char** var_val);

// int   ReadDatabaseName   (FILE* database_file, char* database_name);
Node* ReadExpressionToTree (FILE* database_file, Tree* tree);
int ReadExpressionParams   (FILE* expression_file, ExpressionParams* params);
int   WriteTree            (FILE* stream, Tree* tree, const char* database_name);
int   WriteNode            (FILE* stream, Node* node);

int IsPrintableNode(const Node* node);
int IsOperatorWithOneArgument(enum Operators op_val);
int WriteExpressionInTexFile(const Node* node, FILE* tex_file);
int TranslateNodeToTex(FILE* tex_file, const char* op_text, enum OpTexPrintModes mode, const Node* node); //int is_braces
int TranslateTreeToTex(const Node* node, FILE* tex_file);

int WriteHeadOfTexFile(FILE* tex_file);
int WriteTailOfTexFile(FILE* tex_file);
int CompileTexFile(const char* filename);

#endif
