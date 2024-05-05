
    wish shell, phase 1
*/

#define _GNU_SOURCE
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

//----------------------------------------------------------
// DEBUG_PRINT macro

// Change '1' to '0' before running automated tests!
#define DEBUG 1

#if defined(DEBUG) && DEBUG > 0
#define DEBUG_PRINT(fmt, args...) fprintf(stderr, "DEBUG:%d:%s(): " fmt, __LINE__, __func__, ##args)
#else
#define DEBUG_PRINT(fmt, args...) /* Do nothing */
#endif

//----------------------------------------------------------
// Global variables

#define PATH_MAX 10
#define ARG_MAX 10

char errmsg[] = "An error has occurred\n";
char *pathv[PATH_MAX];
int pathc;
char *myargv[ARG_MAX];
int myargc;

//----------------------------------------------------------
// Function prototypes

void parse_cmd(char *cmd);
bool is_builtin();
void do_builtin();
void do_cmd();
bool whitespace();

// ---------------------------------------------------------

int main(int argc, char *argv[])
{
    const int LINEMAX = 1024;
    char cmd[LINEMAX]; // current command line
    FILE *infile = stdin;
    bool interactive = true; // is this an interactive shell?

    // Initially, the path contains just one element, "/bin".
    pathv[0] = strdup("/bin");
    pathc = 1;

    // batch file 2 or more; err and exit
    if (argc > 2)
    {
        fprintf(stderr, "An error has occurred\n");
        exit(1);
    }

    if (argc == 2)
    {
        // We were invoked with a batch file.
        interactive = false;

        // Use fopen() to open the batch file for reading, and set `infile` to refer to it. (done)
        infile = fopen(argv[1], "r");
        if (infile == NULL)
        {
            fprintf(stderr, "An error has occurred\n");
            exit(1);
        }
    }

    while (1)
    {
        if (interactive)
            printf("wish> ");
        if (fgets(cmd, LINEMAX, infile) == NULL)
        {
            exit(0);
        }
    // Check if the line consists only of whitespace
        if (whitespace(cmd))
            continue;
        // DEBUG_PRINT("cmd=%s", cmd);
        parse_cmd(cmd);
        if (is_builtin())
        {
            do_builtin();
        }
        else
        {
            do_cmd();
        }
    }
}

bool is_builtin()
{
    // Assuming myargv[0] contains the command name
    if (strcmp(myargv[0], "exit") == 0 || strcmp(myargv[0], "cd") == 0 || strcmp(myargv[0], "path") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void do_builtin()
{
    if (strcmp(myargv[0], "exit") == 0)
    {
        // Check if 'exit' command has any arguments; it should not have any
        if (myargc != 1)
        {
            fprintf(stderr, "An error has occurred\n");
        }
        else
        {
            // Properly exit the shell with a status code of 0
            exit(0);
        }
    }
    else if (strcmp(myargv[0], "cd") == 0)
    {
        // Check if 'cd' command has exactly one argument
        if (myargc != 2)
        {
            fprintf(stderr, "An error has occurred\n");
        }
        else
        {
            // Attempt to change the directory to the specified path
            if (chdir(myargv[1]) != 0)
            {
                // Print error if changing the directory fails
                perror("cd failed");
            }
        }
    }
    else if (strcmp(myargv[0], "path") == 0)
    {
        // If 'path' command is called without arguments, print the current path
        if (myargc == 1)
        {
            // Loop through each path in pathv and print it
            for (int i = 0; i < pathc; i++)
            {
                printf("%s", pathv[i]);
                if (i < pathc - 1)  {
                    printf(":"); // Separate paths with colons
                }
            }
            printf("\n");
        }
        else
        {
            // Set the new path from the arguments
            // First, free any existing paths in pathv to avoid memory leaks
            for (int i = 0; i < pathc; i++)
            {
                free(pathv[i]);
            }
            pathc = 0; // Reset the path count

            // Loop through each argument passed to 'path' and add it to pathv
            for (int i = 1; i < myargc && pathc < PATH_MAX; i++)
            {
                // Duplicate the string to avoid modifying the original command line argument
                pathv[pathc] = strdup(myargv[i]);
                pathc++; // Increment the path count
            }
        }
    }
    else
    {
        // This case should not occur as is_builtin should prevent it
        fprintf(stderr, "An error has occurred\n");
    }
}


bool whitespace(const char *str) {
    while (*str) {
        if (!isspace(*str)) {
            return false; // Non-whitespace character found
        }
        str++; // Move to the next character
    }
    return true; // All characters are whitespace
}


void parse_cmd(char *cmd)
{

    // Initialize argument count
    myargc = 0;
    // Print debugging information
    // DEBUG_PRINT("cmd=%s\n", cmd);
    // DEBUG_PRINT("Length of cmd=%zu\n", strlen(cmd));


    // DEBUG_PRINT("cmd=%s", cmd); //it never gets here
    // Split `cmd` into whitespace-separated arguments, storing the arguments in
    char *token = strtok(cmd, " \t\n");
    // the global array `myargv[]` and the number of arguments in global `myargc`.
    while (token != NULL && myargc < ARG_MAX - 1)
    {
        // Store the token in the argument array
        myargv[myargc++] = token;
        // Get the next token
        token = strtok(NULL, " \t\n");
    }

    // Null-terminate the argument list
    myargv[myargc] = NULL;
}







void do_cmd()
{
    // Handles running a normal (not a built-in) command.
    pid_t pid;
    int status;
    // Call fork(). In the child, find the command on the search path
    // and call execv() to execute it. Child does not return. In the parent,
    // wait for the child to exit

    //     call fork() to create a child process
    pid = fork();

    if (pid < 0)
    {
        // fork failed
        fprintf(stderr, "An error has occurred\n");
        exit(1);
    }
    else if (pid == 0)
    { // child process
        // Find the command on the search path.
        char *command = myargv[0];
        char *path = NULL;

        // Loop through each directory in the search path.
        for (int i = 0; i < pathc; i++)
        {
            char *dir = pathv[i];
            // Construct the full path to the command.
            asprintf(&path, "%s/%s", dir, command);
            // Check if the command exists at this path.
            if (access(path, X_OK) == 0)
            {
                // Execute the command.
                execv(path, myargv);
                // If execv returns, it means an error occurred.
                perror("execv");
                exit(1);
            }
            free(path);
        }
        // If we reach here, the command was not found.
        fprintf(stderr, "An error has occurred\n");
        // fprintf(stderr, "%s: command not found\n", command);
        exit(1);
    }
    else
    { // parent process
        // Wait for the child to exit.
        waitpid(pid, &status, 0);
    }
}
