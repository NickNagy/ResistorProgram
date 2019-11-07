#include <iostream>
#include <queue>
#include <set>
#include <math.h>
#include "helpers.h"
#include "globals.h"
#include "circuit.h"
#include "circuitgraphics.h"

using namespace std;

typedef struct CMTracker{
    multiset<int> seenResistors; // multiset or int?
    CircuitMatrix * cMx;
} CMTracker;

// TODO: create hash function for CircuitMatrix
// TODO: should I pass a pointer to a CircuitMatrix in this function??
CircuitMatrix findEquivalentResistanceCircuit (int targetResistance, int maxResistors, float MoE, vector<int> resistors) {
    cout << "Searching for a circuit with equivalent resistance in the range of " << targetResistance*(1-MoE) << " and " << targetResistance*(1+MoE) << " using the following resistors:\n";
    cout << "{" << intVectorToString(resistors) << "}\n";
    CircuitMatrix bestCandidate;// = CircuitMatrix();
    unsigned int sum = 0;
    // should return a NULL CircuitMatrix object if sum(resistors) < target 
    for (int r: resistors) {
        sum += r;
    }
    if (sum >= targetResistance*(1-MoE)) {
        set<int> circuitHashSet;
        CircuitMatrix current;
        queue<CircuitMatrix*> circuitQueue; 
        unsigned int minDifference = 0xFFFF;
        unsigned int difference;
        // initialize BFS queue with a CircuitMatrix* for each resistor in the set -- ignore repeated resistor values
        for (int i = 0; i < resistors.size(); i++) {
            int r = resistors.at(i);
            current.layResistor(r, 0, 1);
            difference = abs(current.getTotalResistance() - targetResistance);
            if (difference <= MoE*targetResistance) {
                return current;
            }
            if (difference < minDifference) {
                minDifference = difference;
                bestCandidate = current;
            }
            int currentHashCode = current.hashCode();
            if (!circuitHashSet.empty() && circuitHashSet.find(currentHashCode) == circuitHashSet.end()){ // segfault happens in this check
                cout << "it's not a condition issue..\n";
                circuitQueue.push(&current);
                circuitHashSet.insert(currentHashCode);
            }
        }
        // continue BFS by adding new resistors to each pulled CM* from the queue, and re-queueing, until a circuit with ~target equiv
        // resistance is found
        while (!circuitQueue.empty()) {
            current = *circuitQueue.front();
            circuitQueue.pop();
            for (int i = 0; i < resistors.size(); i++) { // <- this is gonna fail!!!
            /* also, there is probably a way to check in the first loop that something has
            already been tried before inserting into matrix, like if (r has equaled resistors->get(i)) */
                int r = resistors.at(i);
                int size = current.getSize();
                // TODO: we'd want to check series connections before parallel
                for (int i = 0; i < size-1; i++) {
                    for (int j = 0; j < size; j++) {
                        if (size < maxResistors && i!=j && current.layResistor(r,i,j) && circuitHashSet.find(current.hashCode())==circuitHashSet.end()) {
                            difference = abs(current.getTotalResistance() - targetResistance);
                            if (difference <= MoE*targetResistance) {
                                return current;
                            }
                            if (difference < minDifference) {
                                minDifference = difference;
                                bestCandidate = current;
                            }
                            cout << "current: " << current.toString();
                            cout << "Best current: " << bestCandidate.toString();
                            circuitQueue.push(&current);
                            circuitHashSet.insert(current.hashCode());
                            cout << "Enter q to quit\n";
                            string response;
                            cin >> response;
                            if (!response.compare("q")) return bestCandidate;
                        }
                    }
                }
            }
        }
        cout << "Found a circuit with total resistance = " << bestCandidate.getTotalResistance() << endl;
        cout << "Explanation: \n";
        int size = bestCandidate.getSize();
        for (int i = 0; i < size-1; i++) {
            for (int j = i; j < size; j++) {
                if (i!=j && bestCandidate.getResistance(i,j)>0) {
                    cout << "From node " << i << " to node " << j << ": " << intVectorToString(bestCandidate.getResistors(i,j)) << " in parallel.\n";
                }
            }
        } 
    } else {
        cout << "No resistors from the given set that can achieve this resistance.\n";
    }
    return bestCandidate;
}

CircuitMatrix testFunction() {
    CircuitMatrix test;
    test.layResistor(1, 0, 1);
    return test;
}

int main(int argc, char** argv) {
    vector<int> resistorSet = {100, 100, 500};
    *cMx = testFunction();//findEquivalentResistanceCircuit(50, 3, 0.05, resistorSet);
}