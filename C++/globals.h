#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include "circuit.h"

extern std::unique_ptr<Circuit> circuit;
extern std::vector<unsigned int> resistorCollection;
extern bool graphicsInitialized;

#endif