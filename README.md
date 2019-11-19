# 운영체제 프로젝트 2

## 1.  프로젝트 개요
  - 동기화 Problem인 Producer/Consumer Problem과 Reader/Writer Problem을 구현하고, race condition이 발생하는 것을 확인. 확인 후 semaphore를 활용하여 해당 Problem을 해결하는 프로젝트이다. 

## 2. 코드 설명

### 1) PC_Problem.c  

	int main(){

		buffer = (int*)malloc(sizeof(int)*100);
		pthread_t thread[2];
		pthread_create(&thread[0], NULL, producer, NULL);
		pthread_create(&thread[1], NULL, consumer, NULL);

		∙∙∙∙중략∙∙∙∙
	
	}

	void* producer(void* arg){
		
		∙∙∙∙중략∙∙∙∙
		in++;
		in%=100;
		buffer[in] = rand()+1;
		
		∙∙∙∙중략∙∙∙∙
	}

	void* consumer(void* arg){

		∙∙∙∙중략∙∙∙∙

		out++;	
		out%=100;
		print("%d's data is %d\n",i,buffer[out]);
	
		∙∙∙∙중략∙∙∙∙
	
	}

- pthread_create를 통해 2개의 thread를 생성하고 각각의 thread가 producer함수와 consumer함수의 기능을 동작하도록 한다. producer의 경우 rand함수를 통해 난수를 임의 생성하고 realloc을 통해 증가시킨 buffer의 공간에 해당 난수를 집어 넣는다. 그리고 consumer는 producer가 생성한 난수를 꺼내서 출력하는 동작을 한다. 현재 작성된 코드는 semaphore설정이 되어 있지 않은 상태이다. 위와 같은 상태로 프로그램을 실행시키면 buffer에 rand()+1을 넣었기때문에 0이 나올 수 없는데 0이 출력되는 경우나, 아직 화면에 출력하지 않은 값이 덮여 씌워질 수 있는 경우가 발생할 수 있다. 이는 race condition이 발생한 것이다. 아래에 있는 코드들은 위와 같은 문제가 발생하는 것을 막기 위해 추가해준 semaphore 코드들이다.

	  sem_t product;
	  sem_t consume;

	  int main(int argc, char* argv[]){

	  	∙∙∙∙중략∙∙∙∙
		
		sem_init(&product,0,0);
		sem_init(&consume,0,100);

	  	∙∙∙∙중략∙∙∙∙
	  }

- global variable로 sem_t product와 sem_t consume을 선언해준다. 해당 변수들은 데이터가 생산 및 소비되었는지 확인해주는 역할을 하며 동기화작업을 통해 race condition이 발생하는 것을 막아준다. main함수 시작에 sem_init을 통해 해당 변수들을 초기화 시켜준다. product는 0으로 consume은 100으로 초기화 시켜주는 이유는 consume같은 경우 현재 저장할 수 있는 데이터 공간의 양이고 product는 생산되어 있는 데이터의 양이기 때문이다. 두 변수의 합은 항상 100이다. 

	  void* producer(void* arg){

	  	∙∙∙∙중략∙∙∙∙

	  	sem_wait(&consume);
	  	buffer[in] = rand()+1;
	  	sem_post(&product);

	  	∙∙∙∙중략∙∙∙∙

	  }

- producer함수이다. 버퍼에 난수를 넣기전에 sem_wait함수를 실행시키는 것을 확인할 수 있다. 이는 변수의 값이 0인지 확인하는 것이다. 만약 0이라면 thread의 동작을 멈추고 변수가 0이상이 될 때까지 기다린다. 변수가 0이상이 된다면 변수의 값을 1감소시킨다음 코드를 진행시킨다. 위의 경우 처음 main 함수에서 sem_wait을 통해 consume의 값을 100으로 설정했기 때문에 sem_wait을 지나치게 되고 consume의 값은 1 감소한다. 난수를 버퍼에 넣고, sem_post함수를 통해 product의 값을 1 증가시킨다.

	  void* consumer(void* arg){
	
	  	∙∙∙∙중략∙∙∙∙

	  	sem_wait(&product);
	  	printf("%d's data is %d\n",i,buffer[out]);
	  	sem_post(&consume);

	  	∙∙∙∙중략∙∙∙∙
	
	  }

- consumer함수이다. 우선 producer가 데이터를 생성할 때 까지 기다린다. 데이터를 생성하게 되면 product의 값이 1 증가 된다. 현재 product의 값은 0이므로 sem_wait함수에서 기다리고 있다가 생성이 되서 1이 되면 코드가 진행된다. out을 1증가 시키고 producer가 생성한 데이터를 출력한다. 그리고 데이터를 사용했기 때문에 데이터 저장 공간이 늘어났다는 뜻으로 sem_post함수를 통해 consume 변수의 값을 1 증가시킨다. consume변수의 값이 증가되면 다시 producer함수가 시작된다. 위와 같은 방법으로 저장공간의 최대치를 넘지도 않고 생성되지 않은 데이터를 참조하지도 않기 때문에 race condition문제를 해결한다.

    
### 2) RW_Problem.c

	int buffer = 0;

	int main(){

		∙∙∙∙중략∙∙∙∙

		pthread_t thread[4];
		for(i=0;i<2;i++)
			pthread_creaete(&thread[i],NULL,reader,NULL);
		for(i=0;i<2;i++)
			pthread_create(&thread[i+2],NULL,writer,NULL);

		∙∙∙∙중략∙∙∙∙

	}

	void* reader(void* arg){

		∙∙∙∙중략∙∙∙∙

		printf("read data is %d\n",buffer);

		∙∙∙∙중략∙∙∙∙

	}

	void* writer(void* arg){

		∙∙∙∙중략∙∙∙∙

		buffer += 1;

		∙∙∙∙중략∙∙∙∙

	}

- pthread_create를 통해 총 4개의 thread를 생성해주었다. 2개는 writer로 2개는 reader로 동작한다. writer는 buffer의 값을 1증가시키며 reader는 buffer의 값을 읽어 출력해준다. 위의 코드를 그대로 실행시키면 예상과는 다르게 순서대로 출력되는 것이 아닌 뒤죽박죽하여 출력되는 것을 확인할 수있다. 동기화가 되어있지 않아서 이다. 위와 같은 문제를 해결하기 위해서는 아래의 코드들은 추가해주어야 한다.
