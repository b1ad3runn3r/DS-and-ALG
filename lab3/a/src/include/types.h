#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>

// Types
typedef int BusyType;
typedef char KeyType;
typedef int InfoType; // TODO: make info type size_t
typedef int IndexType;

// Error codes
#define E_OK            0
#define E_ALLOC    (-1)
#define E_NOTFOUND      (-2)
#define E_NULLPTR       (-3)
#define E_INSERT        (-4)
#define E_WRONGINPUT    (-5)

#endif // TYPES_H
