#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <limits.h>

// Types
typedef size_t KeyType;
typedef char DataType;

// Errors

#define E_OK            0
#define E_NOT_FOUND     (-1)
#define E_NULL_PTR      (-2)

// Special constants

#define NO_KEY          ULLONG_MAX

#endif // TYPES_H
