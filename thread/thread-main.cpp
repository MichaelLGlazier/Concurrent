// ----------------------------------------------------------- 
// NAME : Michael Glazier                    User ID: mglazier 
// DUE DATE : 10/28/2016                                       
// PROGRAM ASSIGNMENT 3                                        
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//    Calculates prefix sum of an array using threads      
// ----------------------------------------------------------- 
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cmath>

#include "thread.h"
using namespace std;

int main(int argc, char *argv[]){
	int	i = 0;
	//print start up message
	char buf[1000];
	sprintf(buf, "Concurrent Prefix Sum Computation\n");
	write(1, buf, strlen(buf));

	//get size of array
	int n;
	cin >> n;
	sprintf(buf, "Number of input data = %d\n", n);
	write(1, buf, strlen(buf));

	//calculate k in n = 2^k
	int k;
	double temp = logb((double)n);
	k = (int)temp;
	cout << k << endl;

	//initialize array x with values from stdin
	sprintf(buf, "Input array:\n");
	write(1, buf, strlen(buf));

	sprintf(buf, "");
	int x[n];
	for(i = 0; i < n; i++){
		cin >> x[i];
		sprintf(&buf[strlen(buf)], "%4d", x[i]);
	}
	sprintf(&buf[strlen(buf)], "\n");
	write(1, buf, strlen(buf));

	//initialize the first row of array that has k + 1 rows and n columns
	int **array;
	array = new int *[n];
	for(i = 0; i < n; i++){
		array[i] = new int[k + 1];
	}
	for(i = 0; i < n; i++ ){
		array[0][i] = x[i];
	}
	cout << endl;
	
	int j = 0;
	int gap = 1;
	for(i = 1; i <= k; i++){
		PrefixThread *thread[n];
		sprintf(buf, "Run %d:\n", i);
		write(1, buf, strlen(buf));

		//create threads
		for(j = 0; j < n; j++){
			thread[j] = new PrefixThread(array, i, j, gap);
			thread[j]->Begin();
		}
		//wait for child threads
		for(j = 0; j < n; j++){
			thread[j]->Join();
		}
		//print results
		sprintf(buf, "Result after run %d:\n", i);
		write(1, buf, strlen(buf));
		sprintf(buf, "");
		for(j = 0; j < n; j++){
			sprintf(&buf[strlen(buf)], "%4d", array[i][j]);
		}
		sprintf(&buf[strlen(buf)], "\n");
		write(1, buf, strlen(buf));
		gap = gap * 2;
	}

	//output finale results
	sprintf(buf, "Final results after run %d:\n", k);
	write(1, buf, strlen(buf));
	sprintf(buf, "");
	for(j = 0; j < n; j++){
		sprintf(&buf[strlen(buf)], "%4d", array[k][j]);
	}
	sprintf(&buf[strlen(buf)], "\n");
	write(1, buf, strlen(buf));
	Exit();
}