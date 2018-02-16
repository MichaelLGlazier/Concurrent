// ----------------------------------------------------------- 
// NAME : Michael Glazier                    User ID: mglazier 
// DUE DATE : 12/2/2016                                       
// PROGRAM ASSIGNMENT 5                                        
// FILE NAME :boat-monitor.cpp        
// PROGRAM PURPOSE :                                           
//    Controls the monitor functions of the threads.  
// ----------------------------------------------------------- 
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <time.h> 
#include <stdlib.h>  

#include "ThreadClass.h"
#include "boat-monitor.h"

BoatMon::BoatMon(char *name, int c, int m, int b): Monitor(name, HOARE), c(c), m(m), b(b)
{
	passengers = 0;
	cannibalsOnBoard = 0;
	missionariesOnBoard = 0;
	counter = 0;
	canIndx = 0;
	misIndx = 0;
	inUseIndx = 0;
	rowing = 0;
	inUse[0] = new Condition("inUse1");
	inUse[1] = new Condition("inUse2");
	inUse[2] = new Condition("inUse3");
	loading = new Condition("loading");
}

// ----------------------------------------------------------- 
// FUNCTION  MissionaryArrives                        
//     Handles missionaries attempted to board a boat                          
// PARAMETER USAGE :                                           
//    num: the number of the missionary entering                        
// ----------------------------------------------------------- 
int BoatMon::MissionaryArrives(int num){
	int temp;
	MonitorBegin();

		if(counter >= b){
			temp = -2;
		}
		else if(passengers >= 3){
			//boat is full
			if(rowing == 0){
				//ready to depart
			
				loading->Signal();
			}
			temp = -1;
		}
		//check if a missionary can be loaded up
		else if(passengers >= 0 && cannibalsOnBoard <= 1 ){
			missionariesOnBoard++;
			passengers++;

			//person is on boat
			missionaryArray[misIndx] = num;
			misIndx++;
			
			inUseIndx++;
			
			inUse[inUseIndx - 1]->Wait();
			temp = 1;
		}
		else{
			temp = 0;
		}
		
	MonitorEnd();
	return temp;
}

// ----------------------------------------------------------- 
// FUNCTION  CannibalArrives                        
//     Handles the boarding attempts of cannibles                          
// PARAMETER USAGE :                                           
//    num: the number of the cannible entering                        
// -----------------------------------------------------------
int BoatMon::CannibalArrives(int num)
{
	int temp;
	MonitorBegin();
	//sleep(1);

		
		if(counter >= b){
			temp = -2;
		}
		else if(passengers >= 3){
			//boat is full
			if(passengers >= 3){
				
				
				//ready to depart
				loading->Signal();
			}
			temp = -1;
		}
		//check and see theres a spot for a cannibal
		else if(passengers >= 0 && (cannibalsOnBoard + 1) >! missionariesOnBoard){
			cannibalsOnBoard++;
			passengers++;
			//person on boat
			
			cannibalArray[canIndx] = num;
			
			canIndx++;
			
			inUseIndx++;
			
			inUse[inUseIndx - 1]->Wait();
			temp = 1;
		}
		else{
			temp = 0;
		}
	
	MonitorEnd();
	return temp;
}
// ----------------------------------------------------------- 
// FUNCTION  BoatReady                        
//     Stalls the boat until it is full                         
// Function called :
//	  selectMessage() - handles output message                                                                
// -----------------------------------------------------------
char* BoatMon::BoatReady()
{
	MonitorBegin();
		//if not full wait to fill up
		
		if(passengers <= 3){
			loading->Wait();
			rowing = 1;
			selectMessage();
		}

		
		char array[8];
		if(cannibalsOnBoard == 1){
			sprintf(array, "c%d m%d m%d", cannibalArray[0], missionaryArray[0], missionaryArray[1]);
		}
		else if(cannibalsOnBoard == 3){
			sprintf(array, "c%d c%d c%d"
				, cannibalArray[0], cannibalArray[1], cannibalArray[2]);
		}
		else{
			sprintf(array, "m%d m%d m%d"
				, missionaryArray[0], missionaryArray[1], missionaryArray[2]);
		}
	MonitorEnd();
	return array;
}
// ----------------------------------------------------------- 
// FUNCTION  BoatDone                       
//     Resets the boat and lets other people on it                         
//                       
// -----------------------------------------------------------
int BoatMon::BoatDone()
{
	int ret = 0;
	MonitorBegin();
		passengers = 0;
		cannibalsOnBoard = 0;
		missionariesOnBoard = 0;

		canIndx = 0;
		misIndx = 0;
		inUseIndx = 0;
		rowing = 0;

		inUse[0]->Signal();

		inUse[1]->Signal();

		inUse[2]->Signal();


		if(counter == b){
			char buf[100];
			sprintf(buf, "MONITOR: %3d crosses have been made.\n", counter);
			sprintf(&buf[strlen(buf)], "MONITOR: This river cross is closed indefinitely for renovation.\n");
			lock.Lock();
				write(1, buf,strlen(buf));
			lock.Unlock();
			ret = -1;
		}
	MonitorEnd();
	return ret;
}

// ----------------------------------------------------------- 
// FUNCTION  selectMessage                     
//     handles output                                               
// -----------------------------------------------------------
void BoatMon::selectMessage(){
	char buf[400];
	if(cannibalsOnBoard == 1){
		sprintf(buf, "MONITOR(%d): one cannibal (%d) and two missionaries (%d, %d) are selected\n"
				, counter, cannibalArray[0], missionaryArray[0], missionaryArray[1]);
	}
	else if(cannibalsOnBoard == 3){
		sprintf(buf, "MONITOR(%d): three cannibals (%d, %d, %d) are selected\n"
				, counter, cannibalArray[0], cannibalArray[1], cannibalArray[2]);
	}
	else{
		sprintf(buf, "MONITOR(%d): three missionaries (%d, %d, %d) are selected\n"
				, counter, missionaryArray[0], missionaryArray[1], missionaryArray[2]);
	}
	counter++;
	lock.Lock();
		write(1, buf,strlen(buf));
	lock.Unlock();
}