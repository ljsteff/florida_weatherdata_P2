// Hashmap implemenation
#include <iostream>
#include <vector>
#include <sstring>

using namespace std;

//Making a HashMap class with template because I want to make a nested hashmap (4 times)
template <typename First, typename Second>
struct Node{
    First first;
    Second second;
    Node* next;
};

//I hope this works with separate chaining for the innermost hashmap to insert all temperatures per day
class HashMap{
    //each station/year/month/day is a separate key
    int hashFunction(First first){
      return first % 1;
    }
    public:
      HashMap(int size){
        for (int i=0; i < size; i++){
          table[i] = NULL; //initialize every element in the table to null
        }
      }
      void insert(First first, Second second){
        int index = hashFunction(first); //hashfunction returns the index
        Node<First, Second>* node = table[index];
        while (node != NULL){
          if (node->first == first){ //insert value into index
            node->second = second;
            return;
          }
          node = node->next;
        }
        Node<First, Second>* newnode = new Node<First, Second>(first, second); //create new node
        newnode->next = table[index];
        table[index] = newnode;
      }
      bool find(First first){
        for (int i=0; i < table.size(); i++){
          if (table[i] == first){
            return true;
        }
        return false;
      }
};

//making each hashmap
HashMapDay(31) = HashMap<int, int>;
HashMapMonth(12) = HashMap<int, HashMapDay>;
HashMapYear(14) = HashMap<int, HashMapMonth>;
HashMapStation(113) = HashMap<int, HashMapYear>;

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

//Helper functoin for inserting everything
void insert(string station, int year, int month, int day, int temp){
  HashMapStation stationMap;
  HashMapYear yearMap;
  HashMapMonth monthMap;
  HashMapDay dayMap;

  if (!stationMap.find(station)){
    stationMap = HashMapStation(station);
  }
  if (!yearMap.find(year)){
    yearMap = HashMapYear(year);
  }
  if (!monthMap.find(month)){
    monthMap = HashMapMonth(month);
  }
  if (!dayMap.find(day)){
    dayMap = HashMapDay(day);
  }

  dayMap.insert(day, temp);
  monthMap.insert(month, dayMap);
  yearMap.insert(year, monthMap);
  stationMap.insert(station, yearMap);
}

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
  insert(temp[0], getTime(temp[1])[0:3], getTime(temp[1])[4:5], getTime(temp[1])[6:7]);
}

