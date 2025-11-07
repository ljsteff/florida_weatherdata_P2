//Hashmap Implementation
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
using namespace std;

//Making a HashMap class with template because I want to make a nested hashmap (4 times)
template <typename First, typename Second>
struct Node{
    First first;
    Second second;
    Node* next;
    Node(First f, Second s): first(f), second(s), next(nullptr){}
};

template <typename First, typename Second>
//I hope this works with separate chaining for the innermost hashmap to insert all temperatures per day
class HashMap{
    //each station/year/month/day is a separate key
    vector<Node<First, Second>*> table;
    int size;
    int hashFunction(First first){
      return first % size; //I want each index to be a new day
    }
    public:
      HashMap(int size) : table(size, nullptr){} //initialize all elements to nullptr
      void insert(First first, Second second){
        int index = hashFunction(first); //hashfunction returns the index
        Node<First, Second>* node = table[index];
        while (node != nullptr){
          if (node->first == first){ //insert value into index using linkedlist
            node->second = second;
            return;
          }
          node = node->next;
        }
        //If first does not exist, create a new node and push it front
        Node<First, Second>* newnode = new Node<First, Second>(first, second);
        newnode->next = table[index];
        table[index] = newnode;
      }
      //function to find and return element's second
      Second* find(First first){
        int index = hashFunction(first);
        Node<First, Second>* node = table[index];
        while (node != nullptr){
          if (node->first == first){
            return &node->second;
          }
          node = node->next;
        }
        return nullptr;
      }
};

//making each hashmap
using hashDay = HashMap<int, Temperature>;
using hashMonth = HashMap<int, hashDay>;
using hashYear = HashMap<int, hashMonth>;
using hashStation = HashMap<int, hashYear>;
hashStation stations(113); //there are 113 stations in Florida

//Making a special hashmap for temperatures because it should allow duplicate values
struct NodeTemp{
    int temp;
    NodeTemp* next;
    NodeTemp(int t) : temp(t), next(nullptr){}
};

class Temperature{
    public:
      NodeTemp* first = nullptr;

      void insertTemp(NodeTemp* first, NodeTemp* second){
        NodeTemp* newNode = new NodeTemp(first->temp);
        newNode->next = head;
        head = newNode;
      }

      float average(){
        int sum = 0;
        int count = 0;
        NodeTemp* curr = head;
        while (curr != nullptr){
          sum += curr->temp;
          count++;
          curr = curr->next;
        }
        if (count == 0){
          return 0;
        }
        return sum / count;
      }
};

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
  return finalTime;
}

//Helper function for inserting everything
void insertTemperatures(string station, int year, int month, int day, int temp){
  hashYear* yearMap = stations.find(station);
  if (!yearMap){
    stations.insert(station, yearMap(14); //set tablesize for yearMap to 14 because we have 14 years worth of info
  }
  hashMonth* monthMap = yearMap->find(year);
  if (!monthMap){
    yearMap->insert(year, monthMap(12));
    monthMap = yearMap->find(month);
  }
  hashDay* dayMap = yearMap->find(month);
  if (!dayMap){
    monthMap->insert(month, dayMap(31));
    dayMap = monthMap->find(day);
  }
  Temperature* temps = dayMap->find(temp);
  if (!temps){
    return 0;
  }

  dayMap.insert(day, temp);
  monthMap.insert(month, dayMap);
  yearMap.insert(year, monthMap);
  stationMap.insert(station, yearMap);
}


pair<int, int> averageDaily(string station, int year, int month, int day, bool isFahrenheit){
  hashYear* yearMap = stations.find(station);
  if (!yearMap){
    return 0;
  }
  hashMonth* monthMap = yearMap->find(month);
  if (!monthMap){
    return 0;
  }
  hashDay* dayMap = yearMap->find(day);
  if (!dayMap){
    return 0;
  }
  Temperature* temps = dayMap->find(temp);
  if (!temps){
    return 0;
  }
  if (!isFahrenheit){
    return make_pair(day, (temps->average()-32)*5/9);
  }
  return make_pair(day, temps->average());
}

vector<int, pair<int, int>> weatherMap(const string &metfile, bool isFahrenheit){
  // Parsing logic
  vector<int, pair<int, int>> allTemps;
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
    //inserting everything while trying to
    insertTemperatures(temp[0], getTime(temp[1]).substr(0,3), getTime(temp[1]).substr(4,5), getTime(temp[1]).substr(6,7), temp[2]);
    allTemps.push_back(averageDaily(temp[0], getTime(temp[1]).substr(0,3), getTime(temp[1]).substr(4,5), getTime(temp[1]).substr(6,7), temp[2], isCelsius);
  }
  return allTemps;
}

