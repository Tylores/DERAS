#ifndef SCHEDULEOPERATOR_H_INCLUDED
#define SCHEDULEOPERATOR_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "tsu.h"
#include <string>
#include "Aggregator.hpp"



class ScheduleOperator {
public:

/* Constructor, requires a schedule vector of SetPoint classes. */
/* Also requires a pointer to an aggregator class to send grid commands. */
ScheduleOperator(std::vector<SetPoint> schedule, Aggregator * VPP) :
sched(schedule), ptr_(VPP), index_(0){
// I moved the work in here, to my schedule function, which is called from
// the main line.
}

/* Destructor */
~ScheduleOperator () {};


/* Public Member Variables */
	Aggregator *ptr_;
	std::vector<SetPoint> sched;
    long int currentTime;
    long int days = 1; // Days after 28 Nov 2018.
    long int adj = 103251600 + 86400*days; //Corrects to the right day.
    int testStart = 0;
    int testEnd = 0;
    int testBoth = 0;
    int entry = -1;



/* Public Grid Command function */
void Idle () {
	ptr_->ImportPower(0);
    std::cout << "Idling." << std::endl;
};

void Export (int amount){
    //int watts = std::stoi(amount);
	ptr_->ExportPower(amount);
    std::cout << "Exporting at " << amount << "%." << std::endl;
};

void Import (int amount){
	//int watts = std::stoi(amount);
	ptr_->ImportPower(amount);
	std::cout << "Importing at " << amount << "%." << std::endl;
};


/* Public Loop Function for polling grid the correct grid commands. */
void Loop () {

    currentTime = tsu::GetUTC();  //get the UTC time to compare to sched time.
    currentTime = currentTime - adj; //Only for testing with the test schedule.
    // This will be removed when working with real time schedules.

    /* Iterates through sched vector to find matching entry for currentTime.*/
    for (int i=0 ; i < sched.size() ; i++){

        // The matching entry should have a startTime before currentTime.
        if(sched[i].startDateTime <= currentTime){
            testStart = 1;
        }
        else{
            testStart = 0;
        }

        // The matching entry should have an endTime after currentTime.
        if(sched[i].endDateTime >= currentTime){
            testEnd = 1;
        }
        else{
            testEnd = 0;
        }

        // If both cases true, the matching entry for the currentTime is found.
        if(testStart == 1 && testEnd ==1){
        testBoth = 1;
        entry = i;
        break;
        }

    } // Closes the for loop for finding the matching entry.

    /* Determines which command to send to DERAS. */
    if (testBoth = 0){
        std::cout <<
        "Error. Time not found in Schedule. Default to idle" << std::endl;
        Idle();
    }
    else{
        std::cout << sched[entry].value1 << std::endl;
        if (sched[entry].value1 == 0){
            Idle();
        }
        else{
            Import(sched[entry].value1); // I assumed a RegDown Operation.
        }
    }

};


/* Private member variables */
private:
	unsigned int index_;
};


#endif // SCHEDULEOPERATOR_H_INCLUDED
