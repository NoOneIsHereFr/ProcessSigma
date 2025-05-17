#include "../h/verbose.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>

int verbosemsg(char *msg, char *type) {
    if (strcmp(type, "err") == 0) {
        printf("[-] %s%s%s\n", RED, msg, NRM);
    } else if (strcmp(type, "inf") == 0) {
        printf("[ ] %s%s%s\n", WHT, msg, NRM);
    } else if (strcmp(type, "infs") == 0) {
        printf("[~] %s%s%s\n", BLU, msg, NRM);
    } else if (strcmp(type, "war") == 0) {
        printf("[#] %s%s%s\n", YEL, msg, NRM);
    } else if (strcmp(type, "suc") == 0) {
        printf("[+] %s%s%s\n", GRN, msg, NRM);
    }
    return 0;
}

// Helper function to log wide strings
void verbose_wide(const wchar_t *wide_msg, const char *type) {
    char narrow_msg[MAX_PATH];
    wcstombs(narrow_msg, wide_msg, MAX_PATH);
    verbosemsg(narrow_msg, (char *)type);
}