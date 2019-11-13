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

using namespace std;

template<typename Collection>
string collectionToString(const Collection& collection) {
    ostringstream oss;
    oss << "{";
    if (!collection.empty()){
        copy(collection.begin(), collection.end()-1, ostream_iterator<typename Collection::value_type>(oss, ","));
        oss << collection.back();
    }
    oss << "}";
    return oss.str();
}
string floatToString(float f, int precision);
vector<unsigned int> getRemainingVector(vector<unsigned int> original, vector<unsigned int> subtractor);
int colorToNumber(string s);
vector<string> getColors(unsigned int resistance);
unsigned int getResistance(vector<string> colors);

#endif