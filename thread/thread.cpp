// ----------------------------------------------------------- 
// NAME : Michael Glazier                    User ID: mglazier 
// DUE DATE : 10/28/2016                                       
// PROGRAM ASSIGNMENT 3                                        
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//    Calculates the sum of two values to partially compute
//    the prefix sum of an array      
// ----------------------------------------------------------- 
#include <iostream>
#include "thread.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cmath>

using namespace std;
PrefixThread::PrefixThread(int **A, int I, int J, int G): gap(G)
, i(I), j(J), a(A)
{
	ThreadName.seekp(0, ios::beg);
	ThreadName << "T: " << i << " , " << j << '\0';
}

// ----------------------------------------------------------- 
// FUNCTION PrefixThread::ThreadFunc                          
//     calcuates the sum of two array elements and places the
//	   in the next row of the array.                            
// PARAMETER USAGE :                                           
//    **A - two dimensional array pointer that stores the values
//		    to be summed and the location of the output.
//	  I - row of **A
//    J - column of **A 
//    G - the gap offset (always a power of 2)              
// FUNCTION CALLED :                                           
//    a list of functions that are called by this one          
// ----------------------------------------------------------- 
void PrefixThread::ThreadFunc(){
	Thread::ThreadFunc();
	char buf[140];
	sprintf(buf, "     Thread %d Created\n", j );
	write(1, buf, strlen(buf));
	if((j - gap) < 0){
		//copy a[i - 1, j] to a[i, j]
		a[i][j] = a[i - 1][j];
		sprintf(buf, "     Thread %d copies %d\n", j , a[i - 1][j] );
		write(1, buf, strlen(buf));
	}
	else{
		//calculate temporay prefix value
		int temp = a[i - 1][j] + a[i - 1][j - gap];
		a[i][j] = temp;
		sprintf(buf, "     Thread %d computes %d + %d\n", j, a[i - 1][j], a[i - 1][j - gap] );
		write(1, buf, strlen(buf));
	}
	sprintf(buf, "     Thread %d exits\n", j);
	write(1, buf, strlen(buf));
	Exit();
}