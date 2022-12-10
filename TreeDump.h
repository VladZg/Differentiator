#include "./Config.h"
#include <stdlib.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Tree.h"

int TreeFullDotDump       (const Node* node, FILE* dot_file, enum TreeDumpModes dump_mode);
int TreeCreateFullDotNodes(const Node* node, FILE* dot_file, enum TreeDumpModes dump_mode);
int TreeCreateFullDotEdges(const Node* node, FILE* dot_file, enum TreeDumpModes dump_mode);
int ShowTree              (const Node* node,                 enum TreeDumpModes dump_mode, int open_html_dump = 0);
