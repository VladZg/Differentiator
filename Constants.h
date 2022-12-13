#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdlib.h>
#include <cstring>

enum Constants
{
    MAX_NODE_VAL_LEN = 10,
    WRONG_CALCULATED_NODE = 1423,
    MAX_UNREPLACEBLE_TREE_DEPTH = 10,
    MAX_EXPRESSION_LEN = 100,
    NUM_OF_TRANSITION_PHRASES = 16,
};

enum VarsConsts
{
    MAX_VAR_NAME_LEN = 10,
    NUM_OF_CONSTANTS = 3,
    MAX_NUM_OF_VAR   = 10 + NUM_OF_CONSTANTS,
    NO_VAR_NAME      = -1,
    DEFAULT_UNDEFINED_VAR_VALUE = 1,
};

enum TreeDataType
{
    NULL_TYPE = 0,
    NUM_TYPE ,
    VAR_TYPE ,
    OP_TYPE  ,
};

enum Operators
{
    NULL_OP = 0,
    OP_ADD     ,
    OP_SUB     ,
    OP_MUL     ,
    OP_DIV     ,
    OP_DEG     ,
    OP_EXP     ,
    OP_SQRT    ,
    // OP_RT      ,
    OP_LOG     ,
    OP_LN      ,
    OP_SIN     ,
    OP_COS     ,
    OP_TG      ,
    OP_CTG     ,
    OP_SH      ,
    OP_CH      ,
    OP_ARCSIN  ,
    OP_ARCCOS  ,
    OP_ARCTG   ,
    OP_ARCCTG  ,
};

enum OpTexPrintModes
{
    OP_TEX_PREPRINT_ONE_ARG ,
    OP_TEX_PREPRINT_TWO_ARGS,
    OP_TEX_INPRINT          ,
};

enum TreeDumpModes
{
    SIMPLE_DUMP_MODE   ,
    FULL_FULL_DUMP_MODE,
};

enum TexModes
{
    PRINT_STEPS_TEX_MODE = 1,
    SKIP_STEPS_TEX_MODE  = 0,
};

enum NodeVerifyCodes
{
    NODE_IS_OK_CODE = 0            ,
    NODE_IS_EMPTY_CODE             ,
    VAR_NODE_IS_EMPTY_CODE         ,
    OP_NODE_IS_EMPTY_CODE          ,
    NO_LEFT_SUBNODE_CODE           ,
    NO_RIGHT_SUBNODE_CODE          ,
    PARENT_CONNECTION_IS_WRONG_CODE,

};

enum TexFileConsts
{
    TEX_PAGE_WIDTH  = 25,
    TEX_PAGE_HEIGHT = 75,
};

const float TEX_PAGE_COEFF_BETWEEN_WIDTH_HEIGTH = 1.7;
const float ONE_NODE_TEX_PAGE_WIDTH = 0.5;

enum TexPrintExpressionModes
{
    INPRINT_MODE,
    OUTPRINT_MODE,
};

enum TexPrintModes
{
    CENTRED_PRINT_MODE,
    USUAL_PRINT_MODE  ,
};


#define NUMS_PRINT_ACCURACY "3"
#define FUNCTION_VALUE_IN_POINT_ACCURACY "5"

// #define TEXT_FOR_PRONOUNCING_FILENAME  "./TextForPronouncing.txt"
// #define DATABASE_DUMP_HTML_FILENAME    "./DumpInfo/DatabaseDump.html"
// #define TEXT_FOR_DOT_DUMP_FILENAME     "./DumpInfo/TextForDatabaseDump.dot"
// #define DATABASE_DUMP_PICTURE_FILENAME "DatabaseDump.svg"
// #define DATABASE_DUMP_PICTURE_FOLDER   "./DumpInfo/"

// const char Tree_replacements[] = {'A', 'B', 'C', 'D'}

const char Transitions[NUM_OF_TRANSITION_PHRASES][100] =
           {
                "It's simple as fuck",
                "It's really easy to find"   ,
                "Even my two-aged sister knows that",
                "If someone asked me that in the middle of the night, I wouldn't hesitate to say",
                "Man... Just look",
                "I spend the hole of my life to find the answer and finally it's",
                "thanks to the results of my colleagues' scientific work, I know that",
                "When I was child, my father always told me: \"Remember, son",
                "For centuries, people have hunted for the secret knowledge that",
                "What if",
                "Sounds logical that it is the same as",
                "I was asked not to tell anyone that",
                "My roommate mumbled it in his sleep all night",
                "While preparing for exams, I learned a lot of new things, for example",
                "A true prince must know that",
                "My friends always beat me, because I didn't know that"
            };

#endif
