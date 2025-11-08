#ifndef HASHMAP_H
#define HASHMAP_H
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
        return std::hash<First>{}(first) % size; //I want each index to be a new day
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

//Making a special hashmap for temperatures because it should allow duplicate values
struct NodeTemp{
    int temp;
    NodeTemp* next;
    NodeTemp(int t) : temp(t), next(nullptr){}
};

class Temperature{
public:
    NodeTemp* head = nullptr;

    void insertTemp(int t){
        NodeTemp* newNode = new NodeTemp(t);
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
        return sum/count;
    }
};

string getTime(string selectedYear);
void insertTemperatures(string station, int year, int month, int day, int temp);
pair<int, float> averageDaily(string station, int year, int month, int day, bool isFahrenheit);
vector<pair<int, float>> weatherMap(const string &metfile, bool isFahrenheit);


#endif //HASHMAP_H
