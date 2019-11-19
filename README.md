# 운영체제 프로젝트 2

## 1.  프로젝트 개요
  - 동기화 Problem인 Producer/Consumer Problem과 Reader/Writer Problem을 구현하고, race condition이 발생하는 것을 확인. 확인 후 semaphore를 활용하여 해당 Problem을 해결하는 프로젝트이다. 

## 2. 코드 설명

### 1) PC_Problem.c  

	int main(){

		pthread_t thread[2];
		pthread_create(&thread[0], NULL, producer, NULL);
		pthread_create(&thread[1], NULL, consumer, NULL);

		∙∙∙∙중략∙∙∙∙
	
	}

	void* producer(void* arg){
		
		∙∙∙∙중략∙∙∙∙
		
		buffer[in] = rand()+1;
		buffer = (int*)realloc(buffer, i*sizeof(int));
		
		∙∙∙∙중략∙∙∙∙
	}

	void* consumer(void* arg){

		∙∙∙∙중략∙∙∙∙
	
		print("%d's data is %d\n",i,buffer[out]);
	
		∙∙∙∙중략∙∙∙∙
	
	}

- pthread_create를 통해 2개의 thread를 생성하고 각각의 thread가 producer함수와 consumer함수의 기능을 동작하도록 한다. producer의 경우 rand함수를 통해 난수를 임의 생성하고 realloc을 통해 증가시킨 buffer의 공간에 해당 난수를 집어 넣는다. 그리고 consumer는 producer가 생성한 난수를 꺼내서 출력하는 동작을 한다. 현재 작성된 코드는 semaphore설정이 되어 있지 않은 상태이다. 위와 같은 상태로 프로그램을 실행시키면 buffer에 rand()+1을 넣었기때문에 0이 나올 수 없는데 0이 출력되는 경우가 있다는 것을 확인 할 수 있다. 이는 race condition이 발생한 것이다.


----------동기화 코드 넣기---------

     
### 2) RW_Problem.c


