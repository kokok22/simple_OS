# 운영체제 프로젝트 1

## 1. 프로젝트 개요
  - client와 server가 named pipe(fifo)를 이용하여 서로 통신하는 프로그램이다. client는 access하고자 하는 file name과 option, contents를 입력하고 fifo를 통해 server에게 전달한다. server는 fork 함수를 통해 child proccess를 생성하고 전달받은 내용을 수행시킨다. 수행을 마친 child proccess는 종료되고 수행 결과를 client에게 전달한다.

## 2. 코드 설명

### 1) Client.c  

	if((writefd = open("./fifo1",O_WRONLY)) < 0)
	{
		printf("fail to open fifo1\n");
		exit(1);
	}

	if((readfd = open("./fifo2",O_RDONLY)) < 0)
	{
		printf("fail to open fifo2\n");
		exit(1);
	}

- open을 통해 server가 mkfifo한 named pipe(fifo)와 연결한다. client의 경우 fifo1이 write, fifo2가 read로 설정되어 있고 server는 fifo1이 read, fifo2가 write로 설정되어 있다. 위처럼 2개로 나누어서 설정한 이유는 pipe는 단방향 통신이기 때문이다. fifo도 named pipe로 pipe의 일종이기 때문에 단방향이다. pipe로 양방향 통신을 하기 위해서는 2개 생성하여 read와 write를 각각 생성해 주어야 한다.

      printf("\nenter the service you want\n");
      printf("ex) [filename] -r [number of bytes to read] or [filename] -w [data string to write]\n");
      gets(smsg);

- 받고자하는 smsg의 형식을 printf를 통해 알려주고 gets를 통해 입력받는다. 여기서 gets를 사용하는 이유는 -w option의 경우 sentence를 입력받는 것인데 띄어쓰기가 포함된 sentence가 들어올 수 있기 때문에 scanf가 아닌 gets를 사용한 것이다.

		if(write(writefd, smsg, SIZE) == -1)
		{
			printf("fail to write\n");
			exit(1);
		}

- 입력받은 smsg를 write를 통해 fifo1으로 전달한다. 아래의 server.c에서도 확인 가능하지만 fifo1은 server의 readfd와 연결되어 있다. 

		if(read(readfd, rmsg, SIZE) == -1)
		{
			printf("fail to read\n");
			exit(1);
		}
		
		printf("respond: %s\n",rmsg);

- server.c에서 수행의 결과가 rmsg로 전달이 된다. client는 해당 msg를 출력하고 정상 작동할 경우 read의 경우 읽고자했던 msg가 출력되고 write의 경우 complete to write가 출력된다. 오류가 발생했을 경우 해당하는 오류 메세지가 출력된다.

### 2) server.c

    char* finish;
    
- server에서 수행한 내용을 저장하기 위한 변수 생성. string type이기 때문에 char pointer type으로 변수 선언을 하고 malloc을 통해 mermory를 할당해준다. 위의 변수는 global로 선언된다.

      ptr = strtok(msg, " ");
      
      for(i; i<3; i++)
      {
		if(i==1)
		{
			Arr[i] = ptr;
			continue;
		}
		else if(i==2)
		{
			Arr[i] = ptr+3;
			break;
		}
		Arr[i] = ptr;
		ptr = strtok(NULL, " ");
      }

- 위의 코드는 t_function에 들어있는 코드이다. client로 부터 전달된 msg를 filename, option, sentence부분으로 나눠주는 코드이다. t_function함수는 fork된 child process가 수행하는 부분이다.

      case 'r':
      {
		if((fd = open(Arr[0],O_RDONLY)) < 0)
		{
			finish = "fail to file open\n";
			close(fd);
			break;
		}
		if((tmp = read(fd, temp, size)) < 0)
		{
			perror("read error\n");
			finish = "fail to read\n";
			close(fd);
			break;
		}
			
		else
			strcpy(finish,temp);

		close(fd);
		break;
      }
	
- read옵션으로 msg가 전달되었을 때 실행되는 코드이다. Arr[0]에는 file name이 들어있다. 해당 파일을 읽기전용모드로 open한 뒤 read를 통해 읽고자하는 byte만큼 읽어 temp에 저장한다. 저장된 temp는 finish에 복사되고 finish의 내용이 client로 전달된다. read에 있는 size변수는 Arr[2]에 저장되어 있는 byte 개수 string을 atoi를 통해 10진수 int형태로 변환한 것이다.	
	
	
		case 'w':
		{

			if((fd = open(Arr[0],O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0)
			{	
				finish = "fail to file open\n";
				close(fd);
				break;
			}
			if((tmp = write(fd, Arr[2], strlen(Arr[2]))) < 0 )
			{
				finish = "fail to write\n";
				close(fd);
				break;
			}
			
			finish = "complete to write\n";
			close(fd);
			break;
		}
		
- write 모드로 실행했을 때이다. 마찬가지로 Arr[0]에 들어있는 filename을 바탕으로 O_WRONLY,O_CREAT_O_TURNC 옵션으로 open한다. O_TURNC 옵션을 사용했기 때문에 write를 하게 되면 기존의 내용은 모두 지워지게 된다. 		
		
		default:
			finish = "entered wrong option\n";

- 옵션을 잘 못 입력했을 때 실행되는 error handling이다.


      if(mkfifo("./fifo1",0666) == -1)
      {
		printf("fail to make fifo1\n");
		exit(1);
      }

      if(mkfifo("./fifo2",0666) == -1)
      {
		printf("fail to make fifo2\n");
		exit(1);
      }

      if((readfd = open("./fifo1",O_RDONLY)) < 0)
      {
		printf("fail to connect fifo1\n");
		exit(1);
      }

      if((writefd = open("./fifo2",O_WRONLY)) < 0)
      {
		printf("fail to connect fifo2\n");
		exit(1);
      }

- server에서 mkfifo를 통해 named pipe(fifo)파일을 생성한다. 그 후 open을 통해 fifo와 연결한다. client와는 반대로 fifo1이 read, fifo2가 write를 담당하게 된다. 

      if((rd = read(readfd, msg, SIZE)) < 0)
      {
		printf("fail to read\n");
		exit(1);
      }
		
      pid = fork();
      strcpy(temp,msg);
      if(pid == -1)
      {
		printf("fail to fork\n");
		exit(1);
      }

      if(pid == 0)				// child
      {
		t_function((void *)temp);
		if((wr = write(writefd, finish, SIZE)) < 0)
		{
			printf("fail to write\n");
			exit(1);
		}
		exit(0);
			
		}
		
		else				// parent
		{
			sleep(1);
			wait(&status);
		}
		sleep(5);
      }
      
- main 함수이다. 우선 read를 통해 client가 전달하고자 하는 msg를 전달받는다. msg를 전달받으면 fork를 통해 child process를 생성하고 위에서 보았던 코드들을 실행시킨다. parent process는 child가 종료될 때까지 wait하고 있는다. 
