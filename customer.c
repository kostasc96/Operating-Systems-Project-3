#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <signal.h>
#include <ctype.h>
#include "shared.h"


int main(int argc,char** argv)
{
	int period;
	int people;
	int shmid;
	int id;
	struct Sh_Memory *mem;
	int j;
	for(j=0; j<argc; j++)
    	{
		char* a=argv[j];
        	if(!strcmp(a,"-d"))
        	{
            		period = atoi(argv[j+1]);
        	}
        	if(!strcmp(a,"-n"))
        	{
            		people = atoi(argv[j+1]);
        	}
        	if(!strcmp(a,"-s"))
        	{
            		shmid = atoi(argv[j+1]);
        	}
		if(!strcmp(a,"-i"))
        	{
            		id = atoi(argv[j+1]);
        	}
    	}	
	int retval; 
	//printf("shmid %d\n",shmid);
	mem = (struct Sh_Memory *) shmat(shmid, (void*)0, 0);
	sem_wait(&(mem->sp3));
	struct Group *customers = (struct Group*)(mem + 1);
	customers = customers + id;
	customers->request=1;	
	mem->from_customer = 1;
	customers->number= people;
	printf("Group id %d has %d people\n",id,customers->number);
	sem_post(&(mem->sp2));
	sem_wait(&(mem->sp));
	if(customers->ntable == -1)
	{
		mem->n_serviced++;  //se aftous pou den exei anate8ei trapezi 8ewroume oti serviristhkan
	}
	sem_post(&(mem->sp3));
	
	if(customers->go_bar)
	{
		if((customers->number)<=2)
		{
			sem_wait(&(mem->bar1));
		}
		else if((customers->number>2)&&(customers->number<=4))
		{
			sem_wait(&(mem->bar2));
		}
		else if((customers->number>4)&&(customers->number<=6))
		{
			sem_wait(&(mem->bar3));		
		}
		else if(customers->number>6)
		{
			sem_wait(&(mem->bar4));
		}     
	}  
	
	sem_wait(&(mem->w_mutex));
	customers->request=11;
	sem_post(&(mem->waiter2));
	sem_wait(&(mem->waiter1));
	sem_post(&(mem->w_mutex));  
	printf("Group %d has ordered\n",id); 

	sem_wait(&(mem->w_mutex));
	customers->request=111;
	sem_post(&(mem->waiter2));
	sem_wait(&(mem->waiter1));
	sem_post(&(mem->w_mutex));
	printf("Group %d has paid\n",id);

	shmctl(shmid, IPC_RMID , 0); 
	return 0;
}