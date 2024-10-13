#include <unistd.h> 
#include <string.h> 
#include "linked_list.h"
#define buffer_max_size 128
#define MAX_ARG_COUNT 20
#ifndef BUILTIN_H
#define BUILTIN_H

int help();
int cd(char **args);
int echo(char **args);
int record(char **args);
int mypid(char **args);
int pwd();
char* replay(char **args, list_head* head);
void print_tail(int index, FILE *file);
void print_head(int index, FILE *file);

int add(char **args);
int del(char **args);
int ps(char **args);
int start(char **args);

extern const char *builtin_str[];

extern const int (*builtin_func[]) (char **);

extern int num_builtins();

#endif


int cat(char **args){
	if(!args[1])
		return -1;
	FILE *file = fopen(args[1], "r");
	char buffer[buffer_max_size];

   	if (file == NULL) {
	   	printf("Failed to open file.\n");
	   	return 1;
   	}
	
	if(!args[2]){ //if there is no follow-up instrucions
	  	while (fgets(buffer, sizeof(buffer), file) != NULL) {
			 printf("%s", buffer);
	  	}
   	}else{
   		int i = 2;
   		int index;
   		while(args[i]){
   			if(args[i] && args[i+1] && args[i+2]){
   				
   				if(strcmp(args[i], "|") != 0){
   					break;
   				}
   				//trun third arg into int
   				index = atoi(args[i+2]) * -1;
   				//check second arg
   				if(strcmp(args[i+1], "head") == 0){
   					print_tail(index, file);
   				}else if(strcmp(args[i+1], "tail") == 0){
   					print_tail(index, file);
   				}else{
   					break;
   				}
   				i += 3;
   			}else{
   				perror("command error\n");
   				break;
   			}	
   		}
   		
   	}
   	
   	fclose(file);
   	return 0;
}
void print_tail(int index, FILE *file){
	int line_count = 1;
	fseek(file, 0, SEEK_END);
	long pos = ftell(file) - 1;
	char buffer[buffer_max_size];
	
	 while (pos > 0) {
        pos--;
        fseek(file, pos, SEEK_SET);
        if (fgetc(file) == '\n') {            
            if (line_count == index) {
            	fgets(buffer, sizeof(buffer), file);
            	printf("%s", buffer);
                return;
            }
            line_count++;
        }
     }
     perror("index error\n");
     return;
}

void print_head(int index, FILE *file){
	fseek(file, 0, SEEK_SET);
	char buffer[buffer_max_size];
	int line_count = 1;
	while (fgets(buffer, sizeof(buffer), file) != NULL && line_count != index) {
		 line_count++;
  	}
  	if(line_count == index)
		printf("%s", buffer);
	else
		perror("index error\n");
	return;
}

int cd(char **args) {
    if (args[1] == NULL) {
    	if (chdir("/home") != 0) {
            perror("cd");
        }
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }
    return 1;
}

int pwd() {
    char cwd[buffer_max_size]; 
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd");
    }

    return 1;
}

int echo(char **args){
	int i = 1;
	while(args[i] != NULL && i < MAX_ARG_COUNT){
		if(i == 9){
			printf("%s\n",args[i++]);
			break;
		}else if(args[i+1] == NULL){
			printf("%s\n",args[i++]);
			break;
		}else{
			printf("%s ",args[i++]);
		}
		
	}
	return 0;
}

char* replay(char **args, list_head* head){
	if(args[1]){
		int target = atoi(args[1]);
		if(target <= 0){
			perror("format error\n");
			return NULL;
		}
		return search_queue(head, target);
	}else{
		perror("need argument after replay\n");		
		return NULL;
	}
}



