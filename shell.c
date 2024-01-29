// command to build executable: 
// gcc -o shell shell.c
// command to run executable:
// ./shell
// Rounak Nayee

//Creating a shell with commands
// help, pwd, ls, cd, mkdir, rmdir, cat, touch, grep, | in C language
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_ARGS 100
#define BUFFER_SIZE 1024

//function prototypes
void execute_commands(char *args[]);
void cd(char *args[]);    //function prototypes
void help();    //function prototypes
void ls(char *args[]);   //function prototypes
void pwd();    //function prototypes
void mkdir_c(char *args[]);    //function prototypes
void rmdir_c(char *args[]);    //function prototypes
void cat(char *args[]);         //function prototypes
void touch(char *args[]);     //function prototypes
void grep(char *args[]);    //function prototypes
int execute_pipe(char *args[], int pipe_pos);   //function prototypes


char *read_line(void) {   //this function reads line from user inpur text
    char *line = NULL;    //declaring pointer variable line and addressing as null
    size_t buffer_size = 0;    //declaring variable buffer size address to 0
    getline(&line, &buffer_size, stdin);    //
    return line;   //returns pointer line that point towards input
}

char **parse(char *line) {    //this function parses the user input and returns string array and its commands and arguments
   //maximum arguments can be 100
    char **args = malloc(MAX_ARGS * sizeof(char*));  //memory allocating array of pointers and return pointers
    char *token = strtok(line, " \n");   //tokenizing with space and new line
    int i = 0;   //declaring variable to keep track
    while (token != NULL) {   //continues the loop until token is not null
        args[i] = token;    //argument ith element is assigned to the token and
        //increments i and moves on to the next token with the help of strtok
        i++;
        token = strtok(NULL, " \n");
    }   //set ith element in the array to null
    args[i] = NULL;
    return args;  //returns pointer tokens
}

int main() {   //main function runs in loops by taking arguments and parsing it
    char *line = NULL;    //declaring variable pointers
    char **args = NULL;   //declaring variable pointers
    int run = 1;

    while (run) {              //while runs when its true and reads input line that user inputs and parse into arguments
        printf("Rex's shell> ");
        fflush(stdout);

        // reading input user
        line = read_line();

        // parsing into arguments
        args = parse(line);

        if (args[0] == NULL) {   //in case if user enters empty string or nothing,
            free(line);        //it frees the allocated memory
            free(args);
            continue;    //iterates thru current all the way and starts new iteration so asks user to enter again
        }

       //comparing user input string, if they entered exit
                if (strcmp(args[0], "exit") == 0) {
            run = 0;  // loop becomes 0 and exits
        } else {      //else runs the executing commands accordingly as user asks
            execute_commands(args);
        }

        // Frees the allocated memory
        free(line);
        free(args);
    }
//return
    return 0;
}


//this function executes the command depending on the user input
//It has cd, help, ls, pwd, mkdir, rmdir commands that accounts for
void execute_commands(char *args[]) {

    if (args[0] == NULL) {  //if first element is null or nothing entered
        //just returns
        return;
    }

    int i = 0, pipe_pos = -1;

    // Check for pipe in command
    while (args[i] != NULL) {
        if (strcmp(args[i], "|") == 0) {
            pipe_pos = i;
            break;
        }
        i++;
    }

    // If pipe found, execute pipe command
    if (pipe_pos != -1) {
        execute_pipe(args, pipe_pos);
        return;
    }
    //comparing strings
    //checking first argument in given commands
    if (strcmp(args[0], "cd") == 0) {   //if in first element, user entered cd, executes
        cd(args);
    } else if (strcmp(args[0], "help") == 0) {   //else if in first element, user entered help, executes
        help();
    } else if (strcmp(args[0], "ls") == 0) {    //else if in first element, user entered ls, executes
        ls(args);
    } else if (strcmp(args[0], "pwd") == 0) {   //else if in first element, user entered pwd, executes
        pwd();
    } else if (strcmp(args[0], "mkdir") == 0) {   //ese if in first element, user entered mkdir, executes
        mkdir_c(args);
    } else if (strcmp(args[0], "rmdir") == 0) {   //else if in first element, user entered rmdir, executes
        rmdir_c(args);
    } else if (strcmp(args[0], "cat") == 0) {    // else if in first element, user entered cat, execute
        cat(args);
     } else if (strcmp(args[0], "touch") == 0) {
        touch(args);
    } else if (strcmp(args[0], "grep") == 0) {  //checks if the user entered 'grep' command
        grep(args);
    } else if (strcmp(args[0], "exit") == 0) {

    } else {
        //else prints the error message if the command is different from above given commands
        printf("%s: This Command is not found.\n", args[0]);
    }
}

void cd(char *args[]) {   //this function changes the directory to the addressed directory
    if (args[1] == NULL) {  //if directory is not found or specified
        // then you go to home directory
        chdir(getenv("HOME"));
    } else {  //else it executes and changes the directory
        if (chdir(args[1]) == -1) {    //if chdir returns -1
            perror("cd");    //prints the error handling message
        }
    }
}

void help() {    //this function prints all available commands list and their uses
    printf("    List of the commands:\n");
    //cd
    printf("    cd command ---  Change the shell working directory. Change the current directory to DIR. The default DIR is the value of the HOME shell variable.\n");
    //help
    printf("    help command --- Displays the help message.\n");
    //ls
    printf("    ls command --- Lists the directory contents.\n");
    //pwd
    printf("    pwd command --- Prints the name or the path of the current working directory.\n");
    //mkdir
    printf("    mkdir command --- Makes a new directory or a folder.\n");
    //rmdir
    printf("    rmdir command --- Removes a directory or a folder.\n");
    //cat
    printf("    cat command --- Display the information from the file.\n");
    //touch
    printf("    touch command --- Creates a new file in the directory.\n");
    //grep
    printf("    grep command --- Prints the line containing the user entered word");
    //to exit simply type exit
    printf("    exit --- exiting the shell.\n");
}

void ls(char *args[]) {   //this function lists all the files that available in the current directory or folder
    DIR *dirp;
    struct dirent *dp;

    if (args[1] == NULL) {    //if checks for null,
        dirp = opendir(".");   //if null, then lists current directory
    } else {
        dirp = opendir(args[1]);   //else if not null, then lists directory according to the user
    }

    if (dirp == NULL) {    //dir is null,
        perror("ls");    //prints the error handling message of the system and returns
        return;
    }

    while ((dp = readdir(dirp)) != NULL) {    //while reading not null value
        printf("%s\n", dp->d_name);    //prints all the file names
    }
     //closes the directory
    closedir(dirp);
}

void pwd() {    //this function outputs the current working directory path
    char current_workdirectory[1024];   //declaring variable
    //uses built-in function getcwd to get the path of current working directory and store it in the array
    if (getcwd(current_workdirectory, sizeof(current_workdirectory)) != NULL) {
        printf("%s\n", current_workdirectory);   //prints the path thru printf
    } else {
        perror("pwd");   //else it prints the error handling message accordingly
    }
}

void mkdir_c(char *args[]) {    //mkdir command checks the input and makes a directory, and handles the error if the system isn't able to create a directory
if (args[1] == NULL) {      //if first argument is null, then it prints the error message
printf("mkdir: missing an argument\n");
} else {    //else the system checks second element which is the name of a new directory
if (mkdir(args[1], 0777) == -1) {   //0777 is the permissions for new directory  and it prints any error according to when it returns -1
perror("mkdir");     //prints the perror mkdir message
}
}
}

void rmdir_c(char *args[]) {   //rmdir command checks the input and see if the directory exists, and deletes it or handles the error
if (args[1] == NULL) {     //if the first argument is null, then it prints the error message
printf("rmdir: missing an argument\n");
} else {        //else it runs if it has a value
if (rmdir(args[1]) == -1) {     //if rmdir argument returns -1, that means the directory cannot be deleted or does not exist
perror("rmdir");     //prints the perror message

}
}
}

void cat(char *args[]) {   //This function prints all the information from the selected file as an output
    if (args[1] == NULL) {  // if arg is null, lets user know to enter a file name
        printf("\033[0;31mError: User input: cat... [filename]\n\033[0m");   //prints error message in red
        return;      //exits the function
    }

    FILE *file = fopen(args[1], "r");  //open in read mode

    if (file == NULL) {  // if the file was not found
        printf("\033[0;31mError: Couldn't open or find the file '%s'\n\033[0m", args[1]);   //prints the error message in red
        return;
    }

    char buffer[BUFFER_SIZE];     //creates an array
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {  // reads the file until reaches the end and once nothing to read, it ends
        printf("%s", buffer);  // print the line stores in buffer array
    }
    fclose(file);  // closing the file

    printf("\033[0;32mWhole File displayed successfully.\n\033[0m");    //prints the successful message in green
}

void touch(char *args[]) {    //this function creates a new file
    if (args[1] == NULL) { // if second arg is null, prints the error message
        printf("\033[0;31mtouch: Missing an argument\n\033[0m");   //prints error msg in red
    } else {
        int i = 1;    //else i counter 1
        while (args[i] != NULL) {   //if arg is not null, goes over remaining args
            int fd = open(args[i], O_CREAT | O_EXCL, 0644); // creating new file  and sets of flags and permissions
            if (fd == -1) {
                printf("\033[0;31mError...Try again\n");
                perror(args[i]); // print system error message if the file already is there in red
                printf("\033[0m");

            } else {
                close(fd); // closing descriptor of file
                printf("\033[0;32mNew File called '%s' created successfully.\n\033[0m", args[i]);   //prints successful message in green
            }
            i++;  //incrementing to next arg in array
        }
    }
}

void grep(char *args[]) {    //this function prints all the lines containing the searching word entered by user and the file name.
    if (args[1] == NULL || args[2] == NULL) {  //checks for if user has 2 args and if not
        printf("\033[0;31mError: User input: grep... [pattern]... [file]\n\033[0m");  //if not then prints error message in red
        return;
    }

    char *patt = args[1];  //pointers for pattern
    char *fname = args[2];  //pointers for filename

    FILE *file = fopen(fname, "r");  //opening file for reading
    if (file == NULL) {  //if the file is not found
        printf("\033[0;31mError: Couldn't find or open the file '%s'.\n\033[0m", fname);  //prints error message if file not found in red
        return;
    }

    char line[BUFFER_SIZE];  //create buffer array to store line from file
    while (fgets(line, BUFFER_SIZE, file)) {  //reading every line from  file and checks for matching pattern
        if (strstr(line, patt) != NULL) {  //if pattern found on line
            printf("%s", line);  //prints that line with pattern
        }
    }

    fclose(file);  //closing file
    printf("\033[0;32mSuccessfully printed lines containing the inputted string.\n\033[0m");   //prints successful message in green
}

int execute_pipe(char *args[], int pipe_pos) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }

    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid == 0) { // Child process
        close(pipefd[0]); // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[1]);

        args[pipe_pos] = NULL; // Terminate first command
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        int pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            return -1;
        }

        if (pid2 == 0) { // Second child process
            close(pipefd[1]); // Close unused write end
            dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe
            close(pipefd[0]);

            execvp(args[pipe_pos + 1], &args[pipe_pos + 1]);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else {
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(pid, NULL, 0);
            waitpid(pid2, NULL, 0);
        }
    }
    return 0;
}
