#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <queue>

#define rep(i, n) for(int i = 0 ; i < (n); i++)
#define rep2(i, k, n) for(int i = (k); i < (n); i++)
using ll = long long;
using namespace std;
#define inf 1e18

int main(void) {
    vector<double> arrive;
    vector<double> service;
    queue<pair<double, double>> queue; //左側がサービス時間、右側が到着時間
    vector<double> wait_time;
    vector<pair<double, int>> P; // pair<一定時間, 人数>
    double t = 0.0, service_endtime = inf;
    int i = 0;
    int not_entered = 0;


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
        service.push_back(num);
    }
    file.close();

    
    while( i < arrive.size()) { 
        if(arrive[i] <  service_endtime) {
            P.push_back({arrive[i]-t, queue.size()}); // t秒間にシステム内に人がいたか
            t = arrive[i];
            if(queue.size() < 101) {
                if(queue.empty()) {
                    service_endtime = t + service[i];
                }
                queue.push({service[i], arrive[i]});
            } else {
                not_entered++;
            }
            i++;


        } else {
            P.push_back({service_endtime-t, queue.size()}); // t秒間にシステム内に人がいたか
            t = service_endtime;
            auto[service_time, arrive_time] = queue.front();
            wait_time.push_back(t - service_time - arrive_time);
            queue.pop();
            if(!queue.empty()) { 
                service_endtime = t + queue.front().first;
            } else {
                service_endtime = inf;
            }
            
        }

    }

    //システムに入れず退去する割合
    double not_entered_ratio = (double)not_entered / (double)arrive.size() *100.0;

    //平均1秒間にシステム内にいる人数
    double sum = 0;
    rep(i, P.size()) {
        sum += P[i].first * P[i].second;
    }
    double average = sum / (double)arrive[arrive.size()-1];

    //サービスを受け始めるまでに各客が待つ平均時間
    ll wait_time_sum = 0;
    rep(i, wait_time.size()) {
        wait_time_sum += (ll)wait_time[i];
    }
    double wait_time_average = (double)wait_time_sum / (double)wait_time.size();

    // 追加: 結果をファイルに出力
    ofstream output("result2.txt");
    output << "システムに入れず退去する割合" << not_entered_ratio << "[%]" <<  endl;
    output << "-------------------------" << endl;
    output << "平均1秒間にシステム内にいる人数" << average << "[人]" << endl;
    output << "-------------------------" << endl;
    output << "サービスを受け始めるまでに各客が待つ平均時間" << wait_time_average << "[s]" << endl; 
    output.close();
    
    return 0;
}
