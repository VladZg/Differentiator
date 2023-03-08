#ifndef TESTINGVALUES_H
#define TESTINGVALUES_H

#define TEST_INT   rand()
#define TEST_FLOAT (0.0001  * TEST_INT)

char        TEST_CHAR       = 't';

static char test_char_ptr[] = "test string";
char*       TEST_CHAR_PTR   = test_char_ptr;

#endif
