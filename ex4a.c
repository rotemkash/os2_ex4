/*
======================================================================
File: ex4a.c - File mapping to memory
======================================================================
Written by: Rotem Kashani, ID = 209073352,   login = rotemkash
            David Koplev, ID = 208870279 ,    login = davidkop

The program is a file censoring tool.
It accepts command-line arguments for a filename, a string to censor, and a
replacement character. The program utilizes memory mapping to efficiently 
search for occurrences of the specified string in the file and replaces them 
with the given character. It then syncs the modified file to disk and releases 
the memory mapping.

input: 
Filename: The name of the file that needs to be censored.
String to Censor: The specific string that should be replaced or
censored in the file.
Replacement Character: The character that will be used to substitute 
the censored string in the file.

output:
The program modifies the specified file by replacing occurrences of
the given string with the provided replacement character, and then saves the 
modified file to disk.

Compile: gcc -Wall ex4a.c -o ex4a

Run: ./ex4a "file name" "string to censor" "replacement character"

*/

//-------include section---------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//------prototypes---------------------

void censorFile(const char* filename, const char* stringToCensor, 
char replacementChar);

//---------main section----------------

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <file_name>\n", argv[0]);
		exit(EXIT_FAILURE);
    }

    const char* filename = argv[1]; //The name of the file to censor.
    const char* stringToCensor = argv[2]; //The string to censor in the file.
    char replacementChar = argv[3][0]; //The character to use as a replacement 
									   //for censored strings.

    censorFile(filename, stringToCensor, replacementChar);

    return EXIT_SUCCESS;
}
//-----------functions-------------------


//------------censorFile------------------
void censorFile(const char* filename, const char* stringToCensor,
 char replacementChar) {
    // Open the file for reading and writing
    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        printf("Error opening the file.\n");
        return;
    }

    // Retrieve file information
    struct stat fileStat;
    if (fstat(fd, &fileStat) == -1) {
        printf("Error getting file information.\n");
        close(fd);
        return;
    }

    // Map the file to memory
    char* fileData = mmap(NULL, fileStat.st_size, PROT_READ | PROT_WRITE,
     MAP_SHARED, fd, 0);
    if (fileData == MAP_FAILED) {
        printf("Error mapping the file to memory.\n");
        close(fd);
        return;
    }

    // Perform string censorship
    size_t strLen = strlen(stringToCensor);
    size_t fileSize = fileStat.st_size;

    for (size_t i = 0; i < fileSize - strLen; i++) {
        if (strncmp(&fileData[i], stringToCensor, strLen) == 0) {
            memset(&fileData[i], replacementChar, strLen);
            
        }
    }

    // Sync file changes to disk
    if (msync(fileData, fileStat.st_size, MS_SYNC) == -1) {
        printf("Error syncing file changes to disk.\n");
    }

    // Unmap the file from memory
    if (munmap(fileData, fileStat.st_size) == -1) {
        printf("Error unmapping the file from memory.\n");
    }

    // Close the file
    close(fd);
}
