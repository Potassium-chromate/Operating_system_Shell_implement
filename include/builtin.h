#include <unistd.h> 
#include <string.h> 
#include "linked_list.h"
#define buffer_max_size 128
#define MAX_ARG_COUNT 10
#ifndef BUILTIN_H
#define BUILTIN_H

int help();
int cd(char **args);
int echo(char **args);
int record(char **args);
int mypid(char **args);
int pwd();
char* replay(char **args, list_head* head);

int add(char **args);
int del(char **args);
int ps(char **args);
int start(char **args);

extern const char *builtin_str[];

extern const int (*builtin_func[]) (char **);

extern int num_builtins();

#endif


int help(){
	FILE *file = fopen("help.txt", "r");
	char buffer[buffer_max_size];

   	if (file == NULL) {
	   	printf("Failed to open file.\n");
	   	return 1;
   	}

  	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		 printf("%s", buffer);
  	}

   	if (feof(file)) {
	   	printf("End of file reached.\n");
   	} else if (ferror(file)) {
	   	printf("An error occurred.\n");
   	}

   	fclose(file);
   	return 0;
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



