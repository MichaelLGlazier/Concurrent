// ----------------------------------------------------------- 
// NAME : Michael Glazier                    User ID: mglazier 
// DUE DATE : 11/11/2016                                       
// PROGRAM ASSIGNMENT 4                                        
// FILE NAME : thread-support.h            
// PROGRAM PURPOSE :                                           
//    header file     
// ----------------------------------------------------------- 
#ifndef _THREAD-SUPPORT_H
#define _THREAD-SUPPORT_H
// Header files
#include "ThreadClass.h"
class Support{
public:
		int ready_to_eat(int *arr, int p);
		int finish_eating(int m);
		void goto_sleep();
		void food_ready(int *arr, int size, int f);
};
#endif
