#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>

// Types
typedef int BusyType;
typedef char *KeyType;
typedef int InfoType;
typedef int IndexType;

// Error codes
#define E_OK 0
#define E_ALLOCERROR -2
#define E_NOTFOUND -1
#define E_TABLEOVERFLOW -3
#define E_NULLPTR -4
#define E_DUPLICATE -5
#define E_WRONGINPUT -6

#endif // TYPES_H
