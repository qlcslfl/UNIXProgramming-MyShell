 

### **유닉스 프로그래밍**

###  -MY SHELL **프로젝트-**

**I.**           **요구사항 정의**

1. “cd” 명령어 구현

   \-      myshell> cd ./dirA 처럼 cd 명령어 수행 후 pwd 명령어로 확인하면

   ​		/home/usr1/dirA 처럼 디렉토리가 변경된다.

2. “exit” 명령어 구현

   \-      myshell> exit 하면 process 가 종료 된다.

3. background & foreground 구현

   \-      myshell> sleep 10 명령어를 수행하면 foreground 로 10초 후 명령어 프롬프트가 출력된다.

   \-      myshell> sleep 10 & 명령어를 수행 할 겨우 background 로 명령어 프롬프트가 바로 출력되고, 		background 에서 따로 sleep 명령어를 수행하게 된다.

4. 쉘 에서 제어 키 무시

   \-      myshell> ^\ 나 myshell> ^Z , myshell> ^C 해도 process가 종료되지 않고 계속 명령어 프롬프트를 출력한다.

5. 별도의 프로세스 그룹 생성

   \-      myshell> ps –o “comm. Ppid pid pgid status” 명령어를 통해 각 프로세스의 pgid 를 확인한다.

6. Foreground process 종료 (제어키 ^\)

   \-      Foreground 에서 myshell> sleep 10 후 제어키 ^\를 하면 foreground 의 process가 종료되어 바로 명령어 프롬프트를 출력한다.

7. Foreground 와 background 의 제어터미널 획득

   \-      Process 가 foreground 일 경우 터미널 창에 출력되고 background 일 경우 명령어는 수행되지만 터미널 창에 출력되지는 않는다.

8. SIGCHLD를 이용하여 좀비 처리 하도록 코드구현

   \-      SIGCHILD 를 이용할 경우 ps 를 통해 확인하면 좀비 process 가 생기지 않음을 확인 할 수 있다

9. 리다이렉션 구현

   \-      리다이렉션 기호를 이용하여 파일을 통해 입력을 받거나 파일에 결과를 출력할 수 있다.

10. 파이프 구현

    \-      파이프를 통해 부모와 자식 프로세스간의 통신이 가능함을 확인 할 수 있다.

 

#### II.        **요구사항 결과화면 출력**

**1.**    **“cd”** **명령어 구현**

​	-      myshell> cd ./dirA 처럼 cd 명령어 수행 후 pwd 명령어로 확인하면

​		/home/usr1/dirA 처럼 디렉토리가 변경된다.

![1.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image002.gif)

​	▶ 위와 같이 cd 명령어 수행 후 pwd 로 확인하면 디렉토리가 변경되었음을 확인 할 수 있다. 쉘과 같이 cd 명령		어 후 다음 명령어가 와도 수행이 가능하다.

 

**2.**    **“exit”** **명령어 구현**

​	-      myshell> exit 하면 process 가 종료 된다.

![2.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image005.gif)

 ▶ exit 명령어 수행 후 process 가 종료됨을 확인할 수 있다.

 

**3.**    **background & foreground** **구현**

\-      myshell> sleep 10 명령어를 수행하면 foreground 로 10초 후 명령어 프롬프트가 출력된다.

\-      myshell> sleep 10 & 명령어를 수행 할 겨우 background 로 명령어 프롬프트가 바로 출력되고, background 에서 따로 sleep 명령어를 수행하게 된다.

![3.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image008.gif)

 ▶ sleep 10 명령어를 수행하면 foreground 로써 다음 명령어 프롬프트 가 출력될 때 까지 10 초 기다림을 확인 할 수 있다.

 

![4.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image012.gif)

 ▶ sleep 10 & 명령어 수행하면 sleep 10을 background 에서 수행하기 때문에 바로 명령어 프롬프트가 출력된다.

Ps 명령어로 확인 해 보면 background 에서 sleep 이 계속 수행됨을 확인 할 수 있다.

 

\-      **background** **문자 ‘&’ 가 다음과 같이 중간에 나오는 것은 고려하지 않는다.**

![9.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image015.gif)

 ▶ 명령어 중간에 ‘&’ 문자열이 나올 경우 에러가 발생했음을 확인 할 수 있다.

 

\-      **A; B; C; D&** **일 경우 D 만 background 이고, 나머지는 foreground 로 처리된다.**

![10.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image019.gif)

 ▶ 앞의 명령어는 foreground 로써 실행되고 뒤의 명령어인 sleep 은 background 에서 계속 수행됨을 확인 할 수 있다.

 

**4.**    **쉘 에서 제어 키 무시**

\-      myshell> ^\ 나 myshell> ^Z , myshell> ^C 해도 process가 종료되지 않고 계속 명령어 프롬프트를 출력한다.

![5.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image021.gif)

 

**5.**    **별도의 프로세스 그룹 생성**

\-      myshell> ps –o “comm. Ppid pid pgid status” 명령어를 통해 각 프로세스의 pgid 를 확인한다.

![6.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image024.gif)

 ▶ 각 process 마다 pgid 가 다름을 확인 할 수 있다.

 

**6.**    **Foreground process** **종료 (제어키 ^\)**

\-      Foreground 에서 myshell> sleep 10 후 제어키 ^\를 하면 foreground 의 process가 종료되어 바로 명령어 프롬프트를 출력한다.

![7.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image027.gif)

 

**7.**    **Foreground** **와 background 의 제어터미널 획득**

\-      Process 가 foreground 일 경우 터미널 창에 출력되고 background 일 경우 명령어는 수행되지만 터미널 창에 출력되지는 않는다.

![8.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image029.gif)

 

**8.**    **SIGCHLD****를 이용하여 좀비 처리 하도록 코드구현**

\-      SIGCHILD 를 이용할 경우 ps 를 통해 확인하면 좀비 process 가 생기지 않음을 확인 할 수 있다.

![11.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image031.gif)

 ▶ 좀비 process (<defunct.>) 가 없음을 확인 할 수 있다.



**▶****추가 사항◀**

**11.** **리다이렉션 구현**

\-      리다이렉션 기호를 이용하여 파일을 통해 입력을 받거나 파일에 결과를 출력할 수 있다.

![1.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image034.gif)

▶ 처음에 파일을 통해 test.txt 에 입력을 하고 난 후 파일을 통해 test.txt 를 읽어보면 처음에 작성한 대로 출력됨을 확인할 수 있다.

▶ 두 번째 명령에서 cat의 출력을 test.txt로 보내고 test1.txt에서 test.txt 파일을 읽어 들인다. 결과로 test.txt 의 작성 내용이 test1.txt 로 출력됨을 확인 할 수 있다

▶ 실제 test.txt 파일과 test.1 파일

![3.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image038.gif)

 

**12.** **파이프 구현**

\-      파이프를 통해 부모와 자식 프로세스간의 통신이 가능함을 확인 할 수 있다.

![1.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image034.gif)![2.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image043.gif)

▶ 처음 파이프 명령 수행으로 

ls –l : 현재 디렉토리 내에 있는 파일 리스트 출력,

grep ^d : 파일 중 디렉토리 파일만 분류

wc –l : 라인 수 세기

세가지 명령이 통신하여 현재 디렉토리 내에 있는 디렉토리의 파일의 개수만 출력함을 확인 할 수 있다. (현재 디렉토리 내 또 다른 디렉토리가 존재하지 않아 0이 출력)

▶ ls 의 결과를 ls.txt에 저장한 후 ls.txt 를 읽어보면 ls의 결과가 ls.txt에 저장되었음을 확인할 수 있다.

▶ 실제 ls.txt 파일

![img](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image046.jpg)

▶ 위에서 수행한 파이프 명령(ls.txt | grep ^d| wc-l) 을 dir_num.txt 에 저장한 후 dir_num.txt를 출력하면 ls.txt 파일에 있는 디렉토리의 개수가 출력됨을 확인할 수 있다. (디렉토리가 없으므로 0이 출력)

▶ 실제 dir_num.txt 파일

![img](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image049.jpg)

**III.**      **구현 방법 기술**

**1.**    **cd** **명령어를 구현하기 위한 함수 선언**

![01.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image054.gif)

▶ cmd_cd 함수는 cmd_line 에 cd 명령어가 있을 경우 cd 를 수행 한 후 다음 명령어를 수행하고, cd 명령어가 없을 경우 처음 명령어부터 수행할 수 있는 함수이다. return 정수형 값은 “;” 로 구분한 명령어 배열의 index 를 타나낸다.

 

**line 202** : strstr 함수는 문자열에 특정 문자열이 있는지 확인해 주는 함수이다. strstr 은 cmd_line 문자열에 ch(“cd”) 가 없을 경우 NULL 을 리턴한다.

**line 214** : cmdvectortwo[0] 에는 “cd” 값이 있으므로 cmdvectortwo[1] 의 값을 path 로 지정한다.

**line 217** : chdir 명령어로 디렉토리를 path 로 변경한다.

 

**2.**    **Background** **인지 foreground 인지 판별하기 위한 함수 선언**

![02.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image056.gif)

▶ get_cmd_grp_type() 함수는 명령라인 맨 마지막 글자가 ‘&’ 일 경우 background 를 리턴하는 함수이다. 명령어 수행을 위해 background 일 경우 ‘&’ 문자를 ‘\0’ 값으로 변경한다.

**line 176** : cmd_line 에서 null 값일 때의 index 인 num 을 구한다.

 

**3.**    **명령어 수행 등 쉘을 처리하는 함수**

![11.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image062.gif)

![22.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image068.gif)

![33.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image072.gif)

▶ run_cmd_lind 은 입력받은 문자열을 명령어 단위로 구분한 수 자식 process 를 만들어 수행 해 주는 함수이다.

**Line 85** : 위에서 정의한 get_cmd_grp_type 함수를 이용하여 process 의 ground 타입을 결정한다.

**Line 91** : cmd_line명령어를 “;” 문자로 구분 한 후 cmd_grp_list 배열에 넣는다.

![1.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image074.jpg)

**Line 99** : SIGCHLD 를 사용하여 좀비를 처리한다.. SIGCHLD 를 통해 좀비처리를 수행 할 경우 쉘이 포그라운드 waitpid()시 포그라운드 프로세스가 온전하게 수행된다.

**Line 106** : 자식 process 생성 하기 전 cmd_cd 함수를 호출하여 cd 명령어를 수행 한 후 명령줄의 index 를 변경한다.

**Line 109** : 마지막 명령만이 background 이므로 background 수행을 위한 index 값을 구한다.

**Line 119** : cmd_grp_list 에 있는 각 명령어 들을 “ \t” 로 구분하여 명령어와 옵션을 구분한다.

**Line 123** : shell에서 수행되는 프로세스는 새로운 프로세스 그룹을 형성해야 한다 따라서 setpgid(0, 0) 을 호출 할 경우 각 process 마다 pgid가 변경된다.  

**Line 129** : 현재 우리의 myshell은 제어키(^C, ^\, ^Z)에 대해서 SIG_IGN으로 해 놓았기 때문에 myshell에서 실행되는 모든 자식 프로세스는 제어키(^C,^\, ^Z)를 무시하도록 되어 있다.

따라서 무시하지 않도록 하기 위해 다음과 같은 signal 을 선언한다.

**Line 135** : background 에서의 명령어 수행

**Line 139** : foreground 일 경우 터미널 제어권을 획득한다. 

**Line 146** : 부모 process 일 때 background 일 경우 바로 리턴하여 명령어 프롬프트를 출력한다.

**Line 158** : ^Z는 프로세스를 STOP하게 한다. 그래서 기존 쉘 들 보면 ^Z하면 포그라운드 프로세스가 STOP되면서 백그라운드로 되고 쉘 이 제어터미널을 다시 가져온다. . 

이를 위해waitpid가 자식 STOP될 때 return 되어야 합니다. 그렇게 하기 위해 waitpid에 WUNTRACED옵션을 준다.

**Line 163** : 부모process 가 foreground 일 경우 자식을 기다린 후 제어 권을 회수한다.

 

**4.**    **Main** **함수**

![1111.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image079.gif)

**line 236** : signal을 이용 해 제어키에 의해서도 종료되지 않도록 추가한다.

**Line 250** : 입력받은 문자열이 “exit” 일 경우 while 문을 빠져나가 process 가 종료된다.

**Line 253**: 명령어 수행을 위한 run_cmd_line() 함수를 무한반복 한다.



 

**▶****추가 사항, 변경사항◀**

**5.**    **리다이렉션 기호를 구분하여 수행하는 parse_redirction 함수 추가**

![1.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image081.gif)

▶ parse_redirction(char *cmdline) 함수는 다음과 같이 구현한다.

1. “ <, >” 기호가 있는지 체크하고, 체크된 기호에 따라 변경시킬 프로세스의 표준입력이나 표준출력을 리다이렉트 한다.

2. “<, >” 기호를 식별하고 그 뒤에 파일명을 얻는다.

3. 얻은 파일명을 가지고 식별된 기호(“<, >”) 에 따라 표준출력이나 표준입력으로 리다이렉트 한다.

4. “<, >” 기호 뒤의 문자열은 다 리다이렉트 처리 되었으므로 기호(“<,>”)를 NULL 문자열로 치환하여 기호 뒤의 문자열은 무시하도록 한다.



**6.**    **파이프명령이 들어왔을 때 수행해주는 run_cmd_grp 함수와 run_cmd 함수 추가**

![2.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image083.gif)

▶ run_cmd_grp 함수는 명령어를 받아와 파이프기호(“|”) 로 구분한 후 구분된 각 명령어들을 개별적인 프로세스에서 수행한다.

 예를들어 ls –l | grep ^d | wc –l 명령어가 있을 때, 명령어 전체(ls –l | grep ^d | wc –l)은 run_cmd_grp 함수에서 하나의 프로세스 그룹으로 처리되며, 파이프 기호로 구분한 각 명령어들은(ls –l, grep ^d, wc –l 총 3개의 명령어) 각각 따로 run_cmd 에서 명령을 수행한다.

 

**7.**    **리다이렉션과 파이프명령을 수행하기 위해 main함수 수정**![3.PNG](file:///C:/Users/qlcsl/AppData/Local/Temp/msohtmlclip1/01/clip_image086.gif)

▶ 우선 명령어(cmdline)에 파이프기호(“|”)가 있는지 확인한 후 파이프기호가 없을 때 평소처럼 run_cmd_line 함수를 호출한다. 파이프라인이 없어도 run_cmd_grp 함수를 호출할 경우 fork()가 되지 않아 -1을 리턴하게 된다.

파이프 기호가 있을 경우 파이프기호로 구분된 각 명령어는 각각의 자식프로세스에서 명령을 수행해야 하므로 자식프로세스를 생성한 후 자식프로세스에서 run_cmd_grp 함수를 호출하여 파이프 명령을 처리한다.