#include "AI.h"
#include <cassert>
#include <algorithm>
#include <cmath>
#include <sstream>

using namespace std;

string getAIMoveString(const BuildingState &state) {
    int anger[10] = {0}, wanted = 0, maxAnger = 0;
    bool filled[10] = {false};

    for (int a = 0; a < NUM_FLOORS; a++) {
        for (int i = 0; i < state.floors[a].numPeople; i++)
            anger[a] += state.floors[a].people[i].angerLevel;

        if (maxAnger < anger[a]) {
            maxAnger = anger[a];
            wanted = a;
        }
    }

    for (int e = 0; e < NUM_ELEVATORS; e++) {
        int target = state.elevators[e].targetFloor;
        if (target < 10 && target >= 0) filled[target] = true;
    }

    int minDist = 100, elevator = -1;
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        if (!state.elevators[i].isServicing) {
            int dist = abs(wanted - state.elevators[i].currentFloor);
            if (minDist > dist) {
                minDist = dist;
                elevator = i;
            }
        }
    }

    if (elevator != -1) {
        int currentFloor = state.elevators[elevator].currentFloor;
        int numPeople = state.floors[currentFloor].numPeople;
        if (currentFloor != wanted && !filled[wanted])
            return "e" + to_string(elevator) + "f" + to_string(wanted);
        else if (numPeople != 0)
            return "e" + to_string(elevator) + "p";
    }
    return "";
}

string getAIPickupList(const Move &move, const BuildingState &state, const Floor &floor) {
    int sumAngerUp = 0, sumAngerDown = 0, countUp = 0, countDown = 0;
    string demandUp = "", demandDown = "";

    for (int i = 0, n = floor.getNumPeople(); i < n; i++) {
        Person p = floor.getPersonByIndex(i);
        int deltaFloor = p.getTargetFloor() - p.getCurrentFloor(), anger = p.getAngerLevel() + 1;
        if (deltaFloor > 0) {
            sumAngerUp += anger;
            demandUp += to_string(i);
            countUp++;
        } else {
            sumAngerDown += anger;
            demandDown += to_string(i);
            countDown++;
        }
    }

    return (sumAngerUp > sumAngerDown && countUp < countDown) || (sumAngerUp >= sumAngerDown && countUp > countDown) ||
           countDown == 0 ? demandUp : demandDown;
}
