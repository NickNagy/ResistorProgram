#include<iostream>
#include<vector>
#include<set>
#include<string>
#include<sstream>
#include<iomanip>
#include<stdlib.h> // for abs
#include<graphics.h>
# define sizeof
using namespace std;

string floatToString(float f, int precision) {
    stringstream ss;
    ss << fixed << setprecision(precision) << f;
    return ss.str();
}

class CircuitEdge{
    private:

    float localResistance = 0.0;
    multiset<int> resistors;// = multiset<int>;

    void computeLocalResistance() {
        float sum = 0.0;
        for (int r: resistors) {
            sum+=(1.0/r);
        }
        localResistance = 1.0 / sum;
    }

    public: 
    
    float nonLocalResistance = 0.0;

    void insert(int r) {
        resistors.insert(r);//push_back(r);
        computeLocalResistance();
    }

    // TODO: fix
    int remove(int r) {
        auto itr = resistors.find(r);
        if (itr!=resistors.end()) {
            resistors.erase(itr);
            computeLocalResistance();
            return 1;
        }
        return 0;
    }

    float getLocalResistance() {
        return localResistance;
    }

    float getTotalResistance() {
        if (nonLocalResistance == 0.0 && localResistance == 0.0) return 0.0; // simplify? gate-level?
        if (nonLocalResistance == 0.0) return localResistance;
        if (localResistance == 0.0) return nonLocalResistance;
        return nonLocalResistance*localResistance/(localResistance + nonLocalResistance); // careful of overflow
    }

};

class CircuitMatrix {
    private:
    unsigned int size = 2;
    vector<vector<CircuitEdge*>> matrix = vector<vector<CircuitEdge*>>(size, vector<CircuitEdge*>(size));

    // resizes the matrix
    void resize() {
        size++;
        matrix.resize(size);
        for (int i = 0; i < size; i++) {
            matrix[i].resize(size);
        }
        for (int j = 0; j < size-1; j++) {
            matrix[j][size-1] = new CircuitEdge();
            matrix[size-1][j] = matrix[j][size-1];
        }
        matrix[size-1][size-1] = matrix[0][0];
    }

    // updates all the edges >=x, >=y in the matrix after an insertion
    void refresh(int x, int y) {
        int i = x-1;
        while (i >= 0) {
            matrix[i][y]->nonLocalResistance += matrix[i+1][y]->getTotalResistance() + matrix[i][y-x+i]->getTotalResistance();
            i--;
        }
    }
    
    public:

    CircuitMatrix();

    int layResistor(int value, int x, int y) {
        // TODO: can I simplify this at all?
        if (x < 0 || y < 0 || value <= 0 || x == y || x > size || y > size) {
            return 0;
        }
        if (x == size || y == size) {
            resize();
        }
        matrix[x][y]->insert(value);
        if (x > 0 && y > 0) {
            matrix[x][y]->nonLocalResistance = matrix[x-1][y]->getTotalResistance() + matrix[x][y-1]->getTotalResistance();
        }
        refresh(x,y);
        return 1;
    }

    int removeResistor(int value, int x, int y) {
        // TODO: can I simplify this at all?
        if (x < 0 || y < 0 || value <= 0 || x == y || x > size || y > size) {
            return 0;
        }
        int success = matrix[x][y]->remove(value);
        if (!success) return 0;
        refresh(x,y);
        return 1;
    }

    unsigned int getSize() {
        return size;
    }

    float getResistance(int x, int y) {
        return matrix[x][y]->getTotalResistance();
    }

    float getTotalResistance() {
        return getResistance(0, size-1);
    }

    string toString() {
        string s = "\nMatrix:\n[[";
        for (unsigned int i = 0; i < size-1; i++) {
            for (unsigned int j = 0; j < size-1; j++) {
                if (i == j) {
                    s += "0.00,";
                } else {
                    s += floatToString(matrix[i][j]->getTotalResistance(), 2) + ",";
                }
            }
            s += floatToString(matrix[i][size-1]->getTotalResistance(),2) + "]\n [";
        }
        for (unsigned int k = 0; k < size-1; k++) {
            s += floatToString(matrix[size-1][k]->getTotalResistance(),2) + ",";
        }
        s += "0.00]]\n";
        s += "Total Resistance = " + floatToString(getTotalResistance(), 2) + "\n\n";
        return s;
    }

};

class CircuitGraphics {
    private:
    int bodyHeight = 10;

    void setup() {
        int gd = DETECT, gm;
        initgraph(&gd, &gm, "");
    }

    void drawResistor(int x, int y, int length, int value) {
        // starting point
        // drawn line (about 1/3 length)
        int bodyStart = x + (length/3);
        int bodyEnd = x + (2*length/3);
        line(x, y, bodyStart, y);
        // draw rectangle and colors
        line(bodyStart, y - (bodyHeight/2), bodyStart, y + (bodyHeight/2));
        line(bodyStart, y + (bodyHeight/2), bodyEnd, y + (bodyHeight/2));
        line(bodyStart, y - (bodyHeight/2), bodyEnd, y - (bodyHeight/2));
        line(bodyEnd, y - (bodyHeight/2), bodyEnd, y + (bodyHeight/2));
        // add resistance value as title above the rectangle
        // draw final line
        line(bodyEnd, y, x + length, y);
    }

    public:

    void displayCircuit(CircuitMatrix * cm) {
        unsigned int windowLength = 500; // placeholder
        unsigned int windowHeight = 200; // placeholder
        unsigned int initHeight = windowHeight / 2;
        unsigned int nextHeight = 
        unsigned int resistorLength = windowLength / cm->getSize();
    }

};

CircuitMatrix::CircuitMatrix() {
    CircuitEdge * E = new CircuitEdge();
    CircuitEdge * nul = new CircuitEdge();
    matrix[0][0] = nul;
    matrix[0][1] = E;
    matrix[1][0] = E;
    matrix[1][1] = nul;
}

int main() {
    CircuitMatrix cMx = CircuitMatrix();
    cMx.layResistor(1, 0, 1);
    cout << cMx.toString();
    cMx.layResistor(2, 1, 2);
    cout << cMx.toString();
    cMx.layResistor(3, 2, 3);
    cout << cMx.toString();
    cMx.layResistor(6, 0, 3);
    cout << cMx.toString();
    cMx.removeResistor(6, 0, 3);
    cout << cMx.toString();
}