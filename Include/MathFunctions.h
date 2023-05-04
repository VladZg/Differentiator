#ifndef DIFFERENTIATION_H
#define DIFFERENTIATION_H

#include "../Config.h"
#include <stdlib.h>
#include "./Tree.h"
#include "./Interactors.h"

double CalculateTree(Node* node, const ExpressionParams* params);
Node*  Diff(Node* node, FILE* tex_file, size_t* n_step, enum TexModes tex_mode);
Node*  NDifferentiate(Node* node, size_t n_diff, FILE* tex_file, enum TexModes tex_mode);
Node*  Differentiate(Node* node, FILE* tex_file, enum TexModes tex_mode);
Node*  FindPartialDerivation(const Node* node, const ExpressionVar* vars, int var_index, size_t n_vars, FILE* tex_file);
Node*  DecomposeByMacloren(Node* node, const ExpressionParams* params, double Maklorens_coefficients[], FILE* tex_file);

#include "./DiffDSL.h"

#endif
