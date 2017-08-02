#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#define SIZE 5

typedef struct b{
	int data;
	int countR;
}b;

pthread_mutex_t mutex;
pthread_t readtid[1000],writetid;
b buffer[SIZE]; 
int numC,flag[SIZE][1000],f[1000];
pthread_attr_t attr;
//sem_t Read[SIZE][1000],Write,empty,full;
int limit = 100;

void *reader(void *arg);
void *writer(void *arg);
void writeData(int i);

void initialize()
{
	int i,j;
	for(i=0;i<SIZE;i++)
	{
		buffer[i].data = -1;
		buffer[i].countR = 0;
		for(j=0;j<numC;j++)
			flag[i][j] = 0; 
		f[i]=0;
	}
}

int main()
{
	int i,j,sTime;
	printf("Number of consumers : ");
	scanf("%d",&numC);
	initialize();
	pthread_mutex_init(&mutex,NULL); 
	pthread_attr_init(&attr);
	pthread_create(&writetid,&attr,writer,NULL);
	for(i=0;i<numC;i++)
	{
		int index = i;
		pthread_create(&readtid[i],NULL,reader,(void*)index);
	}
	sleep(100);
  	printf("Exit the program\n");
  	exit(0);
}

void *writer(void *arg)
{
	int temp;
	while(1)
	{
		//sleep(1);
		temp = rand()%SIZE;
		if(buffer[temp].countR%numC == 0 && f[temp]==0)
		{
			pthread_mutex_lock(&mutex);
			writeData(temp);
			pthread_mutex_unlock(&mutex);
		}
		else
		{
			while(buffer[temp].countR%numC != 0 || f[temp]!=0);
			pthread_mutex_lock(&mutex);
			writeData(temp);
			pthread_mutex_unlock(&mutex);
		}
	}
}

void writeData(int i)
{
	int j;
	buffer[i].data = rand()%limit;
	buffer[i].countR = 0;
	printf("Writer writes %d at %d\n",buffer[i].data,i);
	f[i]=1;
	for(j=0;j<numC;j++)
	{
		flag[i][j]=0;
	//	sem_post(&Read[i][j]);
	}
}

void *reader(void *arg)
{
	int a = (int)arg;
	int i,j,temp,pos=0,count=0; 
	while(1)
	{
		sleep(1);
		pthread_mutex_lock(&mutex);
		//printf("%d %d\n",pos,a);
		pos = rand()%SIZE;
		if (flag[pos][a]==0 && buffer[pos].data>=0)
		{
			printf("Reader %d reads %d at position %d\n",a,buffer[pos].data,pos);
			flag[pos][a] = 1;
			buffer[pos].countR += 1;
		}
		if(buffer[pos].countR==numC)
		{
			f[pos]=0;
			buffer[pos].countR = 0;
		}	
		pthread_mutex_unlock(&mutex);
		count++;
	}
}