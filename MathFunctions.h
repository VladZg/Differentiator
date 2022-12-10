#ifndef DIFFERENTIATION_H
#define DIFFERENTIATION_H

#include "./Config.h"
#include <stdlib.h>
#include "./Tree.h"

Node* Diff(Node* node, FILE* tex_file, size_t* n_step, enum TexModes tex_mode);
Node* NDifferentiate(Node* node, size_t n, FILE* tex_file, enum TexModes tex_mode);
Node* Differentiate(Node* node, FILE* tex_file, enum TexModes tex_mode);

#include "./DiffDSL.h"

#endif
