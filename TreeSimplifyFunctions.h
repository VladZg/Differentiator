#ifndef TREESIMPLIFYFUNCTIONS_H
#define TREESIMPLIFYFUNCTIONS_H

#include "./Config.h"
#include <stdlib.h>
#include "./Tree.h"

Node* CalculateConstantSubtrees(Node* node);
Node* DestroyNeutralTreeElements(Node* node);
Node* SimplifyTree(Node** node);

#endif
