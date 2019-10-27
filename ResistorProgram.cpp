#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<iomanip>
#include<stdlib.h> // for abs
# define sizeof
using namespace std;

class Edge{
    private:

    float netResistance = 0.0;
    vector<int> resistors = vector<int>(0);

    public: 
        
    void insert(int r) {
        resistors.push_back(r);
        float sum = 0.0;
        for (int r: resistors) {
            sum+=(1.0/r);
        }
        netResistance = 1.0 / sum;
    }

    float getNetResistance() {
        return netResistance;
    }

};

class CircuitMatrix {
    private:
    int size = 2;
    float totalResistance = 0;
    vector<vector<Edge*>> matrix = vector<vector<Edge*>>(size, vector<Edge*>(size));

    void resize() {
        size++;
        matrix.resize(size);
        for (int i = 0; i < size; i++) {
            matrix[i].resize(size);
        }
        for (int j = 0; j < size-1; j++) {
            matrix[j][size-1] = new Edge();
            matrix[size-1][j] = matrix[j][size-1];
        }
        matrix[size-1][size-1] = matrix[0][0];
    }
    
    public:

    CircuitMatrix();

    int insert(int value, int x, int y) {
        // TODO: can I simplify this at all?
        // also need to update some conditions
        if (x < 0 || y < 0 || value <= 0 || x == y || x > size || y > size) {
            return 0;
        }
        // TODO: fix!
        // want to update totalResistance any and every time a resistor gets added!
        if (x == size || y == size) {
            resize();
            matrix[x][y]->insert(value);
            totalResistance += matrix[size-1][size-2]->getNetResistance(); // = value if resize() only happens @ [size-1][size-2]
        } else if(abs(y-x)==1) {
            totalResistance -= matrix[x][y]->getNetResistance();
            matrix[x][y]->insert(value);
            totalResistance += matrix[x][y]->getNetResistance();
        } else {
            matrix[x][y]->insert(value);
        }
        return 1;
    }

    float getResistance(int x, int y) {
        
    }

    float getTotalResistance() {
        return totalResistance;
    }

    string toString() {
        stringstream ss;
        string s = "\nMatrix:\n[[";
        for (int i = 0; i < size-1; i++) {
            for (int j = 0; j < size-1; j++) {
                if (i == j) {
                    s += "0.00,";
                } else {
                    ss << fixed << setprecision(2) << matrix[i][j]->getNetResistance();
                    s += ss.str() + ",";
                    ss.str("");
                }
            }
            ss << fixed << setprecision(2) << matrix[i][size-1]->getNetResistance();
            s += ss.str() + "]\n [";
            ss.str("");

        }
        for (int k = 0; k < size-1; k++) {
            ss << fixed << setprecision(2) << matrix[size-1][k]->getNetResistance();
            s += ss.str() + ",";
            ss.str("");
        }
        s += "0.00]]\n";
        ss << fixed << setprecision(2) << totalResistance;
        s += "Total Resistance = " + ss.str() + "\n\n";
        return s;
    }

};

CircuitMatrix::CircuitMatrix() {
    Edge * E = new Edge();
    Edge * nul = new Edge();
    matrix[0][0] = nul;
    matrix[0][1] = E;
    matrix[1][0] = E;
    matrix[1][1] = nul;
}


/* class ResistorCircuit {
    private:
    int numResistors;
    int netResistance;
    CircuitMatrix cMx;
    
    public:

    void init(int resistorValue) {
        numResistors = 1;
        netResistance = resistorValue;
        cMx.insert(0,1,resistorValue);
    }

    void layResistor(int n1, int n2, int resistorValue) {
        if (cMx.insert(n1, n2, resistorValue)) numResistors++;
    }

};*/

int main() {
    CircuitMatrix cMx = CircuitMatrix();
    cMx.insert(5, 0, 1);
    cMx.insert(5, 1, 0);
    cout << cMx.toString();
    cMx.insert(3, 1, 2);
    cout << cMx.toString();
    cMx.insert(20,2,3);
    cout << cMx.toString();
    cMx.insert(20,1,3);
    cout << cMx.toString();
    cMx.insert(20,1,3);
    cout << cMx.toString();
}