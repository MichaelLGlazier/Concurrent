//// ----------------------------------------------------------- 
// NAME : Michael Glazier                    User ID: mglazier 
// DUE DATE : 11/11/2016                                       
// PROGRAM ASSIGNMENT 4                                        
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//    Contains the eagles, mother, and init threads. Also 
//	  contains the semaphore and mutex controls that assist
//	  the threads.    
// ----------------------------------------------------------- 
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <time.h> 
#include <stdlib.h>  
#include "thread.h"
#include "thread-support.h"

using namespace std;
Semaphore *slots, *sleepSem;
Mutex **lock;
Mutex counter, counter2;
//tracts the amount of times the babies have eaten
int cycleCounter = 0;
// ----------------------------------------------------------- 
// FUNCTION  eat                          
//     decrements the value of a pot indicating that it was eaten from                            
// PARAMETER USAGE :                                           
//    int p: The pot being eaten from.              
// -----------------------------------------------------------
void eat(int p, int *arr){
	arr[p]--;
}
// ----------------------------------------------------------- 
// FUNCTION  fillPots                         
//     fills the pots in the pots array to a certain level                            
// PARAMETER USAGE : 
//		int *arr - the array of pots 
//	    int size - the size of the arr array
//		int level - the value to set each pot equal too.
// FUNCTION CALLED :                                           
//    ready_to_eat(), finish_eating()        
// -----------------------------------------------------------
void fillPots(int *arr, int size, int level){
	int i = 0;
	for(i; i < size; i++){
		arr[i] = level;
	}
}

MotherThread::MotherThread(int m, int t, int *arr, int f): m(m), t(t), arr(arr), f(f){
	ThreadName.seekp(0, ios::beg);
	ThreadName << "M" << '\0';	
}
ChildThread::ChildThread(int N, int F, int M, int *arr, int T):n(N), fav(F), m(M), arr(arr), t(T)
{
	ThreadName.seekp(0, ios::beg);
	ThreadName << "c: " << n << '\0';
}
InitThread::InitThread(int M, int *arr): m(M), arr(arr)
{
	ThreadName.seekp(0, ios::beg);
	ThreadName << "init" << '\0';
}
// ----------------------------------------------------------- 
// FUNCTION  ChildThread                         
//     Simulates the child eagles play and feeding cycles                            
// PARAMETER USAGE : 
//	    int N - number of eagles
// 		int F - the eagles favorite pot to start at.                                          
//      int M - the number of pots
//		int *arr - the array of pots
//		int T - the number of cycles they will eat 
// FUNCTION CALLED :                                           
//    ready_to_eat(), finish_eating()        
// -----------------------------------------------------------
void ChildThread::ThreadFunc(){
	Thread::ThreadFunc();
	char buf[140];
	//set random seed
	srand(time(NULL));
	int random;
	int i;
	int count = 0;
	int ret;
	int pos;
	int noFood;
	int practice;
	int lastCycle = 0;
	sprintf(buf, "%*s", n + 1, " ");
	sprintf(&buf[strlen(buf)], "Baby eagle %d has started\n", n + 1);
	write(1, buf, strlen(buf));

	Support sup;
	while(1){
		
		//Mom has left, can leave the nest
		counter2.Lock();
		if(cycleCounter >= (t + 1)){
			counter2.Unlock();
			Exit();
		}
		counter2.Unlock();
		noFood = 1;
		//generate random number between 1 and 5
		random = rand() % 12 + 1;
		for(i = 0; i < random; i++){
			//play for random amount of time
			Delay();
		}


		//only m eagles can eat at once
		slots->Wait();
		//printf("%d %d\n", cycleCounter, count);
		counter2.Lock();
		//handles outputs for general case and last cycle case
		if(lastCycle){
			if(count < 2){
				sprintf(buf, "%*s", n + 1, " ");
				sprintf(&buf[strlen(buf)], "Baby eagle %d is ready to eat\n", n + 1);
				write(1, buf, strlen(buf));
			}
		}
		else{
			sprintf(buf, "%*s", n + 1, " ");
			sprintf(&buf[strlen(buf)], "Baby eagle %d is ready to eat\n", n + 1);
			write(1, buf, strlen(buf));
			
		}

		counter2.Unlock();
		for(i = 0; i < m; i++){
			count++;
			pos = (fav + i) % m;
			//try to eat from favorite pot first
			//Lock to prevent others from eating in it at the same time
			lock[pos]->Lock();
			ret = sup.ready_to_eat(arr, pos);
			lock[pos]->Unlock();

			//I got my food, so i can go play and let someone else in.
			if(ret == 1){
				sprintf(buf, "%*s", n + 1, " ");
				sprintf(&buf[strlen(buf)], "Baby eagle %d is eating using feeding pot %d\n", n + 1, pos);
				write(1, buf, strlen(buf));
				slots->Signal();
				noFood = 0;
				break;
			}
			//If I didn't get any food, try the next pot
			//If none had food, cry to mom, and dont send
			//another slot signal
		}
		//can only go through the complete loop if every pot was empty
		if(noFood){
			//protect counter while manipulating value;
			counter.Lock();
			if(sup.finish_eating(m)){
				count = 0;
				counter2.Lock();
				cycleCounter++;
				if(cycleCounter >= (t + 1)){
					for(i = 0; i < m; i++){
						slots->Signal();
					}
				}
				if(cycleCounter <= t){
					sprintf(buf, "%*s", n + 1, " ");
					sprintf(&buf[strlen(buf)], "Baby eagle %d sees all feeding pots are empty and wakes up the mother\n", n + 1);
					write(1, buf, strlen(buf));

					sprintf(buf, "Mother eagle is awoke by baby eagle %d and starts preparing food\n", n + 1);
					write(1, buf, strlen(buf));
					printf("%d\n", cycleCounter);
					if(cycleCounter >= t){
						lastCycle = 1;
					}
				}
				counter2.Unlock();
				
				//wake up mom
				sleepSem->Signal();
			}
			counter.Unlock();		
		}
	}

	Exit();
}

// ----------------------------------------------------------- 
// FUNCTION  MotherThread                         
//     Simulates the mother eagles sleep and food preparing 
//	   cycles                            
// PARAMETER USAGE :                                           
//      int m - the number of pots
//		int t - the number of feeding cycles
//		int *arr - the array of pots
//		int f - the level of the pots             
// FUNCTION CALLED :                                           
//    food_ready()          
// -----------------------------------------------------------
void MotherThread::ThreadFunc(){
	Thread::ThreadFunc();
	int i, j;
	Support sup;
	//set random seed
	srand(time(NULL));
	int random;
	char buf[140];
	sprintf(buf, "Mother eagle started.\n");
	write(1, buf, strlen(buf));

	for(j = 0; j < t; j++){
		sprintf(buf, "Mother eagle takes a nap\n");
		write(1, buf, strlen(buf));
		//mom waits to be woke up
		sleepSem->Wait();
		
		random = rand() % 10 + 1;
		for(i = 0; i < random; i++){
			//Prepare food for random amount of time
			Delay();
		}
		sprintf(buf, "Mother eagle says \"Feeding (%d)\"\n", j + 1);
		write(1, buf, strlen(buf));

		sup.food_ready(arr, m, f);

		//let baby eagles eat again
		for(i = 0; i < m; i++){
			slots->Signal();
		}

	}

	sprintf(buf, "Mother eagle retires after %d feedings. Game ends!!!\n", t);
	write(1, buf, strlen(buf));
	Exit();
}

// ----------------------------------------------------------- 
// FUNCTION InitThread                       
//     Handles initualization of variable semaphores
//     and mutux arrays, as well as fill pots in the
//     pots array.                          
// PARAMETER USAGE :                                           
//      int m - the number of pots
//		int *arr - the array of pots           
// FUNCTION CALLED :                                           
//    food_ready()          
// -----------------------------------------------------------
void InitThread::ThreadFunc(){
	Thread::ThreadFunc();
	//declare value of slots
	slots = new Semaphore("slots", m);
	//mom starts asleep
	sleepSem = new Semaphore("sleep", 0);
	int i;
	char str[15];
	lock = new Mutex *[m];
	for(i = 0; i < m; i++){
		sprintf(str, "lock: %d", i);
		lock[i] = new Mutex(str);
	}
	//fill pots with 2 feeding levels
	fillPots(arr, m, 2);

	Exit();
}
