// ----------------------------------------------------------- 
// NAME : Michael Glazier                    User ID: mglazier 
// DUE DATE : 11/11/2016                                       
// PROGRAM ASSIGNMENT 4                                        
// FILE NAME : thread.h          
// PROGRAM PURPOSE :                                           
//    header file     
// ----------------------------------------------------------- 
#ifndef _THREAD_H
#define _THREAD_H
// Header files
#include "ThreadClass.h"
class MotherThread : public Thread{
	public:
		MotherThread(int m, int t, int *arr, int f);

	private:
		void ThreadFunc();
		int m;
		int t;
		int *arr;
		int f;
};

class ChildThread : public Thread{
	public:
		ChildThread(int N, int F, int M, int *arr, int T);
	private:
		void ThreadFunc();
		int n;
		int fav;
		int m;
		int *arr;
		int t;
};

class InitThread : public Thread{
public:
	InitThread(int M, int *arr);
private:
	void ThreadFunc();
	int m;
	int *arr;
};

#endif
