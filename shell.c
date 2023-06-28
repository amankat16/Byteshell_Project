#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LENGTH 500
#define MAX_SIZE 50

char* history[MAX_SIZE];
int count = 0;

void Read_line(char* input){
    fgets(input, MAX_LENGTH, stdin);
    input[strcspn(input, "\n")] = '\0';  
}

void History(const char* input) {
    if(count<MAX_SIZE){
        history[count] = strdup(input);
        count++;
    }else {
        free(history[0]);
        for (int i=1; i<MAX_SIZE; i++) {
            history[i-1] = history[i];
        }
        history[MAX_SIZE-1] = strdup(input);
    }
}

void Display() {
    for (int i=0; i<count; i++) {
        printf("%d. %s\n",i+1,history[i]);
    }
}

void Print() {
    printf("Enter the command ");
}

void Execute(char* input) {
    pid_t temp = fork();

    if (temp == -1) {
        printf("Error: Failed Fork\n");
        return;
    } else if (temp == 0) {
        char* temp2[MAX_LENGTH];
        int temp2_count = 0;
        char* token = strtok(input, " ");
        while (token != NULL) {
            temp2[temp2_count] = token;
            temp2_count++;
            token = strtok(NULL, " ");
        }
        temp2[temp2_count] = NULL;

        if (execvp(temp2[0], temp2) == -1) {
            printf("Error: Failed in execution of command\n");
            exit(EXIT_FAILURE);
        }
    } else {
        wait(NULL);
    }
}

void cd(char* directory) {
    if (directory != NULL) {
        if (chdir(directory) != 0) {
            printf("Directory not found: %s\n", directory);
        }
    } else {
        printf("Missing argument for 'cd' command\n");
    }
}

void Exit() {
    exit(0);
}

void Caller(){
    pid_t parent_pid = getppid();
    printf("Parent Process ID: %d\n", parent_pid);
}

void Help() {
    printf("Shell Program Help:\n");
    printf("cd <directory>    - Change the directory\n");
    printf("Exit              - Exit the shell\n");
    printf("Help              - Display this help message\n");
    printf("Execute <command> - Execute a command\n");
    printf("Wait              - Wait for child processes to finish\n");
    printf("Fork              - Create a child process\n");
    printf("History           - Display command history\n");
    printf("Caller            - Display the caller process ID\n");
    printf("bg <command>      - Execute a command in the background\n");
}

void bg(char* input) {
    pid_t temp = fork();
    if (temp == -1) {
        printf("Error: Failed fork\n");
        return;
    } else if (temp == 0) {
        Execute(input);
        exit(0);
    } else {
        printf("Background process started with PID: %d\n", temp);
    }
}
 void Execution(char* input){
        if (strcmp(input, "Exit") == 0) {
            Exit();
        } else if (strcmp(input, "Help") == 0) {
            Help();
        } else if (strncmp(input, "Execute", 7) == 0) {
            char* cmd = input + 8;  
            Execute(cmd);
        } else if (strcmp(input, "Wait") == 0) {
            wait(NULL);
        } else if (strcmp(input, "Fork") == 0) {
            fork();
        } else if (strcmp(input, "History") == 0) {
            Display();
        } else if (strncmp(input, "cd", 2) == 0) {
            char* directory = input + 3;  
            cd(directory);
        } else if (strcmp(input, "Caller") == 0) {
            Caller();
        } else if (strncmp(input, "bg", 2) == 0) {
            char* cmd = input + 3;  
            bg(cmd);
        } 
        else{
            printf("Error: Command not found\n");
        }
}
int main() {
    char input[MAX_LENGTH];
    while (1) {
        Print();
        Read_line(input);
        History(input);
        Execution(input);
    }
    return 0;
}
