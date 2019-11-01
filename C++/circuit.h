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
        vector<int> resistors;
        void computeLocalResistance();
    public: 
        float nonLocalResistance = 0.0;
        void insert(int r);
        int remove(int r);
        float getLocalResistance();
        float getTotalResistance();
        vector<int> getResistors();
};

// TODO: giving both classes a getResistors function feels redundant
class CircuitMatrix {
    private:
        unsigned int size = 2;
        vector<vector<CircuitEdge*>> matrix = vector<vector<CircuitEdge*>>(size, vector<CircuitEdge*>(size));
        void resize();
        void refresh(int n1, int n2);
    public:
        CircuitMatrix();
        int layResistor(int value, int n1, int n2);
        int removeResistor(int value, int n1, int n2);
        unsigned int getSize();
        vector<int> getResistors(int n1, int n2);
        float getResistance(int n1, int n2);
        float getTotalResistance();
        int hashCode();
        string toString();
};

#endif