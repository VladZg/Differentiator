#ifndef DIFFERENTIATION_RULES_H
#define DIFFERENTIATION_RULES_H

#include "./Config.h"
#include <stdlib.h>
#include "./Tree.h"

Node* Diff(Node* node, FILE* tex_file, size_t* n_step);

#include "./DiffDSL.h"

#endif
