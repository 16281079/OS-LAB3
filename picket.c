#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<semaphore.h>
  
sem_t *Semsell = NULL; 
sem_t *Semback = NULL; 
sem_t *Sem = NULL; 

//火车票余票数
volatile int ticketCount=1000;


void *sell(void *arg){
//	int i=6;
	while(1){
		sem_wait(Semsell);
		sem_wait(Sem);
		int temp=ticketCount;
	//	pthread_yield();
		temp=temp-1;
	//	pthread_yield();
		sem_post(Semback);
		ticketCount=temp;
		printf("售出一张票，票量余额为%d\n",ticketCount);
		sem_post(Sem);
	}
}

void *back(void *arg){
//	int i=5;
	while(1){
		sem_wait(Semback);
		sem_wait(Sem);
		int temp=ticketCount;
	//	pthread_yield();
		temp=temp+1;
	//	pthread_yield();
		sem_post(Semsell);
		ticketCount=temp;
		printf("退回一张票，票量余额为%d\n",ticketCount);
		sem_post(Sem);
	}
}

int main(int argc,char *argv[]){
	Semsell = sem_open("p1", O_CREAT, 0666, 1000); 
	Semback = sem_open("p2", O_CREAT, 0666, 0); 
	Sem = sem_open("all", O_CREAT, 0666, 1); 
	//printf("票量余额为%d\n",ticketCount);
	pthread_t p1,p2;
	pthread_create(&p1, NULL,sell, NULL);
	pthread_create(&p2, NULL,back, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	sem_close(Semsell);
	sem_close(Semback);
	sem_close(Sem);
	sem_unlink("p1");
	sem_unlink("p2");
	sem_unlink("all");
	return 0;
}
