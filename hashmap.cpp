//Hashmap Implementation
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <set>
#include "hashmap.h"
using namespace std;

//making each hashmap
using hashHour = HashMap<int, float>;
using hashDay = HashMap<int, hashHour>;
using hashMonth = HashMap<int, hashDay>;
using hashYear = HashMap<int, hashMonth>;
using hashStation = HashMap<string, hashYear>;
hashStation stations(113); //there are 113 stations in Florida


// Helper to get the value of the date/time without dashes or colons:
string getTimeString(string selectedYear) {
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
void insertTemperatures(string station, int year, int month, int day, int hour, float temp){
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

//  Temperature* temps = dayMap->find(day);
//  if (temps == nullptr){
//    dayMap->insert(day, Temperature());
//    temps = dayMap->find(day);
//  }

  hashHour* hourMap = dayMap->find(day);
    if (!hourMap){
        dayMap->insert(day, hashHour(24));
        hourMap = dayMap->find(day);
    }

  hourMap->insert(hour, temp);
  //temps->insertTemp(temp);
}


pair<long long, float> averageDaily(string station, int year, int month, int day, bool isCelsius){
  hashYear* yearMap = stations.find(station);
  string yr = to_string(year);
  string mm = to_string(month);
  string dd = to_string(day);

  long long x = stoll(yr + mm + dd);

  if (!yearMap){
    return {x, 0.0};
  }
  hashMonth* monthMap = yearMap->find(year);
  if (!monthMap){
    return {x, 0.0};
  }
  hashDay* dayMap = monthMap->find(month);
  if (!dayMap){
    return {x, 0.0};
  }
  hashHour* hourMap = dayMap->find(day);
  if (!hourMap){
    return {x, 0.0};
  }
  float sum = 0;
  int count = 0;
  for (int i = 0; i < 24; ++i){
    float* temp = hourMap->find(i);
    if (temp){
        sum += *temp;
        count++;
    }
  }
  float avg;
  if (count != 0){
    avg = sum/count;
  }
  else{
    avg = 0.0;
  }
  if (isCelsius){
      avg = (avg - 32) * 5.0 / 9.0;
  }
  return {x, avg};
}

//Front end implementer only has to use this function
vector<pair<long long, float>> weatherMap(const string &metfile, int yearFrom, int yearTo, bool isCelsius, const string& selectedStation){
  //Parsing logic
  vector<pair<int, float>> allTemps;
  set<tuple<string, int, int, int>> seenDays;

  ifstream file(metfile);
  string line;
  if(!file.is_open()) {
    cout << metfile << " failure" << endl;
  }
  //Skip header
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

    if (temp.size() < 3){
      continue;
    }
    if (temp[1].empty() || temp[2].empty() || temp[2][0] == 'M'){
      continue;
    }
    if (temp[0] != selectedStation){
      continue;
    }
    string timeStr = getTimeString(temp[1]);
    int year = stoi(timeStr.substr(0, 4));
    int month = stoi(timeStr.substr(4, 2));
    int day = stoi(timeStr.substr(6, 2));
    int hour = stoi(timeStr.substr(8, 2));

    float temperature = stof(temp[2]);
    if (year < yearFrom || year > yearTo) {
      continue;
    }

    insertTemperatures(temp[0], year, month, day, hour, temperature);
    seenDays.insert({temp[0], year, month, day});
  }
  for (auto &[station, year, month, day] : seenDays) {
    allTemps.push_back(averageDaily(station, year, month, day, isCelsius));
  }
  return allTemps;
}
