#include <iostream>
#include <queue>
#include <set>
#include <unordered_set>
#include <math.h>
#include "helpers.h"
#include "globals.h"
#include "circuit.h"
#include "circuitgraphics.h"

using namespace std;

typedef struct CMTracker{
    CircuitMatrix * cMx;
    vector<unsigned int> seenResistors;

} CMTracker;

void explain(CircuitMatrix * m){
    cout << "Found a circuit with total resistance: " << m->getTotalResistance() << endl;
    cout << "Explanation:\n";
    int size = m->getSize();
    for (int i = 0; i < size-1; i++) {
        for (int j = i; j < size; j++) {
            if (i!=j && m->getResistance(i,j)>0) {
                // TODO: if only one resistor, should not include the "in parallel"... if no resistors, shouldn't say anything
                cout << "From node " << i << " to node " << j << ": " << intVectorToString(m->getResistors(i,j)) << " in parallel.\n";
            }
        }
    } 
}

// TODO: create hash function for CircuitMatrix
// TODO: should I pass a pointer to a CircuitMatrix in this function??
CircuitMatrix * findEquivalentResistanceCircuit (int targetResistance, int maxResistors, float MoE, vector<unsigned int> resistors) {
    cout << "Searching for a circuit with equivalent resistance in the range of " << targetResistance*(1-MoE) << " and " << targetResistance*(1+MoE) << " using the following resistors:\n";
    cout << "{" << intVectorToString(resistors) << "}\n";
    CircuitMatrix * bestCandidate = new CircuitMatrix();// = CircuitMatrix();
    float targetDifference = MoE*targetResistance;
    unsigned int sum = 0;
    // should return a NULL CircuitMatrix object if sum(resistors) < target 
    for (int r: resistors) {
        sum += r;
    }
    if (sum >= targetResistance*(1-MoE)) {
        unordered_set<int> circuitHashSet;
        CircuitMatrix * current;
        CMTracker * tracker;
        queue<CMTracker*> circuitQueue; 
        unsigned int minDifference = 0xFFFF;
        unsigned int difference;
        unsigned int r;
        unsigned int i;
        // initialize BFS queue with a CircuitMatrix* for each resistor in the set -- ignore repeated resistor values
        for (i = 0; i < resistors.size(); i++) {
            r = resistors.at(i);
            current = new CircuitMatrix();
            current->layResistor(r, 0, 1);
            difference = abs(current->getTotalResistance() - targetResistance);
            if (difference <= targetDifference) {
                explain(current);
                return current;
            }
            if (difference < minDifference) {
                minDifference = difference;
                bestCandidate = current;
            }
            int currentHashCode = current->hashCode();
            if (circuitHashSet.find(currentHashCode) == circuitHashSet.end()){ // segfault happens in this check
                //cout << "Pushing to queue:\n" << current->toString() << " with address=" << current << endl;
                tracker = new CMTracker;
                vector<unsigned int> v{r};
                *tracker = {current, v};
                circuitQueue.push(tracker);
                circuitHashSet.insert(currentHashCode);
            }
        }
        // continue BFS by adding new resistors to each pulled CM* from the queue, and re-queueing, until a circuit with ~target equiv
        // resistance is found
        unsigned int j;
        vector <unsigned int> resistorsLeft;
        while (!circuitQueue.empty()) {
            tracker = circuitQueue.front();
            current = tracker->cMx;
            cout << "**********\nCurrent Matrix: \n" << current->toString() << endl;
            circuitQueue.pop();
            // need to skip over resistors that are already in the current circuit!
            resistorsLeft = getRemainingVector(resistors, tracker->seenResistors);//current->getResistors());
            for (unsigned int r : resistorsLeft) { // <- this is gonna fail!!!
            /* also, there is probably a way to check in the first loop that something has
            already been tried before inserting into matrix, like if (r has equaled resistors->get(i)) */
                cout << "Looking at resistor: " << r << endl;
                int size = current->getSize();
                // TODO: we'd want to check series connections before parallel
                for (i = 0; i < size; i++) {
                    for (j = i; j < size+1; j++) {
                        cout << "Trying to set " << r << " between nodes " << i << " and " << j << endl;
                        // have to check this condition first b/c we need to know when to re-remove the resistor again
                        CircuitMatrix * currentCopy = new CircuitMatrix();
                        *currentCopy = *current;
                        if (currentCopy -> layResistor(r,i,j)) {
                            if (size < maxResistors && i!=j && circuitHashSet.find(currentCopy->hashCode())==circuitHashSet.end()) {
                                cout << "success!\nNow current = \n" << currentCopy->toString();
                                difference = abs(currentCopy->getTotalResistance() - targetResistance);
                                //cout << "difference=" << difference << "; aiming to be < " << targetDifference << endl;
                                if (difference <= targetDifference) {
                                    explain(currentCopy);
                                    return currentCopy;
                                }
                                if (difference < minDifference) {
                                    minDifference = difference;
                                    bestCandidate = currentCopy;
                                }
                                //cout << "current: " << current->toString();
                                cout << "Best current: " << bestCandidate->toString();
                                cout << "Pushing to queue:\n" << currentCopy->toString();
                                CMTracker * trackerCopy = new CMTracker;
                                vector<unsigned int> seenResistorsCopy = tracker->seenResistors;
                                seenResistorsCopy.push_back(r);
                                *trackerCopy = {currentCopy, seenResistorsCopy};
                                circuitQueue.push(trackerCopy);
                                circuitHashSet.insert(currentCopy->hashCode());
                            }
                            cout << "Lastly, we re-remove the just newly inserted resistor before checking the next one...\n";
                            current -> removeResistor(r,i,j);
                            cout << "Once more, Current=" << current->toString();
                            cout << "Enter q to quit\n";
                            string response;
                            cin >> response;
                            if (!response.compare("q")) return bestCandidate;
                        }
                    }
                }
            }
        }
        explain(bestCandidate);
    } else {
        cout << "No resistors from the given set that can achieve this resistance.\n";
    }
    return bestCandidate;
}

int main(int argc, char** argv) {
    vector<unsigned int> resistorSet = {100, 100, 500};
    cMx = findEquivalentResistanceCircuit(700, 3, 0.05, resistorSet);
}