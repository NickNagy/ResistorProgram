/* Nick Nagy

This file defines the methods of classes CircuitEdge and Circuit.

In the CircuitEdge class, resistive connections and nodes are analogous to the edge and its vertices, respectively.
The class is used to keep track of resistors and parallel-equivalent resistances between two nodes.

The Circuit class can be used to construct an entire resistive circuit with multiple nodes, and can be used to 
measure equivalent resistances across different parts of the circuit.

"Local resistance" here refers to the equivalent resistance of resistors in parallel connected DIRECTLY to the two
nodes that define the edge. "Non-local resistance" refers to the total equivalent resistance of connections between
the two nodes that are NOT directly connected to both. For example,

[n0]--{R1}--[n1]--{R2}--[n2]
|                         |
|-----------{R3}----------|

NonLocalResistance(n0, n2) = LocalResistance(n0, n1) + LocalResistance(n1, n2) = R1 + R2
LocalResistance(n0, n2) = R3
TotalResistance(n0, n2) = LocalResistance(n0, n2) || NonLocalResistance(n0, n2) = (R1 + R2)*R3 / (R1 + R2 + R3)

*/

#include "circuit.h"

// returns the total resistance = local resistance || non-localresistance
float Circuit::CircuitEdge::getTotalResistance(){
    if (nonLocalResistance == 0.0 && localResistance == 0.0) return 0.0; // simplify? gate-level?
    if (nonLocalResistance == 0.0) return localResistance;
    if (localResistance == 0.0) return nonLocalResistance;
    return nonLocalResistance*localResistance/(localResistance + nonLocalResistance); // careful of overflow    
}

// computes the local parallel equivalent resistance of all resistors connected to both vertices the edge spans
void Circuit::CircuitEdge::computeLocalResistance() {
    float sum = 0.0;
    for (unsigned int r: resistors) {
        sum+=(1.0/r);
    }
    localResistance = (sum == 0.0) ? 0.0 : 1.0/sum;
}

// constructor
// intuition behind matrix: edge(i,i) is meaningless, and other methods check for that;
//      edge(i,j) = edge(j,i)
Circuit::Circuit() {
    matrix = vector<vector<shared_ptr<CircuitEdge>>>(size, vector<shared_ptr<CircuitEdge>>(size));
    shared_ptr<CircuitEdge> E = make_shared<CircuitEdge>();
    shared_ptr<CircuitEdge> nullEdge = make_shared<CircuitEdge>(); // just called so b/c it's not to be used at all
    matrix[0][0] = nullEdge;
    matrix[0][1] = E;
    matrix[1][0] = E;
    matrix[1][1] = nullEdge;
}

// destructor
// frees matrix from memory
Circuit::~Circuit() {
//    vector<vector<shared_ptr<CircuitEdge>>>().swap(matrix);
}

// resizes matrix to (size+1)*(size+1), then updates size field
void Circuit::resize(size_t newSize) {
    matrix.resize(newSize);
    for (unsigned int i = 0; i < newSize; i++) {
        matrix[i].resize(newSize);
    }
    if (newSize > size) {
        for (unsigned int i = size; i < newSize; i++) {
            for (unsigned int j = 0; j <= i; j++) {
                matrix[j][i] = make_shared<CircuitEdge>();
                if (i!=j) matrix[i][j] = matrix[j][newSize-1];
            }
        }
    }
    size = newSize;
}

// updates all affected edges' non-local resistances after inserting/removing a new resistor into/from the circuit
void Circuit::refresh(unsigned n1, unsigned n2) {
    int i = n1-1;
    float nonLocalResistance = 0.0;
    while (i >= 0) {
        matrix[i][n2]->nonLocalResistance = matrix[i+1][n2]->getTotalResistance() + matrix[i][n2-n1+i]->getTotalResistance();
        i--;
    }
}

// adds a resistor with resistance==value to edge(n1,n2), then returns 0, IF:
//      - the two nodes n1 and n2 exist and that they are not the same node
//      - the resistance value is reasonable
// otherwise, returns 1
bool Circuit::layResistor(unsigned int value, unsigned int n1, unsigned int n2) {
    if (n1 == n2) return 1;
    if (n1 > n2) {
        int temp = n1;
        n1 = n2;
        n2 = temp;
    }
    if (value== 0 || n2 > size || (n2==size && n1 < size-1)) return 1;
    if (n2 == size) {
        resize(size + 1);
    }
    matrix[n1][n2]->resistors.push_back(value);
    matrix[n1][n2]->computeLocalResistance();
    if (n1 > 0) {
        matrix[n1][n2]->nonLocalResistance = matrix[n1-1][n2]->getTotalResistance() + matrix[n1][n2-1]->getTotalResistance();
    }
    refresh(n1,n2);
    return 0;    
}

// removes a resistor with resistance==value from edge(n1,n2), then returns 0, IF:
//      - the two nodes n1 and n2 exist and they are not the same node
//      - the resistor value is reasonable and exists in the edge
// otherwise, returns 1
bool Circuit::removeResistor(unsigned int value, unsigned int n1, unsigned int n2) {
    if (n1 == n2) return 1;
    if (n1 > n2) {
        unsigned int temp = n1;
        n1 = n2;
        n2 = temp;
    }
    if (value== 0 || n2 > size) return 1;
    // check to make sure that removing the resistor won't cause a hole in the circuit
    int holeCheck = matrix[n1][n2]->resistors.size() - 1;
    if (n2 < size - 1 && holeCheck<1) return 1;
    vector<unsigned int>::iterator firstInstance = find(matrix[n1][n2]->resistors.begin(), matrix[n1][n2]->resistors.end(), value);
    if (firstInstance!=matrix[n1][n2]->resistors.end()) {
        matrix[n1][n2]->resistors.erase(firstInstance);
        matrix[n1][n2]->computeLocalResistance();
        if (size > 2 && n2 == size-1 && !matrix[n1][n2]->resistors.size()) resize(size-1);
        refresh(n1,n2);
        return 0;
    }
    return 1;
}

// returns the size (number of edges, or number of nodes-1) of the circuit
unsigned int Circuit::getSize() { return size; }

// returns the set of resistors directly connected between nodes n1 and n2
vector<unsigned int> Circuit::getResistors(unsigned int n1, unsigned int n2) { return matrix[n1][n2]->resistors; }

// returns the total equivalent parallel resistance between nodes n1 and n2
float Circuit::getResistance(unsigned int n1, unsigned int n2) { return matrix[n1][n2]->getTotalResistance(); }

// retursn the total equivalent resistance of the Circuit from node 0 to node (size-1)
float Circuit::getTotalResistance() { return getResistance(0, size-1); }

// returns the matrix' hash-code
// PLEASE NOTE: NOT EVERY MATRIX HAS ITS OWN UNIQUE HASHCODE. This function is primarily designed to
// compare two matrixes with each other and check if they have a) the same number of nodes, and b) the
// same total 
int Circuit::hashCode() {
    return size*getTotalResistance();
}

// TODO:
char Circuit::equals(Circuit *other) {
    return 0;
}

// returns a copy of this Circuit object
// TODO: make a function that we can insert multiple resistors simultaneously rather than having to computeLocalResistance every time a single resistor is inserted
unique_ptr<Circuit> Circuit::copy() {
    unique_ptr<Circuit> thisCopy = make_unique<Circuit>(); 
    if (size != 2) thisCopy->resize(size);
    for (unsigned int i = 0; i < size-1; i++) {
        for (unsigned int j = i+1; j < size; j++) {
            if (i!=j) {
                for (unsigned int r: matrix[i][j]->resistors) {//getResistors(i,j)) {
                    thisCopy->layResistor(r, i, j);
                }
            }    
        }
    }
    return thisCopy;
}

// returns a string representation of the circuit as a 2d-matrix, that visualizes each edge's total equivalent
// resistance
string Circuit::toString() {
    string s = "\nMatrix:\n[[";
    for (unsigned int i = 0; i < size-1; i++) {
        for (unsigned int j = 0; j < size-1; j++) {
            if (i == j) {
                s += "0.00,";
            } else {
                s += floatToString(matrix[i][j]->getTotalResistance(), 2) + ",";
            }
        }
        s += floatToString(matrix[i][size-1]->getTotalResistance(),2) + "]\n [";
    }
    
    for (unsigned int k = 0; k < size-1; k++) {
        s += floatToString(matrix[size-1][k]->getTotalResistance(),2) + ",";
    }
    s += "0.00]]\n";
    s += "Total Resistance = " + floatToString(getTotalResistance(), 2) + "\n\n";
    return s;    
}