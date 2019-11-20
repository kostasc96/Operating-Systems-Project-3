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
#include <errno.h>
#include "shared.h"
#include <time.h>



int main(int argc, char *argv[])
{
	srand(time(NULL));
	int n_groups;
	char configfile[30];
	int seconds;
	int j=0;
	for(j=0; j<argc; j++)
    	{
		char* a=argv[j];
        	if(!strcmp(a,"-n"))
        	{
            		n_groups = atoi(argv[j+1]);
        	}
        	if(!strcmp(a,"-l"))
        	{
            		strcpy(configfile,argv[j+1]);
        	}
        	if(!strcmp(a,"-d"))
        	{
            		seconds = atoi(argv[j+1]);
        	}
    	}
	int Waiters;
	int t2,t4,t6,t8,bars,bill,sleep;
	FILE *fp = fopen(configfile,"r");
	fscanf(fp,"%d%d%d%d%d%d%d%d",&Waiters,&t2,&t4,&t6,&t8,&bars,&bill,&sleep);
	//printf("%d %d %d %d %d %d %d %d\n",Waiters,t2,t4,t6,t8,bars,bill,sleep);
	key_t key;
        int shmid;
        char *data;
        int mode;
	int W=Waiters;
	int N=n_groups;
	int i=0;
	int pid=0;
	int id=0;
	struct Sh_Memory *mem;
	int retval;
	int circles;
	int time=sleep;
	int period=10;
	int moneyammount=bill;
	int people=10;
	
	shmid = shmget(IPC_PRIVATE,sizeof(struct Sh_Memory)+sizeof(struct Group)*N,0777); 
	printf("shmid %d\n",shmid);
	if (shmid == -1) 
		perror ("Creation");
	else 	
		printf("Allocated. %d\n",(int)id);
	
	mem = (struct Sh_Memory *) shmat(shmid, (void*)0, 0);
	mem->customers_left = N;
	mem->table2=t2;
	mem->table4=t4;
	mem->table6=t6;
	mem->table8=t8;
	mem->barsize=bars;
	mem->from_customer=0;
	mem->groups_bar = 0;
	mem->n_serviced = 0;
	if(sem_init(&(mem->sp), 1, 0) < 0)
	{
		perror("sem_init");
   		exit(EXIT_FAILURE);
	}
	if(sem_init(&(mem->sp2), 1, 0) < 0)
	{
		perror("sem_init");
   		exit(EXIT_FAILURE);
	}
	if(sem_init(&(mem->sp3), 1, 1) < 0)
	{
		perror("sem_init");
   		exit(EXIT_FAILURE);
	}
	if(sem_init(&(mem->bar1), 1, 0) < 0)
	{
		perror("sem_init");
   		exit(EXIT_FAILURE);
	}
	if(sem_init(&(mem->bar2), 1, 0) < 0)
	{
		perror("sem_init");
   		exit(EXIT_FAILURE);
	}
	if(sem_init(&(mem->bar3), 1, 0) < 0)
	{
		perror("sem_init");
   		exit(EXIT_FAILURE);
	}
	if(sem_init(&(mem->bar4), 1, 0) < 0)
	{
		perror("sem_init");
   		exit(EXIT_FAILURE);
	}
	if(sem_init(&(mem->waiter1), 1, 0) < 0)
	{
		perror("sem_init");
   		exit(EXIT_FAILURE);
	}
	if(sem_init(&(mem->waiter2), 1, 0) < 0)
	{
		perror("sem_init");
   		exit(EXIT_FAILURE);
	}
	if(sem_init(&(mem->w_mutex), 1, 1) < 0)
	{
		perror("sem_init");
   		exit(EXIT_FAILURE);
	}



	//struct Group *customers;
	//customers = (struct Group *)mem + sizeof(struct Sh_Memory);
	struct Group *customers = (struct Group*)(mem + 1);
	int k;
	for(k=0;k<N;k++)
	{
		customers->request = 0;
		customers->seira = 2*N;
		customers->go_bar=0;
		customers->ntable=-1;
		customers++;
	}


	for (i = 0; i < W; i++)
	{
		pid = fork();
		//exec(...)
		if (pid < 0)
		{
			printf("Error");
			//exit(0);
		}
		else if (pid == 0)
		{
			char periods[20],moneyammounts[20],shmids[30],groups1[30],wid[20];
			sprintf(periods,"%d",period);
			sprintf(wid,"%d",i);
			sprintf(moneyammounts,"%d",moneyammount);
			sprintf(shmids,"%d",shmid);
			sprintf(groups1,"%d",n_groups);
			execl("waiter","-d",periods,"-m",moneyammounts,"-s",shmids,"-n",groups1,"-w",wid,NULL);
			exit(0);
		}
	}
	
	for (i = 0; i < N; i++)
	{
		pid = fork();
		if (pid < 0)
		{
			printf("Error");
			//exit(0);
		}
		else if (pid == 0)
		{
			//struct Group *temp = customers + i;
			int num_cust = rand() % 8 + 1;
			char periods2[20],peoples[20],shmids2[30],is[20];
			sprintf(periods2,"%d",period);
			sprintf(peoples,"%d",num_cust);
			//printf("%d\n",temp->number);
			sprintf(shmids2,"%d",shmid);
			sprintf(is,"%d",i);
			execl("customer","-d",periods2,"-n",peoples,"-s",shmids2,"-i",is,NULL);
			exit(0);
		}		
	}
	
	pid = fork();
	if(pid < 0)
	{
		printf("Error");
		//exit(0);
	}
	else if (pid == 0)
	{
		char times[20],shmids3[30],groups[20];
		sprintf(times,"%d",time);
		sprintf(shmids3,"%d",shmid);
		sprintf(groups,"%d",n_groups);
		execl("doorman","-d",times,"-s",shmids3,"-n",groups,NULL);
		exit(0);
	}
	
	int js = N + W + 1;
	for(j=0;j<js;j++)
	{
		wait(&circles);
	}
	sem_destroy(&(mem->sp));
	sem_destroy(&(mem->sp2));
	sem_destroy(&(mem->sp3));
	sem_destroy(&(mem->bar1));
	sem_destroy(&(mem->bar2));
	sem_destroy(&(mem->bar3));
	sem_destroy(&(mem->bar4));
	sem_destroy(&(mem->waiter1));
	sem_destroy(&(mem->waiter2));
	sem_destroy(&(mem->w_mutex));
	shmctl(shmid, IPC_RMID , 0); 
	exit(0);
}