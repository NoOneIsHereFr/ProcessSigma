#include "../h/verbose.h"
#include <windows.h> // For MAX_PATH
#include <stdlib.h>  // For wcstombs
#include <string.h>  // For strcmp

int verbosemsg(char *msg, char *type) {
    if (strcmp(type, "err") == 0) {
        printf("<<< %s%s%s\n", RED, msg, NRM);
    } else if (strcmp(type, "inf") == 0) {
        printf("||| %s%s%s\n", WHT, msg, NRM);
    } else if (strcmp(type, "war") == 0) {
        printf("||| %s%s%s\n", YEL, msg, NRM);
    } else if (strcmp(type, "suc") == 0) {
        printf(">>> %s%s%s\n", GRN, msg, NRM);
    }
    return 0;
}

// Helper function to log wide strings
void verbose_wide(const wchar_t *wide_msg, const char *type) {
    char narrow_msg[MAX_PATH];
    wcstombs(narrow_msg, wide_msg, MAX_PATH); // Convert wide string to narrow string
    verbosemsg(narrow_msg, (char *)type);
}