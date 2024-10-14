#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 100
#define MAX_PIPES 10

void execute_command(char *cmd, int in_fd, int out_fd, int is_last_cmd, int index, int out_file_fd) {
    char *buffer[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];
    int arg_count = 0;
    char *token = strtok(cmd, " ");
    
    // Split the command into arguments
    while (token != NULL) {
    	if(strcmp(token, "<") != 0){
    		args[arg_count++] = token;
    	}        
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;  // Null-terminate the argument list
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        if (index != 0) {    // If not the first command, redirect input
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (!is_last_cmd) {   // If not the last command, redirect output
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        if(is_last_cmd && out_file_fd >= 0){ //if last command, and need to write into the file
            dup2(out_file_fd, STDOUT_FILENO);        
        }
        
        execvp(args[0], args);   // Execute the command
        perror("execvp failed"); // If execvp fails
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        /*
        if (is_last_cmd) {
            printf("PID of last command: %d\n", pid); // Print PID of the last command
        }*/
        wait(NULL);
    } else {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
}

void run_pipeline(char *commands[], int num_cmds, char *out_file) {
    int in_fd = 0;
    int pipe_fd[2];
    int out_file_fd = -1;
    if(out_file){
    	//If need to redirect to output file
    	out_file_fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    	if (out_file_fd < 0) {
            perror("out_file open failed\n");
            return;
        }
    }
    
    for (int i = 0; i < num_cmds; i++) {
        if (i < num_cmds - 1) {
            pipe(pipe_fd); // Create a new pipe
        } else {
            pipe_fd[1] = 1; // Last command outputs to STDOUT
        }
        
        // Execute the command with proper redirection
        execute_command(commands[i], in_fd, pipe_fd[1], i == num_cmds - 1, i, out_file_fd);
        if(i != num_cmds - 1){
            close(pipe_fd[1]);  // Close the write end of the pipe
        }
        in_fd = pipe_fd[0]; // Input for the next command comes from this pipe      
    }
    

    // Wait for all children to finish if not running in background
    while (wait(NULL) > 0);

    
    return;
}

void pre_execute_command(char *commands[], int num_cmds, char *out_file, int background) {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process: run the pipeline
        run_pipeline(commands, num_cmds, out_file);
        exit(EXIT_SUCCESS);  // Ensure the child exits after running the command
    } else if (pid > 0) {
    
        if(background){// If have to work at background, then need not to wait
        	printf("[Pid]: %d\n", pid);       	
        }else{ //wait the child process
        	waitpid(pid, NULL, 0);
        }
        return;
        
          // Parent does not wait for the child (background task)
    } else {
        // Fork failed
        perror("fork error");
        exit(EXIT_FAILURE);
    }
}


int cat(char* input) {
    char *commands[MAX_PIPES];
    int background = 0;
    int is_out_redir = 0;
    char *out_file = NULL;
    int num_cmds = 0;
    
    if (strstr(input, "&")) {
        background = 1; // If the command contains '&', run in background
        input[strlen(input) - 1] = '\0'; // Remove '&'
    }
    
    // Split the input by pipes
    char *token = strtok(input, "|");   
    while (token != NULL) {
        commands[num_cmds++] = token;
        token = strtok(NULL, "|");
    }
    
    // Split the input by redirect output
    if(strstr(commands[num_cmds - 1], ">")){
    	strtok(commands[num_cmds - 1], ">");
        out_file = strtok(NULL, ">");
        out_file = strtok(out_file, " ");
    }
    pre_execute_command(commands, num_cmds, out_file, background);

      
    return 0;
}

