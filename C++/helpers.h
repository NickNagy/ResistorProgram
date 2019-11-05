/* Nick Nagy

This file defines some functions that are used by different files and classes in the same project directory

*/

#ifndef HELPERS_H
#define HELPERS_H

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
string intVectorToString(vector<int> v);
string floatToString(float f, int precision);
int colorToNumber(string s);
string * getColors(int resistance);
int getResistance(string * colors);

#endif