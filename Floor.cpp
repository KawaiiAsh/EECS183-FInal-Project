/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Floor.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * XinLerou Liu, Xinyu Yang, Yushen Dong, Yujie Yang
 * xinlerou,yxinyu,bildong,jkjkyang
 *
 * Final Project - Elevators
 */


#include "Floor.h"
#include <algorithm>

using namespace std;

int Floor::tick(int currentTime) {
    int count = 0;
    int idx[MAX_PEOPLE_PER_FLOOR];
    
    // Ticks each person on this floor
    // Also removes any Person who explodes
    // Returns the number of exploded people
    for (int i = 0; i < numPeople; i++) {
        if (people[i].tick(currentTime)) {
            idx[count] = i;
            count++;
        }
    }
    removePeople(idx, count);
    return count;
}

void Floor::addPerson(Person newPerson, int request) {
    
    // If there is still room, add newPerson to people.
    if (numPeople < MAX_PEOPLE_PER_FLOOR) {
        people[numPeople] = newPerson;
        numPeople++;
    }
    
    // Updates hasUpRequest or hasDownRequest based on value of request
    if (request != 0) {
        if (request > 0) {
            hasUpRequest = true;
        } else {
            hasDownRequest = true;
        }
    }
}

void Floor::removePeople(const int indicesToRemove[MAX_PEOPLE_PER_FLOOR], 
                         int numPeopleToRemove) {
    
    // Removes objects from people[] at indices specified in indicesToRemove[].
    // The size of indicesToRemove[] is given by numPeopleToRemove.
    int sortedIndices[MAX_PEOPLE_PER_FLOOR];
    copy(indicesToRemove, indicesToRemove + numPeopleToRemove, sortedIndices);
    sort(sortedIndices, sortedIndices + numPeopleToRemove);

    for (int i = numPeopleToRemove - 1; i >= 0; i--) {
        int indexToRemove = sortedIndices[i];
        for (int j = indexToRemove; j < numPeople - 1; j++) {
            people[j] = people[j + 1];
        }
        numPeople--;
    }
    
    // After removals, calls resetRequests() to update hasUpRequest and hasDownRequest.
    resetRequests();
}

// Recalculate requests whenever the people on this floor are added or removed.
void Floor::resetRequests() {
    
    // Search through people to find if there are any pending up requests or down requests.
    hasUpRequest = false;
    hasDownRequest = false;
    
    // Check if values of hasUpRequest and hasDownRequest appropriately.
    for (int i = 0; i < numPeople; i++) {
        int currentFloor = people[i].getCurrentFloor();
        int targetFloor = people[i].getTargetFloor();
        
        if (currentFloor < targetFloor) {
            hasUpRequest = true;
        }
        else if (currentFloor > targetFloor) {
            hasDownRequest = true;
        }
    }
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Floor::Floor() {
    hasDownRequest = false;
    hasUpRequest = false;
    numPeople = 0;
}

void Floor::prettyPrintFloorLine1(ostream &outs) const {
    string up = "U";
    outs << (hasUpRequest ? up : " ") << " ";
    for (int i = 0; i < numPeople; ++i) {
        outs << people[i].getAngerLevel();
        outs << ((people[i].getAngerLevel() < MAX_ANGER) ? "   " : "  ");
    }
    outs << endl;
}

void Floor::prettyPrintFloorLine2(ostream &outs) const {
    string down = "D";
    outs << (hasDownRequest ? down : " ") << " ";
    for (int i = 0; i < numPeople; ++i) {
        outs << "o   ";
    }
    outs << endl;
}

void Floor::printFloorPickupMenu(ostream &outs) const {
    cout << endl;
    outs << "Select People to Load by Index" << endl;
    outs << "All people must be going in same direction!";
    /*  O   O
    // -|- -|-
    //  |   |
    // / \ / \  */
    outs << endl << "              ";
    for (int i = 0; i < numPeople; ++i) {
        outs << " O   ";
    }
    outs << endl << "              ";
    for (int i = 0; i < numPeople; ++i) {
        outs << "-|-  ";
    }
    outs << endl << "              ";
    for (int i = 0; i < numPeople; ++i) {
        outs << " |   ";
    }
    outs << endl << "              ";
    for (int i = 0; i < numPeople; ++i) {
        outs << "/ \\  ";
    }
    outs << endl << "INDEX:        ";
    for (int i = 0; i < numPeople; ++i) {
        outs << " " << i << "   ";
    }
    outs << endl << "ANGER:        ";
    for (int i = 0; i < numPeople; ++i) {
        outs << " " << people[i].getAngerLevel() << "   ";
    }
    outs << endl << "TARGET FLOOR: ";
    for (int i = 0; i < numPeople; ++i) {
        outs << " " << people[i].getTargetFloor() << "   ";
    }
}

void Floor::setHasUpRequest(bool hasRequest) {
    hasUpRequest = hasRequest;
}

bool Floor::getHasUpRequest() const {
    return hasUpRequest;
}

void Floor::setHasDownRequest(bool hasRequest) {
    hasDownRequest = hasRequest;
}

bool Floor::getHasDownRequest() const {
    return hasDownRequest;
}

int Floor::getNumPeople() const {
    return numPeople;
}

Person Floor::getPersonByIndex(int index) const {
    return people[index];
}
