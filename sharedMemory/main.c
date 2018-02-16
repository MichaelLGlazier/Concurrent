// ----------------------------------------------------------- 
// NAME : Michael Glazier                        User ID: mglazier 
// DUE DATE : 10/14/2016                                       
// PROGRAM ASSIGNMENT 2                                        
// FILE NAME : main.c            
// PROGRAM PURPOSE :                                           
//    This program creates a quick sort process that sorts
//    concurrently, and a merge program that merges two 
//    sorted list concurrently. This program is responsible
//    for managing shared resources amongst all the processes.      
// ----------------------------------------------------------- 

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void cleanMem(int id, int key, int *ptr, char *text, char *string);

int main(int argc, char* argv[]){
	char buf[1];
	int k, m, n;
	key_t keyQuick, keyMerge, keyValidate, keyX, keyY;
	char string[] = {"*** MAIN: "};
	char temp[300];
	int quick_id, merge_id, validate_id, x_id, y_id;
	int *quick_ptr, *merge_ptr, *validate_ptr, *x_ptr, *y_ptr;
	pid_t  pid;
	//read k
	scanf("%s", buf);
	k = atoi(buf);
	int a[k];

	sprintf(temp, "Quicksort and Binary Merge with Multiple Processes:\n", string);
	write(1, temp, strlen(temp));
	int i;
	//Read array a
	for(i = 0; i < k; i++){
		scanf("%s", buf);
		a[i] = atoi(buf);
	}
	//read array m	
	scanf("%s", buf);
	m = atoi(buf);
	int x[m];
	for(i = 0; i < m; i++){
		scanf("%s", buf);
		x[i] = atoi(buf);
	}

	//read 	array n
	scanf("%s", buf);
	n = atoi(buf);
	int y[n];
	for(i = 0; i < m; i++){
		scanf("%s", buf);
		y[i] = atoi(buf);
	}
	/*Create shared memory for array y*/
	keyY = ftok(".", 'n');
	sprintf(temp, "%sarray y shared memory key = %d\n", string, keyY);
	write(1, temp, strlen(temp));

	
	y_id = shmget(keyY, n * sizeof (int), IPC_CREAT | 0666);
	sprintf(temp, "%sarray y shared memory created\n", string);
	write(1, temp, strlen(temp));	

	y_ptr = (int*)shmat(y_id, NULL, 0);
	sprintf(temp, "%sarray y shared memory attached and is ready to use\n", string);
	write(1, temp, strlen(temp));
	/*Create shared memory for array x*/
	keyX = ftok(".", 'm');
	sprintf(temp, "%sarray x shared memory key = %d\n", string, keyX);
	write(1, temp, strlen(temp));

	
	x_id = shmget(keyX, m * sizeof (int), IPC_CREAT | 0666);
	sprintf(temp, "%sarray x shared memory created\n", string);
	write(1, temp, strlen(temp));	

	x_ptr = (int*)shmat(x_id, NULL, 0);
	sprintf(temp, "%sarray x shared memory attached and is ready to use\n", string);
	write(1, temp, strlen(temp));
	//Quick Sort
	//generate quicksort memory
	keyQuick = ftok(".", 'x');
	sprintf(temp, "%sqsort shared memory key = %d\n", string, keyQuick);
	write(1, temp, strlen(temp));
	
	//generate quickid
	quick_id = shmget(keyQuick, (k + 1) * sizeof (int), IPC_CREAT | 0666);
	sprintf(temp, "%sqsort shared memory created\n", string);
	write(1, temp, strlen(temp));	
	//attach quick memory
	quick_ptr = (int*)shmat(quick_id, NULL, 0);
	sprintf(temp, "%sqsort shared memory attached and is ready to use\n", string);
	write(1, temp, strlen(temp));

	//Merge
	keyMerge = ftok("./", 'y');
	sprintf(temp, "%smerge shared memory key = %d\n", string, keyMerge);
	write(1, temp, strlen(temp));
	
	//generate mergeid
	merge_id = shmget(keyMerge, (n + m) * sizeof(int), IPC_CREAT | 0666);
	sprintf(temp, "%smerge shared memory created\n", string);
	write(1, temp, strlen(temp));	

	//attach merge
	merge_ptr = (int*)shmat(merge_id, NULL, 0);
	sprintf(temp, "%smerge shared memory attached and is ready to use\n", string);
	write(1, temp, strlen(temp));
	
	//generate validation memory
	keyValidate = ftok(".", 'v');
	sprintf(temp, "%svalidation shared memory key = %d\n", string, keyValidate);
	write(1, temp, strlen(temp));
	
	//generate validate_id
	validate_id = shmget(keyValidate, k * sizeof (char), IPC_CREAT | 0666);
	sprintf(temp, "%svalidation shared memory created\n", string);
	write(1, temp, strlen(temp));	
	//attach validate memory
	validate_ptr = (int*)shmat(validate_id, NULL, 0);
	sprintf(temp, "%svalidation shared memory attached and is ready to use\n", string);
	write(1, temp, strlen(temp));
	sprintf(temp, "Input array x[] for merge has %d elements\n", m);	
	//initialize array x memory
	for(i = 0; i < m; i++){
		x_ptr[i] = x[i];
		sprintf(&temp[strlen(temp)],"%4d", x_ptr[i]);
	}
	sprintf(&temp[strlen(temp)],"\n");
	write(1, temp, strlen(temp));
	sprintf(temp, "Input array y[] for merge has %d elements\n", n);
	//initialize array y memory
	for(i = 0; i < n; i++){
		y_ptr[i] = y[i];
		sprintf(&temp[strlen(temp)],"%4d", y_ptr[i]);
	}
	sprintf(&temp[strlen(temp)],"\n");
	write(1, temp, strlen(temp));
	sprintf(temp, "Input array for qsort has %d elements\n", k);
	//initialize validation memory and qsort memory
	for(i = 0; i < k; i++){
		validate_ptr[i] = 0;
		quick_ptr[i] = a[i];
		sprintf(&temp[strlen(temp)],"%4d", quick_ptr[i]);
	}
	sprintf(&temp[strlen(temp)],"\n");
	write(1, temp, strlen(temp));

	//initialize merge memory as int max, except for the last value which is
	//set to int_min.
	for(i = 0; i < (n + m); i++){
		if(i == (n + m - 1)){
			merge_ptr[i] = INT_MIN;
		}
		else{
			merge_ptr[i] = INT_MAX;
		}
	}
	
	char mBuf[10];
	char nBuf[10];
	sprintf(mBuf, "%d", m);
	sprintf(nBuf, "%d", n);
	char *args[] = {"./merge", "1", mBuf, nBuf, "-1", NULL};
	
	sprintf(temp, "%sabout to spawn the process for merge\n", string);
	write(1, temp, strlen(temp));
	/*Call Merge program*/
	if((pid = fork()) < 0){
		/*Fork failed*/
		cleanMem(merge_id, keyMerge, merge_ptr, "merge", string);
		cleanMem(y_id, keyY, y_ptr, "array y", string);
		char error[] = {"fork failed\n"};
		write(1, error, strlen(error));
		write(stderr, error, strlen(error));
		exit(-1);	
	}
	/*child process*/
	else if(pid ==0){
		int err = execvp(args[0], args);
		/*Execute qsort*/
		if(err < 0){
			cleanMem(merge_id, keyMerge, merge_ptr, "merge", string);
			cleanMem(x_id, keyX, x_ptr, "array x", string);
			cleanMem(y_id, keyY, y_ptr, "array y", string);
			char error[] = {"execvp Failed to execute qsort\n"};
			write(1, error, strlen(error));
			write(stderr, error, strlen(error));
			exit(-1);
		}		
	}
	/*Call quick sort*/
	char rightBuf[10];
	char kBuf[10];
	sprintf(rightBuf, "%d", (k - 1));
	sprintf(kBuf, "%d", k);
	char *args2[] = {"./qsort", "0", rightBuf, kBuf, NULL};
	
	sprintf(temp, "%sabout to spawn the process for qsort\n", string);
	write(1, temp, strlen(temp));
	if((pid = fork()) < 0){
		/*Fork failed*/
		cleanMem(quick_id, keyQuick, quick_ptr, "qsort", string);
		cleanMem(merge_id, keyMerge, merge_ptr, "merge", string);
		cleanMem(validate_id, keyValidate, validate_ptr, "validation", string);
		char error[] = {"fork failed\n"};
		write(1, error, strlen(error));
		write(stderr, error, strlen(error));
		exit(-1);	
	}
	/*child process*/
	else if(pid ==0){
		int err = execvp(args2[0], args2);
		/*Execute qsort*/
		if(err < 0){
			cleanMem(quick_id, keyQuick, quick_ptr, "qsort", string);
			cleanMem(merge_id, keyMerge, merge_ptr, "merge", string);
			cleanMem(validate_id, keyValidate, validate_ptr, "validation", string);
			cleanMem(x_id, keyX, x_ptr, "array x", string);
			char error[] = {"execvp Failed to execute qsort\n"};
			write(1, error, strlen(error));
			write(stderr, error, strlen(error));
			exit(-1);
		}		
	}
	/*wait for qsort- implemented by busy waiting*/
	unsigned char finishFlag = 0;
	
	while(finishFlag == 0){
		finishFlag = 1;
		/*Check to see all pieces of memory are done being operated on*/
		for(i = 0; i < k; i++){
			if(validate_ptr[i] != 1){
				finishFlag = 0;
				break;
			}
		}
	}

	sprintf(temp, "%ssorted array by qsort\n", string);
	for(i = 0; i < k; i++){
		sprintf(&temp[strlen(temp)],"%4d", quick_ptr[i]);
	}
	sprintf(&temp[strlen(temp)], "\n");
	write(1, temp, strlen(temp));
	
	/*wait for merge- implemented by busy waiting*/
	finishFlag == 0;
	int previous = 0;
	while(finishFlag == 0){
		finishFlag = 1;
		for(i = 1; i < m + n; i++){
			if(merge_ptr[i - 1] < merge_ptr[i]){
				//continue
			}
			else{
				finishFlag = 0;
				break;
			}	
		}

	}
		
	sprintf(temp, "%smerged array:\n", string);
	for(i = 0; i < m + n; i++){
		sprintf(&temp[strlen(temp)],"%4d", merge_ptr[i]);
	} 
	sprintf(&temp[strlen(temp)],"\n");
	write(1, temp, strlen(temp));
	//remove y array 
	cleanMem(y_id, keyY, y_ptr, "array y", string);
	//remove x array
	cleanMem(x_id, keyX, x_ptr, "array x", string);
	//remove qsort
	cleanMem(quick_id, keyQuick, quick_ptr, "qsort", string);
	//remove merge
	cleanMem(merge_id, keyMerge, merge_ptr, "merge", string);
	//remove validation memory
	cleanMem(validate_id, keyValidate, validate_ptr, "validation", string);


	
}
// ----------------------------------------------------------- 
// FUNCTION  cleanMem                         
//     Handles detaching and removing shared memory                            
// PARAMETER USAGE :                                           
//    int id: Shared memory id
//    int key: shared memory key
//    int *ptr: shared memory pointer
//    char *text: Text specifing what is being removed
//    char *string: Text that indicates what program is in use.                       
// ----------------------------------------------------------- 
void cleanMem(int id, int key, int *ptr, char *text, char *string){
	char temp[100];
	int err = 0;
	
	shmdt(ptr);
	sprintf(temp, "%s%s shared memory successfully detached\n", string, text);
	write(1, temp, strlen(temp));	

	//clear memory
	err = shmctl(id, IPC_RMID, (struct shmid_ds *) NULL);

	sprintf(temp, "%s%s shared memory successfully removed\n", string, text);
	write(1, temp, strlen(temp));

	if(err == -1){
		printf("Memory clear failed\n");
	}

}


