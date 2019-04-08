#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>
#include<sys/types.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<semaphore.h>
#include<pthread.h> 
  
sem_t *Sem2 = NULL; 
sem_t *Sem3 = NULL; 
sem_t *Sem23 = NULL; 

int main(int argc, char *argv) {
	Sem2 = sem_open("p2", O_CREAT, 0666, 0); 
	Sem3 = sem_open("p3", O_CREAT, 0666, 0); 
	Sem23 = sem_open("p23", O_CREAT, 0666, 0); 

	printf("This is root.\n");
	pid_t pid;

//	bool flag = false;
	int i=0, j=0;
	for( i=0; i<1; ++i) {
		pid = fork();
		if(pid == 0) {
			//p1
			printf("I am process1!\n");
			sem_post(Sem23);
			if( i == 0) {
				//if( flag == false) if not OK
				//flag=true;
				for( j=0; j<1; ++j) {
					pid_t p1=fork();
					if(p1 == 0) {
						//p2
						sleep(1);
						sem_wait(Sem23);
						printf("I am process2!\n");
						sem_post(Sem23);
						sem_post(Sem2);
						return 0;
					}
					else {
						//p3
						sleep(1);
						sem_wait(Sem23);
						printf("I am process3!\n");
						sem_post(Sem23);
						sem_post(Sem3);
					}
					//sleep(1);
				}				
			}
			return 0;
		}
		else {
			//p4
			sem_wait(Sem2);
			sem_wait(Sem3);
			printf("I am process4!\n");
		}
		sleep(1);
	}
	sem_close(Sem2);
	sem_close(Sem3);
	sem_close(Sem23);
	sem_unlink("p2");
	sem_unlink("p3");
	sem_unlink("p23");
	return 0;

}
