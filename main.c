#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<fcntl.h>

void execute_cmd(char *input);
void handle_redirection(char **,int *,int *);

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
	char *commands[100];
	char *token;
	int cmd_count=0;
	int pipe_fd[2];
	int in_fd=0,out_fd=0;
	
	
	commands[cmd_count]=strtok(input,"|");
	while(commands[cmd_count]!=NULL){
		cmd_count++;
		commands[cmd_count]=strtok(NULL,"|");
	}
	
	for(int i=0;i<cmd_count;i++){
		char *command=commands[i];
		int j=0;
		token=strtok(command," ");
		while(token!=NULL){
			args[j++]=token;
			token=strtok(NULL," ");
		}
		args[j]=NULL;
		
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
		
		handle_redirection(args,&in_fd,&out_fd);
		
		if(i<cmd_count-1){
			if(pipe(pipe_fd)==-1){
				perror("pipe error");
				exit(EXIT_FAILURE);
			}
		}
		
		
		pid_t pid;
		if((pid=fork())<0){
			perror("fork error");
			exit(EXIT_FAILURE);
		}
		else if(pid==0){
			if(in_fd!=0){
				dup2(in_fd,STDIN_FILENO);
				close(in_fd);
			}
			if(out_fd!=0){
				dup2(out_fd,STDOUT_FILENO);
				close(out_fd);
			}
			if(i<cmd_count-1){
				dup2(pipe_fd[1],STDOUT_FILENO);
				close(pipe_fd[1]);
				close(pipe_fd[0]);
			}
			if(execvp(args[0],args)==-1){
				perror("exec error");
				exit(EXIT_FAILURE);
			}
		}
		else{
			waitpid(pid,NULL,0);
			if(in_fd!=0) close(in_fd);
			if(i<cmd_count-1){
				close(pipe_fd[1]);
				in_fd=pipe_fd[0];
			}	
		}		
	}
}

void handle_redirection(char **args,int *in_fd,int *out_fd){
	for(int i=0;args[i]!=NULL;i++){
		if(strcmp(args[i],"<")==0){
			*in_fd=open(args[i+1],O_RDONLY);
			if(*in_fd<0){
				perror("Input redirection failed");
				exit(EXIT_FAILURE);
			}
			args[i]=NULL;
			break;
		}
		if(strcmp(args[i],">")==0){
			*out_fd=open(args[i+1],O_WRONLY | O_CREAT | O_TRUNC,0644);
			if(*out_fd<0){
				perror("output redirection error");
				exit(EXIT_FAILURE);
			}
			args[i]=NULL;
			break;
		}
	}
}
