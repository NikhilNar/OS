#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
/*
The function checks if the current character in the input is a space or newliner or tab or string terminator
*/
int isDelimiterCharacter(char *input)
{
    switch (*input)
    {
    case ' ':
        return 1;
    case '\n':
        return 1;
    case '\t':
        return 1;
    case '\0':
        return 1;
    default:
        return 0;
    }
}

/*
The function will split the input into arguments
eg. if the input is "cat shell.c" then args[0] will store the address of string "cat" and args[1] will store the address of string "shell.c"
*/
void normalizeCommand(char *input, char **args)
{
    while (*input != '\0')
    {
        // mark delimiter characters as string termination so that args can reference only the word in the input
        while (isDelimiterCharacter(input))
            *input++ = '\0';
        *args++ = input;
        while (!isDelimiterCharacter(input))
            input++;
    }
    *args = '\0';
}
/*
The function creates a child process and executes the input command
*/
void executeCommand(char **args)
{
    int status;

    // fork new child process
    pid_t pid = fork();

    if (pid == 0)
    {
        // execute the command with the arguments
        if (execvp(*args, args) < 0)
        {
            printf("Failed to execute the command!\n");
            exit(1);
        }
    }
    else if (pid < 0)
    {
        printf("Child process did not forked!\n");
        exit(1);
    }
    else
    {
        // The child process pid is known to the parent process
        // Wait the parent process till the child process has completed execution of the command.
        while (wait(&status) != pid)
            ;
    }
}

void main()
{
    char input[100];
    char *args[10];

    while (1)
    {
        printf("-> ");
        gets(input);
        printf("\n");
        normalizeCommand(input, args);
        // if the string has a substring as exit then exit the shell
        if (strstr(input, "exit") != NULL)
        {
            exit(0);
        }
        executeCommand(args);
    }
}