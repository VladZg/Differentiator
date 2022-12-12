#include <stdlib.h>
#include <string.h>
// #include "./CheckFile.h"

// const char  DATABASE_FILENAME_DEFAULT[] = "./Expression.txt";
// const char* DATABASE_FILENAME           = nullptr;

int main() //const int argc, const char** argv
{
    // if (!CheckFile(argc, argv, &DATABASE_FILENAME))
    //     DATABASE_FILENAME = DATABASE_FILENAME_DEFAULT;

    // -fsanitize=leak
    // -pedantic

    system("g++ -Wall -fsanitize=address -O0 -g "
           "./main.cpp                          "
           "./Stack/Stack.cpp                   "
           "./Stack/Log.cpp                     "
           "./CheckFile.cpp                     "
           "./Tree.cpp                          "
           "./TreeDump.cpp                      "
           "./ExpressionParams.cpp              "
           "./ExpressionReader.cpp              "
           "./Interactors.cpp                   "
           "./TexTreeTranslateFunctions.cpp     "
           "./TreeSimplifyFunctions.cpp         "
           "./MathFunctions.cpp                 "
           "./TexCreateFunctions.cpp            "
           "-o main                             ");

    char cmd[100] = "./main";

    // strcat(cmd, DATABASE_FILENAME);

    system(cmd);

    return 1;
}
