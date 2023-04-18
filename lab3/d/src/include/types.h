#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>

// Types
typedef size_t InfoType;
typedef size_t KeyType;
typedef long RelType;

// Error codes
#define E_OK            0
#define E_NOTFOUND      (-1)
#define E_ALLOC         (-2)
#define E_NULLPTR       (-3)
#define E_INSERT        (-4)
#define E_WRONGINPUT    (-5)
#define E_OVERFLOW      (-6)
#define E_NOFILE        (-7)

#endif // TYPES_H
