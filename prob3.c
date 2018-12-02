#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Instructions
// 1. Opens the file named as the command-line argument for reading
// 2. Reads a value from the file
// 3. Forks a child process that calls unlink() to delete the file
// 4. Waits on the child process
// 5. Tries to read the now-deleted file

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        printf("Forgot file name\n");
        exit(0);
    }
    
    // 1. Opens the file names as the command-line argument for reading
    FILE *file;
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Unable to open file: %s\n", argv[1]);
        exit(1);
    }
    
    // 2. Reads a value from the file
    char readVal[1];
    if (fgets(readVal, 1, file) == NULL)
    {
        fprintf(stderr, "Unable to read value from file: %s\n", argv[1]);
        exit(2);
    }
    // 3. Forks a child process that calls unlink() to delete the file
    if (fork() == 0)
    {
        unlink(argv[1]);
        exit(0);
    }
    else
        wait(NULL); // 4. Waits on the child process

    // 5. Tries to read the now-deleted file
    if (fgets(readVal, 1, file) == NULL)
        printf("Unable to read from file: %s\n", argv[1]);
    else
        printf("Able to read from file: %s\n", argv[1]);
    
    fclose(file);
    return 0;
}