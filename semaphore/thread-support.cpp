// ----------------------------------------------------------- 
// NAME : Michael Glazier                    User ID: mglazier 
// DUE DATE : 11/11/2016                                       
// PROGRAM ASSIGNMENT 4                                        
// FILE NAME : thread-support.cpp            
// PROGRAM PURPOSE :                                           
//    Contains helper functions for the threads to run     
// ----------------------------------------------------------- 
#include "thread.h"
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "thread-support.h"

using namespace std;
int count = 0; //used for counting when a baby eagle is the last
			   //one to look at the pots

// ----------------------------------------------------------- 
// FUNCTION  ready_to_eat                        
//     Eats from a pot, if p is empty return -1, else returns 1                         
// PARAMETER USAGE : 
//		int *arr - the array of pots 
//	    int p - the pot that will be eaten from
// -----------------------------------------------------------
int Support::ready_to_eat(int *arr, int p){
	//pot is empty
	if(arr[p] == 0){
		return -1;
	}
	//eat from pot
	else{
		arr[p]--;
		return 1;
	}
}
// ----------------------------------------------------------- 
// FUNCTION  finish_eating                        
//     counts how many eagles have finished eating.
//     returns 1 when m eagles have finished.                       
// PARAMETER USAGE : 
//		int m - the number of pots
// -----------------------------------------------------------
int Support::finish_eating(int m){
	count++;
	if(count == m){
		//reset count
		count = 0;
		return 1;
	}
	else{
		return 0;
	}
}
void Support::goto_sleep(){

}
// ----------------------------------------------------------- 
// FUNCTION  food_ready                        
//     fills the pots to full.                      
// PARAMETER USAGE : 
//		int *arr - the array of pots
//		int size - the size of the array
//		int f - the level to fill the pots too
// -----------------------------------------------------------
void Support::food_ready(int *arr, int size, int f){
	int i;
	//fills each pot
	for(i = 0; i < size; i++){
		arr[i] = f;
	}

}
