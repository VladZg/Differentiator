#include "../Include/Config.h"
#include <stdio.h>
#include <string.h>
#include "../Include/Constants.h"
#include "../Include/DefineColourConsts.h"
#include "../Libs/Stack/Include/Assert.h"
#include "../Include/Tree.h"
#include "../Include/Interactors.h"
#include "../Include/TexTreeTranslateFunctions.h"

int WriteExpressionInTexFile(const Node* node, FILE* tex_file, enum TexPrintExpressionModes print_mode)
{
    ASSERT(node     != nullptr);
    ASSERT(tex_file != nullptr);

    VERIFY_NODE(node);

    TEX_PRINT("$%s", (print_mode == OUTPRINT_MODE ? "$" : ""));

    TranslateTreeToTex(node, tex_file);

    TEX_PRINT("$%s", (print_mode == OUTPRINT_MODE ? "$" : ""));

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

            if ( prev_op_val  == OP_DIV           ||
                 prev_op_val  == OP_SQRT          ||
               ( node->op_val == OP_DEG    &&
                 prev_op_val  != OP_DEG      )    ||
               ((node->op_val == OP_ADD    ||
                 node->op_val == OP_SUB      ) &&
                (prev_op_val  == OP_ADD)     )    ||
                 prev_op_val  == OP_EXP             )

                is_print_brackets = 0;

            if (prev_op_val == OP_DEG   ||
                (node->op_val == OP_MUL    &&
                (prev_op_val  == OP_LOG    ||
                 prev_op_val  == OP_LN     ||
                 prev_op_val  == OP_SIN    ||
                 prev_op_val  == OP_COS    ||
                 prev_op_val  == OP_TG     ||
                 prev_op_val  == OP_CTG    ||
                 prev_op_val  == OP_SH     ||
                 prev_op_val  == OP_CH     ||
                 prev_op_val  == OP_ARCSIN ||
                 prev_op_val  == OP_ARCCOS ||
                 prev_op_val  == OP_ARCTG  ||
                 prev_op_val  == OP_ARCCTG   )))

                is_print_brackets = 1;
        }
    }

    else is_print_brackets = 0;

    if (is_print_brackets) TEX_PRINT("(");

    if (mode == OP_TEX_PREPRINT_ONE_ARG)
    {
        TEX_PRINT("%s", op_text);
        TranslateTreeToTex(node->right, tex_file);
    }

    else if (mode == OP_TEX_INPRINT)
    {
        TranslateTreeToTex(node->left, tex_file);
        TEX_PRINT("%s", op_text);
        TranslateTreeToTex(node->right, tex_file);
    }

    else if (mode == OP_TEX_PREPRINT_TWO_ARGS)
    {
        TEX_PRINT("%s", op_text);
        TranslateTreeToTex(node->left, tex_file);
        TranslateTreeToTex(node->right, tex_file);
    }

    else return 0;

    if (is_print_brackets) TEX_PRINT(")");

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
//         TEX_PRINT("{");
//
//         TEX_PRINT("(replacement)");
//         // NodeValPrint(node, tex_file);
//
//         TEX_PRINT("}");
//
//         return 1;
//     }

    if (node->val_type == OP_TYPE)
    {
        if (!((node->left) && (node->right))) return 0;

        ASSERT(node->left  != nullptr);
        ASSERT(node->right != nullptr);

        TEX_PRINT("{");

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

        TEX_PRINT("}");

        return 1;
    }

    TEX_PRINT("{");

    // if (IsPrintableNode(node)) NodeValPrint(node, tex_file);

    int is_negative_num = (node->val_type == NUM_TYPE &&
                           node->num_val < 0 && node->prev ? 1 : 0);

    if (is_negative_num) TEX_PRINT("(");

    NodeValPrint(node, tex_file);

    if (is_negative_num) TEX_PRINT(")");

    TEX_PRINT("}");

    return 1;
}
