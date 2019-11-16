#include <iostream>
#include "circuit.h"

using namespace std;

int main() {
    Circuit c = Circuit();
    c.layResistor(100, 0, 1);
    c.layResistor(100, 1, 0);
    c.layResistor(75, 1, 2);
    cout << c.toString();
    c.removeResistor(75, 1, 2);
    cout << c.toString();
    unique_ptr<Circuit> c2 = c.copy();
    cout << c2->toString();
    return 0;
}