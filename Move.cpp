/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Move.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * XinLerou Liu, Xinyu Yang, Yushen Dong, Yujie Yang
 * xinlerou,yxinyu,bildong,jkjkyang
 *
 * Final Project - Elevators
 */
 
#include <cmath>
#include <sstream>
#include <stdio.h>      
#include <stdlib.h>
#include "Move.h"

using namespace std;

// creates a Move object based on the characters in commandString
Move::Move(string commandString) : Move() {
    
    // empty commandString
    if (commandString.empty()) {
        isPass = true;
        return;
    }

    char firstChar = commandString.at(0);

    if (firstChar == 'q' || firstChar == 'Q') {
        isQuit = true;
        return;
    }
    else if (firstChar == 's' || firstChar == 'S') {
        isSave = true;
        return;
    }
    else if (firstChar == 'e') {
        elevatorId = static_cast<int>(commandString.at(1)) - 48;

        if (commandString.at(2) == 'p') {
            isPickup = true;
        }
        else if (commandString.at(2) == 'f') {
            targetFloor = static_cast<int>(commandString.at(3)) - 48;
        }
    }
}

// returns true if this Move instance is valid, false otherwise
bool Move::isValidMove(Elevator elevators[NUM_ELEVATORS]) const {
    if (isPassMove() || isQuitMove() || isSaveMove()) {
        return true;
    }

    if (elevatorId >= 0 && elevatorId < NUM_ELEVATORS && !elevators[elevatorId].isServicing()) {
        if (isPickupMove()) {
            return true;
        }
        else if (targetFloor >= 0 && targetFloor < NUM_FLOORS && targetFloor != elevators[elevatorId].getCurrentFloor()) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}


void Move::setPeopleToPickup(const string& pickupList, const int currentFloor, const Floor& pickupFloor) {
    
    // sets numPeopleToPickup and totalSatisfaction to 0
    // adds the indices specified in pickupList to peopleToPickup
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
    int maxFloorDiff = 0;

    // increments numPeopleToPickup by 1 for each person being picked up
    for (int i = 0; i < pickupList.length(); i++) {
        peopleToPickup[i] = static_cast<int>(pickupList.at(i)) - 48;
        numPeopleToPickup++;
        
        // adds satisfaction gained from each person picked up to totalSatisfaction
        Person p = pickupFloor.getPersonByIndex(peopleToPickup[i]);
        totalSatisfaction += (MAX_ANGER - p.getAngerLevel());
        
        // sets targetFloor to be the most extreme floor of those
        // being picked up (furthest up or down)
        int floorDiff = abs(p.getTargetFloor() - currentFloor);
        if (floorDiff > maxFloorDiff) {
            maxFloorDiff = floorDiff;
            targetFloor = p.getTargetFloor();
        }
    }
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Move::Move() {
    elevatorId = -1;
    targetFloor = -1;
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
	isPass = false;
    isPickup = false;
    isSave = false;
    isQuit = false;
}

bool Move::isPickupMove() const {
    return isPickup;
}

bool Move::isPassMove() const {
	return isPass;
}

bool Move::isSaveMove() const {
	return isSave;
}

bool Move::isQuitMove() const {
	return isQuit;
}

int Move::getElevatorId() const {
    return elevatorId;
}

int Move::getTargetFloor() const {
    return targetFloor;
}

int Move::getNumPeopleToPickup() const {
    return numPeopleToPickup;
}

int Move::getTotalSatisfaction() const {
    return totalSatisfaction;
}

void Move::setTargetFloor(int inTargetFloor) {
    targetFloor = inTargetFloor;
}

void Move::copyListOfPeopleToPickup(int newList[MAX_PEOPLE_PER_FLOOR]) const {
    for (int i = 0; i < numPeopleToPickup; ++i) {
        newList[i] = peopleToPickup[i];
    }
}
