// ----------------------------------------------------------- 
// NAME : Michael Glazier                        User ID: mglazier 
// DUE DATE : 10/14/2016                                       
// PROGRAM ASSIGNMENT 2                                        
// FILE NAME : qsort.c            
// PROGRAM PURPOSE :                                           
//    This program runs a quickSort algarithm, but creates other
//    processes in place of recursive call to solve the problem
//    recursively.    
// ----------------------------------------------------------- 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int partition(int left, int right, int *quick_ptr);
int main(int argc, char* argv[]){
	int left, right;
	int keyQuick, keyValidate;
	int *quick_ptr, *validate_ptr;
	int quick_id, validate_id;
	int k = 0;
	int i = 0;
	pid_t pid;
	char temp[300];
	char string[30];
	sprintf(string, "   ### PROC(%d): ", (int)getpid());
	int p;
	char done = 0;
	if(argc < 4){
		write(1, "Incorrect Arguments\n", 23);
		exit(-1);
	}
	else{
		left = atoi(argv[1]);
		right = atoi(argv[2]);
		k = atoi(argv[3]);
	}

	
	//printf("left: %d, right: %d\n", left, right);
	/*Get qsort array*/
	keyQuick = ftok(".", 'x');
	quick_id = shmget(keyQuick, (k + 1) * sizeof (int), 0666);
	if(quick_id < 0){
		sprintf(temp, "%sexits\n", string);
		write(1, temp, strlen(temp));
		exit(0);
	}
	quick_ptr = (int*)shmat(quick_id, NULL, 0);
	
	/*Get validation array*/
	//generate validation memory
	keyValidate = ftok(".", 'v');

	//generate validate_id
	validate_id = shmget(keyValidate, k * sizeof (char), 0666);
	//attach validate memory
	validate_ptr = (int*)shmat(validate_id, NULL, 0);
;

	/*Qsort Logic*/
	if(left < right){
		sprintf(temp, "%sentering with a[%d..%d]\n", string, left, right);
		sprintf(&temp[strlen(temp)], "   ");
		for(i = 0; i <= right - left; i++){
			
			sprintf(&temp[strlen(temp)], "    %d", quick_ptr[left + i]); 
		}
		sprintf(&temp[strlen(temp)], "\n"); 
		write(1, temp, strlen(temp));
		/*Partition and find pivot*/
		p = partition(left, right, quick_ptr);

		sprintf(temp, "%spivot element is at a[%d] = %d\n", string, p, quick_ptr[p]);
		write(1, temp, strlen(temp));
		
		sprintf(temp, "%ssection a[%d..%d]\n", string, left, right);
		sprintf(&temp[strlen(temp)], "   ");
		for(i = 0; i <= right - left; i++){
			sprintf(&temp[strlen(temp)], "    %d", quick_ptr[left + i]); 
		}
		sprintf(&temp[strlen(temp)], "\n"); 
		write(1, temp, strlen(temp));
		
		
		if((pid = fork()) < 0){
			/*Fork failed*/
			char error[100];
			sprintf(error, "fork 1 inside %d failed\n", (int)getpid());
			write(1, error, strlen(error));
			exit(-1);	
		}
		/*child process*/
		else if(pid ==0){
			/*Arguements*/
			char leftBuf[10];
			char rightBuf[10];
			char kBuf[10];
			sprintf(leftBuf, "%d", left);
			sprintf(rightBuf, "%d", p - 1);
			sprintf(kBuf, "%d", k);
			char *args[] = {"./qsort", leftBuf, rightBuf, kBuf, NULL};

			/*Execute qsort*/
			int err = execvp(args[0], args);
			if(err < 0){
				sprintf(temp, "%sexits\n", string);
				write(1, temp, strlen(temp));
				exit(-1);
			}		
		}
		//parent process
		else{
			if((pid = fork()) < 0){
			/*Fork failed*/
			char error[100];
			sprintf(error, "fork 2 inside %d failed\n", (int)getpid());
			write(1, error, strlen(error));
			exit(-1);	
		}
		/*child process*/
		else if(pid ==0){
			/*Arguements*/
			char leftBuf[10];
			char rightBuf[10];
			char kBuf[10];
			sprintf(leftBuf, "%d", p + 1);
			sprintf(rightBuf, "%d", right);
			sprintf(kBuf, "%d", k);
			char *args[] = {"./qsort", leftBuf, rightBuf, kBuf, NULL};
			int err = execvp(args[0], args);
			/*Execute qsort*/
			if(err < 0){
				sprintf(temp, "%sexits\n", string);
				write(1, temp, strlen(temp));
				exit(-1);
			}		
		}

		}
		validate_ptr[p] = 1;
	}
	/*Mark piece of memory as complete*/
	else{
		if(left != -1 || left <= k){
			validate_ptr[left] = 1;
		}
		if(right != -1 || right <= k){
			validate_ptr[right] = 1;
		}
	}

	shmdt(quick_ptr);
	shmdt(validate_ptr);

	sprintf(temp, "%sexits\n", string);
	write(1, temp, strlen(temp));
	exit(1);

}

// ----------------------------------------------------------- 
// FUNCTION  partition :                          
//     This function sorts the data in the array around the pivot                            
// PARAMETER USAGE :                                           
//    int left: The left index of the array segment
//    int right: The right index of the array segment
//    int *quick_ptr: Pointer to array being worked on          
// ----------------------------------------------------------- 
int partition(int left, int right, int *quick_ptr){
	int pivot = quick_ptr[right];
	int i = left;
	int temp;

	int j;
	for( j = left; j < right; j++){
		if(quick_ptr[j] <= pivot){
			//swap
			temp = quick_ptr[i];
			quick_ptr[i] = quick_ptr[j];
			quick_ptr[j] = temp;
			i++;
		}
	}
	//swap i with right
	temp = quick_ptr[i];
	quick_ptr[i] = quick_ptr[right];
	quick_ptr[right] = temp;


	return i; 
}










