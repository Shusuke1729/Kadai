#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <queue>
#include <thread>
#include <chrono>
#define rep(i, n) for(int i = 0 ; i < (n); i++)
#define rep2(i, k, n) for(int i = (k); i < (n); i++)
using ll = long long;
using namespace std;
#define inf 1e18

int main(void) {
    vector<double> arrive;
    vector<double> serivce;
    queue<double> queue; 
    double t = 0.0, serive_endtime = inf;
    int i = 0;

    ifstream file("arrive.txt");
    string line;

    while(getline(file, line)) {
        double num = stod(line);
        arrive.push_back(num);
    }
    file.close();
    file.open("service.txt");
    while(getline(file, line)) {
        double num = stod(line);
        serivce.push_back(num);
    }
    file.close();
    
    
    while(t <= arrive[arrive.size()-1] && i < serivce.size()) { 
        if(arrive[i] <  serive_endtime) {
            t = arrive[i];
            if(queue.size() < 101) {
                if(queue.empty()) {
                    serive_endtime = t + serivce[i];
                }
                queue.push(serivce[i]);
            }
            i++;


        } else {
            t = serive_endtime;
            queue.pop();
            if(!queue.empty()) { 
                serive_endtime = t + queue.front();
            } else {
                serive_endtime = inf;
            }
        }
    }
    cout << queue.size() << endl;

    // 追加: 結果をファイルに出力
    ofstream output("result1.txt");
    output << "10万人の客が到達した直後にシステム内にいる客数: " << queue.size() << endl;
    output.close();
    
    return 0;
}
