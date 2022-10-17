#include<bits/stdc++.h>
using namespace std;

class Process {
public:
    int _id;
    int priority;
    int arrival_time;
    vector <int> burst_time;
    Process(int _id, int priority, int arrival_time){
        this->_id = _id;
        this->priority = priority;
        this->arrival_time = arrival_time;
    }
};

class Process_fcfs {
public:  
    int _id;
    int priority;
    int arrival_time;
    Process_fcfs (int _id, int priority, int arrival_time){
        this->_id = _id;
        this->priority = priority;
        this->arrival_time = arrival_time;
    }
};

int main(){
    freopen("input.txt", "r", stdin);
    int id;
    int priority;
    int arrival_time;

    while (){
        cin >> id;
        cin >> priority;
        cin >> arrival_time;

        auto temp = Process(id, priority, arrival_time);

    }
}


