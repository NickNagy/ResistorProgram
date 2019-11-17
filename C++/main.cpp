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
    if (DEBUG) cout << m->toString();
    cout << "Explanation:\n";
    int size = m->getSize();
    for (int i = 0; i < size-1; i++) {
        for (int j = i; j < size; j++) {
            if (i!=j && m->getResistance(i,j)>0) {
                // TODO: if only one resistor, should not include the "in parallel"... if no resistors, shouldn't say anything
                vector<unsigned int> resistors = m->getResistors(i,j);
                cout << "From node " << i << " to node " << j << ": ";
                if (resistors.size() > 1) {
                    cout << collectionToString<vector<unsigned int>>(resistors) << " ohm resistors in parallel.\n";
                } else if (resistors.size()==1) {
                    cout << resistors.at(0) << " ohm resistor.\n"; 
                }
            }
        }
    } 
}

// TODO: add back in a bestCandidate ptr, in case nothing found before queue runs out
// TODO: @ start of function, resize all circuits to maxResistors*maxResistors, so we don't have to keep calling resize()
// TODO: change some of the data types so floats don't get rounded to ints during operations
unique_ptr<Circuit> findEquivalentResistanceCircuit (int targetResistance, int maxResistors, float MoE, vector<unsigned int> resistors) {
    unique_ptr<Circuit> current; //= make_unique<Circuit>();
    if (!resistors.size()) return current;
    typedef struct CircuitTracker{
        unsigned int differenceFromTarget = 0xFFFF;
        vector<unsigned int> resistorsSeen;
        shared_ptr<Circuit> circuit = make_shared<Circuit>(); // changed to shared_ptr for sake of priority_queue.top()
    } CircuitTracker;
    typedef struct CompareDifference{
        bool operator()(CircuitTracker const & t1, CircuitTracker const & t2) { return t1.differenceFromTarget > t2.differenceFromTarget; }
    } CompareDifference;
    cout << "Searching for a circuit with equivalent resistance in the range of " << targetResistance*(1-MoE) << " and " << targetResistance*(1+MoE) << " using the following resistors:\n";
    cout << collectionToString(resistors) << endl;
    float targetDifference = MoE*targetResistance;
    unsigned int sum = 0;
    for (unsigned int r: resistors) {
        sum += r;
    }
    if (sum >= targetResistance*(1-MoE)) {
        vector<unsigned int> resistorsSeen;
        unordered_set<float> circuitHashSet;
        CircuitTracker tracker;
        priority_queue<CircuitTracker, vector<CircuitTracker>, CompareDifference> circuitQueue;
        unsigned int difference, r;
        // initialize BFS queue with a Circuit* for each resistor in the set -- ignore repeated resistor values
        for (unsigned int r: resistors) {
            if (circuitHashSet.find(r)==circuitHashSet.end()) {
                current = make_unique<Circuit>();
                current -> layResistor(r, 0, 1);
                difference = r>targetResistance ? r-targetResistance:targetResistance-r;
                if (difference <= targetDifference) {
                    return current;
                }
                resistorsSeen = vector<unsigned int>(1,r);
                circuitQueue.push(CircuitTracker{difference, resistorsSeen, move(current)});
                circuitHashSet.insert(r);
            }
        }
        // continue BFS by adding new resistors to each pulled CM* from the queue, and re-queueing, until a circuit with ~target equiv resistance is found
        vector <unsigned int> resistorsLeft;
        shared_ptr<Circuit> sharedCurrent = make_shared<Circuit>();
        if (DEBUG) cout << "***BEGIN***\n";
        while (!circuitQueue.empty()) {
            resistorsSeen = circuitQueue.top().resistorsSeen;
            sharedCurrent = circuitQueue.top().circuit; // have to use shared_ptr for pQ.top() to work w/o deletion
            circuitQueue.pop();
            resistorsLeft = getRemainingVector(resistors, resistorsSeen);
            for (unsigned int r : resistorsLeft) {
                int size = sharedCurrent->getSize();
                // loops are reversed to check adding resistor to the end before trying any parallel connections
                for (int i = size-1; i >= 0; i--) {
                    for (int j = size; j > i; j--) {
                        unique_ptr<Circuit> currentCopy = sharedCurrent->copy();
                        if (!currentCopy -> layResistor(r,i,j) && resistorsSeen.size() < maxResistors && circuitHashSet.find(currentCopy->getTotalResistance())==circuitHashSet.end()) {
                            float totalResistance = currentCopy->getTotalResistance();
                            difference = abs(totalResistance - targetResistance);
                            if (difference <= targetDifference) {
                                return currentCopy;
                            }
                            resistorsSeen.push_back(r);
                            circuitQueue.push(CircuitTracker{difference, resistorsSeen, move(currentCopy)});
                        }
                    }
                }
            }
        }
        cout << "Did not find a circuit with an equivalent resistance in the desired range.\n";
    } else {
        cout << "No resistors from the given set that can achieve this resistance.\n";
    }
    return current;
}

void addResistorsToSet() {
    unsigned int resistance;
    string greeting = "Enter a resistance value to add (1 ohm to 1000000 ohms), or 0 to return to main menu: ";
    cout << greeting;
    cin >> resistance;
    while (resistance > 0) {
        if (resistance > 1000000) {
            cout << "Sorry, that is not a valid resistance.\n";
        } else {
            resistorCollection.push_back(resistance);
            cout << "Added!\nYour set: " << collectionToString<vector<unsigned int>>(resistorCollection) << endl; 
        }
        cout << greeting;
        cin >> resistance;
    } 
}

void checkResistances() {
    vector <string> colors(3);
    char response;
    unsigned int resistance;
    string greeting = "If you're submitting a resistance value to look up its band colors, type 'r' again.\n" 
                    "If you're entering three colors to look up the resistance, type 'c'.\n"
                    "Type anything else to quit: ";
    cout << greeting;
    cin >> response;
    while (1) {
        switch(response) {
            case 'r': {
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
            case 'c': {
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
            default: return;
        }
        cout << greeting;
        cin >> response;
    }
}

void removeResistorsFromSet() {
    unsigned int resistance;
    string greeting = "Enter a resistance value to remove (1 ohm to 1000000 ohms), or 0 to return to the main menu: ";
    cout << greeting;
    cin >> resistance;
    while (resistance > 0) {
        if (resistance > 1000000) {
            cout << "Sorry, that is not a valid resistance.\n";
        } else {
            vector<unsigned int>::iterator it = find(resistorCollection.begin(), resistorCollection.end(), resistance);
            if (it != resistorCollection.end()) {
                resistorCollection.erase(it);
                cout << "Removed.\nYour set: " << collectionToString<vector<unsigned int>>(resistorCollection) << endl;
            } else {
                cout << "We don't see that resistor in your collection.\n";
            }           
        }
        cout << greeting;
        cin >> resistance;       
    }
}

void userSelection() {
    char response = 0;
    unsigned int resistance = 0;
    while (1) {
        cout << "\na - add a resistor to your collection\nb - build a circuit from your resistor collection\nc - check a resistor's band colors\n"
                "r - remove a resistor from your collection\ns - search for a circuit with a specific net resistance\nany other key - quit"
                "\n\nWhat would you like to do? ";
        cin >> response;
        response = (response < 97) ? response+32 : response; // add to rest of functions?
        switch(response) {
            case 'a': addResistorsToSet();
                break;
            case 'b':
                cout << "This feature is coming soon!\n";
                break;
            case 'c': checkResistances();
                break;
            case 'r': removeResistorsFromSet();
                break;
            case 's': {
                    int maxResistors = resistorCollection.size();
                    float marginOfError = 0.05;
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
                    unique_ptr<Circuit> found = findEquivalentResistanceCircuit(resistance, maxResistors, marginOfError, resistorCollection);
                    explain(found.get());
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
    cout << "Your set: " << collectionToString<vector<unsigned int>>(resistorCollection);
    userSelection();
    saveResistorSetToFile(filename, resistorCollection);
    return 0;
}