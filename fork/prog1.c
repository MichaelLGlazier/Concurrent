//------------------------------------------------------
//Name : Michael Glazier		User ID:mglazier
//Due Date : 09/23/2016
//Program Assignment 1
//File Name : prog1.c
//Program Purpose : This programs purpose is to solve
// four different problems concurrently.

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <float.h>

/*Prototypes*/
long fib(long n);
void buildHeap(int *ptr, int *unsortedArr, int size);
void maxHeapify(int *ptr, int index, int size);
void heapSort(int *ptr);

int main(int argc, char* argv[]){
	int m; int n; int r; int s;
	char buf[2000];
	if(argc != 5){
		sprintf(buf, "error, not enough arguements \n");
		write(1, buf, strlen(buf));
		exit(-1);
	}
	else{
		m = atoi(argv[1]);
		n  = atoi(argv[2]);
		r = atoi(argv[3]);
		s = atoi(argv[4]);
	}
	sprintf(buf, "Main Process Started\n");
	write(1, buf, strlen(buf));
	/*Intializes random number generator*/
	srand(time(NULL));
	pid_t pid;
	int status;
	int runNum = 0;
	int i = 0;
	for(i = 0; i < 4; i++){
		pid = fork();
		/*If a child then do something else*/
		if(pid == 0){
			switch(i){
				/*Creation statements*/
				case 0:
					sprintf(buf, "Heap Sort Process Created\n");
					write(1, buf, strlen(buf));
					break;
				case 1:
					sprintf(buf, "Buffon's Needle Iterations\n");
					write(1, buf, strlen(buf));
					break;
				case 2:
					sprintf(buf, "Fibonacci Process Created\n");
					write(1, buf, strlen(buf));
					break;
				case 3:
					sprintf(buf, "Integration Process Created\n");
					write(1, buf, strlen(buf));
					break;
				default:
					break;
			}
			runNum = i;
			break;
		}
		if(i == 3){
			
			runNum = 4;
		}
	}

	/*Run heap Sort*/
	if(runNum == 0){
	  sprintf(buf, "   Heap Sort Process Started\n");
	  write(1, buf, strlen(buf));

	  int arr[m];
	  int offset = 0;
	  /*Calculate rows of 20 for output*/
	  float rows = (float)m / 20;
	  rows = ceilf(rows);
	  
	  
	  /*fill an array of m size with random numbers*/
	  char const rndGen[] = {"   Random Numbers Generated:\n"};
	  sprintf(buf, "%s", rndGen);

	  offset = offset + (int)strlen(rndGen);
	  for(i = 0; i < m; i++){
	    arr[i] = rand() % 100;
	  }
	  /*format random numbers for printing and 
	    add random numbers to buffer for printing*/
	  int a = 0;	  
	  for(i = 0; i < (int)rows; i++){
		/*Print columns of 20*/
		for(a = 0; a < 20; a++){

			/*Handle case where arr is out of elements*/
			if((a + (i * 20)) >= m){
	  			sprintf(&buf[m * 4 + offset], "\n");
				break;
			}
			/*handle end of row scenario*/
			if(a == 19){
				arr[19] = 11;
				sprintf(&buf[((a + (i * 20))) * 4 + offset], "%4d", arr[a + (i * 20)]);
				offset = offset + 4;
	  			sprintf(&buf[((a + (i * 20))) * 4 + offset], "\n");
				offset = offset - 3;
			}
			else if(a == 0){
				sprintf(&buf[((a + (i * 20))) * 4 + offset], "   %4d", arr[a + (i * 20)]);
				offset = offset + 3;
			}
			/*Handle adding element to column*/
			else{
				sprintf(&buf[((a + (i * 20))) * 4 + offset], "%4d", arr[a + (i * 20)]);	
			}			
		}
	  }
	  write(1, buf, strlen(buf));
	

	  /*Build heap*/
	  /*allocate memory for heap*/
	  int *heap = malloc(m * sizeof *heap);
	  int *unsortedArr = arr;
	  buildHeap(heap, unsortedArr, m);

	  int output[m];

	  /*HeapSort*/
	  for(i = m - 1; i >= 0; i--){
		output[i] = heap[0];
	    	int temp = heap[0];
    		heap[0] = heap[i];
    		heap[i] = temp;
		maxHeapify(heap, 0, i);
	  }
	  sprintf(buf, "   Sorted Sequence:\n");
	  offset = (int)strlen(buf);

	  for(i = 0; i < (int)rows; i++){
		/*Print columns of 20*/
		for(a = 0; a < 20; a++){

			/*Handle case where arr is out of elements*/
			if((a + (i * 20)) >= m){
	  			sprintf(&buf[m * 4 + offset], "\n");
				break;
			}
			/*handle end of row scenario*/
			if(a == 19){
				arr[19] = 11;
				sprintf(&buf[((a + (i * 20))) * 4 + offset], "%4d", output[a + (i * 20)]);
				offset = offset + 4;
	  			sprintf(&buf[((a + (i * 20))) * 4 + offset], "\n");
				offset = offset - 3;
			}
			/*Handle first of row*/
			else if(a == 0){
				sprintf(&buf[((a + (i * 20))) * 4 + offset], "   %4d", output[a + (i * 20)]);
				offset = offset + 3;
			}
			/*Handle adding element to column*/
			else{
				sprintf(&buf[((a + (i * 20))) * 4 + offset], "%4d", output[a + (i * 20)]);	
			}			
		}
	  }
	  offset = (int)strlen(buf);
	  sprintf(&buf[offset],"   Heap Sort Process Exits\n");
	  write(1, buf, strlen(buf));
	  /*free heap memory*/
	  free(heap);
	  
	}
	/*Run Fibonacci Recursivly*/
	else if(runNum == 1){
		int offset = 0;
	  	sprintf(buf, "      Fibinacci Process Started\n");
		write(1, buf, strlen(buf));		
		
		sprintf(buf, "      Input Number %d\n", n);
		write(1, buf, strlen(buf));

		long results = fib((long)n);
		//printf("%ld\n", results);

		sprintf(buf, "      Fibonacci Number f(%d) is %ld\n", n, results);
		offset = (int)strlen(buf);
		sprintf(&buf[offset], "      Fibonacci Process Exits\n");
		write(1, buf, strlen(buf));
	}
	/*Run Buffon's Needle Algorithm*/
	else if(runNum == 2){
		sprintf(buf, "         Buffon's Needle Process Started\n");
		int offset = (int)strlen(buf);
		sprintf(&buf[offset], "         Input Number %d\n", r);
		write(1, buf, strlen(buf));
		int i;
		float d;
		float a;
		int t = 0;
		float results;
		float finalResults;
		/*Calculate when a needle crosses a line*/
		for(i = 0; i < r; i++){
			d = ((float)(rand()/ (float)(RAND_MAX)));
			a = ((float)(rand() / (float)(RAND_MAX)) * (2 * M_PI - 0));

			results = d + 1 * sinf(a);
			if(results < 0 || results > 1){
				t = t + 1;
				
			}			
		}

		finalResults = (float)t/r;
		
		sprintf(buf, "         Estimated Probability is %.5f\n", finalResults);
		offset = (int)strlen(buf);
		sprintf(&buf[offset], "         Buffon's Needle Process Exits\n");
		write(1, buf, strlen(buf));
	}
	/*Run Integration Algorithm*/
	else if(runNum == 3){
		sprintf(buf, "            Integration Process Started\n");
		int offset = (int)strlen(buf);
		sprintf(&buf[offset], "            Input Number %d\n", s);
		write(1, buf, strlen(buf));
		float a;
		float b;
		float sinA;
		float results;
		int i = 0;
		int t = 0;
		for(i = 0; i < s; i++){
			/*Calculate random a and b from range 0 <= a < PI and 0 <= b < 1 */
			/*Subtract FLT_MIN to ensure that random number never equals PI or 1*/
			a = ((float)(rand()/ (float)(RAND_MAX))) * (M_PI - FLT_MIN);
			b = ((float)(rand() / (float)(RAND_MAX)) * (1.0 - FLT_MIN));
			sinA = sinf(a);
			if(b <= sinA){
				t = t + 1;	
			}
		}	
		sprintf(buf, "            Total Hit %d\n", t);
		write(1, buf, strlen(buf));
		results = ((float)t / (float)s) * M_PI;

		sprintf(buf, "            Estimated Area is %f\n", results);
		offset = (int)strlen(buf);
		sprintf(&buf[offset], "            Integration Process Exits\n");
		write(1, buf, strlen(buf));
	}
	else if(runNum == 4){
		int i = 0;
		int offset = 0;
		sprintf(buf, "Main Process Started\n");
		offset = (int)strlen(buf);

		sprintf(&buf[offset], "Number of Random Numbers   = %d\n", n);
		offset = (int)strlen(buf);
		
		sprintf(&buf[offset], "Fibonacci Input            = %d\n", m);
		offset = (int)strlen(buf);
		
		sprintf(&buf[offset], "Buffon's Needle Iterations = %d\n", s);
		offset = (int)strlen(buf);

		sprintf(&buf[offset], "Integration Iterations     = %d\n", r);
		offset = (int)strlen(buf);
		
		sprintf(&buf[offset], "Main Process Waits\n");
		offset = (int)strlen(buf);
		write(1, buf, strlen(buf));
		/*Wait for child processes to exit*/
		for(i = 0; i < 4; i++){
			wait(&status);
		} 
	
		sprintf(buf, "Main Process Exits\n");
		write(1, buf, strlen(buf));
	}
	/*Improper amount of child processes created*/
	else{
		sprintf(buf, "Failed to assign process to this child %1d \n", 
		getpid());
		write(1, buf, strlen(buf)); 
	}

	exit(1);
		
}
//--------------------------------------------------
//FUNCTION fib:
//	Recursively calls fib to calculate the nth 
//	fib number.
//PARAMETER USAGE:
//	long n - nth fibannaci number to be calculated
//FUNCTION CALLED:
//	fib(long n)
//--------------------------------------------------
long fib(long n){
		if(n == 0){
			return 0;
		}
		else if(n == 1){
			return 1;
		}
		else{
			return (fib(n - 1) + fib(n - 2));
		}
}
//--------------------------------------------------
//FUNCTION buildHeap:
//	Builds a heap by calling maxHeapify. Requires
//	memory allocation by the function caller.
//PARAMETER USAGE:
//	int *ptr - points to heap array
//	int *unsortedArr - points to unsorted array
//			   that is going to be sorted
//	int size - size of the heap array
//FUNCTION CALLED:
//	maxHeapify(int *ptr, int index, int size);
//--------------------------------------------------
void buildHeap(int *ptr, int *unsortedArr, int size){
  int i = 0;
  /*add unsorted array to heap*/
  for(i = 0; i < size; i++){
    ptr[i] = *(unsortedArr + i);
    /* printf("Pointer test %d %d\n", *(ptr + i), *(unsortedArr + i));*/
  }
  for(i = size / 2 - 1; i >= 0; i--){
    /*Call maxHeapify*/
    maxHeapify(ptr, i, size);
    //printf("%d %d \n", l , r);
  }
}
//--------------------------------------------------
//FUNCTION maxHeapify:
//	Correctly places all elements into the heap
//	by maintaining max heap properties.
//PARAMETER USAGE:
//	int *ptr - points to heap array
//	int index - index of heap
//	int size - size of the heap
//FUNCTION CALLED:
//	maxHeapify(int *ptr, int index, int size);
//--------------------------------------------------
void maxHeapify(int *ptr, int index, int size){
  int l = 2 * index + 1;
  int r = 2 * index + 2;
  int greatest = index;
  if(l < size && ptr[l] > ptr[index]){
    greatest = l;
  }
  if(r < size  && ptr[r] > ptr[greatest]){
    greatest = r;
  }
  if(greatest != index){

    int temp = ptr[index];
    ptr[index] = ptr[greatest];
    ptr[greatest] = temp;
    /*Recursive call to properly place element into heap*/
    maxHeapify(ptr, greatest, size);
  }
  
}


