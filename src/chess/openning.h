#ifndef OPENNING_H
#define OPENNING_H

#include <glib.h>
#include "moves.h"

typedef struct Openning {
    gchar *name;
    Move move_list[20];
} Openning;

#endif