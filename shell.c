#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

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

void normalizeCommand(char *input, char **args)
{
    while (*input != '\0')
    {
        while (isDelimiterCharacter(input))
            *input++ = '\0';
        *args++ = input;
        while (!isDelimiterCharacter(input))
            input++;
    }
    *args = '\0';
}

void executeCommand(char **argv)
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
    { /* fork a child process           */
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0)
    { /* for the child process:         */
        if (execvp(*argv, argv) < 0)
        { /* execute the command  */
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else
    {                                /* for the parent:      */
        while (wait(&status) != pid) /* wait for completion  */
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
        if (strstr(input, "exit") != NULL)
        {
            exit(0);
        }
        executeCommand(args);
    }
}