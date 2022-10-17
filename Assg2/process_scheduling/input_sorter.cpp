#include<bits/stdc++.h>
using namespace std;

int main(){
    freopen("output.txt", "r", stdin);
    freopen("output_pp.txt", "w", stdout);


    int t;
    cin >> t;
    cout << t << endl;
    vector < vector<int> > res;
    while (t--){
        int a;
        vector <int> lol;
        cin >> a;
        do {
            lol.push_back(a);
            cin >> a;
        } while (a != -1);
        res.push_back(lol);
    }

    sort(res.begin(), res.end(), [&](vector<int> &a, vector<int> &b){
        return a[1] < b[1];
    });

    for (auto i : res){
        for (auto j : i){
            cout << j << " ";
        } cout << -1 << endl;
    }
}