#ifndef TREESIMPLIFYFUNCTIONS_H
#define TREESIMPLIFYFUNCTIONS_H

#include "./Config.h"
#include <stdlib.h>
#include "./Tree.h"
#include "./Interactors.h"

double CalculateOperatorNode(enum Operators op_val, double left_num_val, double right_num_val);
Node*  CalculateConstantSubtrees(Node** node);
double CalculateTree(Node* node, const ExpressionParams* params);
int InsertConstsInExpression(Node* node, const ExpressionParams* params);
Node*  DestroyNeutralTreeElements(Node** node);
Node*  SimplifyTree(Node** node);

#endif
