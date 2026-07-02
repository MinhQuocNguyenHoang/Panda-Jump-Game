#ifndef __ARROW_H__
#define __ARROW_H__

#include <stdint.h>
#include "scr_setting.h"

#define MAX_ARROWS (5)
typedef struct
{
    int16_t x;
    int16_t y;
    int16_t speed;
    bool active; // true = active, false = inactive/dead
} arrow_t;

extern arrow_t arrows[MAX_ARROWS];
#endif /*__ARROW_H__*/
