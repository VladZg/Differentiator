#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdlib.h>
#include <cstring>

enum Constants
{
    MAX_NODE_VAL_LEN = 10,
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

enum DumpModes
{
    SIMPLE_DUMP_MODE   ,
    FULL_FULL_DUMP_MODE,
};

// #define TEXT_FOR_PRONOUNCING_FILENAME  "./TextForPronouncing.txt"
// #define DATABASE_DUMP_HTML_FILENAME    "./DumpInfo/DatabaseDump.html"
// #define TEXT_FOR_DOT_DUMP_FILENAME     "./DumpInfo/TextForDatabaseDump.dot"
// #define DATABASE_DUMP_PICTURE_FILENAME "DatabaseDump.svg"
// #define DATABASE_DUMP_PICTURE_FOLDER   "./DumpInfo/"

#endif
