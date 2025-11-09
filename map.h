#pragma once
#include <map>
using namespace std;

map<long long,string> createTinyMap(long long key, string value);
void updateTinyMap(map<long long,string> &input, long long key, string value);
long long getTime(string selectedYear);
map<string, map<long long, string>> createMap(const string &metFile);
vector<pair<long long,string>> outputFromMap(map<string, map<long long,string>> dataMap, long long selectedYearStart, long long selectedYearEnd, string selectedStation);
vector<pair<long long,string>> averageCalc(vector<pair<long long,string>> input);
vector<pair<long long,string>> celsiusCalc(vector<pair<long long,string>> input);
