/*
======================================================================
File: ex4b.c - Review of Guides
======================================================================
Written by: Rotem Kashani, ID = 209073352,   login = rotemkash
            David Koplev, ID = 208870279 ,    login = davidkop
This program gets a name of a file that exists in the same dirctory 
and checks if there is a hard link or not.

input: file name

output: if there is hard link, the output is "hard link found" and the name 
of the file with hard link.
if there isn't hard link, the output is "No hard link".

Compile: gcc -Wall ex4b.c -o ex4b

Run: ./ex4b "file name"

*/

//-------include section---------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>

//------define section---------------------
#define MAX_HARD_LINKS 100

//------prototypes---------------------
void check_link(const char* fd, char** linkedFiles, int* count);

//---------main section----------------
int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* filename = argv[1];
    char* linkedFiles[MAX_HARD_LINKS];
    int count = 0;

    check_link(filename, linkedFiles, &count);

    if (count > 0) {
        printf("Hard link found:\n");
        for (int i = 0; i < count; i++) {
            printf("%s\n", linkedFiles[i]);
            free(linkedFiles[i]);  // Free the dynamically allocated memory
        }
    } else {
        printf("No hard link\n");
    }

    return EXIT_SUCCESS;
}

//-----------functions-------------------

//------------check_link------------------
void check_link(const char* fd, char** linkedFiles, int* count) {
    // Check if the file exists and can be accessed
    if (access(fd, F_OK) == -1) {
        printf("File does not exist or cannot be accessed\n");
        return;
    }

    struct stat fileStat;

    // Get the file information, including the number of hard links
    if (lstat(fd, &fileStat) == -1) {
        printf("Error getting file information\n");
        return;
    }

    // Check if the file has more than one hard link
    if (fileStat.st_nlink > 1) {
		// Store the name of the hard-linked file
        linkedFiles[*count] = strdup(fd);  
        (*count)++;  // Increment the count of hard-linked files
    }
}
