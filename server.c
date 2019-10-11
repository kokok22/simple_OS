#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<wait.h>

#define SIZE 100

char* finish;

void *t_function(void* data)
{
	char* msg = (char*)data;
	char* ptr;
	char* Arr[3] = {NULL, };	// save the msg

	int i = 0;

	int fd;
	int tmp;
	char temp[50];

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

	int size = atoi(Arr[2]);

	switch(Arr[1][1])
	{
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
		default:
			finish = "entered wrong option\n";
	}

}

int main(int argc, char* argv[])
{
	int readfd, writefd;
	char* msg;
	int rd, wr;

	pid_t pid;
	int status;

	char* temp = (char*)malloc(sizeof(char)*SIZE);

	finish = (char*)malloc(sizeof(char)*SIZE);
	msg = (char*)malloc(sizeof(char)*SIZE);

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

	while(1)
	{
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
		else					// parent
		{
			sleep(1);
			wait(&status);
		}
		sleep(5);
	}
	exit(0);
}
