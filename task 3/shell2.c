#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <stdbool.h>

void signal_handler(int sig){
    write(STDOUT_FILENO,"You typed Control-C!",20);
    printf("\n");
}

int main()
{
    char command[1024];
    char *token;
    char *outfile;
    char *prompt_name = "hello";
    int i, fd, amper, redirect, retid, status;
    char *argv[10];
    bool status_command = false;

    while (1)
    {
        if(signal(SIGINT, signal_handler)){
            continue;
        }
        printf("%s: ", prompt_name); // print prompt
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0';

        /* parse command line */
        i = 0;
        token = strtok(command, " ");
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        argv[i] = NULL;

        /* Is command empty */
        if (argv[0] == NULL)
            continue;

        /* Does command line end with & */
        if (!strcmp(argv[i - 1], "&"))
        {
            amper = 1;
            argv[i - 1] = NULL;
        }
        else
        {
            amper = 0;
        }

        // redirect to stderr
        if (!strcmp(argv[i - 2], "2>"))
        {
            redirect = 2;
            argv[i - 2] = NULL;
            outfile = argv[i - 1];
        }
        // redirect to file
        else if (!strcmp(argv[i - 2], ">"))
        {
            redirect = 1;
            argv[i - 2] = NULL;
            outfile = argv[i - 1];
        }
        else
        {
            redirect = 0;
        }

        // change prompt name
        if (!strcmp(argv[0], "prompt") && !strcmp(argv[1], "=") && argv[2] != NULL)
        {
            prompt_name = argv[2];
            status_command = true;
        }

        if (!strcmp(argv[0], "echo"))
        {
            if (argv[1] == "$?")
            {
            }
            else
            {
                int k = 1;
                while (argv[k] != NULL)
                {
                    printf("%s ", argv[k++]);
                }
                printf("\n");
                continue;
            }
            status_command = true;
        }

        // change directory
        if (!strcmp(argv[0], "cd"))
        {
            char path[100];

            if (!chdir(argv[1]))
            {
                // Save the current directory in path
                if (getcwd(path, sizeof(path)) == NULL)
                {
                    perror("getcwd error! - path is NULL");
                }
            }
            else
            {
                perror("chdir error!"); // not succeed to change the path
            }

            continue;
        }

        if(!strcmp(argv[0],"quit")){
            
            exit(0);
        }


        /* for commands not part of the shell command language */

        if (fork() == 0)
        {
            /* redirection of IO ? */

            // for output to terminal
            if (redirect == 0)
            {
            }
            // write to file
            else if (redirect == 1)
            {
                fd = creat(outfile, 0660);
                close(STDOUT_FILENO);
                dup(fd);
                close(fd);
            }
            // write to stderr
            else if (redirect == 2)
            {
                fd = creat(outfile, 0660); // equal to open(outfile,O_WRONLY|O_CREAT|O_TRUNC,mode)
                close(STDOUT_FILENO);
                dup(fd);
                close(fd);
            }

            execvp(argv[0], argv);
        }

        /* parent continues here */
        if (amper == 0)
        {
            retid = wait(&status);
        }
    }
}
