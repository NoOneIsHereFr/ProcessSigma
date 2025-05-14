#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char *conFiS(float fInput) {
    static char buffer[50];
    snprintf(buffer, sizeof(buffer), "%.2f", fInput);
    return buffer;
}

char *conFiI(int iInput) {
    static char buffer[50];
    snprintf(buffer, sizeof(buffer), "%d", iInput);
    return buffer;
}
