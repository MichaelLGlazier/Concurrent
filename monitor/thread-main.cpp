// ----------------------------------------------------------- 
// NAME : Michael Glazier                    User ID: mglazier 
// DUE DATE : 12/2/2016                                       
// PROGRAM ASSIGNMENT 5                                        
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
// 	Simulates missionaries and canibals trying to cross a river
//  in one boat      
// ----------------------------------------------------------- 
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include "thread.h"

using namespace std;


int main(int argc, char *argv[]){
	int c, m, b;
	char buf[1000];

	//input validation
	if(argc != 4){
		//don't have the proper input
		sprintf(buf, "Invalid Input\n");
		write(1, buf, strlen(buf));
		Exit();
	}
	else{

		c = atoi(argv[1]);
		m = atoi(argv[2]);
		b = atoi(argv[3]);
		//check for default values
		if(m == 0 || c == 0 || b == 0){
			c = 8;
			m = 8;
			b = 5;
		}
	}
	//create thread that declares monitor
	Init *init = new Init(c, m ,b);
	init->Begin();
	init->Join();
	int i = 0;
	Boat *boat = new Boat(c, m, b);
	boat ->Begin();

	//create cannibals
	
	Cannibal *can[c];
	for(i = 0; i < c; i++){
		can[i] = new Cannibal(c, m, b, i);
		can[i]->Begin();
	}

	//create missionaries
	Missionary *mis[m];
	for(i = 0; i < m; i++){
		mis[i] = new Missionary(c, m, b, i);
		mis[i]->Begin();
	}

	boat->Join();
	//join
	
	for(i = 0; i < c; i++){
		can[i]->Join();
	}
	
	for(i = 0; i < m; i++){
		mis[i]->Join();
	}
	

	Exit();
}