#include "helpers.h"

using namespace std;

/* string intSetToString(multiset<int> s) {
    ostringstream stream;
    copy(s.begin(), s.end(), ostream_iterator<string>(stream, ","));
    return stream.str();
}*/

string floatToString(float f, int precision) {
    stringstream ss;
    ss << fixed << setprecision(precision) << f;
    return ss.str();
}

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

int getResistance(string * colors) {
    // assumes length(colors)==3
    int resistance = colorToNumber(colors[0]) * 10;
    resistance += colorToNumber(colors[1]);
    return resistance * (int)pow(10, colorToNumber(colors[2]));
}