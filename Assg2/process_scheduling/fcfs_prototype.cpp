#include<bits/stdc++.h>
using namespace std;

class process{
public:
    int _id;
    int priority;
    int arrival_time;
    vector<int>bts;
    process(int _id, int priority, int arrival_time, vector<int>bts){
        this->_id = _id;
        this->priority = priority;
        this->arrival_time = arrival_time;
        this->bts = bts;
    }
};

bool comparator(pair<int, int> &a, pair<int, int>&b){
    return a.first < b.first;
}

int main(){
    int no_of_jobs;
    cin >> no_of_jobs;
    vector<process> process_list;

    for(int i = 0; i < no_of_jobs; i++){
        int _id;
        int priority;
        int arrival_time;
        vector<int>bts;

        cin >> _id;
        cin >> priority;
        cin >> arrival_time;
        int t;
        do {
            cin >> t;
            if (t==-1) break;
            bts.push_back(t);
        } while (t!=-1);
        process_list.push_back(process(_id, priority, arrival_time, bts));
    }

    sort(process_list.begin(), process_list.end(), [&](process &a, process &b){
        return a._id < b._id;
    });
    vector<pair<pair<int, int>, int> > gantt_chart;
    priority_queue<pair<int, int>> pq;
    for (auto process :process_list){

    } 


}