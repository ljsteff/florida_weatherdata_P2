//Hashmap Implementation
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include "hashmap.h"
using namespace std;

//making each hashmap
using hashDay = HashMap<int, Temperature>;
using hashMonth = HashMap<int, hashDay>;
using hashYear = HashMap<int, hashMonth>;
using hashStation = HashMap<string, hashYear>;
hashStation stations(113); //there are 113 stations in Florida



// Helper to get the value of the date/time without dashes or colons:
string getTime(string selectedYear) {
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
  return finalTime;
}

//Helper function for inserting everything
void insertTemperatures(string station, int year, int month, int day, int temp){
  hashYear* yearMap = stations.find(station);
  if (!yearMap){
    stations.insert(station, hashYear(14)); //set tablesize for yearMap to 14 because we have 14 years worth of info
    yearMap = stations.find(station);
  }
  hashMonth* monthMap = yearMap->find(year);
  if (!monthMap){
    yearMap->insert(year, hashMonth(12));
    monthMap = yearMap->find(year);
  }
  hashDay* dayMap = monthMap->find(month);
  if (!dayMap){
    monthMap->insert(month, hashDay(31));
    dayMap = monthMap->find(month);
  }
  Temperature* temps = dayMap->find(day);
  if (!temps){
    dayMap->insert(day, Temperature());
    temps = dayMap->find(day);
  }

  temps->insertTemp(temp);
}


pair<int, float> averageDaily(string station, int year, int month, int day, bool isCelsius){
  hashYear* yearMap = stations.find(station);
  if (!yearMap){
    return {day, 0.0};
  }
  hashMonth* monthMap = yearMap->find(year);
  if (!monthMap){
    return {day, 0.0};
  }
  hashDay* dayMap = monthMap->find(month);
  if (!dayMap){
    return {day, 0.0};
  }
  Temperature* temps = dayMap->find(day);
  if (!temps){
    return {day, 0.0};
  }
  if (isCelsius){
    return make_pair(day, (temps->average()-32)*5.0/9.0);
  }
  return make_pair(day, temps->average());
}

//Front end implementer only has to use this function
vector<pair<int, float>> weatherMap(const string &metfile, int yearFrom, int yearTo, bool isCelsius){
  // Parsing logic
  vector<pair<int, float>> allTemps;
  ifstream file(metfile);
  string line;
  if(!file.is_open()) {
    cout << metfile << " failure" << endl;
  }
  // Skip header
  getline(file,line);
  while(getline(file,line)) {
    if (line.empty() || line[0] == 'M') {
      continue;
    }
    istringstream iss(line);
    string point;
    vector<string> temp;
    while (getline(iss, point, ',')) {
      temp.push_back(point);
    }
    //inserting everything while trying to
    string timeStr = getTime(temp[1]);
    int year = stoi(timeStr.substr(0, 4));
    int month = stoi(timeStr.substr(4, 2));
    int day = stoi(timeStr.substr(6, 2));
    int temperature = stoi(temp[2]);
    if (year < yearFrom || year > yearTo) {
      continue;
    }

    insertTemperatures(temp[0], year, month, day, temperature);
    allTemps.push_back(averageDaily(temp[0], year, month, day, isCelsius));
  }
  return allTemps;
}
