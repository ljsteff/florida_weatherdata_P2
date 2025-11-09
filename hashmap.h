#ifndef HASHMAP_H
#define HASHMAP_H
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <set>
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
        //return std::hash<First>{}(first) % size; //I want each index to be a new day
        if constexpr (std::is_same_v<First, std::string>) {
            int hash = 0;
            for (char c : first) hash += c;
            return hash % size;
        }
        else if constexpr (std::is_same_v<First, int>) {
            if (size == 16) {
                return first - 2010;
            }
            else if (size == 12) {
                return first - 1;
            }
            else if (size == 31) {
                return first - 1;
            }
            else {
                return first % size;
            }
        }
        else {
            // generic fallback
            return first % size;
        }
    }
public:
    HashMap(int size) : table(size, nullptr){} //initialize all elements to nullptr
    ~HashMap() {
        for (auto node : table) {
            while (node) {
                Node<First, Second>* temp = node;
                node = node->next;
                delete temp;
            }
        }
    }
    void insert(First first, Second second){
        int index = hashFunction(first);
        Node<First, Second>* node = table[index];
        while (node != nullptr){
            if (node->first == first){
                node->second = second;
                return;
            }
            node = node->next;
        }
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

//Making a special hashmap for temperatures because it should allow duplicate values
struct NodeTemp{
    float temp;
    NodeTemp* next;
    NodeTemp(float t) : temp(t), next(nullptr){}
};

class Temperature{
public:
    NodeTemp* head = nullptr;
    ~Temperature() {
        while (head) {
            NodeTemp* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void insertTemp(float t){
        NodeTemp* newNode = new NodeTemp(t);
        newNode->next = head;
        head = newNode;
    }

    float average(){
        float sum = 0.0;
        float count = 0.0;
        NodeTemp* curr = head;
        while (curr != nullptr){
            sum += curr->temp;
            count++;
            curr = curr->next;
        }
        if (count == 0){
            return 0;
        }
        return sum/count;
    }
};

string getTimeString(string selectedYear);
void insertTemperatures(string station, int year, int month, int day, float temp);
pair<long long, float> averageDaily(string station, int year, int month, int day, bool isCelsius);
vector<pair<long long, float>> weatherMap(const string &metfile, int yearFrom, int yearTo, bool isCelsius, const string& selectedStation);


#endif //HASHMAP_H
