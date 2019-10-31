#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include "helpers.h"

using namespace std;

class CircuitEdge{
    private:
        float localResistance = 0.0;
        multiset<int> resistors;// = multiset<int>;
        void computeLocalResistance();
    public: 
        float nonLocalResistance = 0.0;
        void insert(int r);
        int remove(int r);
        float getLocalResistance();
        float getTotalResistance();
        multiset<int> getResistors();
};

// TODO: giving both classes a getResistors function feels redundant
// TODO: change x and y to n1 and n2
class CircuitMatrix {
    private:
        unsigned int size = 2;
        vector<vector<CircuitEdge*>> matrix = vector<vector<CircuitEdge*>>(size, vector<CircuitEdge*>(size));
        void resize();
        void refresh(int x, int y);
    public:
        CircuitMatrix();
        int layResistor(int value, int x, int y);
        int removeResistor(int value, int x, int y);
        unsigned int getSize();
        multiset<int> getResistors(int x, int y);
        float getResistance(int x, int y);
        float getTotalResistance();
        int hashCode();
        string toString();
};

#endif