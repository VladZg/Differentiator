#ifndef TEXCREATEFUNCTIONS_H
#define TEXCREATEFUNCTIONS_H

#include "./Config.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Stack/Stack.h"
#include "./Tree.h"
#include "./TreeDump.h"
#include "./ExpressionReader.h"
#include "./Interactors.h"
#include "./TexTreeTranslateFunctions.h"
#include "./TreeSimplifyFunctions.h"
#include "./MathFunctions.h"
#include "./DiffDSL.h"

int WriteHeadOfTexFile                 (FILE* tex_file);
int ShitSomeCringeIntroductionInTexFile(FILE* tex_file);
int ShitSomeCringeConclusionInTexFile  (FILE* tex_file);
int IntroduceParameters(const ExpressionParams* params, FILE* tex_file);

int FindFirstDerivationTex           (      Node*  expression, const ExpressionParams* params, FILE* tex_file);
int SimplifyExpressionTex            (      Node** expression, const ExpressionParams* params, FILE* tex_file);
int CalculateExpressionTex           (      Node*  expression, const ExpressionParams* params, FILE* tex_file);
int FindNDerivationTex               (      Node*  expression, const ExpressionParams* params, FILE* tex_file);
int FindFullDerivationTex            (const Node*  expression, const ExpressionParams* params, FILE* tex_file);
int ExploreFunctionOfManyVariablesTex(const Node*  expression, const ExpressionParams* params, FILE* tex_file);

int PrintExpressionAsFunction                   (const Node* expression,  const ExpressionParams* params, const char* function_name, FILE* tex_file);
Node* TranslateExpresiionToFunctionOfTheFirstVar(const Node*  expression, const ExpressionParams* params);
int TranslateTreeToGnuplotFormula               (const Node*  node, char* formula);
int CreateGraph                                 (      Node** function_of_one_variable, int n_graphs, const char* var_diapasone, FILE* tex_file);

int DecomposeOnMaklorensFormulaTex      (Node*  function_of_the_first_variable, const ExpressionParams* params, FILE* tex_file);
int EquationsInThePointTex              (Node*  function_of_the_first_variable,       ExpressionParams* params, FILE* tex_file);
int GraphOfFunction                     (Node*  function_of_the_first_variable, const ExpressionParams* params, FILE* tex_file);
int ExploreFunctionOfTheFirstVariableTex(Node** function_of_the_first_variable,       ExpressionParams* params, FILE* tex_file);

int FillTexFile       (FILE* tex_file, ExpressionParams* params);
int WriteTailOfTexFile(FILE* tex_file);

int ChangeTexPapersize(FILE* tex_file);
int CompileTexFile(const char* filename);
int CreateTexFile (const char* filename, ExpressionParams* params);

#endif
