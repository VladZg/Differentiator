#include "../Include/Config.h"
#include <stdio.h>
#include "./stdlib.h"
#include "../Libs/Stack/Assert.h"
#include "../Include/CheckFile.h"

size_t IsFileExist(const char* filename)
{
    ASSERT(filename != nullptr);

    FILE* file = fopen(filename, "rb+");

    if (file)
    {
        fclose(file);
        return 1;
    }

    return 0;
}

size_t CheckFile(const int argc, const char** argv, const char** filename_input)
{
    ASSERT(argv != nullptr);
    ASSERT(filename_input != nullptr);

    if (argc == 2)
    {
        if (IsFileExist(argv[1]))
        {
            *filename_input = argv[1];

            return 1;
        }

        else
        {
            fprintf(stderr, "  NO FILE \"%s\" IN THIS DIRECTORY\n\n", argv[1]);

            exit(1);
        }
    }

    else return 0;
}
