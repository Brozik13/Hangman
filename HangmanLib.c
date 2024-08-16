#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#define MAX_LENGTH 40
#define FILENAME "WordstoGuess.txt"

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
 * @brief Checks if a given word already exists in the specified file.
 * 
 * Opens the file for reading and searches for the word. If the word is found,
 * prints a message indicating that the word is already in the file.
 * 
 * @param filename The name of the file to be checked.
 * @param word The word to search for in the file.
 * @return true if the word is found, false otherwise.
 */
bool WordAlreadyInFile(const char *filename, const char *word) {
    FILE *file;
    char line[MAX_LENGTH];
    bool found = false;

    FileOpenError(&file, FILENAME, "r");

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        // Check if the line matches the word
        if (strcmp(line, word) == 0) {
            found = true;
            break;
        }
    }
    fclose(file);
    if (found) {
        printf("The word '%s' is already in the file.\n", word);
    }
    return found;
}

/**
 * @brief Validates if the input is a real word.
 * 
 * This function checks if the provided word meets certain criteria to be considered valid. 
 * A valid word must be non-empty and contain only alphabetic characters. The function
 * ensures that the word is at least a specified length and does not contain any invalid
 * characters.
 * 
 * @param word The word to be validated. It should be a null-terminated string.
 * 
 * @return true if the word is valid, false otherwise.
 */
bool IsValidWord(const char *word) {
    // Check if the word is non-empty and contains only alphabetic characters
    if (strlen(word) < 2) {
        printf("Invalid word %s\n", word);
        return false;
    }
    for (size_t i = 0; i < strlen(word); i++) {
        if (!isalpha(word[i])) {
            printf("Invalid word %s\n", word);
            return false; // Invalid character found
        }
    }
    return true;
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
    while (fgets(line, MAX_LENGTH, stdin) != 0)
    { 
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, "0") == 0)
        {
            break;
        }
        if(!IsValidWord(line)){
            return;
        }
        FileOpenError(&file, FILENAME, "a");
        if (!WordAlreadyInFile(FILENAME, line))
        {
            fprintf(file, "%s\n", line);
            printf("Word %s has been added.\n", line);
        }
        else if( strlen(line) <= 0)
        {
            printf("Invalid word. Please enter a non-empty word.\n");
        }
        printf("If you want to continue adding write the words. Otherwise 0: ");
        CloseFile(&file);
    }
}

/**
 * @brief Counts the total number of lines in the given file.
 * 
 * This function reads through the file line by line, incrementing a counter for each line encountered.
 * The file is expected to be open before passing it to this function. The function returns the total 
 * number of lines found in the file.
 * 
 * @param file Pointer to the open file where the lines are to be counted. 
 * 
 * @return The total number of lines in the file.
 */
int CountWordsInFile(FILE *file){
    int NumOfLines = 0; char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        NumOfLines++;
    }
    return NumOfLines;
}

/**
 * @brief Prints the current state of the hangman based on the number of incorrect guesses.
 * 
 * This function displays an ASCII art representation of the hangman, with different stages
 * corresponding to the number of incorrect guesses made by the player. The final state indicates
 * that the game is over.
 * 
 * @param state The current number of incorrect guesses (ranging from 0 to 6).
 */
void PrintState(int state) {  // ASCII art by ChatGPT <33
    switch (state) {
        case 0:
            printf("   +---+\n");
            printf("   |   |\n");
            printf("       |\n");
            printf("       |\n");
            printf("       |\n");
            printf("       |\n");
            printf("==========\n");
            break;
        case 1:
            printf("   +---+\n");
            printf("   |   |\n");
            printf("   O   |\n");
            printf("       |\n");
            printf("       |\n");
            printf("       |\n");
            printf("=========\n");
            break;
        case 2:
            printf("   +---+\n");
            printf("   |   |\n");
            printf("   O   |\n");
            printf("   |   |\n");
            printf("       |\n");
            printf("       |\n");
            printf("==========\n");
            break;
        case 3:
            printf("   +---+\n");
            printf("   |   |\n");
            printf("   O   |\n");
            printf("  /|   |\n");
            printf("       |\n");
            printf("       |\n");
            printf("==========\n");
            break;
        case 4:
            printf("   +---+\n");
            printf("   |   |\n");
            printf("   O   |\n");
            printf("  /|\\  |\n");
            printf("       |\n");
            printf("       |\n");
            printf("==========\n");
            break;
        case 5:
            printf("   +---+\n");
            printf("   |   |\n");
            printf("   O   |\n");
            printf("  /|\\  |\n");
            printf("  /    |\n");
            printf("       |\n");
            printf("==========\n");
            break;
        case 6:
            printf("   +---+\n");
            printf("   |   |\n");
            printf("   O   |\n");
            printf("  /|\\  |\n");
            printf("  / \\  |\n");
            printf("       |\n");
            printf("==========\n\n");
            break;
    }
}

/**
 * @brief Prompts the user to enter a single letter and checks if it's valid.
 * 
 * This function reads a single character from the user input using `fgetc`, 
 * then checks if the input is a valid alphabetic letter (A-Z or a-z). It also 
 * ensures that no extra characters (like newline or multiple characters) are entered.
 * 
 * @return The valid letter entered by the user, or '\0'  if the input was invalid.
 */
char GetValidLetter() {
    char ch;    int extra;
    ch = fgetc(stdin);

    if (isalpha(ch)) {
        // Check if there are any extra characters
        extra = fgetc(stdin);
        if (extra != '\n' && extra != EOF) {
            while (fgetc(stdin) != '\n' && fgetc(stdin) != EOF);
            printf("Invalid input. Please enter only one letter.\n");
            return '\0';  
        }
        return ch;  // Return the valid letter
    } else {
        while (fgetc(stdin) != '\n' && fgetc(stdin) != EOF);
        printf("Invalid input. Please enter only one letter.\n");
        return '\0'; 
    }
}

/**
 * @brief Converts the word to a board with underscores.
 * 
 * This function takes a word and replaces each letter in the word with an 
 * underscore (`_`), creating a new string that can be used as the initial
 * display for the hangman game.
 * 
 * @param WordToGuess The original word to be guessed.
 * @param board The board where each letter of the word is replaced by `_`.
 */
void ConvertToBoard(const char WordToGuess[], char board[]) {
    for (int i = 0; i < strlen(WordToGuess); i++) {
        if (isalpha(WordToGuess[i])) { 
            board[i] = '_';            // Replace with underscore
        } else {
            board[i] = WordToGuess[i]; // Preserve non-alphabetic characters
        }
    }
    board[strlen(WordToGuess)] = '\0'; 
}

/**
 * @brief Updates the game state based on the player's guess.
 * 
 * This function checks whether the player's guessed letter is present in the 
 * word to be guessed. It updates the board with the correctly guessed letters 
 * and prints the current hangman state. The function also ensures that the 
 * player enters a valid letter.
 * 
 * @param WordToGuess The word that the player is trying to guess.
 * @param board The current state of the guessed word, with unguessed letters 
 *        represented by underscores (_).
 * @param state The current state of the hangman (number of incorrect guesses).
 * @return true if the guessed letter is found in the word, false otherwise.
 */
bool ResolveState (const char WordToGuess[MAX_LENGTH], char board[MAX_LENGTH], int state){
    char letter; bool found = false;

    PrintState(state);
    if(state > 5){
        printf("Game Over!\nThe word was %s. \n", WordToGuess);
        return found;
    }

    printf("Make your guess %s      ", board);
    do {
        letter = GetValidLetter();
    } while (letter == '\0');

    // Check if the letter is in the word
    for (int i = 0; i < MAX_LENGTH; i++) {
        if (WordToGuess[i] == letter) {
            board[i] = letter;
            found = true;
        }
    }
    return found;
}

/**
 * @brief Selects a random word from the given file based on the number of lines.
 * 
 * This function selects a random line from the file, which is assumed to contain words,
 * one per line. The file is first opened in read mode, and the function reads up to the 
 * randomly selected line. The selected word is then printed to the console.
 * 
 * @param file Pointer to the file containing the words. The file is opened within the function.
 * @param NumOfLines The total number of lines (words) in the file, used to determine the range for random selection.
 */
void SelectWord(FILE *file, int NumOfLines, char selectedword[MAX_LENGTH]){
    srand(time(NULL));
    int randomLine = rand() % NumOfLines;   // Random num from array

    FileOpenError(&file, FILENAME, "r");

    for (int i = 0; i <= randomLine; i++) {
        if (fgets(selectedword, MAX_LENGTH, file) == NULL) {
            fclose(file);
        }
    }
    // Remove the newline character at the end of the selected word, if it exists
    selectedword[strcspn(selectedword, "\n")] = 0;
}

/**
 * @brief Manages the main logic for the word guessing game.
 * 
 * This function orchestrates the word guessing game by selecting a random word from the file,
 * converting it to a hidden board representation, and then allowing the player to guess letters
 * until they either win or lose. The player's state (number of wrong guesses) is updated with
 * each incorrect guess.
 * 
 * @param file Pointer to the file containing the list of words to guess from.
 */
void WordGuessing(FILE *file){
    int NumOfLines = CountWordsInFile(file);
    CloseFile(&file);
    if(NumOfLines <= 0){
        printf("No words to guess.\n");
        exit(0);
    }
    char WordToGuess[MAX_LENGTH]; char board[MAX_LENGTH]; int state = 0;
    SelectWord(file, NumOfLines, WordToGuess);
    ConvertToBoard(WordToGuess, board);
    while(state < 7 && (strcmp(WordToGuess, board))){
        if(!ResolveState(WordToGuess, board, state)){
            state++;
        }
        if(!strcmp(WordToGuess, board)){
            printf("You WON!\n");
        }
    }
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
        FileOpenError(file, FILENAME, "r");
        
        WordGuessing(*file);
        CloseFile(file);
    } 
    else if (strcmp(line, "add\n") == 0) {
        WordInsertion(*file, line);
    } else {
        printf("Looks like you do not want to do any of that. Bye!\n");
        return false;
    }
    printf("Do you want to continue play or add ?");
    return true;
}