#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include "circuit.h"

using namespace std;

extern unique_ptr<Circuit> circuit;
extern vector<unsigned int> resistorCollection;
extern char graphicsInitialized;

#endif