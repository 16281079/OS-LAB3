#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<semaphore.h>

char buf[10];
  
sem_t *Sembuff = NULL;  //strings which is waiting print
sem_t *Semrest = NULL;  //sapce which buf left

void *typer(void *arg){
	int i=0;
	while(1){
		sem_wait(Semrest);
		scanf("%c",&buf[i%10]);
		i++;
		sem_post(Sembuff);
	}
}

void *print(void *arg){
	int i=0;
	while(1){
		sem_wait(Sembuff);
		printf("%d:buf[%d] is %c\n",i,i%10,buf[i%10]);
		sleep(1);
		i++;
		sem_post(Semrest);
	}
}

int main(int argc,char *argv[]){
	Sembuff = sem_open("p1", O_CREAT, 0, 0); 
	Semrest = sem_open("p2", O_CREAT, 0, 10); 
	pthread_t p1,p2;
	pthread_create(&p1, NULL,typer, NULL);
	pthread_create(&p2, NULL,print, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	sem_close(Sembuff);
	sem_close(Semrest);

	sem_unlink("p1");
	sem_unlink("p2");

	return 0;
}
