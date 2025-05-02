#ifndef AUTOSTART_H
#define AUTOSTART_H

#include <stdbool.h>

void toggle_autostart();
void copy_to_startup();
void remove_from_startup();
bool is_in_startup();

#endif
