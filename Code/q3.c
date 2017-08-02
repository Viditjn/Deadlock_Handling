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

typedef struct Booth
{
  pthread_mutex_t ready,vote;
  pthread_cond_t cond;
  int numVoters,numEvm,currEvm,numSlots,boothId,curr1,curr2;
}Booth;

void *Voter_waiting(Booth *booth,int in);
void *poll_ready(Booth *booth,int count,int evmId);
void booth_init(Booth *booth);
void *Evm(void *arg);
void *Voter(void *arg);

int main()
{
    int i,j,numBooths,a,b,c;
    Booth booth[100];
    scanf("%d",&numBooths);
    for(i=0;i<numBooths;i+=1)
    {
       scanf("%d %d %d",&a,&b,&c);
        booth[i].numVoters = a;
        booth[i].numEvm = b;
        booth[i].numSlots = c;
        booth[i].boothId=i; 
    }
    for(i=0;i<numBooths;i+=1)
       booth_init(&booth[i]);
    pthread_exit(NULL);
}

void *poll_ready(Booth *booth,int count,int evmId)
{
    pthread_mutex_lock(&(*booth).ready);
    if( (*booth).numVoters<=0)
    {
      pthread_mutex_unlock(&(*booth).ready); 
      return NULL; 
    }
    printf("Evm %d ready with %d slots at Booth %d \n",evmId,count,(*booth).boothId);
    int i=0;
    (*booth).currEvm=evmId;
    while(i<count)
    {
        if((*booth).numVoters<0)
            break;
       (*booth).numVoters--;
        pthread_cond_signal(&(*booth).cond);
        sleep(1);
        i += 1;
    }
    pthread_mutex_unlock(&(*booth).ready); 
    printf("Evm %d moves to voting stage at Booth %d \n",evmId,(*booth).boothId);
    if((*booth).numVoters<=0)
      printf("Booth %d done with voting\n",(*booth).boothId);  
}

void *Voter_waiting(Booth *booth,int in)
{ 
    pthread_cond_wait(&(*booth).cond,&(*booth).vote);
    printf("Voter %d moves to evm %d at Booth %d \n",in,(*booth).currEvm,(*booth).boothId);
    pthread_mutex_unlock(&(*booth).vote); 
}

void booth_init(Booth *booth)
{
    int i;
    pthread_t tid; 
    pthread_mutex_init(&(*booth).vote, NULL);
    pthread_mutex_init(&(*booth).ready, NULL);
    pthread_cond_init(&(*booth).cond, NULL);
    for(i=0;i<(*booth).numVoters;i+=1)
        pthread_create(&tid,NULL,Voter,booth);
    for(i=0;i<(*booth).numEvm;i+=1) 
       pthread_create(&tid,NULL,Evm,booth);
}

void *Evm(void *arg)
{
    struct timespec ts;
    /*ts.tv_sec = 0;
    ts.tv_nsec = 500000000;*/
    Booth *booth= ((Booth*)(arg));
    int in=(*booth).curr1;
    (*booth).curr1+=1;
    while((*booth).numVoters>0)
        poll_ready(booth,(*booth).numSlots,in);         
}

void *Voter(void *arg)
{
    Booth *booth= ((Booth*)(arg));
    int in;
    in =(*booth).curr2;
    (*booth).curr2+=1;
    Voter_waiting(booth,in);
}
