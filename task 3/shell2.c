#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <stdbool.h>

typedef struct node{
    char name[10];
    char value[10];

}node;

void signal_handler(int sig){
    write(STDOUT_FILENO,"You typed Control-C!",20);
    printf("\n");
}

int if_exists_in_array(char* target , node n [],int size_of_array){
    
    for(int index=0; index < size_of_array; index++){
        if(!strcmp(n[index].name,target)){
            return index;
        }
    }
    return -1;
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
    char *last_command[10];
    int last_command_size = 0;
    node variable[100];
    int index_of_variable = 0;

    while (1)
    {
        signal(SIGINT, signal_handler);
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


        //copy last command to execute last command
        if(!strcmp(argv[0] , "!!")){
            for(int v=0;  v < last_command_size; v++){
                argv[v] =  (char*)malloc(strlen(last_command[v]) + 1);
                strcpy(argv[v], last_command[v]);
             }
        }

        //copy my current command
        else if(strcmp(argv[0] , "!!")){
            for(int v=0; v < i && argv[v]!=NULL; v++){
                last_command[v] =  (char*)malloc(strlen(argv[v]) + 1);
                strcpy(last_command[v] , argv[v]);
                last_command_size = v+1;
            }
        }


        /* Is command empty */
        if (argv[0] == NULL){
            continue;
        }


        
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
                ///
            }
            else
            {
                int k = 1;
                while (argv[k] != NULL)
                {   
                    int exists_val = if_exists_in_array(argv[k],variable,100);
                    if(exists_val != -1){
                        printf("%s ", variable[exists_val].value);
                    }
                    else{
                        printf("%s ", argv[k]);
                    }
                    k++;
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
        if(argv[0][0] == '$' && !strcmp(argv[1],"=") && argv[2] != NULL && argv[3] == NULL){ ////////////////////////
            int answer_exists = if_exists_in_array(argv[0],variable,100);
            if(answer_exists != -1){
                strcpy(variable[answer_exists].value,argv[2]);
            }
            else{
               node *n0 = NULL;
               n0 = (struct node *)malloc(sizeof(struct node));
               strcpy(n0->name , argv[0]);
               strcpy(n0->value , argv[2]);
               variable[index_of_variable] = *n0;
                index_of_variable++;
            }
        }
        
        if(!strcmp(argv[0],"read") && argv[1] != NULL && argv[2] == NULL){
            char user_value[20];
            scanf("%[^\n]%*c", user_value); //get value from user
            //fgets(user_value, 20, stdin);
            char sign[50] = "$";
            strcat(sign , argv[1]);
            int answer_exists = if_exists_in_array(sign,variable,100);
            
            if(answer_exists != -1){
                strcpy(variable[answer_exists].value,user_value);
            }
            else{
               node *n0 = NULL;
               n0 = (struct node *)malloc(sizeof(struct node));
               strcpy(n0->name , sign);
               strcpy(n0->value ,user_value);
               variable[index_of_variable] = *n0;
               index_of_variable++;
            }
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
