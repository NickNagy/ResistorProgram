/* Nick Nagy

This file defines some functions that are used by different files and classes in the same project directory

*/

#ifndef HELPERS_H
#define HELPERS_H

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <map>
#include <string>
#include <sstream>
#include <set>
#include <vector>
#include <math.h>

template<typename Collection>
std::string collectionToString(const Collection& collection) {
    std::ostringstream oss;
    oss << "{";
    if (!collection.empty()){
        copy(collection.begin(), collection.end()-1, std::ostream_iterator<typename Collection::value_type>(oss, ","));
        oss << collection.back();
    }
    oss << "}";
    return oss.str();
}
std::string floatToString(float f, int precision);
std::vector<unsigned int> getRemainingVector(std::vector<unsigned int> original, std::vector<unsigned int> subtractor);
int colorToNumber(std::string s);
std::vector<std::string> getColors(unsigned int resistance);
unsigned int getResistance(std::vector<std::string> colors);

#endif