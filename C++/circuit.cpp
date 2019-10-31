#include "circuit.h"

void CircuitEdge::computeLocalResistance() {
    float sum = 0.0;
    for (int r: resistors) {
        sum+=(1.0/r);
    }
    localResistance = 1.0 / sum;
}

void CircuitEdge::insert(int r) {
    resistors.insert(r);//push_back(r);
    computeLocalResistance();
}

// TODO: fix
int CircuitEdge::remove(int r) {
    auto itr = resistors.find(r);
    if (itr!=resistors.end()) {
        resistors.erase(itr);
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

multiset<int> CircuitEdge::getResistors() {
    multiset<int> resistorsCopy = resistors;
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

void CircuitMatrix::refresh(int x, int y) {
    int i = x-1;
    while (i >= 0) {
        matrix[i][y]->nonLocalResistance += matrix[i+1][y]->getTotalResistance() + matrix[i][y-x+i]->getTotalResistance();
        i--;
    }
}

int CircuitMatrix::layResistor(int value, int x, int y) {
    // TODO: can I simplify this at all?
    if (x < 0 || y < 0 || value <= 0 || x == y || x > size || y > size) {
        return 0;
    }
    if (x == size || y == size) {
        resize();
    }
    matrix[x][y]->insert(value);
    if (x > 0 && y > 0) {
        matrix[x][y]->nonLocalResistance = matrix[x-1][y]->getTotalResistance() + matrix[x][y-1]->getTotalResistance();
    }
    refresh(x,y);
    return 1;    
}

int CircuitMatrix::removeResistor(int value, int x, int y) {
    // TODO: can I simplify this at all?
    if (x < 0 || y < 0 || value <= 0 || x == y || x > size || y > size) {
        return 0;
    }
    int success = matrix[x][y]->remove(value);
    if (!success) return 0;
    refresh(x,y);
    return 1;
}

unsigned int CircuitMatrix::getSize() { return size; }

multiset<int> CircuitMatrix::getResistors(int x, int y) {
    return matrix[x][y]->getResistors();
}

float CircuitMatrix::getResistance(int x, int y) { return matrix[x][y]->getTotalResistance(); }

float CircuitMatrix::getTotalResistance() { return getResistance(0, size-1); }

int CircuitMatrix::hashCode() {
    return 0;
}

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