<h1> Task 3 - "stree" </h1>

<h3> Implementation of a shell program in the C language </h3>

**A program consisting of:**

- shell.c
- Makefile


**The implementation includes the following commands:**

- ls - Prints to the screen the names of the files and folders in the current folder
- ls -l - Prints to the screen the names of the files and folders that are in the current folder in detail: Permissions - User, Group, Others, when the files were last modified.
- echo - screen printing
- echo $? - Print the status of the last executed command.
- !! - A command that repeats the last command executed
- Rename - The option to rename the prompt "hello: prompt = new_name"
- cd - Change the location in the file tree: "cd folder_name, "cd .."
- SIGINT handler - ctrl + C will not take us out of the program
- quit - Exit the shell program


Also the shell program supports "&" which causes the program not to wait for the previous command we ran and immediately displays the prompt again