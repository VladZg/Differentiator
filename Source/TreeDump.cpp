#include "../Include/Config.h"
#include <stdlib.h>
#include <time.h>
#include "../Include/Constants.h"
#include "../Include/DefineColourConsts.h"
#include "../Libs/Stack/Assert.h"
#include "../Include/Tree.h"

extern size_t N_dump_picture;

size_t N_dump_picture = 0;

static const char* NodeDataType(const Node* node)
{
    if (node->val_type == NUM_TYPE)
        return "NUM";

    if (node->val_type == VAR_TYPE)
        return "VAR";

    if (node->val_type == OP_TYPE)
        return "OP";

    return "NULL";
}

int TreeCreateFullDotNodes(const Node* node, FILE* dot_file, enum TreeDumpModes dump_mode)
{
    ASSERT(node != nullptr);
    ASSERT(dot_file != nullptr);
    // VERIFY_NODE(node);

    if (!node) return 0;

    if (dump_mode == SIMPLE_DUMP_MODE)
    {
        fprintf(dot_file, "    node%ld [style = filled, fillcolor = lightblue, label = \"", node->index);

        NodeValPrint(node, dot_file);

        fprintf(dot_file, "\"];\n");
    }

    else if (dump_mode == FULL_FULL_DUMP_MODE)
    {
        fprintf(dot_file, "    node%ld [shape = record, style = filled, fillcolor = lightblue, label = \"{{ <data_type> data_type:\\n %s | <data> ",
                        node->index, NodeDataType(node));

        NodeValPrint(node, dot_file);

        fprintf(dot_file, " } | { <this> this:\\n%p | <prev> prev:\\n%p } | { <left> left:\\n%p | <right> right:\\n%p }}\"];\n",
                        (void*) node, (void*) node->prev, (void*) node->left, (void*) node->right);
    }

    else return 0;

    if (node->left) TreeCreateFullDotNodes( node->left,  dot_file, dump_mode);

    if (node->right) TreeCreateFullDotNodes(node->right, dot_file, dump_mode);

    return 1;
}

int TreeCreateFullDotEdges(const Node* node, FILE* dot_file, enum TreeDumpModes dump_mode)
{
    ASSERT(node != nullptr);
    ASSERT(dot_file != nullptr);
    // VERIFY_NODE(node);

    if (!node) return 0;

    if (node->left)
    {
        fprintf(dot_file, "    node%ld -> node%ld;\n", node->index, node->left->index);
        TreeCreateFullDotEdges(node->left, dot_file, dump_mode);
    }

    if (node->right)
    {
        TreeCreateFullDotEdges(node->right, dot_file, dump_mode);
        fprintf(dot_file, "    node%ld -> node%ld;\n", node->index, node->right->index);
    }

    return 1;
}

int TreeFullDotDump(const Node* node, FILE* dot_file, enum TreeDumpModes dump_mode)
{
    ASSERT(node != nullptr);
    ASSERT(dot_file != nullptr);
    // VERIFY_NODE(node);

    VERIFY_NODE(node);

    fprintf(dot_file, "digraph G{\n");

    fprintf(dot_file, "    rankdir = TB;\n"
                      "    edge[ colour = black ];\n");

    if (dump_mode == SIMPLE_DUMP_MODE)
        fprintf(dot_file, "    node[ fillcolor = lightgreen, colour = black, shape = circle, fontsize = 20 ];\n");

    else if (dump_mode == FULL_FULL_DUMP_MODE)
        fprintf(dot_file, "    node[ colour = black, shape = rectangle, fontsize = 10 ];\n");

    TreeCreateFullDotNodes(node, dot_file, dump_mode);
    TreeCreateFullDotEdges(node, dot_file, dump_mode);

    fprintf(dot_file, "}\n");

    return 1;
}

int ShowTree(const Node* node, enum TreeDumpModes dump_mode, int open_html_dump)
{
    ASSERT(node != nullptr);
    // VERIFY_NODE(node);

    VERIFY_NODE(node);

    FILE* dot_file = fopen("./TreeDump/TextForTreeDump.dot", "w");
    ASSERT(dot_file != nullptr);

    TreeFullDotDump(node, dot_file, dump_mode);
    fclose(dot_file);

    char cmd[150] = {};

    char html_file_opening_mode[3] = {};

    N_dump_picture != 0 ? html_file_opening_mode[0] = 'a' : html_file_opening_mode[0] = 'w';

    sprintf(cmd, "dot " "./TreeDump/TextForTreeDump.dot" " -Tsvg -o" "./TreeDump/DumpPictures/TreeDump%ld.svg", ++N_dump_picture);

    system(cmd);

    FILE* file_html = fopen("./TreeDump/HtmlTreeDump.html", html_file_opening_mode);
    ASSERT(file_html != nullptr);

    // fseek(file_html, 0, SEEK_SET);

    time_t now        = time(0);
    tm     *real_time = localtime(&now);

    fprintf(file_html, "<pre>\n");
    fprintf(file_html, "    <h1> TREE DUMP №%ld (%02d:%02d:%02d  %02d.%02d.%04d) </h1>\n",
                        N_dump_picture,
                        real_time->tm_hour, real_time->tm_min, real_time->tm_sec,
                        real_time->tm_mday, 1 + real_time->tm_mon, 1900 + real_time->tm_year);

    fprintf(file_html,  "    \n"
                        "        <img src = \"" "./DumpPictures/TreeDump%ld.svg" "\">\n"
                        "<!-- ------------------------------------------------------------ -->\n"
                        "    <hr>\n",
                        N_dump_picture);

    // fprintf(file_html, "</pre>\n\n");

    fclose(file_html);

    #ifndef NOPEN_DUMPS
    if (open_html_dump) system("xdg-open " "./TreeDump/HtmlTreeDump.html");
    #endif

    return 1;
}
