#include "circuitgraphics.h"

using namespace std;

void initGraphics(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(WINDOW_LENGTH, WINDOW_HEIGHT);
    glutInitWindowPosition(WINDOW_CENTER_X, WINDOW_CENTER_Y);
    graphicsInitialized = 1;
}

void colorFromString(string color) {
    if (!color.compare("Black"))  glColor3ubv(black);
    if (!color.compare("Brown"))  glColor3ubv(brown);
    if (!color.compare("Red"))    glColor3ubv(red);
    if (!color.compare("Orange")) glColor3ubv(orange);
    if (!color.compare("Yellow")) glColor3ubv(yellow);
    if (!color.compare("Green"))  glColor3ubv(green);
    if (!color.compare("Blue"))   glColor3ubv(blue);
    if (!color.compare("Violet")) glColor3ubv(violet);
    if (!color.compare("Grey"))   glColor3ubv(grey);
    if (!color.compare("White"))  glColor3ubv(white);
}

void drawLine(int startX, int startY, int endX, int endY) {
    glVertex2f(startX, startY);
    glVertex2f(endX, endY);
}

void drawGrid(unsigned int resistorLength, unsigned int numSpaces) {
    glColor3ubv(green);
    for (int i = 0; i < numSpaces; i++) {
        drawLine(i*resistorLength, WINDOW_HEIGHT/2, i*resistorLength, 0-WINDOW_HEIGHT/2);
    }
}

void drawResistor(int x, int y, int length, int value) {
    // drawn line (about 1/3 length)
    string * resistorColors = getColors(value);
    int bodyStart = x + (length/3);
    int bodyEnd = x + (2*length/3);
    int bodyLength = bodyEnd - bodyStart;
    glColor3ubv(white);
    glBegin(GL_LINES);
        drawLine(x, y, bodyStart, y);
        drawLine(bodyEnd, y, x + length, y);
    glEnd();
    glColor3ubv(beige);
    glRecti(bodyStart, y, bodyStart + bodyLength, y + RESISTOR_BODY_HEIGHT);
    // draw rectangle and colors
    // TODO: add resistance value as title above the rectangle
    // draw final line
    
}

void drawCircuit() {
    unsigned int initHeight = WINDOW_HEIGHT / 2;
    unsigned int nextHeight = 30; // placeholder
    unsigned int circuitSize = cMx -> getSize();
    unsigned int resistorLength = WINDOW_LENGTH / circuitSize;
    cout << "resistor length: " << resistorLength << endl;
    drawGrid(resistorLength, circuitSize);
    for (int i = 0; i < circuitSize - 1; i++) {
        for (int j = i+1; j < circuitSize; j++) {
            char y = 0;
            for (int r : cMx->getResistors(i,j)) {
                drawResistor(i*resistorLength,y,resistorLength,r);
                y+=nextHeight;
            }
        }
    }
}

void displayCircuit() {
    glutCreateWindow("");
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(drawCircuit);
    glFlush();
    glutMainLoop();
}

