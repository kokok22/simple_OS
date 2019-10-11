''''c
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define SIZE 100

int main(int argc, char* argv[])
{
	int readfd, writefd;
	int rd, wr;
	char* smsg;
	char* rmsg;
	int SP = 1;
  
	smsg = (char*)malloc(sizeof(char)*SIZE);
	rmsg = (char*)malloc(sizeof(char)*SIZE);


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


	char* ptr;			// strtok pointer

	while(1)
	{
		printf("\nenter the service you want\n");
		printf("ex) [filename] -r [number of bytes to read] or [filename] -w [data string to write]\n");
		gets(smsg);				// enter를 제외한 모든 문자 입력 받기

		if(write(writefd, smsg, SIZE) == -1)
		{
			printf("fail to write\n");
			exit(1);
		}
		
		if(read(readfd, rmsg, SIZE) == -1)
		{
			printf("fail to read\n");
			exit(1);
		}
		
		printf("respond: %s\n",rmsg);
	}
}
''''
