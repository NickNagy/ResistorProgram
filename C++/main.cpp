#include <iostream>
#include <queue>
#include "helpers.h"
#include "globals.h"
#include "circuit.h"
#include "circuitgraphics.h"

using namespace std;

// TODO: create hash function for CircuitMatrix
CircuitMatrix * findEquivalentResistanceCircuit (int targetResistance, float MoE, multiset<int> resistors) {
    set<int> circuitHashSet;
    queue<CircuitMatrix*> circuitQueue;
    /* pseudocode for non-recursive approach:
        resistorSetCopy = resistors
        for resistor in resistorSetCopy:
            if resistor == target return resistor
            else
                cM = new CircuitMatrix()
                cM.insert(resistor, 0, 1)
                if hashSet !contain cM:
                    circuitQ.put(cM)
        while !found:
            cM = circuitQ.get()
            for resistor in resistorSetCopy:
                for all possible placements of resistor:
                    cM.insert(resistor, <placement>)
                    if cM.totalResistance == target: return cM
                    else if hashSet !contain cM:
                        circuitQ.put(cM)
    */

    /* pseudocode for recursive approach:
        findEquivalentResistance(int target, CircuitMatrix * cMx, set * circuiHashSet, set * resistors) {
            if cM.totalResistance == target return cM
            for resistor in resistorSetCopy:
                resistors.remove(resistor)
                for all possible placements of resistor:
                    cMx->insert(resistor, placement)
                    if circuitHashSet !contain cMx:
                        return findEquivalentResistance(target, cMx, circuitHashset)
                resistors.insert(resistor) <- ??
        } 
    */
}

int main(int argc, char** argv) {
    cMx = new CircuitMatrix();
    initGraphics(argc, argv);
    cMx->layResistor(1, 0, 1); 
    cMx->layResistor(1, 1, 2);
    cMx->layResistor(1, 2, 3);
    cout << cMx->toString();
    displayCircuit();
}