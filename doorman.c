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
	int time;
	int shmid;
	int n_groups;
	struct Sh_Memory *mem;
	int j;
	for(j=0; j<argc; j++)
    	{
		char* a=argv[j];
        	if(!strcmp(a,"-d"))
        	{
            		time = atoi(argv[j+1]);
        	}
        	if(!strcmp(a,"-s"))
        	{
            		shmid = atoi(argv[j+1]);
        	}
		if(!strcmp(a,"-n"))
        	{
            		n_groups = atoi(argv[j+1]);
        	}
    	}
	int i;
	int proteraiothta=0;
	int min_priority,temp;
	temp = 1000;
	mem = (struct Sh_Memory *) shmat(shmid, (void*)0, 0);
	
	while(mem->customers_left > 0)
	{
		sem_wait(&(mem->sp2));
		//sleep(time);
		struct Group *customers = (struct Group*)(mem + 1);
		struct Group *temp1 = customers;
		struct Group *temp2;
		for(i=0;i<mem->customers_left;i++)
		{
			//min_priority = min(temp,customers->seira);
			if((temp1->seira)<temp)
			{
				temp2 = temp1;
				temp = temp1->seira;
			}
			temp1++;
		}
		for(i=0;i<mem->groups_bar;i++)
		{
			//printf("priority %d\n",temp);
			if((temp2->number<=2)&&(temp2->go_bar==1)&&(mem->table2>0))
			{
				sem_post(&(mem->bar1));
				temp2->go_bar=0;
				temp2->ntable = 2;
				mem->table2 = mem->table2 - 1;
				printf("table2 allocated\n");
				mem->groups_bar--;
				mem->barsize = mem->barsize + temp2->number;
			}
			else if((temp2->number<=4)&&(temp2->go_bar==1)&&(mem->table4>0))
			{
				sem_post(&(mem->bar2));
				temp2->go_bar=0;
				temp2->ntable = 4;
				mem->table4 = mem->table4 - 1;	
				printf("table4 allocated\n");
				mem->groups_bar--;
				mem->barsize = mem->barsize + temp2->number;
			}
			else if((temp2->number<=6)&&(temp2->go_bar==1)&&(mem->table6>0))
			{
				sem_post(&(mem->bar3));
				temp2->go_bar=0;
				temp2->ntable = 6;
				mem->table6 = mem->table6 - 1;
				printf("table6 allocated\n");
				mem->groups_bar--;
				mem->barsize = mem->barsize + temp2->number;
			}
			else if((temp2->number<=8)&&(temp2->go_bar==1)&&(mem->table8>0))
			{
				sem_post(&(mem->bar4));
				temp2->go_bar=0;
				temp2->ntable = 8;
				mem->table8 = mem->table8 - 1;
				printf("table8 allocated\n");
				mem->groups_bar--;
				mem->barsize = mem->barsize + temp2->number;
			}
		}   
		if(mem->from_customer)
		{
			for(i=0;i<n_groups;i++)
			{
				if((customers->request)==1)
				{
					//printf("in doorman customer number %d\n",customers->number);
					printf("table2:%d table4:%d table6:%d table8:%d\n",mem->table2,mem->table4,mem->table6,mem->table8);
					if((customers->number<=2)&&(mem->table2>0))
					{
						customers->ntable = 2;
						mem->table2 = mem->table2 - 1;
						printf("table2 allocated\n");
					}
					else if((customers->number<=4)&&(mem->table4>0))
					{
						customers->ntable = 4;
						mem->table4 = mem->table4 - 1;
						printf("table4 allocated\n");
					}
					else if((customers->number<=6)&&(mem->table6>0))
					{
						customers->ntable = 6;
						mem->table6 = mem->table6 - 1;
						printf("table6 allocated\n");
					}
					else if((customers->number<=8)&&(mem->table8>0))
					{
						customers->ntable = 8;
						mem->table8 = mem->table8 - 1;
						printf("table8 allocated\n");
					}
					else if((mem->barsize - customers->number) >= 0)
					{
						customers->go_bar = 1;	
						printf("Group goes to bar\n");
						mem->groups_bar++;
						printf("%d Groups in bar\n",mem->groups_bar);
						mem->barsize = mem->barsize - customers->number;
						printf("barsize: %d\n",mem->barsize);
						customers->seira = proteraiothta;
						proteraiothta++;
					}
				break;
				}
			customers++;
			}
		}
		mem->customers_left = mem->customers_left - 1;
		sem_post(&(mem->sp));
	}
	shmctl(shmid, IPC_RMID , 0); 
	return 0;
}
