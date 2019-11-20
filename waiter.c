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
	int moneyamount;
	int shmid;
	int id_w;  //waiter id
	int n_groups;
	struct Sh_Memory *mem;
	int j;
	for(j=0; j<argc; j++)
    	{
		char* a=argv[j];
        	if(!strcmp(a,"-d"))
        	{
            		period = atoi(argv[j+1]);
        	}
        	if(!strcmp(a,"-m"))
        	{
            		moneyamount = atoi(argv[j+1]);
        	}
        	if(!strcmp(a,"-s"))
        	{
            		shmid = atoi(argv[j+1]);
        	}
		if(!strcmp(a,"-n"))
        	{
            		n_groups = atoi(argv[j+1]);
        	}
		if(!strcmp(a,"-w"))
        	{
            		id_w = atoi(argv[j+1]);
        	}
    	}

	mem = (struct Sh_Memory *) shmat(shmid, (void*)0, 0);
	int i;
	
	while(mem->n_serviced < n_groups)
	{
		//waiter waits the customer to order
		sem_wait(&(mem->waiter2));
		struct Group *customers = (struct Group*)(mem + 1);
		for(i=0;i<n_groups;i++)
		{
			if(customers->request == 11)    //gia paraggelia
			{
				printf("waiter %d about to take order\n",id_w);
				customers->request = 0;
				customers->waiter_n = id_w;
				break;
			}
			else if(customers->request == 111)    //gia plhrwmh
			{
				if(customers->waiter_n == id_w)
				{
					sem_wait(&(mem->sp3));
					if(customers->ntable == 2)
					{
						mem->table2 = mem->table2 + 1;
					}
					if(customers->ntable == 4)
					{
						mem->table4 = mem->table4 + 1;
					}
					if(customers->ntable == 6)
					{
						mem->table6 = mem->table6 + 1;
					}
					if(customers->ntable == 8)
					{
						mem->table8 = mem->table8 + 1;
					}
					mem->n_serviced = mem->n_serviced + 1;
					int rand_bill = rand() % moneyamount + 1;
					printf("Bill is %d\n",rand_bill);
					sem_post(&(mem->sp2));
					sem_post(&(mem->sp3));
				}
			}
		customers++;
		}
		sleep(period);
		sem_post(&(mem->waiter1));
	} 
	
	shmctl(shmid, IPC_RMID , 0); 
	return 0;
}