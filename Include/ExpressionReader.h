#ifndef EXPRESSION_READER
#define EXPRESSION_READER

#include "../Config.h"
#include <./stdlib.h>
#include "./Constants.h"
#include "./Tree.h"
#include "./ExpressionParams.h"

void SyntaxErrorPrint();

Node* GetG(const char* str, const char** expression_str, ExpressionParams* params); // Считывание всего выражения
Node* GetE(                 const char** expression_str, ExpressionParams* params); // Считывание операций +, -
Node* GetT(                 const char** expression_str, ExpressionParams* params); // Считывание операций *, /
Node* GetD(                 const char** expression_str, ExpressionParams* params); // Считывание операции ^
Node* GetP(                 const char** expression_str, ExpressionParams* params); // Считывание выражений в скобочках (...)
Node* GetW(                 const char** expression_str, ExpressionParams* params); // Считывание переменных и функций
char* GetV(                 const char** expression_str                          ); // Считывание названий
Node* GetN(                 const char** expression_str                          ); // Считывание неотрицательных целых чисел

#endif
