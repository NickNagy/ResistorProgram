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

string intSetToString(multiset<int> s);
string intVectorToString(vector<unsigned int> v);
string floatToString(float f, int precision);
vector<unsigned int> getRemainingVector(vector<unsigned int> original, vector<unsigned int> subtractor);
int colorToNumber(string s);
string * getColors(unsigned int resistance);
unsigned int getResistance(string * colors);

#endif