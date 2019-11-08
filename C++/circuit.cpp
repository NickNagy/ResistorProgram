/* Nick Nagy

This file defines the methods of classes CircuitEdge and CircuitMatrix.

In the CircuitEdge class, resistive connections and nodes are analogous to the edge and its vertices, respectively.
The class is used to keep track of resistors and parallel-equivalent resistances between two nodes.

The CircuitMatrix class can be used to construct an entire resistive circuit with multiple nodes, and can be used to 
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

// constructor
CircuitEdge::CircuitEdge() {}

// destructor
CircuitEdge::~CircuitEdge() {
    vector<int>().swap(resistors);
}

// computes the local parallel equivalent resistance of all resistors connected to both vertices the edge spans
void CircuitEdge::computeLocalResistance() {
    float sum = 0.0;
    for (int r: resistors) {
        sum+=(1.0/r);
    }
    localResistance = (sum == 0.0) ? 0.0 : 1.0/sum;
}

// adds a resistor with resistance r to the edge, and updates the edge's "local" resistance
void CircuitEdge::insert(int r) {
    resistors.push_back(r);//push_back(r);
    numResistors++;
    computeLocalResistance();
}

// removes a single instance of resistance r from the edge resistors, if found
int CircuitEdge::remove(int r) {
    vector<int>::iterator firstInstance = find(resistors.begin(), resistors.end(), r);
    if (firstInstance!=resistors.end()) {
        resistors.erase(firstInstance);
        numResistors--;
        computeLocalResistance();
        return 1;
    }
    return 0;
}

int CircuitEdge::getNumResistors() {
    return numResistors;
}

// returns the parallel equivalent resistance of the edge
float CircuitEdge::getLocalResistance(){ return localResistance; }

// returns the total resistance = local resistance || non-localresistance
float CircuitEdge::getTotalResistance(){
    if (nonLocalResistance == 0.0 && localResistance == 0.0) return 0.0; // simplify? gate-level?
    if (nonLocalResistance == 0.0) return localResistance;
    if (localResistance == 0.0) return nonLocalResistance;
    return nonLocalResistance*localResistance/(localResistance + nonLocalResistance); // careful of overflow    
}

// returns the set of resistors directly connected between both edges
vector<int> CircuitEdge::getResistors() {
    vector<int> resistorsCopy = resistors;
    return resistorsCopy;
}

// constructor
// intuition behind matrix: edge(i,i) is meaningless, and other methods check for that;
//      edge(i,j) = edge(j,i)
CircuitMatrix::CircuitMatrix() {
    CircuitEdge * E = new CircuitEdge();
    CircuitEdge * nul = new CircuitEdge();
    matrix[0][0] = nul;
    matrix[0][1] = E;
    matrix[1][0] = E;
    matrix[1][1] = nul;
}

// destructor
// frees matrix from memory
CircuitMatrix::~CircuitMatrix() {
    vector<vector<CircuitEdge*>>().swap(matrix);
}

// resizes matrix to (size+1)*(size+1), then updates size field
void CircuitMatrix::resize(unsigned int newSize) {
    matrix.resize(newSize);
    for (int i = 0; i < newSize; i++) {
        matrix[i].resize(newSize);
    }
    if (newSize > size) {
        for (int j = 0; j < newSize-1; j++) {
            matrix[j][newSize-1] = new CircuitEdge();
            matrix[newSize-1][j] = matrix[j][newSize-1];
        }
        matrix[newSize-1][newSize-1] = matrix[0][0];  
    }
    size = newSize;
}

// updates all affected edges' non-local resistances after inserting/removing a new resistor into/from the circuit
void CircuitMatrix::refresh(int n1, int n2) {
    int i = n1-1;
    while (i >= 0) {
        matrix[i][n2]->nonLocalResistance += matrix[i+1][n2]->getTotalResistance() + matrix[i][n2-n1+i]->getTotalResistance();
        i--;
    }
}

// adds a resistor with resistance==value to edge(n1,n2), then returns 1, IF:
//      - the two nodes n1 and n2 exist and that they are not the same node
//      - the resistance value is reasonable
// otherwise, returns 0
int CircuitMatrix::layResistor(int value, int n1, int n2) {
    if (n1 == n2) return 0;
    if (n1 > n2) {
        int temp = n1;
        n1 = n2;
        n2 = temp;
    }
    if (n1 < 0 || value <= 0 || n2 > size || (n2==size && n1 < size-1)) return 0;
    if (n2 == size) {
        resize(size + 1);
    }
    matrix[n1][n2]->insert(value);
    if (n1 > 0) {
        matrix[n1][n2]->nonLocalResistance = matrix[n1-1][n2]->getTotalResistance() + matrix[n1][n2-1]->getTotalResistance();
    }
    refresh(n1,n2);
    return 1;    
}

// removes a resistor with resistance==value from edge(n1,n2), then returns 1, IF:
//      - the two nodes n1 and n2 exist and they are not the same node
//      - the resistor value is reasonable and exists in the edge
// otherwise, returns 0
int CircuitMatrix::removeResistor(int value, int n1, int n2) {
    if (n1 == n2) return 0;
    if (n1 > n2) {
        int temp = n1;
        n1 = n2;
        n2 = temp;
    }
    if (n1 < 0 || value <= 0 || n2 > size) return 0;
    // check to make sure that removing the resistor won't cause a hole in the circuit
    int holeCheck = matrix[n1][n2]->getNumResistors() - 1;
    if (n2 < size - 1 && holeCheck<1) return 0;
    int success = matrix[n1][n2]->remove(value);
    if (!success) return 0;
    if (size > 2 && n2 == size-1 && !matrix[n1][n2]->getNumResistors()) resize(size-1);
    refresh(n1,n2);
    return 1;
}

// returns the size (number of edges, or number of nodes-1) of the circuit
unsigned int CircuitMatrix::getSize() { return size; }

// returns the set of resistors directly connected between nodes n1 and n2
vector<int> CircuitMatrix::getResistors(int n1, int n2) {
    return matrix[n1][n2]->getResistors();
}

// returns the total equivalent parallel resistance between nodes n1 and n2
float CircuitMatrix::getResistance(int n1, int n2) { return matrix[n1][n2]->getTotalResistance(); }

// retursn the total equivalent resistance of the CircuitMatrix from node 0 to node (size-1)
float CircuitMatrix::getTotalResistance() { return getResistance(0, size-1); }

// returns the matrix' hash-code
// PLEASE NOTE: NOT EVERY MATRIX HAS ITS OWN UNIQUE HASHCODE. This function is primarily designed to
// compare two matrixes with each other and check if they have a) the same number of nodes, and b) the
// same total 
int CircuitMatrix::hashCode() {
    return size*getTotalResistance();
}

// TODO:
int CircuitMatrix::equals(CircuitMatrix *other) {
    return 0;
}

// TODO: optimize using ostringstream
// returns a string representation of the circuit as a 2d-matrix, that visualizes each edge's total equivalent
// resistance
string CircuitMatrix::toString() {
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