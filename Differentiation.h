#ifndef DIFFERENTIATION_H
#define DIFFERENTIATION_H

#include "./Config.h"
#include <stdlib.h>
#include "./Tree.h"

Node* Diff(Node* node, FILE* tex_file, size_t* n_step);
Node* NDiff(Node* node, size_t n, FILE* tex_file);
Node* Differentiate(Node* node, FILE* tex_file);

#include "./DiffDSL.h"

#endif
