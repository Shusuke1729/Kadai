#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#define rep(i, n) for(int i = 0 ; i < (n); i++)

using namespace std;

int main(void) {
    vector<double> arrive;
    vector<double> service;
    queue<pair<double, int>> queue; //左:サービス時間、何番目に来た人か（待ち行列）
    vector<double> wait_time;
    vector<pair<double, int>> P; // pair<一定時間, 人数>
    double t = 0.0;
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
        service.push_back(num);
    }
    file.close();

    
    while( i < arrive.size()) { 
        if(window.size() == 0) {
            P.push_back( {arrive[i]-t, 0} ); // t秒間にシステム内に人がいたか
            t = arrive[i];
            window.push({t + service[i], i});
            i++;
            continue;
        }
        if(arrive[i] <  window.top().first) {
            
            P.push_back( {arrive[i] - t, queue.size() + window.size() }); // t秒間にシステム内に人がいたか
            t = arrive[i];
            if(window.size() < m) {
                window.push({t + service[i], i});
            } else {
                if(queue.size() < 100) {
                    queue.push({service[i], i});
                } else {
                    not_entered++;
                }
            }
            i++;
        } else {
            auto[service_endtime, num1] = window.top(); // 退出した人のサービス終了時間,何番目に来た人か
            P.push_back({service_endtime - t , queue.size() + window.size() }); // t秒間にシステム内に人がいたか

            t = service_endtime;
            wait_time.push_back(t - arrive[num1] -service[num1]); // サービスを受け始めるまでに各客が待つ時間
            window.pop();
            if(!queue.empty()) { 
                auto[service_time, num2] = queue.front(); // 待ち行列の先頭のサービスに要する時間,何番目に来た人か
                window.push({t + service_time, num2});
                queue.pop();
            } 
        }
    }
    //システム内にいる人数
    int num = queue.size()+ window.size(); 

    while(!window.empty()) {
        auto[service_endtime, num1] = window.top(); // 退出した人のサービス終了時間,何番目に来た人か
        t = service_endtime;
        wait_time.push_back(t - arrive[num1] -service[num1]); // サービスを受け始めるまでに各客が待つ時間
        window.pop();
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
    double wait_time_sum = 0.0;
    rep(i, wait_time.size()) {
        wait_time_sum += wait_time[i];
    }
    double wait_time_average = (double)wait_time_sum / (double)wait_time.size();

    // 追加: 結果をファイルに出力
    ofstream output("result3.txt");
    output << "-------------------------" << endl;
    output << "サービス窓口数" << m << "[個]" <<  endl;
    output << "-------------------------" << endl;
    output << "システム内にいる人数" << num << "[人]" << endl;
    output << "-------------------------" << endl;
    output << "システムに入れず退去する割合" << not_entered_ratio << "[%]" <<  endl;
    output << "-------------------------" << endl;
    output << "平均1秒間にシステム内にいる人数" << average << "[人]" << endl;
    output << "-------------------------" << endl;
    output << "サービスを受け始めるまでに各客が待つ平均時間" << wait_time_average << "[s]" << endl; 
    output.close();
    
    return 0;
}
