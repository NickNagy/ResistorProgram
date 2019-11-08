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

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include "helpers.h"

using namespace std;

class CircuitEdge{
    private:
        float localResistance = 0.0;
        int numResistors = 0;
        vector<int> resistors;
        void computeLocalResistance();
    public: 
        CircuitEdge();
        ~CircuitEdge();
        float nonLocalResistance = 0.0;
        void insert(int r);
        int remove(int r);
        int getNumResistors();
        float getLocalResistance();
        float getTotalResistance();
        vector<int> getResistors();
};

// TODO: giving both classes a getResistors function feels redundant
class CircuitMatrix {
    private:
        unsigned int size = 2;
        vector<vector<CircuitEdge*>> matrix = vector<vector<CircuitEdge*>>(size, vector<CircuitEdge*>(size));
        void resize(unsigned int newSize);
        void refresh(int n1, int n2);
    public:
        CircuitMatrix();
        ~CircuitMatrix();
        int layResistor(int value, int n1, int n2);
        int removeResistor(int value, int n1, int n2);
        unsigned int getSize();
        vector<int> getResistors(int n1, int n2);
        float getResistance(int n1, int n2);
        float getTotalResistance();
        int hashCode();
        int equals(CircuitMatrix * other); // TODO
        string toString();
};

#endif