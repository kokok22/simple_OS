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

