#ifndef TREESIMPLIFYFUNCTIONS_H
#define TREESIMPLIFYFUNCTIONS_H

#include "./Config.h"
#include <stdlib.h>
#include "./Tree.h"
#include "./Interactors.h"

double CalculateOperatorNode(enum Operators op_val, double left_num_val, double right_num_val);
Node*  CalculateConstantSubtrees(Node** node);
Node*  DestroyNeutralTreeElements(Node** node);
Node*  SimplifyTree(Node** node);

int    InsertConstsInExpression(Node* node, const ExpressionParams* params);
Node* ReplaceVarsWithNumsExceptOne(Node* node, const ExpressionVar* vars, int inreplaceble_var_index, size_t n_vars);

#endif
