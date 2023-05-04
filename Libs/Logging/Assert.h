#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>

//! @brief Макрос, печатающий сообщение об ошибке в заданном условии, если оно неверно
//! @param[in] condition Проверяемое условие

#ifdef KNRM

#define ASSERT(condition)                                                             \
        {                                                                             \
        if (!(condition))                                                             \
            fprintf(stderr, KRED "Error in '%s' in line %d in file %s in %s\n" KNRM,  \
                    #condition, __LINE__, __FILE__, __PRETTY_FUNCTION__);             \
        }

#else

#define ASSERT(condition)                                                             \
        {                                                                             \
        if (!(condition))                                                             \
            fprintf(stderr, "Error in '%s' in line %d in file %s in %s\n",            \
                    #condition, __LINE__, __FILE__, __PRETTY_FUNCTION__);             \
        }

#endif

#endif
