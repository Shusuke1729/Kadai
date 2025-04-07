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
    queue<pair<double, int>> queue; //左:サービス時間、何番目に来た人か（待ち行列）
    vector<double> wait_time;
    vector<pair<double, int>> P; // pair<一定時間, 人数>
    double t = 0.0, serive_endtime = inf;
    int i = 0; 
    int m;  //サービス窓口の数
    int not_entered = 0;


    cout << "サービス窓口の数を入力してください" << endl;
    cin >> m;

    //優先度付きキュー
    using di = pair<double, int>;
    priority_queue<di, vector<di>, greater<di>> window; 

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
        if(window.size() == 0) {
            t = arrive[i];
            window.push({t + serivce[i], i});
            i++;
            continue;
        }
        if(arrive[i] <  window.top().first) {
            P.push_back( {arrive[i] - t, queue.size() + window.size() }); // t秒間にシステム内に人がいたか
            t = arrive[i];
            if(window.size() <= m) {
                window.push({t + serivce[i], i});
            } else {
                if(queue.size() < 100) {
                    queue.push({serivce[i], i});
                } else {
                    not_entered++;
                }
            }
            i++;



        } else {
            P.push_back({window.top().first - t , queue.size() + window.size() }); // t秒間にシステム内に人がいたか
            t = window.top().first;
            wait_time.push_back(t - arrive[window.top().second]);
            window.pop();
            if(!queue.empty()) { 
                window.push({t + queue.front().first, queue.front().second});
                queue.pop();
            } 
            cout << "退却" << t << endl;

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
    ofstream output("result3.txt");
    output << "サービス窓口数" << m << "[個]" <<  endl;
    output << "-------------------------" << endl;
    output << "システムに入れず退去する割合" << not_entered_ratio << "[%]" <<  endl;
    output << "-------------------------" << endl;
    output << "平均1秒間にシステム内にいる人数" << average << "[人]" << endl;
    output << "-------------------------" << endl;
    output << "サービスを受け始めるまでに各客が待つ平均時間" << wait_time_average << "[s]" << endl; 
    output.close();
    
    return 0;
}
