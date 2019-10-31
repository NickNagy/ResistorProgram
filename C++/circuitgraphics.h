#ifndef CIRCUITGRAPHICS_H
#define CIRCUITGRAPHICS_H

#include <GL/glut.h>
#include "circuit.h"
#include "globals.h"

#define WINDOW_LENGTH       1200
#define WINDOW_HEIGHT       900
#define WINDOW_CENTER_X     100
#define WINDOW_CENTER_Y     100
#define RESISTOR_BODY_HEIGHT 30

static const unsigned char black[] = {0, 0, 0};
static const unsigned char brown[] = {139, 69, 19};
static const unsigned char red[] = {255, 0, 0};
static const unsigned char orange[] = {255, 165, 0};
static const unsigned char yellow[] = {255, 255, 0};
static const unsigned char green[] = {0, 128, 0};
static const unsigned char blue[] = {0, 0, 255};
static const unsigned char violet[] = {138,43,226};
static const unsigned char grey[] = {128,128,128};
static const unsigned char white[] = {255,255,255};
static const unsigned char beige[] = {245,222,179};

void colorFromString(string color);
void drawLine(int startX, int startY, int endX, int endY);
void drawGrid(unsigned int resistorLength, unsigned int numSpaces);
void drawResistor(int x, int y, int length, int value);
void initGraphics(int argc, char** argv);
void drawCircuit();
void displayCircuit();

#endif