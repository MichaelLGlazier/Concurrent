// ----------------------------------------------------------- 
// NAME : Michael Glazier                    User ID: mglazier 
// DUE DATE : 12/2/2016                                       
// PROGRAM ASSIGNMENT 5                                        
// FILE NAME : thread.h          
// PROGRAM PURPOSE :                                           
//    header file     
// ----------------------------------------------------------- 
#ifndef _THREAD_H
#define _THREAD_H
// Header files
#include "ThreadClass.h"
class Cannibal: public Thread{
	public:
		Cannibal(int c, int m, int b, int num);

	private:
		void ThreadFunc();
		int c;
		int m;
		int num;
		int b;
};

class Missionary : public Thread{
	public:
		Missionary(int c, int m, int b, int num);
	private:
		void ThreadFunc();
		int c;
		int m;
		int b;
		int num;
};

class Boat : public Thread{
	public:
		Boat(int c, int m, int b);
	private:
		void ThreadFunc();
		int c;
		int m;
		int b;
};

class Init : public Thread{
	public:
		Init(int c, int m, int b);
	private:
		void ThreadFunc();
		int c;
		int m;
		int b;

};
#endif
