#ifndef TEXFUNCTIONS_H
#define TEXFUNCTIONS_H

#include "../Config.h"
#include <stdio.h>
#include "./Constants.h"
#include "./Tree.h"

int IsPrintableNode(const Node* node);
int WriteExpressionInTexFile(const Node* node, FILE* tex_file, enum TexPrintExpressionModes print_mode);
int TranslateNodeToTex(FILE* tex_file, const char* op_text, enum OpTexPrintModes mode, const Node* node); //int is_braces
int TranslateTreeToTex(const Node* node, FILE* tex_file);

int WriteHeadOfTexFile(FILE* tex_file);
int WriteTailOfTexFile(FILE* tex_file);
int CompileTexFile(const char* filename);

#endif
