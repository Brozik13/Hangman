#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "HangmanLib.c"

#define MAX_LENGTH 40

int main() {
    FILE *file = NULL;
    char line[MAX_LENGTH];

    printf("Do you want to play or add words? ");
    while (fgets(line, MAX_LENGTH, stdin) != NULL) {
        if (!GameContinues(&file, line)) {
            break;
        }
    }

    CloseFile(&file);
    return 0;
}