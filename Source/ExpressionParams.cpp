#include "../Include/Config.h"
#include <stdio.h>
#include "../Include/Constants.h"
#include "../Include/DefineColourConsts.h"
#include "../Libs/Stack/Include/Assert.h"
#include "../Include/Tree.h"
#include "../Include/ExpressionParams.h"

void ExpressionParamsDump(FILE* stream, const ExpressionParams* params)
{
    fprintf(stream, KBLU "\n! Expression params:\n"  KNRM);

    fprintf(stream, "! \n"
                    "!-Constants (" KYEL "%d" KNRM "):    \n", NUM_OF_CONSTANTS);

    for (size_t const_i = 0; const_i < NUM_OF_CONSTANTS; const_i++)
        fprintf(stream, "! [" KGRN "%ld" KNRM "] " KYEL "%10s" KNRM " = %lf\n", const_i, params->vars[const_i].name, params->vars[const_i].value);

    fprintf(stream, "! \n"
                    "!-Variables (" KMAG "%d" KNRM "):    \n", (int) params->n_vars - NUM_OF_CONSTANTS);

    for (size_t var_i = NUM_OF_CONSTANTS; var_i < params->n_vars; var_i++)
        fprintf(stream, "! [" KGRN "%ld" KNRM "] " KMAG "%10s" KNRM " = %lf\n", var_i, params->vars[var_i].name, params->vars[var_i].value);

    fprintf(stream, "! \n"
                    "!-Number of differentiates: %ld\n"
                    "!-Macloren's accuracy:      %ld\n"
                    "!-Tanget point:             %lf\n"
                    "!-Delta_coverage:           %lf\n"
                    "!-Graph diapasone:          %s \n\n", params->n_differentiate, params->Makloren_accuracy, params->tangent_point, params->delta_coverage, params->graph_diapasone);
}

int ExpressionParamsDtor(ExpressionParams* params)
{
    ASSERT(params  != nullptr);

    NodeDtor(params->expression);
    free(params->expression);

    params->Makloren_accuracy = 0;
    params->n_differentiate   = 0;
    params->tangent_point     = 0;
    params->delta_coverage    = 0;

    for (size_t var_i = 0; var_i < params->n_vars; var_i++)
    {
        free(params->vars[var_i].name);
        params->vars[var_i].name = nullptr;
    }

    free(params->vars);
    params->vars = nullptr;

    free((void*) params->graph_diapasone);

    params = nullptr;

    return 1;
}

int AddVar(char* var_name, double var_val, ExpressionParams* params)
{
    params->vars[(params->n_vars)++] = {var_name, var_val};

    return 1;
}

int FindVarIndex(const char* var_name, const ExpressionVar* vars, size_t n_vars)
{
    for (size_t var_i = 0; var_i < n_vars; var_i++)
    {
        if (!strcasecmp(var_name, vars[var_i].name))
            return var_i;
    }

    return NO_VAR_NAME;
}
