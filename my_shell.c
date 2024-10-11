#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "include/shell.h"
#include "include/builtin.h"
#include "include/command.h"


#define MAX_INPUT_SIZE 1024


int history_count;
char *history[MAX_RECORD_NUM];
void shell();
void split_input(char *input, char **args);
int command_processor(char **args, list_head* head);

int main(int argc, char *argv[])
{
	history_count = 0;
	for (int i = 0; i < MAX_RECORD_NUM; ++i)
    	history[i] = (char *)malloc(BUF_SIZE * sizeof(char));

	shell();

	for (int i = 0; i < MAX_RECORD_NUM; ++i)
    	free(history[i]);

	return 0;
}

void shell(){
	char input[MAX_INPUT_SIZE];
	char input_cpy[MAX_INPUT_SIZE];
	char *args[MAX_ARG_COUNT];
	list_head* head = new_head();

	while(1){
		printf(">>> $ ");
		if (fgets(input, sizeof(input), stdin) == NULL) {
		    perror("fgets failed");
		    continue;
		}		
		input[strcspn(input, "\n")] = 0;		
		strncpy(input_cpy, input, MAX_INPUT_SIZE);
		
		split_input(input, args);
		
		if(!args[0])
			continue;
		if(strcmp(args[0],"replay") != 0)
			insert_tail(head, input_cpy);
		if(command_processor(args, head) < 0)
			break;
			
		//reset args
		for(int i = 0; i < MAX_ARG_COUNT; i++){
			args[i] = NULL;
		}
	}
}

int command_processor(char **args, list_head* head){	
	if(strcmp(args[0],"exit") == 0){
    	return -1;
	}else if(strcmp(args[0],"help") == 0){
		help();
		return 0;
	}else if(strcmp(args[0],"cd") == 0){
		cd(args);
		return 0;
	}else if(strcmp(args[0],"pwd") == 0){
		pwd();
		return 0;
	}else if(strcmp(args[0],"echo") == 0){
		echo(args);
		return 0;
	}else if(strcmp(args[0],"record") == 0){
		print_queue(head);
		return 0;
	}else if(strcmp(args[0],"replay") == 0){
		char *input = replay(args, head);
		split_input(input, args);
		command_processor(args, head);
	}else{
		return 0;
	}
}

void split_input(char *input, char **args){
	int i = 0;
	args[i] = strtok(input, " ");
	while (args[i] != NULL && i < MAX_ARG_COUNT - 1) {
	    args[++i] = strtok(NULL, " ");
	}
	args[i] = NULL;
}

