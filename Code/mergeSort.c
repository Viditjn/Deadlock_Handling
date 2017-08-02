#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>

key_t key = IPC_PRIVATE;

void merge(int array[],int left,int mid,int right);
void selection(int array[],int length);

void mergeSort(int array[],int left,int right)
{
	int length,i,result,mid;
	length = right - left + 1;
	mid = left + length/2 -1;
	if(length<5)
	{
		selection(array+left,length);
		return;
	}
	pid_t l_c_pid,r_c_pid;
	l_c_pid = fork();
	if(l_c_pid < 0)
	{
		fprintf(stderr,"Failed forking Left Child Process\n");
		exit(EXIT_FAILURE);
	}
	else if(l_c_pid == 0)
	{
		mergeSort(array,left,mid);
		exit(EXIT_SUCCESS);
	}
	else
	{
		r_c_pid = fork();
		if(r_c_pid < 0)
		{	
			fprintf(stderr,"Failed forking Left Child Process\n");
			exit(EXIT_FAILURE);
		}
		else if(r_c_pid == 0)
		{
			mergeSort(array,mid+1,right);
			exit(EXIT_SUCCESS);
		}
	}
	waitpid(l_c_pid,&result,0);
	waitpid(r_c_pid,&result,0);
	merge(array,left,mid,right);
	return;
}

void merge(int array[],int left,int mid,int right)
{
	int length = right - left + 1;
	int sorted[length];
	int i,j,k=0;
	for(i=left,j=mid+1;i<=mid && j<=right;)
	{
		if(array[i]>array[j])
		{
			sorted[k]=array[j];
			j++;
			k++;
		}
		else
		{
			sorted[k]=array[i];
			i++;
			k++;
		}
	}
	while(i<=mid)
	{
		sorted[k]=array[i];
		k++;
		i++;
	}
	while(j<=right)
	{
		sorted[k]=array[j];
		j++;
		k++;
	}
	for(i=0,j=left;i<length;i++,j++)
		array[j]=sorted[i];
	return;
}



void selection(int array[],int length)
{
	int i,j,t;
	for(i=0;i<length-1;i++)
	{
		int temp = i;
		for(j=i;j<length;j++)
			if(array[j]<array[temp])
				temp = j;
		t = array[i];
		array[i] = array[temp];
		array[temp] = t;
	}
	return;
}

int main()
{
	int shmid,i,j,k,status,n;
	scanf("%d",&n);
	int *sharedArray;
	

	/* Create a shared Memory segment of required size and get its id */
	shmid = shmget(key, sizeof(int) *n, IPC_CREAT| 0666);
	if(shmid == -1){
		perror("Shmget failed");
		exit(1);
	}
	
	/* Attach the shared memory Segment to the given pointer */
	sharedArray = shmat(shmid, 0, 0);
	if(sharedArray == (void *)-1){
		perror("Shmat failed");
		exit(1);
	}
	
	/* Modify the contents before fork */
	printf("Enter the contents of array:\n");
	for(i = 0; i < n; i++){
		scanf("%d", &sharedArray[i]);
	}
	mergeSort(sharedArray,0,n-1);
	for(i=0;i<n;i++)
		printf("%d ",sharedArray[i]);
	printf("\n");
	return 0;
}