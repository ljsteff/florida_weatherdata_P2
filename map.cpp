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
map<long long,string> createTinyMap(long long key, string value){
    map<long long, string> insideMap;
    //logic to implement all dates and temps into this map
    insideMap[key] = value;
    return insideMap;
}

//Updating tiny map helper function:
void updateTinyMap(map<long long,string> &input, long long key, string value) {
    // pass by reference
    input[key] = value;
}

// Helper to get the value of the date/time without dashes or colons:
long long getTime(string selectedYear) {
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
    return stoll(finalTime);
}

// Setup for the big map:
map<string, map<long long, string>> createMap(const string &metFile){
    // Need a large map that is the overall data structure holding all of the Florida weather data.
    // The key will be a string that designates the station in Florida that the data is being pulled from.
    // The value will be an additional map that stores two strings.
        // The key to that inside map is a string that designates the specific time of the data point.
        // The value to that inside map is a string, with the rest of the data from the line.

    map<string, map<long long,string>> map;

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
            updateTinyMap(map[temp[0]], getTime(temp[1]), temp[2]);
        }
    }
    return map;
}

// Input will be the result of the map from the previous function and a range of years?
vector<pair<long long,string>> outputFromMap(map<string, map<long long,string>> dataMap, long long selectedYearStart, long long selectedYearEnd, string selectedStation){
    vector<pair<long long,string>> outputFahrenheit;
    // Search the big map for the station selected.
    map<long long,string> stationMap = dataMap[selectedStation];
    pair<long long,string> stationInfo;
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

vector<pair<long long,string>> averageCalc(vector<pair<long long,string>> input){
  // Takes the average of all the datapoints for each day and leaves it in the pair
    vector<pair<long long,string>> output;
    string store = to_string(input[0].first).substr(0,8);
    vector<int> temp;
    for (auto it = input.begin(); it != input.end(); it++) {
    	int avg = 0;
    	string day = to_string(it->first).substr(0,8);
    	if (store == day) {
    		temp.push_back(stoi(it->second));
     	}
    	else {
    		for (int i = 0 ; i < temp.size(); i++) {
    			avg += temp[i];
    		}
    	avg /= temp.size();
    	output.push_back(make_pair(stoll(store), to_string(avg)));
	 	temp.clear();
    	temp.push_back(stoi(it->second));
    	store = day;
    	}
	}
	if(!temp.empty()) {
    	int average = 0;
    	for(int j = 0; j < temp.size(); j++) {
        average += temp[j];
    	}
      average /= temp.size();
      output.push_back(make_pair(stoll(store), to_string(average)));
    }
    return output;
}

vector<pair<long long,string>> celsiusCalc(vector<pair<long long,string>> input){
  // Translating all temperture values into Celsius from Fahrenheit
  vector<pair<long long,string>> output;
  for (auto it = input.begin(); it != input.end(); it++) {
    int newTemp = static_cast<int>((stoi(it->second)-32)*5.0/9.0);
    pair<long long, string> newPair;
    newPair = make_pair(it->first, to_string(newTemp));
    output.push_back(newPair);
  }
  return output;
}

