#include "circuit.h"

void CircuitEdge::computeLocalResistance() {
    float sum = 0.0;
    for (int r: resistors) {
        sum+=(1.0/r);
    }
    localResistance = 1.0 / sum;
}

void CircuitEdge::insert(int r) {
    resistors.push_back(r);//push_back(r);
    computeLocalResistance();
}

// TODO: fix
int CircuitEdge::remove(int r) {
    vector<int>::iterator firstInstance = find(resistors.begin(), resistors.end(), r);
    if (firstInstance!=resistors.end()) {
        resistors.erase(firstInstance);
        computeLocalResistance();
        return 1;
    }
    return 0;
}

float CircuitEdge::getLocalResistance(){ return localResistance; }

float CircuitEdge::getTotalResistance(){
    if (nonLocalResistance == 0.0 && localResistance == 0.0) return 0.0; // simplify? gate-level?
    if (nonLocalResistance == 0.0) return localResistance;
    if (localResistance == 0.0) return nonLocalResistance;
    return nonLocalResistance*localResistance/(localResistance + nonLocalResistance); // careful of overflow    
}

vector<int> CircuitEdge::getResistors() {
    vector<int> resistorsCopy = resistors;
    return resistorsCopy;
}

CircuitMatrix::CircuitMatrix() {
    CircuitEdge * E = new CircuitEdge();
    CircuitEdge * nul = new CircuitEdge();
    matrix[0][0] = nul;
    matrix[0][1] = E;
    matrix[1][0] = E;
    matrix[1][1] = nul;
}

void CircuitMatrix::resize() {
    size++;
    matrix.resize(size);
    for (int i = 0; i < size; i++) {
        matrix[i].resize(size);
    }
    for (int j = 0; j < size-1; j++) {
        matrix[j][size-1] = new CircuitEdge();
        matrix[size-1][j] = matrix[j][size-1];
    }
    matrix[size-1][size-1] = matrix[0][0];    
}

void CircuitMatrix::refresh(int n1, int n2) {
    int i = n1-1;
    while (i >= 0) {
        matrix[i][n2]->nonLocalResistance += matrix[i+1][n2]->getTotalResistance() + matrix[i][n2-n1+i]->getTotalResistance();
        i--;
    }
}

int CircuitMatrix::layResistor(int value, int n1, int n2) {
    // TODO: can I simplify this at all?
    if (n1 < 0 || n2 < 0 || value <= 0 || n1 == n2 || n1 > size || n2 > size) {
        return 0;
    }
    if (n1 == size || n2 == size) {
        resize();
    }
    matrix[n1][n2]->insert(value);
    if (n1 > 0 && n2 > 0) {
        matrix[n1][n2]->nonLocalResistance = matrix[n1-1][n2]->getTotalResistance() + matrix[n1][n2-1]->getTotalResistance();
    }
    refresh(n1,n2);
    return 1;    
}

int CircuitMatrix::removeResistor(int value, int n1, int n2) {
    // TODO: can I simplify this at all?
    if (n1 < 0 || n2 < 0 || value <= 0 || n1 == n2 || n1 > size || n2 > size) {
        return 0;
    }
    int success = matrix[n1][n2]->remove(value);
    if (!success) return 0;
    refresh(n1,n2);
    return 1;
}

unsigned int CircuitMatrix::getSize() { return size; }

vector<int> CircuitMatrix::getResistors(int n1, int n2) {
    return matrix[n1][n2]->getResistors();
}

float CircuitMatrix::getResistance(int n1, int n2) { return matrix[n1][n2]->getTotalResistance(); }

float CircuitMatrix::getTotalResistance() { return getResistance(0, size-1); }

int CircuitMatrix::hashCode() {
    return size*getTotalResistance();
}

// TODO: optimize using ostringstream
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