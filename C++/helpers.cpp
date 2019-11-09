/* Nick Nagy

This file defines some functions that are used by different files and classes in the same project directory

*/

#include "helpers.h"

using namespace std;

/* string intSetToString(multiset<int> s) {
    ostringstream stream;
    copy(s.begin(), s.end(), ostream_iterator<string>(stream, ","));
    return stream.str();
}*/

// returns a string representation of vector v
string intVectorToString(vector<unsigned int> v) {
    ostringstream oss;
    if (!v.empty()) {
        copy(v.begin(), v.end()-1, ostream_iterator<unsigned int>(oss, ","));
        oss << v.back();
    }
    return oss.str();   
}

// returns f as a string with (precision) number of digits after the decimal
string floatToString(float f, int precision) {
    stringstream ss;
    ss << fixed << setprecision(precision) << f;
    return ss.str();
}

// returns a vector that removes the elements of subtractor, not including repeats and ignoring values in subtractor
// that are not also in original
// eg:
//  original = {1, 2, 3, 4, 5, 5, 6}
//  subtractor = {2, 5, 7}
//  result = {1, 3, 4, 5, 6}
vector<unsigned int> getRemainingVector(vector<unsigned int> original, vector<unsigned int> subtractor) {
    vector<unsigned int> result = original;
    for (unsigned int i: subtractor) {
        vector<unsigned int>::iterator it = find(result.begin(), result.end(), i);
        if (it != result.end()) result.erase(it);
    }
    return result;
}

// returns the appropriate digit based on the color defined by string s, to match the resistor color code guide
int colorToNumber(string s) {
    if (!s.compare("Black"))  return 0;
    if (!s.compare("Brown"))  return 1;
    if (!s.compare("Red"))    return 2;
    if (!s.compare("Orange")) return 3;
    if (!s.compare("Yellow")) return 4;
    if (!s.compare("Green"))  return 5;
    if (!s.compare("Blue"))   return 6;
    if (!s.compare("Violet")) return 7;
    if (!s.compare("Grey"))   return 8;
    if (!s.compare("White"))  return 9;
    return -1;
}

// returns a string vector of size 3 - each string defines one of the three resistor band colors corresponding to the given resistance
string * getColors(int resistance) {
    string colorStrings[] = {"Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Grey", "White"};
    string * colors = new string[3];
    int multiplier = 1;
    while (resistance / multiplier >= 100) {
        multiplier *= 10;
    }
    if (resistance % multiplier != 0) {
        int removed = resistance % multiplier;
        cout << "NOTE: simplifying " << resistance << " to " <<  resistance - removed << endl;
        resistance -= removed;
    }
    int firstBand = resistance / (multiplier*10);
    int secondBand = (resistance % (multiplier*10)) / multiplier;
    colors[0] = colorStrings[firstBand];
    colors[1] = colorStrings[secondBand];
    colors[2] = colorStrings[(int)log10(multiplier)];
    return colors;
}

// returns the resistance corresponding to the three band colors passed in colors
int getResistance(string * colors) {
    // assumes length(colors)==3
    int resistance = colorToNumber(colors[0]) * 10;
    resistance += colorToNumber(colors[1]);
    return resistance * (int)pow(10, colorToNumber(colors[2]));
}