#include <iostream>
#include <queue>
#include <set>
#include <math.h>
#include "helpers.h"
#include "globals.h"
#include "circuit.h"
#include "circuitgraphics.h"

using namespace std;

// TODO: need to figure out how to keep track of resistors
CircuitMatrix * findEquivalentResistanceCircuit (CircuitMatrix * cMx, int targetResistance, int maxResistors, float MoE, vector<int> * resistors, set<int> * circuitHashSet) {
    CircuitMatrix * bestCandidate = cMx;
    CircuitMatrix * candidate;
    queue<CircuitMatrix*> circuitQueue; // for non-recursive
    unsigned int minDifference = 0xFFFF; // for non-recursive
    unsigned int difference;
    for (int i = 0; i < resistors->size(); i++) {
        int r = resistors -> at(i);
        candidate = new CircuitMatrix(); //bestCandidate;
        candidate->layResistor(r, 0, 1);
        difference = abs(candidate->getTotalResistance() - targetResistance);
        if (difference <= MoE*targetResistance) {
            return candidate;
        }
        if (difference < minDifference) {
            minDifference = difference;
            bestCandidate = candidate;
        }
        if (!circuitHashSet->count(candidate->hashCode())){
            circuitQueue.push(candidate);
        }
    }
    while (!circuitQueue.empty()) {
        candidate = circuitQueue.front();
        circuitQueue.pop();
        for (int i = 0; i < resistors->size(); i++) { // <- this is gonna fail!!!
        /* also, there is probably a way to check in the first loop that something has
        already been tried before inserting into matrix, like if (r has equaled resistors->get(i)) */
            int r = resistors->at(i);
            int size = candidate->getSize();
            for (int i = 0; i < size-1; i++) {
                for (int j = 0; j < size; j++) {
                    if (size < maxResistors && i!=j && candidate->layResistor(r,i,j) && !circuitHashSet->count(candidate->hashCode())) {
                        difference = abs(candidate->getTotalResistance() - targetResistance);
                        if (difference <= MoE*targetResistance) {
                            return candidate;
                        }
                        if (difference < minDifference) {
                            minDifference = difference;
                            bestCandidate = candidate;
                        }
                        cout << "Candidate: " << candidate->toString();
                        cout << "Best candidate: " << bestCandidate->toString();
                        circuitQueue.push(candidate);
                        cout << "Enter q to quit\n";
                        string response;
                        cin >> response;
                        if (!response.compare("q")) return bestCandidate;
                    }
                }
            }
        }
    }
    return bestCandidate;
}

// TODO: create hash function for CircuitMatrix
CircuitMatrix * findEquivalentResistanceCircuit (int targetResistance, int maxResistors, float MoE, vector<int> resistors) {
    cout << "Searching for a circuit with equivalent resistance in the range of " << targetResistance*(1-MoE) << " and " << targetResistance*(1+MoE) << " using the following resistors:\n";
    cout << "{" << intVectorToString(resistors) << "}\n";
    set<int> circuitHashSet;
    CircuitMatrix * bestCandidate = new CircuitMatrix();
    unsigned int sum = 0;
    for (int r: resistors) {
        sum += r;
    }
    if (sum >= targetResistance*(1-MoE)) {
        vector<int> resistorsCopy = resistors;
        bestCandidate = findEquivalentResistanceCircuit(bestCandidate, targetResistance, maxResistors, MoE, &resistorsCopy, &circuitHashSet);
        cout << "Found a circuit with total resistance = " << bestCandidate->getTotalResistance() << endl;//:\n" << bestCandidate->toString();
        cout << "Explanation: \n";
        int size = bestCandidate->getSize();
        for (int i = 0; i < size-1; i++) {
            for (int j = i; j < size; j++) {
                if (i!=j && bestCandidate->getResistance(i,j)>0) {
                    cout << "From node " << i << " to node " << j << ": " << intVectorToString(bestCandidate->getResistors(i,j)) << " in parallel.\n";
                }
            }
        } 
    } else {
        cout << "No resistors from the given set that can achieve this resistance.\n";
    }
    return bestCandidate;
}

int main(int argc, char** argv) {
    vector<int> resistorSet = {100, 100, 500};
    CircuitMatrix * cMx = findEquivalentResistanceCircuit(50, 3, 0.05, resistorSet);
}