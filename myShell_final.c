#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#define MAX_CMD_ARG 10
#define BACKGROUND 0
#define FOREGROUND 1


const char *prompt = "myshell> "; //prompt 선언
char* cmd_grp_list[MAX_CMD_ARG]; //처음 ';' 문자로 구분 한 후 구분한 문자열을 담을 배열
char* cmdvectortwo[MAX_CMD_ARG]; //cmd_grp_list 를 " \t"로 구분 한 후 구분한 문자열을 담을 배열
char  cmdline[BUFSIZ]; //입력받은 문자열을 담을 배열

/* 에러를 출력 해주는 fatal 함수 */
void fatal(char *str){
	perror(str);
	exit(1);
}

/* 입력받은 문자열을 특정 구분자로 구분한 후 문자열형 배열에 담는  makelist 함수 */
/* 기존 makelist 함수에서 추가로 delimiters 로 큰따옴표("")를 갖는다. */
int makelist(char *s, const char *delimiters, char** argvp, int max_list)
{
	int i = 0;
	int numtokens = 0;
	char *snew = NULL;
	int flag = 0;

	/* 문자열이 없거나 구분자가 없을 때 에러 */
	if( (s==NULL) || (delimiters==NULL) )
	{
		return -1;
	}

	snew = s+strspn(s, delimiters);

	argvp[numtokens]=strtok(snew, delimiters);
	if ( argvp[numtokens][0] == '"'){
		flag = 1;
		argvp[numtokens] = argvp[numtokens]+1;
		if(argvp[numtokens][strlen(argvp[numtokens])-1] == '"'){
			argvp[numtokens][strlen(argvp[numtokens])-1] = 0;
			flag = 0;
		}
	}

	if( argvp[numtokens] !=NULL)
		for(numtokens=1; (argvp[numtokens]=strtok(NULL, delimiters)) != NULL; numtokens++)
		{
			if(argvp[numtokens][0]=='"'){
				flag = 1;
				argvp[numtokens] = argvp[numtokens]+1;
				if(argvp[numtokens][strlen(argvp[numtokens])-1] == '"'){
					argvp[numtokens][strlen(argvp[numtokens])-1] = 0;
					flag = 0;
				}
				continue;
			}
			if(flag){
				argvp[numtokens-1][strlen(argvp[numtokens-1])] = ' ';
				if(argvp[numtokens][strlen(argvp[numtokens])-1] == '"'){
					argvp[numtokens][strlen(argvp[numtokens])-1] = 0;
					flag = 0;
				}
				numtokens--;
			}

			if(numtokens == (max_list-1)) return -1;
		}

	if( numtokens > max_list) return -1;

	return numtokens;
}


/* 명령어에 따라 맞는 명령어를 수행하는 run_cmd_line 함수 */
int run_cmd_line(char *cmd_line){

	//명령어를 읽은 후 get_cmd_grp_type 함수를 통해 type 을 확인한다.
	//명령라인 끝에 '&' 가 있을 시 background, 아니면 foreground.
	int type = get_cmd_grp_type(cmd_line);
	int i=0; //for 문을 위한 정수형 변수 i, 초기값은 '0' ㅇ다.
	int j, state, status, max; //상태, index, return 값을 받기 위한 정수형 변수들
	pid_t pid; //자식 생성 후 process의 pid를 확인하기 위해 선언
	
	//인자로 받은 명령어를 문자 ';' 로 구분 한 후 각 명령어 마다 cmd_grp_list 에 집어넣는다.
	makelist(cmd_line, ";", cmd_grp_list, MAX_CMD_ARG);

	// SIGCHLD 를 이용하여 좀비처리 한다. 다음과 같이 처리할 경우 좀비(defunct) 가 나타나지 않는다.
	/* SIGCHLD 에 대한 signal 행동방식을 SIG_IGN 시그널 무시로 해놓아
	자식 process 가 종료 되더라도 좀비 process 가 생성되지 않도록 한다.*/
	struct sigaction sa;
	sa.sa_handler = SIG_IGN; //핸들러에 액션 등록
	sa.sa_flags = 0;
	sigaction(SIGCHLD, &sa, NULL);

	// 자식 생성 전, cd 명령어를 수행하는 함수를 호출한다.
	/* cd 명령어가 제대로 수행될 시 i 값이 1로 바뀐다.
	cd 명령어가 없을 경우 cmd_grp_list 문자열 배열에서 cmd_grp_list[0] 부터 명령을 수행 할 것이고
	cd 명령어가 있어서 제대로 수행 될 경우 cmd_grp_list[0] 은 이미 cd 명령어를 수행하였으므로 
	cmd_grp_list[1] 부터 명령을 수행 할 수 있게 된다. */
	if(j = cmd_cd(cmd_line)) i=1; 
	
	// background 수행을 위해 cmd_grp_list 가 NULL 일 때의 index 값 num 을 구한다.
	for(max=0; cmd_grp_list[max]; max++);
	
	// cmd_grp_list 가 NULL 값을 만날 때 까지 for문 loop
	/* 위에서 cd 명령어가 수행할 경우 cmd_grp_list[1] 부터, 
	cd 명령어가 수행하지 않았을 경우 cmd_grp_list[0] 부터 명령을 수행한다.*/
	for(i; cmd_grp_list[i]; i++){
		switch(pid=fork()){ //자식 생성 후
		case 0: //자식 일 경우
		
			//리다이렉션 기호를 구분해준다.
			parse_redirction(cmd_grp_list[i]);
			//cmd_grp_list 에 있던 명령어 들을 " \t" 로 명령어와 옵션을 구분하여 cmdvectortwo 에 집어넣는다.
			makelist(cmd_grp_list[i]," \t", cmdvectortwo, MAX_CMD_ARG);

			//shell에서 수행되는 프로세스는 새로운 프로세스 그룹을 형성해야 한다.
			//이를 위해 자식 process 는 setpgid(0,0) 을 호출한다.
			setpgid(0,0);
			
			//main 에서 signal 로 process 는 제어키를 무시하도록 하였다.
			//모든 제어키를 무시하지 않고 작동할 수 있도록 아래의 signal 을 추가한다.
			signal(SIGQUIT, SIG_DFL);
			signal(SIGINT, SIG_DFL);
			signal(SIGTSTP, SIG_DFL);
		
			// type 이 background 이고 index 가 마지막 명령일 때
			if(type == BACKGROUND && i == max-1)
			{
			// background 의 명령을 수행한다.
			execvp(cmdvectortwo[0], cmdvectortwo);
			}
			
			// background 가 아닌 모든 경우 일 때, 즉 foreground 일 때
			tcsetpgrp(STDIN_FILENO, getpgid(0)); // 제어권을 획득, 제어터미널을 가져간다.
			execvp(cmdvectortwo[0], cmdvectortwo); //foreground 의 명령을 수행 한다.
		case -1:
			fatal("fork error"); //자식 process 생성 실패 시 오류
		default: //부모 process 일 때
		
			// background 상태 일 때 자식을 기다려 주지 않으므로 바로 return 한다.
			if(type == BACKGROUND && i == max-1)
			{
				return 0;
			}
			
			//foreground 상태 일 때
			else
			{
				/* ^Z 는 process 를 STOP 하게 한다. ^Z 하면 foreground process 가 STOP 되면서
				background 로 되고 shell 이 제어터미널을 다시 가져오게 된다.
				이렇게 하기 위해서 waitpid 가 자식 STOP 될 때 리턴되어야 한다.
				따라서 waitpid 에 WUNTRACED 옵션을 준다.*/
				waitpid(pid, &status, WUNTRACED);
				tcsetpgrp(STDIN_FILENO, getpgid(0));
				if (WIFSTOPPED(status)) {
						printf("[%d] stopped\n", pid);
				}
				tcsetpgrp(STDIN_FILENO, getpgid(0)); //쉘이 기다렸다가 제어권을 회수한다.  
			}
		}
	}
	return 0; 
}

/* Foreground 인지 background 인지 판별해 주는 get_cmd_grp_type 함수 */
/* 명령라인의 맨 끝의 문자가 '&' 일 때만 background 타입으로 지정함에 유의한다. */
int get_cmd_grp_type(char *cmd_line){
	int num = 0; //cmd_line 버퍼의 index 값

	// cmd_line 버퍼에서 Null 값('\0') 이 나올 때의 index 인 num 을 구한다.
	for(num=0;cmd_line[num];num++);
  
	// 명령라인의 맨 끝의 문자가 '&' 일 때
	//(배열의 끝은 NULL 값 이므로 index 값은 num-1 이다)
	if(cmd_line[num-1] =='&') {
		//명령라인의 맨 끝의 문자를 NULL 값으로 변경한 후.
		cmd_line[num-1] = '\0'; 
		return BACKGROUND; //type 을 BACKGROUND 으로 지정한다.
	}
	
	//위의 경우가 아닐 시 FOREGROUND 으로 지정한다.
	else
		return FOREGROUND;
}

/* cd 명령어를 수행해 주는 cmd_cd 함수 */
/* (cd 명령어 같은 경우 일반 명령어와 같은 방식으로 사용 할 수 없으므로 */
/* 따로 함수를 지정 후 자식 process 생성 전에 호출한다) */
int cmd_cd(char *cmd_line)
{
	// 버퍼 cmd_line 에 "cd" 가 있는지 확인하기 위한 변수 선언
	char *location;
	char *ch = "cd";
	
	//strstr() 함수는 cmd_line 에 문자열 "cd" 가 없을 경우
	//NULL 을 리턴하는 함수 이다.
	location = strstr(cmd_line, ch);
	char *path; //경로 지정을 위한 문자열 path
	
		//cmd_line 에 cd 가 없을 경우 0을 리턴한다.
		if(location == NULL) return 0;
		//cmd_line 에 cd 가 있을 경우 아래를 수행 수 1을 리턴한다.
		else{
				//문자열을 " \t" 로 구분 후 cmdvectortwo 문자열 배열에 집어 넣는다.
				makelist(cmd_line," \t", cmdvectortwo, MAX_CMD_ARG);
				
				//띄어쓰기로 옵션을 구분 하였으므로 cmdvectortwo[0] 은 명령어 "cd" 이므로
				//cmdvectortwo[1] 은 path 로 지정한다.
				path = cmdvectortwo[1];
				
				//path 로 directory 경로를 지정 한다. 지정할 수 없을 때 오류메세지를 호출한다.
				if(chdir(path) < 0)
				{
					printf("chdir() call fail\n");
					exit(1);
				}
		return 1;
		}
}

/* 리다이렉션 기호('>, <') 을 구분해 주는 parse_redirction 함수 */
int parse_redirction(char* cmdline){
  int i=0;	
  int fd =0;
  int length=0;
  char* filename = NULL;
  length = strlen(cmdline); /* cmdline의 문자열 길이(14)를 pos에 저장 */
    for(i = length -1 ; i >=0  ; i--){/* 뒤에서부터 한 글자씩 스캔 하면서 redirect처리 한다. */

      if ( cmdline[i] == '>') {
          filename = strtok(&cmdline[i+1], " \t");		
          fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 0644); /* 파일을 오픈하여 그 파일의 파일 디스크립터를 얻는다. */
          dup2(fd, 1); /* 파일 디스크립터를 표준출력으로 복사한다. */
          close(fd);   /* 불필요한 디스크립터 종료 */
          cmdline[i] = '\0';
      }
      if ( cmdline[i] == '<') {
        /* 위 코드와 비슷 */
          filename = strtok(&cmdline[i+1], " \t");		
          fd = open(filename, O_RDONLY , 0644);

        dup2(fd, 0);
		close(fd);   /* 불필요한 디스크립터 종료 */

          cmdline[i] = '\0';
      }
    }
	return 0;
}

/* 받은 명령으로 프로세스를 바꾸어주는 run_cmd 함수 */
void run_cmd(char *cmd){
	parse_redirction(cmd); /* 파이프 기호("|") 와 리다이렉선 기호("<,>")가 동시에 나올 수 있으므로 parse_redirction 호출 */
	makelist(cmd, " \t", cmdvectortwo, MAX_CMD_ARG); /* 명령을 명령어와 옵션으로 구분한 후 명령을 수행한다. */
	execvp(cmdvectortwo[0], cmdvectortwo);
	fatal("run_cmd");
}

/* 파이프 명령이 들어왔을 때 자식 프로세스에서 수행해 주는 run_cmd_grp 함수 */
void run_cmd_grp(char *cmdline){
	int p[2], i; /* 파이프를 담을 정수형 배열 선언 */
	int cnt;
	
	setpgid(0,0);
	cnt = makelist(cmdline, "|", cmdvectortwo, MAX_CMD_ARG); /* cmdline 을 "|" 문자로 구분 후 cmdvector 에 넣는다. */
	for(i=0; i<cnt-1; i++){
	pipe(p);
		if(fork()){ /* 부모 프로세스 일 때 */
			dup2(p[0],0); /* 표준입력을 파이프 입력으로 바꾼다. */
			close(p[0]);
			close(p[1]);
		} else{
			dup2(p[1],1); /* 표준출력을 파이프 출력으로 바꾼다. */
			close(p[0]);
			close(p[1]);
			break;
		}
	}
	run_cmd(cmdvectortwo[i]); /* exec 를 수행해 주는 run_cmd 함수를 호출하여 해당 명령을 수행한다. */
}

/* main 함수 */
int main(int argc, char**argv){
  int i=0;
  pid_t pid;

	// shell은 process 를 종료시키는 제어키 등에 의해서 종료 되지 않는다.
	// 따라서 signal 을 이용해 shell 이 제어키를 무시하도록 signal 명령을 추가한다.
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);

	while (1) {

		/* "myshell>" 출력 후 문자열을 입력받아 BUFSIZ 만큼 cmdline 배열에 집어넣는다.*/
		fputs(prompt, stdout);
		fgets(cmdline, BUFSIZ, stdin);
		/* cmdline 배열의 맨 끝은 NULL값으로 할당한다. */
		cmdline[ strlen(cmdline) -1] ='\0';
		
		
	/* 입력받은 명령어에 파이프기호("|") 가 있는지 검사하기 위한 변수 선언 */
	char *location;
	char *ch = "|";
	
	//strstr() 함수는 cmd_line 에 문자열 "cd" 가 없을 경우
	//NULL 을 리턴하는 함수 이다.
	location = strstr(cmdline, ch);

		/* 현재 makelist 함수는 들어오는 문자열이 없을 때 ('\0') */
		/* 세그먼트 오류가 발생하므로 들어오는 문자열이 없을 시 continue 시킨다. */
		if(strlen(cmdline)==0) continue;
		
		/* 입력 문자열이 "exit" 인 경우 loop 를 빠져나가 종료된다. */
		if(!strcmp(cmdline, "exit")) exit(1);

		/* prompt를 띄운 후 입력을 받아 명령을 수행한다. 파이프 기호의 유무에 따라 호출 하는 함수는 다르다. */
		/* 인자는 위에서 입력받은 cmdline 배열이다. */		
		if(location == NULL){ /* 입력받은 문자열에 파이프기호("|") 가 없을 시 */
			run_cmd_line(cmdline);
		} else {  /* 입력받은 문자열에 파이프기호("|") 가 있을 때 */
		switch(pid = fork()){ /* 자식을 생성하여 자식에서 명령을 수행한다. */
		case 0:
		run_cmd_grp(cmdline);
		case -1:
			fatal("main()");
		default:
			waitpid(pid, NULL, 0);
			tcsetpgrp(STDIN_FILENO, getpgid(0));
		}
		}
	}
		return 0;
}
	


