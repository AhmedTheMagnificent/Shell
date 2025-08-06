#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE 80

void parseCommand(char *inputBuffer, char *args[]){
	int argCount = 0;
	inputBuffer[strcspn(inputBuffer, "\n")] = 0;
	args[argCount] = strtok(inputBuffer, " ");
	while(args[argCount] != NULL){
		argCount++;
		args[argCount] = strtok(NULL, " ");
	}
}

int main(void){
	char *args[MAX_LINE/2 + 1];
	int should_run = 1;
	char inputBuffer[MAX_LINE];
	char previousCommand[MAX_LINE] = "";
	
	while(should_run){
		printf("osh>");
		fflush(stdout);
		
		if(fgets(inputBuffer, MAX_LINE, stdin) == NULL){ break; }
		
		if(strncmp(inputBuffer, "!!", 2) == 0){
			if(previousCommand[0] == '\0'){
				printf("No commands in history!");
				continue;
			}
			strcpy(inputBuffer, previousCommand);
			printf("%s", inputBuffer);
		}
		
		if(strncmp(inputBuffer, "!!", 2) != 0 && strncmp(inputBuffer, "exit", 4) != 0 && inputBuffer[0] != '\n'){ strcpy(previousCommand, inputBuffer); };		
		parseCommand(inputBuffer, args);
		
		if(args[0] == NULL){ continue; }
		
		if(strcmp(args[0], "exit") == 0){
			should_run = 0;
			continue;
		}
		
		
		
		pid_t pid = fork();
		
		if(pid < 0){
			fprintf(stderr, "Fork failed");
			return 1;
		}
		else if(pid == 0){
			execvp(args[0], args);
			perror("execvp failed!");
			exit(1);
		}
		else{
			wait(NULL);
		}
	}
	return 0;
}
