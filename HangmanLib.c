#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Handles errors that may occur while opening a file.
 * 
 * If the provided file pointer is NULL, an error message is printed using perror,
 * and the program exits with an exit code of 1.
 * 
 * @param file Pointer to the file to be checked for opening errors.
 */
void FileOpenError(FILE **file, const char *filename, const char *mode) {
    *file = fopen(filename, mode);
    if (*file == NULL) {
        perror("Error opening file");
        exit(1);
    }
}

/**
 * @brief Closes the given file and sets the file pointer to NULL.
 * 
 * This function safely closes the file pointed to by the file pointer. 
 * It also ensures that the file pointer is set to NULL after closing to avoid
 * accidental operations on an already closed file, which could lead to
 * undefined behavior or double closing errors.
 * 
 * @param file Double pointer to the file that needs to be closed. 
 *        This pointer should point to a valid `FILE*` that was opened using
 *        functions like `fopen`. After closing, the pointer is set to `NULL`.
 */
void CloseFile(FILE **file) {
    // Check if the file pointer is not NULL, meaning the file is currently open
    if (*file != NULL) {
        fclose(*file); // Close the file to release resources
        *file = NULL; // Set the file pointer to NULL to prevent double closing
    }
}

/**
 * @brief Handles the case when a file is empty or an error occurs while reading it.
 * 
 * If the file is empty, a message indicating the end of the file is reached is printed.
 * If an error occurs while reading the file, an error message is printed using perror.
 * 
 * @param filename Name of the file to be checked for emptiness or read errors.
 * @param mode Mode in which the file should be opened.
 * @param line Buffer to store the read line from the file.
 * @return true if the file is empty, false otherwise.
 */
bool FileEmpty(const char *filename, const char *mode, char *line) {
    FILE *file;
    FileOpenError(&file, filename, mode);

    if (fgets(line, sizeof(line), file) == NULL) {
        if (feof(file)) {
            printf("End of file reached.\n");
            fclose(file);
            return true;
        } else if (ferror(file)) {
            perror("Error reading file");
            fclose(file);
            return true;
        } else {
            printf("An unknown error occurred.\n");
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}

/**
 * @brief Allows the user to insert words into the file, handling input until the user enters "0".
 * 
 * This function continuously prompts the user to input words, which are then added to the file.
 * The user can stop the insertion process by entering "0". The function also checks for empty input
 * and ensures that only non-empty words are added to the file.
 * 
 * @param file Pointer to the file where words will be added. The file must be opened in append mode.
 * @param line Buffer to store the input word from the user.
 */
void WordInsertion (FILE *file, char *line){
    printf("Please enter the word you want to add: ");

    while (fgets(line, sizeof(line), stdin) != 0)
    { 
        // Remove the newline character at the end of the input
        line[strcspn(line, "\n")] = 0;
        // Check if the input is "0"
        if (strcmp(line, "0") == 0)
        {
            break; // Exit the loop if the input is "0"
        }
        // Check if the entered word is not empty
        if (strlen(line) > 0)
        {
                                        //------------------------// Check if word isnt already in file TODO
            fprintf(file, "%s\n", line);
            printf("Word successfully added.\n");
        }
        else
        {
            printf("Invalid word. Please enter a non-empty word.\n");
        }
        printf("If you want to continue adding write the words. Otherwise 0: ");
    }
}



void WordGuessing(){
    printf("guessing");
}



/**
 * @brief Handles user commands to either play a game or add words to a file.
 * 
 * The function checks the user's input (`line`) to determine whether they want to play the game or 
 * add words to a file. Depending on the input, it opens the file in the appropriate mode and 
 * either starts the game or calls the `WordInsertion` function to add words to the file.
 * If the user inputs an unrecognized command, the function returns `false` to indicate that the 
 * game should not continue.
 * 
 * @param file Pointer to the file where words will be read from or written to. This file must be 
 *        opened in the appropriate mode based on the user's input.
 * @param line Buffer to store the user's input command.
 * @return true if the game should continue, false otherwise.
 */
bool GameContinues(FILE **file, char *line){

    if (strcmp(line, "play\n") == 0) {
        *file = fopen("WordstoGuess.txt", "r");
        FileOpenError(file, "WordstoGuess.txt", "r");
        
        WordGuessing();
        CloseFile(file);
    } 
    else if (strcmp(line, "add\n") == 0) {
        *file = fopen("WordstoGuess.txt", "a");  
        FileOpenError(file, "WordstoGuess.txt", "a");

        WordInsertion(*file, line);
        CloseFile(file);
    } else {
        printf("Looks like you do not want to do any of that. Bye!\n");
        return false;
    }
    
    printf("Do you want to continue play or add ?");
    return true;
}