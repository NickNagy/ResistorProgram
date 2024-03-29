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

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <memory>
#include "helpers.h"

// TODO: keep track of legal moves separately from size, so we don't have to call resize() so frequently
class Circuit {
    private:
        typedef struct CircuitEdge {
            float localResistance = 0.0;
            float nonLocalResistance = 0.0;
            std::vector<unsigned int> resistors;
            float getTotalResistance();
            void computeLocalResistance();
        } CircuitEdge;
        size_t size = 2;
        std::vector<std::vector<std::shared_ptr<CircuitEdge>>> matrix;
        void resize(size_t newSize);
        void refresh(unsigned int n1, unsigned int n2);
    public:
        Circuit();
        ~Circuit();
        bool layResistor(unsigned int value, unsigned int n1, unsigned int n2);
        bool removeResistor(unsigned int value, unsigned int n1, unsigned int n2);
        unsigned int getSize();
        std::vector<unsigned int> getResistors(unsigned int n1, unsigned int n2);
        float getResistance(unsigned int n1, unsigned int n2);
        float getTotalResistance();
        int hashCode();
        bool equals(Circuit * other); // TODO
        std::unique_ptr<Circuit> copy();
        std::string toString();
};

#endif