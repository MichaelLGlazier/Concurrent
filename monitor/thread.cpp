//// ----------------------------------------------------------- 
// NAME : Michael Glazier                    User ID: mglazier 
// DUE DATE : 12/02/2016                                       
// PROGRAM ASSIGNMENT 5                                        
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//    This class controls the boat, missionary, and cannibals  
// ----------------------------------------------------------- 
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <time.h> 
#include <stdlib.h>  
#include "thread.h"
#include "boat-monitor.h"

using namespace std;
Mutex stdoutput;

static BoatMon *bM;

Cannibal::Cannibal(int c, int m, int b, int num): c(c), m(m), b(b), num(num){
	ThreadName.seekp(0, ios::beg);
	ThreadName << "can" << '\0';	
}
Missionary::Missionary(int c, int m, int b, int num): c(c), m(m), b(b), num(num)
{
	ThreadName.seekp(0, ios::beg);
	ThreadName << "miss" << '\0';
}
Boat::Boat(int c, int m, int b): c(c), m(m), b(b)
{
	ThreadName.seekp(0, ios::beg);
	ThreadName << "boat" << '\0';
}

Init::Init(int c, int m, int b): c(c), m(m), b(b)
{
	ThreadName.seekp(0, ios::beg);
}

void Boat::ThreadFunc(){
	Thread::ThreadFunc();
	char buf[200];
	int random;
	int done = 0;
	srand(time(NULL));
	char *list;
	int j = 0;
	//printf("boat start\n");
	for(j = 0; j < b; j++){
		random = rand() % 10 + 1;
		int i = 0;
		//take a rest
		for(i = 0; i < random; i++){
			Delay();
		}

		//ready for next round
		list = bM->BoatReady();

		sprintf(buf, "***** Boat Load (%d): Passenger list (%s)\n", j, list);
		stdoutput.Lock();
			write(1, buf, strlen(buf));
		stdoutput.Unlock();

		//row the boat
		random = rand() % 10 + 1;
		for(i = 0; i < random; i++){
			Delay();
		}

		done = bM->BoatDone();
		if(done == -1){

			Exit();
		}

		sprintf(buf, "***** Boat load (%d): Completed\n", j);
		stdoutput.Lock();
			write(1, buf, strlen(buf));
		stdoutput.Unlock();
	}

	Exit();
}

void Missionary::ThreadFunc(){
	Thread::ThreadFunc();
	char buf[200];
	int random;
	int onBoat = 0;
	char padding[40];
	int m;
	for(m = 0; m < num; m++){
		padding[m] =  ' ';
	}
	srand(time(NULL));

	if(num == 0){
		sprintf(buf, "Missionary %d starts\n", padding, num, num);
	}
	else{
		sprintf(buf, "%sMissionary %d starts\n", padding, num, num);
	}
	stdoutput.Lock();
		write(1, buf, strlen(buf));
	stdoutput.Unlock();
	while(1){
		random = rand() % 10 + 1;
		int i = 0;
		//take a rest
		for(i = 0; i < random; i++){
			Delay();
		}
		onBoat = 0;
		//register to cross river
		//keep getting in line if you can't get on the boat
		while(onBoat != 1){
			onBoat = bM->MissionaryArrives(num);
			//check if boat is gone
			if(onBoat == -2){
				Exit();
			}
			if(onBoat != 1){
				for(i = 0; i < random; i++){
					Delay();
				}
			}
		}

		if(num == 0){
			sprintf(buf, "Missionary %d arrives\n", num, num);
		}
		else{
			sprintf(buf, "%sMissionary %d arrives\n", padding, num, num);
		}
		stdoutput.Lock();
			write(1, buf, strlen(buf));
		stdoutput.Unlock();
		//do stuff
		random = rand() % 20 + 5;
		for(i = 0; i < random; i++){
			Delay();
		}

	}
	Exit();
}

void Cannibal::ThreadFunc(){
	Thread::ThreadFunc();
	char buf[200];
	int onBoat = 0;
	int random;

	char padding[40];
	int m;
	for(m = 0; m < num; m++){
		padding[m] =  ' ';
	}
	if(num == 0){
		sprintf(buf, "Cannibal %d starts\n", num, num);
	}
	else{
		sprintf(buf, "%sCannibal %d starts\n", padding, num, num);
	}
	stdoutput.Lock();
		write(1, buf, strlen(buf));
	stdoutput.Unlock();
	srand(time(NULL));

	while(1){
		random = rand() % 10 + 1;
		int i = 0;
		//take a rest
		for(i = 0; i < random; i++){
			Delay();
		}
		onBoat = 0;
		//printf("%d %d %d %d cannibal\n", c, m, b, num);
		//register to cross river
		while(onBoat != 1){
			onBoat = bM->CannibalArrives(num);

			//check if on boat
			if(onBoat == -2){
				Exit();
			}
			//walk to the back of the line
			if(onBoat != 1){
				for(i = 0; i < random; i++){
					Delay();
				}
			}	
		}
		if(num == 0){
			sprintf(buf, "Cannibal %d arrives\n", num, num);
		}
		else{
			sprintf(buf, "%sCannibal %d arrives\n", padding, num, num);
			
		}
		stdoutput.Lock();
			write(1, buf, strlen(buf));
		stdoutput.Unlock();
		//do stuff
		random = rand() % 20 + 5;
		for(i = 0; i < random; i++){
			Delay();
		}

	}
	Exit();
}

void Init::ThreadFunc(){
	Thread::ThreadFunc();
	
	bM = new BoatMon("boatMon", c, m ,b);
	//make monitor

	Exit();
}
