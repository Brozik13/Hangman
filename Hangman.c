#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "HangmanLib.c"

#define MAX_LENGTH 40

int main() {
    FILE *file;
    char line[MAX_LENGTH];

    printf("Do you want to play or add words? ");
    fgets(line, sizeof(line), stdin);

    if (strcmp(line, "play\n") == 0) {
        file = fopen("WordstoGuess.txt", "r");
        FileOpenError(&file, "WordstoGuess.txt", "r");
        // Placeholder for guessing

    } else if (strcmp(line, "add\n") == 0) {
        file = fopen("WordstoGuess.txt", "a");  
        FileOpenError(&file, "WordstoGuess.txt", "a");


        ///SHOULD  BE IN WHILE LOOP-------------------------------------------------------
        printf("Please enter the word you want to add: ");
        fgets(line, sizeof(line), stdin);

        // Remove newline character from the entered word
        size_t length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        // Check if the entered word is not empty
        if (strlen(line) > 0) {
            fprintf(file, "%s\n", line);
            printf("Word successfully added.\n");
        } else {
            printf("Invalid word. Please enter a non-empty word.\n");
        }
        ///SHOULD  BE IN WHILE LOOP-------------------------------------------------------


    } else {
        printf("Looks like you do not want to do any of that. Bye!\n");
    }

    fclose(file);
    return 0;
}