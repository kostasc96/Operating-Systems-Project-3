#ifndef __shared__
#define __shared__


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


int number_groups;

int min(int a,int b)
{
	if(a<b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

struct Table {
       int id;
       int space;
};

struct Group {
       int number;
       int request;
       int ntable;         //which kind of table 
       int go_bar;         //if this group is to be sent to bar
       int seira;          //priority
       int waiter_n;   //to request a waiter
};

struct Sh_Memory {
       int n_serviced;     //how many groups have been served
       int from_customer;
       int groups_bar;
       int table2;
       int table4;
       int table6;
       int table8;
       int barsize;
       int customers_left;
       sem_t sp;
       sem_t sp2;
       sem_t sp3;     //mutex
       sem_t bar1;
       sem_t bar2;
       sem_t bar3;
       sem_t bar4;
       sem_t w_mutex;	
       sem_t waiter1;
       sem_t waiter2;
};


#endif