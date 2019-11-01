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
CircuitMatrix * findEquivalentResistanceCircuit (Circuit Matrix * cMx, int targetResistance, int maxResistors, float MoE, vector<int> resistors, set<int> circuitHashSet) {
    CircuitMatrix * bestCandidate = cMx;
    queue<CircuitMatrix*> circuitQueue; // for non-recursive
    unsigned int minDifference = 0xFFFF; // for non-recursive
    for (int i = 0; i < resistors->size(); i++) {
        int r = resistors -> get(i);
        CircuitMatrix candidate = *bestCandidate;
        candidate.layResistor(r, 0, 1);
        int difference = abs(candidate.getTotalResistance - targetResistance);
        if (difference < MoE*targetResistance) {
            return &candidate;
        }
        if (difference < minDifference) {
            minDifference = difference;
            bestCandidate = &candidate;
        }
        if (circuitHashSet->find(candidate.hashCode())!=circuitHashSet.end()) {
            circuitQueue.push(&candidate);
        }
    }
    while (!circuitQueue.empty()) {
        candidate = circuitQueue.pop();
        for (int i = 0; i < resistors->size(); i++) { // <- this is gonna fail!!!
        /* also, there is probably a way to check in the first loop that something has
        already been tried before inserting into matrix, like if (r has equaled resistors->get(i)) */
            int r = resistors->get(i);
            int size = candidate->getSize();
            for (int i = 0; i < size-1; i++) {
                for (int j = 0; j < size; j++) {
                    if (size < maxResistors && i!=j && candidate->layResistor(r,i,j) && circuitHashSet->find(candidate->hashCode())!=circuitHashSet->end()) {
                        difference = abs(candidate->getTotalResistance() - targetResistance);
                        if (difference < MoE*targetResistance) {
                            return candidate;
                        }
                        if (difference < minDifference) {
                            minDifference = difference;
                            bestCandidate = candidate;
                        }
                        circuitQueue.push(candidate);
                    }
                }
            }
        }
    }
    return bestCandidate;
}

// TODO: create hash function for CircuitMatrix
CircuitMatrix * findEquivalentResistanceCircuit (int targetResistance, int maxResistors, float MoE, vector<int> resistors) {
    cout << "Target resistance: " << MoE*targetResistance << endl;
    set<int> circuitHashSet;
    CircuitMatrix * bestCandidate = new CircuitMatrix();
    unsigned int sum = 0;
    for (int r: resistors) {
        sum += r;
    }
    if (sum >= targetResistance*(1-MoE)) {
        vector<int> resistorsCopy = resistors;
        return findEquivalentResistanceCircuit(bestCandidate, targetResistance, maxResistors, MoE, &resistorsCopy, &circuitHashSet);
    }
    return bestCandidate;
}

int main(int argc, char** argv) {
    vector<int> resistorSet = {100, 100, 500};
    CircuitMatrix * cMx = findEquivalentResistanceCircuit(50, 3, 0.05, resistorSet);
    cout << cMx->toString();
}