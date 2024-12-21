#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

void execute_cmd(char *input);

int main(){
	char *input=NULL;
	size_t len;
	while(1){
		printf("Shelly>");
		if(getline(&input,&len,stdin)==-1){
			perror("getline error");
			free(input);
			exit(EXIT_FAILURE);
		}
		
		input[strcspn(input,"\n")]='\0';
		if(strlen(input)==0){
			continue;
		}
		if(strcmp(input,"exit")==0){
			free(input);
			exit(0);	
		}
		execute_cmd(input);
		free(input);
		input=NULL;
	}
	return 0;
}

void execute_cmd(char *input){
	char *args[100];
	char *token;
	int i=0;
	token=strtok(input," ");
	while(token!=NULL){
		args[i++]=token;
		token=strtok(NULL," ");
	}
	args[i]=NULL;
	if(strcmp(args[0],"cd")==0){
		
		if(args[1]==NULL){
			const char *home=getenv("HOME");
			if(home==NULL){
				fprintf(stderr,"cd: home is not set\n");
			}
			else if(chdir(home)!=0){
				perror("cd");
			}
		}
		else if(chdir(args[1])!=0){
			perror("cd");
		}
	}
	else{
		pid_t pid;
		if((pid=fork())<0){
			perror("fork error");
		}
		else if(pid==0){
			if(execvp(args[0],args)==-1){
				perror("exec error");
				exit(EXIT_FAILURE);
			}
		}
		else{
			wait(NULL);	
		}
		
	}
}
