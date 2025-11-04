// Map implementation
#include <map>
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

// Should be a function that takes in the dates desired by the user and returns the two temp values.
// The map is developed based on parsing the data from the online data set.

// Tiny map helper function:
map<int,string> createTinyMap(int key, string value){
    map<int, string> insideMap;
    //logic to implement all dates and temps into this map
    insideMap[key] = value;
    return insideMap;
}

//Updating tiny map helper function:
map<int,string> updateTinyMap(map<int,string> input, int key, string value) {
    // maybe check if you need an if statement to check the date?
    input[key] = value;
    return input;
}

// Helper to get the value of the date/time without dashes or colons:
int getTime(string selectedYear) {
    string year;
    istringstream in(selectedYear);

    getline(in, year, '-');

    string month;
    getline(in, month, '-');

    string day;
    getline(in, day, ' ');

    string hour;
    getline(in, hour, ':');

    string minute;
    getline(in, minute);

    string finalTime = year + month + day + hour + minute;
    return stoi(finalTime);
}

// Setup for the big map:
map<string, map<int, string>> createMap(const string &metFile){
    // Need a large map that is the overall data structure holding all of the Florida weather data.
    // The key will be a string that designates the station in Florida that the data is being pulled from.
    // The value will be an additional map that stores two strings.
        // The key to that inside map is a string that designates the specific time of the data point.
        // The value to that inside map is a string, with the rest of the data from the line.

    map<string, map<int,string>> map;

    // Parsing logic
    ifstream file(metFile);
    string line;
    if(!file.is_open()) {
        cout << metFile << " failure" << endl;
    }
    // Skip header
    getline(file,line);
    while(getline(file,line)) {
        if (line.empty()) {
            continue;
        }
        istringstream iss(line);
        string point;
        vector<string> temp;
        while (getline(iss, point, ',')) {
            temp.push_back(point);
        }
        if (map.find(temp[0]) == map.end()) {
            map[temp[0]] = createTinyMap(getTime(temp[1]), temp[2]);
        }
        else if (map.find(temp[0]) != map.end()) {
            map[temp[0]] = updateTinyMap(map[temp[0]], getTime(temp[1]), temp[2]);
        }
    }
    return map;
}

// Input will be the result of the map from the previous function and a range of years?
vector<pair<int,string>> outputFromMap(map<string, map<int,string>> dataMap, int selectedYearStart, int selectedYearEnd, string selectedStation){
    vector<pair<int,string>> outputFahrenheit;
    // Search the big map for the station selected.
    map<int,string> stationMap = dataMap[selectedStation];
    pair<int,string> stationInfo;
    // To find a range of key values, use upper and lower bounds. Info here: https://www.geeksforgeeks.org/cpp/searching-map-using-stdmap-functions-c/
    auto it1 = stationMap.lower_bound(selectedYearStart);
    auto it2 = stationMap.upper_bound(selectedYearEnd);


    for (auto it = it1; it != it2; it++) {
        stationInfo.first = it->first;
        stationInfo.second = it->second;
        outputFahrenheit.push_back(stationInfo);
    }
    // Return a vector of pairs, first value in the pair is the date, second is the temp value
  return outputFahrenheit;
}

