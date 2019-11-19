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

- pthread_create를 통해 2개의 thread를 생성하고 각각의 thread가 producer함수와 consumer함수의 기능을 동작하도록 한다. producer의 경우 rand함수를 통해 난수를 임의 생성하고 realloc을 통해 증가시킨 buffer의 공간에 해당 난수를 집어 넣는다. 그리고 consumer는 producer가 생성한 난수를 꺼내서 출력하는 동작을 한다. 현재 작성된 코드는 semaphore설정이 되어 있지 않은 상태이다. 위와 같은 상태로 프로그램을 실행시키면 buffer에 rand()+1을 넣었기때문에 0이 나올 수 없는데 0이 출력되는 경우가 있다는 것을 확인 할 수 있다. 이는 race condition이 발생한 것이다. 아래에 있는 코드들은 위와 같은 문제가 발생하는 것을 막기 위해 추가해준 semaphore 코드들이다.

	  sem_t product;
	  sem_t consume;

	  int main(int argc, char* argv[]){

	  	∙∙∙∙중략∙∙∙∙
	  	sem_post(&consume);

	  	∙∙∙∙중략∙∙∙∙
	  }

- global variable로 sem_t product와 sem_t consume을 선언해준다. 해당 변수들은 데이터가 생산 및 소비되었는지 확인해주는 역할을 하며 동기화작업을 통해 race condition이 발생하는 것을 막아준다. main함수 시작에 sem_post(&consume)을 해주는데 이는 consume의 변수값을 1증가시켜 주는 것이다. 아까 위에서 선언해준 변수들은 초기값인 0으로 설정되어있다. 이 중 consume을 sem_post함수를 통해 1증가시켜 줌으로써 consumer가 먼저 실행될 수 있도록 해준다.

	  void* producer(void* arg){

	  	∙∙∙∙중략∙∙∙∙

	  	sem_wait(&consume);
	  	buffer[in] = rand()+1;
	  	buffer = (int*)realloc(buffer, i*sizeof(int));
	  	sem_post(&product);

	  	∙∙∙∙중략∙∙∙∙

	  }

- producer함수이다. 버퍼에 난수를 넣기전에 sem_wait함수를 실행시키는 것을 확인할 수 있다. 이는 변수의 값이 0인지 확인하는 것이다. 만약 0이라면 thread의 동작을 멈추고 변수가 0이상이 될 때까지 기다린다. 변수가 0이상이 된다면 변수의 값을 1감소시킨다음 코드를 진행시킨다. 위의 경우 처음 main 함수에서 sem_wait을 통해 consume의 값을 1 증가시켰기 때문에 sem_wait을 지나치게 되고 consume의 값은 0으로 변한다. 난수를 버퍼에 넣고, sem_post함수를 통해 product의 값을 1 증가시킨다.

     
### 2) RW_Problem.c


