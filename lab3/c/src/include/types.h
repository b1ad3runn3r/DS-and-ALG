#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>

// Types
typedef int BusyType;
typedef size_t KeyType;
typedef int RelType;
typedef size_t InfoType;
typedef int IndexType;

// Error codes
#define E_OK            0
#define E_ALLOC         (-1)
#define E_NOTFOUND      (-2)
#define E_NULLPTR       (-3)
#define E_INSERT        (-4)
#define E_WRONGINPUT    (-5)
#define E_OVERFLOW      (-6)
#define E_WRONGRELEASE  2

#endif // TYPES_H
