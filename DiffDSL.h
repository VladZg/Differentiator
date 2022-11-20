#ifndef DIFFDSL_H
#define DIFFDSL_H

#define DL Diff(node->left , tex_file, n_step)
#define DR Diff(node->right, tex_file, n_step)
#define CN CopyNode(node)
#define CL CopyNode(node->left)
#define CR CopyNode(node->right)

#define CREATE_NUM(num)                CreateNode(NUM_TYPE, num, nullptr, NULL_OP, nullptr, nullptr)
#define CREATE_OP(op_val, left, right) CreateNode(OP_TYPE,  0  , nullptr, op_val , left,    right)

#define ADD( left, right) CREATE_OP(OP_ADD , left         , right)
#define SUB( left, right) CREATE_OP(OP_SUB , left         , right)
#define MUL( left, right) CREATE_OP(OP_MUL , left         , right)
#define DIV( left, right) CREATE_OP(OP_DIV , left         , right)
#define DEG( left, right) CREATE_OP(OP_DEG , left         , right)
#define EXP(       right) CREATE_OP(OP_EXP , CREATE_NUM(0), right)
#define SQRT(      right) CREATE_OP(OP_SQRT, CREATE_NUM(0), right)
// #define RT(  left, right) CREATE_OP(OP_RT,   left, right)
#define LOG(       right) CREATE_OP(OP_LOG , CREATE_NUM(0), right)
#define LN(        right) CREATE_OP(OP_LN  , CREATE_NUM(0), right)
#define SIN(       right) CREATE_OP(OP_SIN , CREATE_NUM(0), right)
#define COS(       right) CREATE_OP(OP_COS , CREATE_NUM(0), right)
#define TG(        right) CREATE_OP(OP_TG  , CREATE_NUM(0), right)
#define CTG(       right) CREATE_OP(OP_CTG , CREATE_NUM(0), right)
#define SH(        right) CREATE_OP(OP_SH  , CREATE_NUM(0), right)
#define CH(        right) CREATE_OP(OP_CH  , CREATE_NUM(0), right)

#endif
