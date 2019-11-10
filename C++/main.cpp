#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <unordered_set>
#include <math.h>
#include "helpers.h"
#include "globals.h"
#include "circuit.h"
#include "circuitgraphics.h"

#define DEBUG 0

using namespace std;

// TODO: check for bad lines in file
vector<unsigned int> loadResistorSetFromFile(string filename) {
    vector<unsigned int> resistors;
    ifstream file(filename, ios::out);
    if(!file.is_open()) {
        cout << "Error accessing file.\n";
    } else {
        string resistor;
        while(getline(file, resistor)) {
            resistors.push_back((unsigned int)stoi(resistor));
        }
        file.close();
    }
    return resistors;
}

void saveResistorSetToFile(string filename, vector<unsigned int> resistors) {
    ofstream file(filename, ios::in);
    if(!file.is_open()) {
        cout << "Error accessing file.\n";
    } else {
        for(unsigned int resistor: resistors) {
            file << to_string(resistor) << endl;
        }
        file.close();
        cout << "Resistors saved.\n";
    }
}

void explain(Circuit * m){
    cout << "A circuit with total resistance: " << m->getTotalResistance() << " ohms.\n";
    cout << "Explanation:\n";
    int size = m->getSize();
    for (int i = 0; i < size-1; i++) {
        for (int j = i; j < size; j++) {
            if (i!=j && m->getResistance(i,j)>0) {
                // TODO: if only one resistor, should not include the "in parallel"... if no resistors, shouldn't say anything
                vector<unsigned int> resistors = m->getResistors(i,j);
                cout << "From node " << i << " to node " << j << ": ";
                if (resistors.size() > 1) {
                    cout << intVectorToString(resistors) << " ohm resistors in parallel.\n";
                } else {
                    cout << resistors.at(0) << " ohm resistor.\n"; 
                }
            }
        }
    } 
}

Circuit * findEquivalentResistanceCircuit (int targetResistance, int maxResistors, float MoE, vector<unsigned int> resistors) {
    // I do not like this struct, and I do NOT think it should exist anywhere outside of this function, but it is better than many alternatives
    typedef struct CMTracker{
        Circuit * cMx;
        vector<unsigned int> seenResistors;
    } CMTracker;
    cout << "Searching for a circuit with equivalent resistance in the range of " << targetResistance*(1-MoE) << " and " << targetResistance*(1+MoE) << " using the following resistors:\n";
    cout << "{" << intVectorToString(resistors) << "}\n";
    Circuit * bestCandidate = new Circuit();
    float targetDifference = MoE*targetResistance;
    unsigned int sum = 0;
    // should return a NULL Circuit object if sum(resistors) < target 
    for (int r: resistors) {
        sum += r;
    }
    if (sum >= targetResistance*(1-MoE)) {
        unordered_set<int> circuitHashSet;
        Circuit * current;
        CMTracker * tracker;
        queue<CMTracker*> circuitQueue; 
        unsigned int minDifference = 0xFFFF;
        unsigned int difference;
        unsigned int r;
        unsigned int i;
        // initialize BFS queue with a Circuit* for each resistor in the set -- ignore repeated resistor values
        for (i = 0; i < resistors.size(); i++) {
            r = resistors.at(i);
            current = new Circuit();
            current->layResistor(r, 0, 1);
            difference = abs(current->getTotalResistance() - targetResistance);
            if (difference <= targetDifference) {
                explain(current);
                return current;
            }
            if (difference < minDifference) {
                minDifference = difference;
                bestCandidate = current->copy();
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
        vector <unsigned int> resistorsSeen;
        while (!circuitQueue.empty()) {
            tracker = circuitQueue.front();
            current = tracker->cMx;
            if (DEBUG) cout << "**********\nQueue Size: " << circuitQueue.size() << "\nCurrent Matrix: \n" << current->toString() << endl;
            circuitQueue.pop();
            // need to skip over resistors that are already in the current circuit!
            resistorsSeen = tracker->seenResistors;
            resistorsLeft = getRemainingVector(resistors, resistorsSeen);//current->getResistors());
            if (DEBUG) {
                cout << "Resistors seen: " << intVectorToString(resistorsSeen) << endl;
                cout << "Resistors left: " << intVectorToString(resistorsLeft) << endl;
            }
            for (unsigned int r : resistorsLeft) {
                if (DEBUG) cout << "Looking at resistor: " << r << endl;
                int size = current->getSize();
                // TODO: we'd want to check series connections before parallel
                for (i = 0; i < size; i++) {
                    for (j = i; j < size+1; j++) {
                        // have to check this condition first b/c we need to know when to re-remove the resistor again
                        Circuit * currentCopy = current->copy();
                        if (i!=j && currentCopy -> layResistor(r,i,j)) {
                            if (DEBUG) cout << "Trying to set " << r << " between nodes " << i << " and " << j << endl;
                            if (resistorsSeen.size() < maxResistors && circuitHashSet.find(currentCopy->hashCode())==circuitHashSet.end()) {
                                if (DEBUG) cout << "success!\nNow current = \n" << currentCopy->toString();
                                difference = abs(currentCopy->getTotalResistance() - targetResistance);
                                if (difference <= targetDifference) {
                                    explain(currentCopy);
                                    return currentCopy;
                                }
                                if (difference < minDifference) {
                                    minDifference = difference;
                                    bestCandidate = currentCopy->copy();
                                }
                                if (DEBUG) {
                                    cout << "Best current: " << bestCandidate->toString();
                                    cout << "Pushing to queue:\n" << currentCopy->toString();
                                }
                                CMTracker * trackerCopy = new CMTracker;
                                vector<unsigned int> seenResistorsCopy = resistorsSeen;
                                seenResistorsCopy.push_back(r);
                                *trackerCopy = {currentCopy, seenResistorsCopy};
                                circuitQueue.push(trackerCopy);
                                circuitHashSet.insert(currentCopy->hashCode());
                            }
                            /* if (DEBUG) {
                                cout << "Enter q to quit\n";
                                string response;
                                cin >> response;
                                if (!response.compare("q")) return bestCandidate;
                            }*/
                        }
                    }
                }
            }
        }
        cout << "Did not find a circuit with an equivalent resistance in the desired range.\n";
        explain(bestCandidate);
    } else {
        cout << "No resistors from the given set that can achieve this resistance.\n";
    }
    return bestCandidate;
}

void userSelection() {
    char response = 0;
    unsigned int resistance = 0;
    while (1) {
        cout << "\na - add a resistor to your collection\nb - build a circuit from your resistor collection\nc - check a resistor's band colors\n"
                "r - remove a resistor from your collection\ns - search for a circuit with a specific net resistance\nany other key - quit"
                "\n\nWhat would you like to do? ";
        cin >> response;
        response = (response < 97) ? response+32 : response;
        switch(response) {
            case 'a': {
                    cout << "Enter a resistance value to add (1 ohm to 1000000 ohms): ";
                    cin >> resistance;
                    if (resistance > 1000000) {
                        cout << "Sorry, that is not in the accepted range.\n";
                    } else {
                        resistorCollection.push_back(resistance);
                        cout << "Added!\n";
                    }
                }
                break;
            case 'b':
                cout << "This feature is coming soon!\n";
                break;
            case 'c': {
                    string * colors = new string[3];
                    cout << "If you're submitting a resistance value to look up its band colors, type 'c' again. If you're entering three colors to look up the resistance, type any key. ";
                    cin >> response;
                    switch(response) {
                        case 'c': {
                                cout << "Enter a resistance value to check (1 ohm to 1000000 ohms): ";
                                cin >> resistance;
                                if (resistance > 1000000) {
                                    cout << "Sorry, that is not in the accpeted range.\n";
                                } else {
                                    colors = getColors(resistance);
                                    cout << colors[0] << ", " << colors[1] << ", " << colors[2] << endl;
                                }
                            }
                            break;
                        default: {
                                cout << "Enter three colors from the following options: Black, Brown, Red, Orange, Yellow, Green, Blue, Violet, Grey, White\nFirst color: ";
                                cin >> colors[0];
                                cout << "Second color: ";
                                cin >> colors[1];
                                cout << "Third color: ";
                                cin >> colors[2];
                                resistance = getResistance(colors);
                                if (!resistance) {
                                    cout << "There was an error with your submissions.\n";
                                } else {
                                    cout << "Resistor {" << colors[0] << ", " << colors[1] << ", " << colors[2] << "} ~= " << resistance << " ohms.\n";
                                }
                            }
                            break;
                    }
                    delete [] colors;
                }
                break;
            case 'r': {
                    cout << "Enter a resistance value to remove (1 ohm to 1000000 ohms): ";
                    cin >> resistance;
                    if (resistance > 1000000) {
                        cout << "Sorry, that is not in the accepted range.\n";
                    } else {
                        vector<unsigned int>::iterator it = find(resistorCollection.begin(), resistorCollection.end(), resistance);
                        if (it != resistorCollection.end()) resistorCollection.erase(it);
                        cout << "Removed.\n";
                    }
                }
                break;
            case 's': {
                    int maxResistors = resistorCollection.size();
                    int marginOfError = 0.05;
                    cout << "Enter a net resistance you are hoping to achieve from your resistor collection: ";
                    cin >> resistance;
                    cout << "Is there a limit to how many resistors you want to use? If so, enter 'y', otherwise enter any key. ";
                    cin >> response;
                    if (response == 'y') {
                        cout << "What's the max number of resistors you want to use? ";
                        cin >> maxResistors;
                        maxResistors = (maxResistors > resistorCollection.size()) ? resistorCollection.size() : maxResistors;
                    }
                    cout << "What's your margin of error for the circuit found? Default is 0.05 (Example: for a target of 100 ohms we'd search for a circuit between 95 and 105 ohms. ";
                    cin >> marginOfError;
                    Circuit * found = findEquivalentResistanceCircuit(resistance, maxResistors, marginOfError, resistorCollection);
                }
                break;
            default: 
                cout << "Terminating...\n";
                return;
        }
    }
}

int main(int argc, char** argv) {
    char response;
    string filename = "resistors.txt";
    if (argc > 1) {
        filename = argv[1];
        resistorCollection = loadResistorSetFromFile(filename);
    } else {
        cout << "Load resistor collection from text file? [y/n] ";
        cin >> response;
        if (response == 'Y' || response == 'y') {
            cout << "\nText file to load from: ";
            cin >> filename;
            resistorCollection = loadResistorSetFromFile(filename);
        }
    }
    userSelection();
    saveResistorSetToFile(filename, resistorCollection);
    return 0;
}