#include "./Config.h"
#include <stdlib.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Tree.h"

int TreeFullDotDump       (const Node* node, FILE* dot_file, enum DumpModes dump_mode);
int TreeCreateFullDotNodes(const Node* node, FILE* dot_file, enum DumpModes dump_mode);
int TreeCreateFullDotEdges(const Node* node, FILE* dot_file, enum DumpModes dump_mode);
int ShowTree              (const Node* node,                 enum DumpModes dump_mode, int open_html_dump = 0);
