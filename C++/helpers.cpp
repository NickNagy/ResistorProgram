/* Nick Nagy

This file defines some functions that are used by different files and classes in the same project directory

*/

#include "helpers.h"

// returns f as a string with (precision) number of digits after the decimal
std::string floatToString(float f, int precision) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << f;
    return ss.str();
}

// returns a vector that removes the elements of subtractor, not including repeats and ignoring values in subtractor
// that are not also in original
// eg:
//  original = {1, 2, 3, 4, 5, 5, 6}
//  subtractor = {2, 5, 7}
//  result = {1, 3, 4, 5, 6}
std::vector<unsigned int> getRemainingVector(std::vector<unsigned int> original, std::vector<unsigned int> subtractor) {
    if (!original.size()) return original;
    std::vector<unsigned int> result = original;
    for (unsigned int i: subtractor) {
        std::vector<unsigned int>::iterator it = find(result.begin(), result.end(), i);
        if (it != result.end()) result.erase(it);
    }
    return result;
}

// returns the appropriate digit based on the color defined by string s, to match the resistor color code guide
int colorToNumber(std::string s) {
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
std::vector<std::string> getColors(unsigned int resistance) {
    std::string colorStrings[] = {"Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Grey", "White"};
    std::vector<std::string> colors(3);
    int multiplier = 1;
    while (resistance / multiplier >= 100) {
        multiplier *= 10;
    }
    if (resistance % multiplier != 0) {
        int removed = resistance % multiplier;
        std::cout << "NOTE: simplifying " << resistance << " to " <<  resistance - removed << std::endl;
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
unsigned int getResistance(std::vector<std::string> colors) {
    int first = colorToNumber(colors[0]);
    int second = colorToNumber(colors[1]);
    int third = colorToNumber(colors[2]);
    if (first < 0 || second < 0 || third < 0) return 0;
    int resistance = first * 10;
    resistance += second;
    return resistance * (int)pow(10, third);
}