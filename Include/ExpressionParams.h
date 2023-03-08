#ifndef EXPRESSIONPARAMS_H
#define EXPRESSIONPARAMS_H

#include "./Config.h"
#include "./stdio.h"
#include "./Constants.h"
#include "./Tree.h"
#include "./ExpressionParams.h"

struct ExpressionVar
{
    char*  name;
    double value;
};

struct ExpressionParams
{
    Node** expression;

    ExpressionVar* vars;
    size_t         n_vars;

    size_t n_differentiate;
    size_t Makloren_accuracy;
    double tangent_point;
    double delta_coverage;
    char*  graph_diapasone;
};

int  ExpressionParamsDtor(ExpressionParams* params);
void ExpressionParamsDump(FILE* stream, const ExpressionParams* params);
int  AddVar(char* var_name, double var_val, ExpressionParams* params);
int  FindVarIndex(const char* var_name, const ExpressionVar* vars, size_t n_vars);

#endif
