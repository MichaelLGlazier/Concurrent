// ----------------------------------------------------------- 
// NAME : Michael Glazier                    User ID: mglazier 
// DUE DATE : 10/28/2016                                       
// PROGRAM ASSIGNMENT 3                                        
// FILE NAME : thread.h            
// PROGRAM PURPOSE :                                           
//    Header file for thread.cpp
// ----------------------------------------------------------- 
#ifndef _THREAD_H
#define _THREAD_H
// Header files
#include "ThreadClass.h"
class PrefixThread : public Thread{
	public:
		PrefixThread(int **A, int I, int J, int G);

	private:
		void ThreadFunc();
		int **a;
		int i;
		int j;
		int n;
		int gap;
};
#endif
