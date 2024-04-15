/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Elevator.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * XinLerou Liu, Xinyu Yang, Yushen Dong, Yujie Yang
 * xinlerou,yxinyu,bildong,jkjkyang
 *
 * Final Project - Elevators
 */

#include "Elevator.h"
#include <iostream>

using namespace std;

void Elevator::tick(int currentTime) {
    
    // Check if the currentTime is divisible by TICKS_PER_ELEVATOR_MOVE and
    // the elevator is servicing a request
    if (currentTime % TICKS_PER_ELEVATOR_MOVE == 0 && servicing) {
        
        // Moves the value of current floor by 1 in the direction
        // of the target floor
        if (currentFloor < targetFloor) {
            currentFloor++;
            
        // Does nothing since currentFloor > targetFloor
        } else if (currentFloor > targetFloor) {
            currentFloor--;
        }
        
        // If the new current floor is the target floor:
        // servicing is set to false
        if (currentFloor == targetFloor) {
            servicing = false;
        }
    }
}

// Sets the targetFloor and marks the Elevator as currently servicing
void Elevator::serviceRequest(int floorNum) {
    targetFloor = floorNum;
    servicing = true;
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

void Elevator::print(ostream &outs) {
    outs << currentFloor;
    if (!servicing){
        outs << "w";
    } else {
        outs << "s" << targetFloor;
    }
}

Elevator::Elevator() {
    currentFloor = 0;
    servicing = false;
	targetFloor = 0;
}

void Elevator::setCurrentFloor(int currentFloorIn) {
    currentFloor = currentFloorIn;
}

bool Elevator::isServicing() const {
	return servicing;
}

int Elevator::getCurrentFloor() const {
    return currentFloor;
}

int Elevator::getTargetFloor() const {
    return targetFloor;
}
