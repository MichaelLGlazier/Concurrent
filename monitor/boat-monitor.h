#ifndef _BOAT_MONITOR_H
#define _BOAT_MONITOR_H

#include "ThreadClass.h"
class BoatMon: public Monitor{
	public:
		BoatMon(char* name, int c, int m, int b);
		int MissionaryArrives(int num);
		int CannibalArrives(int num);
		char* BoatReady();
		int BoatDone();

	private:
		int c;
		int m;
		int b;
		int passengers;
		int cannibalsOnBoard;
		int missionariesOnBoard;
		int counter;
		char waiting;
		int cannibalArray[3];
		int canIndx;
		int missionaryArray[3];
		int misIndx;
		int inUseIndx;
		int rowing;
		Condition *inUse[3];
		Condition *loading;
		Mutex lock;

		void selectMessage();
};
#endif