// ----------------------------------------------------------- 
// NAME : Michael Glazier                        User ID: mglazier 
// DUE DATE : 10/14/2016                                       
// PROGRAM ASSIGNMENT 2                                        
// FILE NAME : merge.c            
// PROGRAM PURPOSE :                                           
//          This program merges to sorted list by assigning 
//	    each array element to a process which calculates
// 	    where to put it by a modified binary search.
// ----------------------------------------------------------- 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]){

	int control = 0;
	int x_id, y_id, merge_id;
	int *x_ptr, *y_ptr, *merge_ptr;
	int keyX, keyY, keyMerge ;
	pid_t pid;
	char buf[300] = {};
	char string[35];
	sprintf(string, "      $$$ M-PROC(%d): ", (int)getpid());
	int err;
	int n, m, task;
	//get arguements
	if(argc < 5){
		
		exit(-1);
	}
	else{
		
		control = atoi(argv[1]);
		m = atoi(argv[2]);
		n = atoi(argv[3]);
		task = atoi(argv[4]);
	}
	
	keyX = ftok(".", 'm');		
	x_id = shmget(keyX, (m) * sizeof(int), 0666);
	keyY = ftok(".", 'n');
	y_id = shmget(keyY, (n) * sizeof(int), 0666);	
	keyMerge = ftok("./", 'y');
	merge_id = shmget(keyMerge, (m + n) * sizeof(int), 0666);
	//attach merge
	x_ptr = (int*)shmat(x_id, NULL, 0);
	y_ptr = (int*)shmat(y_id, NULL, 0);
	merge_ptr = (int*)shmat(merge_id, NULL, 0);
	/*Control block assigns all the children processes.
	 *Only main calls the control merge.
	*/
	if(control == 1){

		int i = 0;
		for(i; i < m; i++){
			if((pid = fork()) < 0){
				/*Fork failed*/
				shmdt(x_ptr);
				char error[] = {"fork failed\n"};
				write(1, error, strlen(error));
				exit(-1);	
			}
			/*child process*/
			else if(pid ==0){
				/*Create arguments for merge*/
				//memory location child will handle
				char assignmentBuf[10];
				//m size		
				char mBuf[10];
				//n size		
				char nBuf[10];
				sprintf(mBuf, "%d", m);
				sprintf(nBuf, "%d", n);
				sprintf(assignmentBuf, "%d", i); 
				char *args[] = {"./merge", "0", mBuf, nBuf, assignmentBuf, NULL};
				/*Execute merge*/
				int err = execvp(args[0], args);
				if(err < 0){
					shmdt(x_ptr);
					char error[] = {"execvp Failed to execute qsort\n"};
					write(1, error, strlen(error));
					exit(-1);
				}		
			}		
		}
		for(i = 0; i < n; i++){
			if((pid = fork()) < 0){
				/*Fork failed*/
				shmdt(x_ptr);
				char error[] = {"fork failed\n"};
				write(1, error, strlen(error));
				exit(-1);	
			}
			/*child process*/
			else if(pid ==0){
				char assignmentBuf[10];		
				char mBuf[10];
				char nBuf[10];
				sprintf(mBuf, "%d", m);
				sprintf(nBuf, "%d", n);
				sprintf(assignmentBuf, "%d", i + m); 
				char *args[] = {"./merge", "0", mBuf, nBuf, assignmentBuf, NULL};
				/*Execute merge*/
				int err = execvp(args[0], args);
				if(err < 0){
					shmdt(x_ptr);
					char error[] = {"execvp Failed to execute qsort\n"};
					write(1, error, strlen(error));
					exit(-1);
				}		
			}	
		}
		sprintf(&buf[strlen(buf)], "\n");
		write(1, buf, strlen(buf));
		//clear memory
		shmdt(x_ptr);
		exit(1);
	}
	else{
		int y0 = y_ptr[0];
		int j;
		//calculating on a y value
		if(task >= m){
			int loc = task - m;
			sprintf(buf, "%shandling y[%d] = %d\n", string, loc, y_ptr[loc]);  
			write(1, buf, strlen(buf));
			//y[i] > x[m - 1], place in position i	
			if(y_ptr[loc] > x_ptr[m - 1]){
				sprintf(buf, "%sy[%d] = %d is found to be larger than x[%d] = %d\n", 
					string, loc, y_ptr[loc], m - 1, x_ptr[m - 1] );  
				write(1, buf, strlen(buf));
				sprintf(buf, "%sabout to write y[%d] = %d into position %d of the output array\n", 
					string, loc, y_ptr[loc], task );  
				write(1, buf, strlen(buf));
				merge_ptr[task] = y_ptr[loc];
				shmdt(x_ptr);
				shmdt(y_ptr);
				shmdt(merge_ptr);
				exit(1);
			}
			//y[i] < x[0]
			if(y_ptr[loc] < x_ptr[0]){
				sprintf(buf, "%sy[%d] = %d is found to be smaller than x[0] = %d\n", 
					string, loc, y_ptr[loc], x_ptr[0] );  
				write(1, buf, strlen(buf));
				sprintf(buf, "%sabout to write y[%d] = %d into position %d of the output array\n", 
					string, loc, y_ptr[loc], loc );  
				write(1, buf, strlen(buf));
				merge_ptr[task - m] = y_ptr[task - m];
				shmdt(x_ptr);
				shmdt(y_ptr);
				shmdt(merge_ptr);
				exit(1);
			}
			if(y_ptr[loc] >= x_ptr[0] && y_ptr[loc] <= x_ptr[m - 1]){
				//binary search
				int low = 0;
				int high = m - 1;
				int mid;
				//binary search to find proper value
				while(low <= high){
					mid = (low + high) / 2;
					//edge condition: Has to find where to insert value
					if(low == mid || mid == high){
						if(y_ptr[loc] < x_ptr[mid]){
							sprintf(buf, "%sabout to write y[%d] = %d into position %d of the output array\n", 
								string, loc, y_ptr[loc], loc + mid );  
							write(1, buf, strlen(buf));
							merge_ptr[loc + mid] = y_ptr[loc];
						}
						else if(y_ptr[loc] > x_ptr[mid] && y_ptr[loc] < x_ptr[high]){
							sprintf(buf, "%sabout to write y[%d] = %d into position %d of the output array\n", 
								string, loc, y_ptr[loc], loc + mid + 1 );  
							write(1, buf, strlen(buf));
							merge_ptr[loc + mid + 1] = y_ptr[loc];
						}
						else if(y_ptr[loc] > x_ptr[mid] && y_ptr[loc] > x_ptr[high]){
							sprintf(buf, "%sabout to write y[%d] = %d into position %d of the output array\n", 
								string, loc, y_ptr[loc], loc + high + 1 );  
							write(1, buf, strlen(buf));
							merge_ptr[loc + high + 1] = y_ptr[loc];
						}
						//after value is placed exit
						shmdt(x_ptr);
						shmdt(y_ptr);
						shmdt(merge_ptr);
						exit(1);	

					}
					else if(x_ptr[mid] > y_ptr[loc]){
						high = mid - 1;
					}
					else if(x_ptr[mid] < y_ptr[loc]){
						low = mid + 1;
					}
					if(low != high){
						sprintf(buf, "%sy[%d] = %d is found between x[%d] = %d and x[%d] = %d\n", 
							string, task, y_ptr[loc], low, x_ptr[low], high, x_ptr[high]);  
						write(1, buf, strlen(buf));
					}
				}
			
			}
				
		}
		//calculating on an x value
		else{
			sprintf(buf, "%shandling x[%d] = %d\n", string, task, x_ptr[task]);  
			write(1, buf, strlen(buf));
			//x[i] < y[0], place in position i	
			if(x_ptr[task] < y0){
				sprintf(buf, "%sx[%d] = %d is found to be smaller than y[0] = %d\n", 
					string, task, x_ptr[task], y_ptr[0] );  
				write(1, buf, strlen(buf));
				sprintf(buf, "%sabout to write x[%d] = %d into position %d of the output array\n", 
					string, task, x_ptr[task], task ); 
				merge_ptr[task] = x_ptr[task];
				shmdt(x_ptr);
				shmdt(y_ptr);
				shmdt(merge_ptr);
				exit(1);
			}
			//x[i] > y[n - 1]
			if(x_ptr[task] > y_ptr[n - 1]){
				//a[i + n]
				sprintf(buf, "%sx[%d] = %d is found to be larger than y[%d] = %d\n", 
					string, task, x_ptr[task], n - 1, y_ptr[n - 1] );  
				write(1, buf, strlen(buf));
				sprintf(buf, "%sabout to write x[%d] = %d into position %d of the output array\n", 
					string, task, x_ptr[task], task + n );  
				write(1, buf, strlen(buf));
				merge_ptr[task + n] = x_ptr[task];
				shmdt(x_ptr);
				shmdt(y_ptr);
				shmdt(merge_ptr);
				exit(1);
			}
			//between x[k - 1] and y[k]
			if(x_ptr[task] >= y0 && x_ptr[task] <= y_ptr[n - 1]){
				//binary search
				int low = 0;
				int high = n - 1;
				int mid;
				while(low <= high){
					mid = (low + high) / 2;
					//edge condition: Has to find where to insert value					
					if(low == mid || mid == high){
						if(x_ptr[task] < y_ptr[mid]){
							sprintf(buf, "%sabout to write x[%d] = %d into position %d of the output array\n", 
								string, task, x_ptr[task], task + mid );  
							write(1, buf, strlen(buf));
							merge_ptr[task + mid] = x_ptr[task];
						}
						else if(x_ptr[task] > y_ptr[mid] && x_ptr[task] < y_ptr[high]){
							sprintf(buf, "%sabout to write x[%d] = %d into position %d of the output array\n", 
								string, task, x_ptr[task], task + mid + 1);  
							write(1, buf, strlen(buf));
							merge_ptr[task + mid + 1] = x_ptr[task];
						}
						else if(x_ptr[task] > y_ptr[mid] && x_ptr[task] > y_ptr[high]){
							sprintf(buf, "%sabout to write x[%d] = %d into position %d of the output array\n", 
								string, task, x_ptr[task], task + high + 1);  
							write(1, buf, strlen(buf));
							merge_ptr[task + high + 1] = x_ptr[task];
						}
						//after value is place exit
						shmdt(x_ptr);
						shmdt(y_ptr);
						shmdt(merge_ptr);
						exit(1);	

					}
					else if(y_ptr[mid] > x_ptr[task]){
						high = mid - 1;
					}
					else if(y_ptr[mid] < x_ptr[task]){
						low = mid + 1;
					}
					if(low != high){
						sprintf(buf, "%sx[%d] = %d is found between y[%d] = %d and y[%d] = %d\n", 
							string, task, x_ptr[task], low, y_ptr[low], high, y_ptr[high]);  
						write(1, buf, strlen(buf));
					}
				}
			
			}	
		}
	}
	//clean exit: should be unreachable
	shmdt(x_ptr);
	shmdt(y_ptr);
	shmdt(merge_ptr);
	exit(2);
	
	
}
