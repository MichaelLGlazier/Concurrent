// ----------------------------------------------------------- 
// NAME : Michael Glazier                    User ID: mglazier 
// DUE DATE : 11/11/2016                                       
// PROGRAM ASSIGNMENT 4                                        
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//    Simulates a mother eagle feeding n baby eagles t times
//    from m pots.      
// ----------------------------------------------------------- 
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include "thread-support.h"
#include "thread.h"

using namespace std;


int main(int argc, char *argv[]){
	int m, n, t;
	char buf[1000];

	//input validation
	if(argc != 4){
		//don't have the proper input
		sprintf(buf, "Invalid Input\n");
		write(1, buf, strlen(buf));
		Exit();
	}
	else{
		m = atoi(argv[1]);
		n = atoi(argv[2]);
		t = atoi(argv[3]);
		//check that output is valid
		if(m == 0){
			m = 10;
		}
		if(n == 0){
			n = 10;
		}
		if(t == 0){
			t = 10;
		}
		if(m > n || n < 0 || m < 0 || t < 0){
			sprintf(buf, "Invalid Parameters\n");
			write(1, buf, strlen(buf));
		}
	}
	//allocate space for pots
	int *ptr;
	ptr = (int *)malloc(m);
	//initialize semaphores and pots and wait to ensure that it is set
	//before running the other threads
	InitThread *init = new InitThread(m, ptr);
	init->Begin();
	init->Join();

	//make mother thread
	MotherThread *mother;
	mother = new MotherThread(m, t, ptr, 2);
	mother->Begin();

	//make child threads
	ChildThread *child[n];
	int i = 0;
	for(i = 0; i < n; i++){
		child[i] = new ChildThread(i, (i % m), m, ptr, t);
		child[i]->Begin();
	}

	//wait for threads to complete
	mother->Join();
	for(i = 0; i < n; i++){
		child[i]->Join();
	}

	//free memory
	free(ptr);

	Exit();
}