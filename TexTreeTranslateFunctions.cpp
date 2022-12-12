#include "./Config.h"
#include <stdio.h>
#include <string.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Tree.h"
#include "./Interactors.h"
#include "./TexTreeTranslateFunctions.h"

int WriteExpressionInTexFile(const Node* node, FILE* tex_file)
{
    ASSERT(node     != nullptr);
    ASSERT(tex_file != nullptr);

    VERIFY_NODE(node);

    fprintf(tex_file, "\\[");

    TranslateTreeToTex(node, tex_file);

    fprintf(tex_file, "\\]\n");

    return 1;
}

int TranslateNodeToTex(FILE* tex_file, const Node* node, const char* op_text, enum OpTexPrintModes mode, int is_brackets)
{
    ASSERT(node        != nullptr);
    ASSERT(node->left  != nullptr);
    ASSERT(node->right != nullptr);

    int is_print_brackets = is_brackets && node->prev;

    if (node->prev)
    {
        if (node->prev->val_type == OP_TYPE)
        {
            enum Operators prev_op_val = node->prev->op_val;

            if ( prev_op_val    == OP_DIV         ||
                 prev_op_val    == OP_SQRT        ||
               ( node->op_val   == OP_DEG  &&
                 prev_op_val    != OP_DEG    )    ||
               ((node->op_val   == OP_ADD  ||
                 node->op_val   == OP_SUB    ) &&
                (prev_op_val    == OP_ADD)   )    ||
                 prev_op_val    == OP_EXP)

                is_print_brackets = 0;

            if (prev_op_val == OP_DEG) is_print_brackets = 1;
        }
    }

    if (is_print_brackets) fprintf(tex_file, "(");

    if (mode == OP_TEX_PREPRINT_ONE_ARG)
    {
        fprintf(tex_file, "%s", op_text);
        TranslateTreeToTex(node->right, tex_file);
    }

    else if (mode == OP_TEX_INPRINT)
    {
        TranslateTreeToTex(node->left, tex_file);
        fprintf(tex_file, "%s", op_text);
        TranslateTreeToTex(node->right, tex_file);
    }

    else if (mode == OP_TEX_PREPRINT_TWO_ARGS)
    {
        fprintf(tex_file, "%s", op_text);
        TranslateTreeToTex(node->left, tex_file);
        TranslateTreeToTex(node->right, tex_file);
    }

    else return 0;

    if (is_print_brackets) fprintf(tex_file, ")");

    return 1;
}

int IsPrintableNode(const Node* node)
{
    if (node->val_type == NUM_TYPE && node->num_val == 0)
        return 0;

    return 1;
}

int TranslateTreeToTex(const Node* node, FILE* tex_file)
{
    ASSERT(node != nullptr);
    ASSERT(tex_file != nullptr);

    if (!node) return 1;

//     if (TreeDepth(node) == MAX_UNREPLACEBLE_TREE_DEPTH)
//     {
//         fprintf(tex_file, "{");
//
//         fprintf(tex_file, "(replacement)");
//         // NodeValPrint(node, tex_file);
//
//         fprintf(tex_file, "}");
//
//         return 1;
//     }

    if (node->val_type == OP_TYPE)
    {
        if (!((node->left) && (node->right))) return 0;

        ASSERT(node->left  != nullptr);
        ASSERT(node->right != nullptr);

        fprintf(tex_file, "{");

        switch(node->op_val)
        {
            case OP_ADD   : { TranslateNodeToTex(tex_file, node, "+"                , OP_TEX_INPRINT          , 1); break; }
            case OP_SUB   : { TranslateNodeToTex(tex_file, node, "-"                , OP_TEX_INPRINT          , 1); break; }
            case OP_MUL   : { TranslateNodeToTex(tex_file, node, "\\cdot"           , OP_TEX_INPRINT          , 0); break; }
            case OP_DIV   : { TranslateNodeToTex(tex_file, node, "\\frac"           , OP_TEX_PREPRINT_TWO_ARGS, 0); break; }
            case OP_DEG   : { TranslateNodeToTex(tex_file, node, "^"                , OP_TEX_INPRINT          , 1); break; }
            case OP_EXP   : { TranslateNodeToTex(tex_file, node, "e^"               , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_SQRT  : { TranslateNodeToTex(tex_file, node, "\\sqrt"           , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            // case OP_R    : { TranslateNodeToTex(tex_file, node, "+", OP_TEX_INPRINT); break;}
            case OP_LOG   : { TranslateNodeToTex(tex_file, node, "\\log"            , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_LN    : { TranslateNodeToTex(tex_file, node, "\\ln"             , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_SIN   : { TranslateNodeToTex(tex_file, node, "\\sin"            , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_COS   : { TranslateNodeToTex(tex_file, node, "\\cos"            , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_TG    : { TranslateNodeToTex(tex_file, node, "\\tan"            , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_CTG   : { TranslateNodeToTex(tex_file, node, "\\cot"            , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_SH    : { TranslateNodeToTex(tex_file, node, "\\sinh"           , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_CH    : { TranslateNodeToTex(tex_file, node, "\\cosh"           , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_ARCSIN: { TranslateNodeToTex(tex_file, node, "\\arcsin"         , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_ARCCOS: { TranslateNodeToTex(tex_file, node, "\\arccos"         , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_ARCTG : { TranslateNodeToTex(tex_file, node, "\\arctan"         , OP_TEX_PREPRINT_ONE_ARG , 0); break; }
            case OP_ARCCTG: { TranslateNodeToTex(tex_file, node, "\\pi/2-\\arctan"  , OP_TEX_PREPRINT_ONE_ARG , 1); break; }

            default      : return 0;
        }

        fprintf(tex_file, "}");

        return 1;
    }

    fprintf(tex_file, "{");

    // if (IsPrintableNode(node)) NodeValPrint(node, tex_file);

    int is_negative_num = (node->val_type == NUM_TYPE &&
                           node->num_val < 0 && node->prev ? 1 : 0);

    if (is_negative_num) fprintf(tex_file, "(");

    NodeValPrint(node, tex_file);

    if (is_negative_num) fprintf(tex_file, ")");

    fprintf(tex_file, "}");

    return 1;
}
