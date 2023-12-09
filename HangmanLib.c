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
