#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Tree.cpp"

using namespace std;

int gcd(int a, int b)
{
    if (a > b) {
        if (b == 0) {
            return a;
        }
        return gcd(a % b, b);
    }
    else {
        if (a == 0) {
            return b;
        }
        return gcd(a, b % a);
    }
}

string trim(const string& s)
{
    if (s.length() == 0) {
        return s;
    }

    size_t beg = s.find_first_not_of(" \a\b\f\n\r\t\v");
    size_t end = s.find_last_not_of(" \a\b\f\n\r\t\v");
    if (beg == string::npos) { // No non-spaces
        return "";
    }

    return string(s, beg, end - beg + 1);
}

double cal_exp(int k, int height, vector<long long>& vec)
{
    long long num_leaf = k;
    for (int i = 1; i < height; i++) {
        num_leaf *= k;
    }

    long long vec_sum = 0;
    for (long long& n : vec) {
        vec_sum += n;
    }

    vec.push_back(num_leaf - vec_sum);

    long long numerator = 0;
    for (long long& n : vec) {
        long long m = n;
        int d = height;
        long long p = 1;
        while (m != 0) {
            numerator += d * (m % k) * p;
            m /= k;
            d -= 1;
            p *= k;
        }
    }

    vec.pop_back();

    return (double)numerator / (double)vec_sum;
}

int main()
{
    string k_in;
    int k;

    cout << "시행 도구에서 k가지 경우가 동일한 확률로 나오는 경우, 해당 k 값을 입력해주세요." << endl;
    cout << "(Ex. 동전 2 / 주사위 6)" << endl;

    bool k_pass = false;

    while (!k_pass) {
        cout << "k 값 입력 : ";
        getline(cin, k_in);
        k_in = trim(k_in);

        k_pass = true;
        for (char c : k_in) {
            if (!isdigit(c)) {
                k_pass = false;
                cout << "2 이상의 정수를 1개만 소수점 없이 입력해주세요." << endl;
                break;
            }
        }

        if (k_pass) {
            try {
                k = stoi(k_in);
            }
            catch (const exception& expn) {
                k = -1;
                k_pass = false;
                cout << "int 범위 내의 값을 입력해주세요." << endl;
            }
            if (k < 2) {
                k_pass = false;
                cout << "2 이상의 정수를 1개만 소수점 없이 입력해주세요." << endl;
            }
        }
    }

    string vec_in;
    vector<int> vec;

    cout << "각각의 사건이 일어날 확률들의 자연수비를 구성하는 숫자를 차례대로 입력해주세요." << endl;
    cout << "(Ex. 당첨 30% 꽝 70% -> 3 7 / A 5%, B 20%, C 75% -> 1 4 15)" << endl;

    bool vec_pass = false;

    while (!vec_pass) {
        vec.clear();
        cout << "자연수비 구성 숫자 입력 : ";
        getline(cin, vec_in);
        vec_in = trim(vec_in);
        
        stringstream vec_in_stream(vec_in);
        string s;
        int p;

        while (!vec_in_stream.eof()) {
            vec_in_stream >> s;

            vec_pass = true;
            for (char c : s) {
                if (!isdigit(c)) {
                    vec_pass = false;
                    cout << "자연수비 구성 숫자는 자연수만 소수점 없이 입력해주세요." << endl;
                    break;
                }
            }

            if (vec_pass) {
                try {
                    p = stoi(s);
                }
                catch (const exception& expn) {
                    p = -1;
                    vec_pass = false;
                    cout << "int 범위 내의 값을 입력해주세요." << endl;
                }
                if (p < 1) {
                    vec_pass = false;
                    cout << "자연수비 구성 숫자는 자연수만 소수점 없이 입력해주세요." << endl;
                }
            }

            if (!vec_pass) {
                break;
            }
            else {
                vec.push_back(p);
            }
        }

        if (vec.size() == 1) {
            vec_pass = false;
            cout << "자연수비 구성 숫자는 2개 이상 입력해주세요." << endl;
        }
    }

    int g = gcd(vec[0], vec[1]);
    for (int i = 2; i < vec.size(); i++) {
        g = gcd(g, vec[i]);
        if (g == 1) {
            break;
        }
    }

    if (g > 1) {
        cout << "자연수비를 구성하는 숫자들의 최대공약수가 " << g << "입니다." << endl;
        cout << "입력된 자연수비 구성 숫자들을 최대공약수로 나누어 다음과 같이 바꿉니다." << endl;
        for (int& n : vec) {
            n /= g;
            cout << n << " ";
        }
        cout << endl;
    }

    string edl_in;
    int edl;

    cout << "extra depth limit을 입력해주세요." << endl;
    cout << "(시행 규칙의 높이가 최솟값보다 extra depth limit 만큼 큰 경우까지 탐색합니다.)" << endl;

    bool edl_pass = false;

    while (!edl_pass) {
        cout << "extra depth limit 값 입력 : ";
        getline(cin, edl_in);
        edl_in = trim(edl_in);

        edl_pass = true;
        for (char c : edl_in) {
            if (!isdigit(c)) {
                edl_pass = false;
                cout << "0 이상의 정수를 1개만 소수점 없이 입력해주세요." << endl;
                break;
            }
        }

        if (edl_pass) {
            try {
                edl = stoi(edl_in);
            }
            catch (const exception& expn) {
                edl = -1;
                edl_pass = false;
                cout << "int 범위 내의 값을 입력해주세요." << endl;
            }
            if (k < 0) {
                edl_pass = false;
                cout << "0 이상의 정수를 1개만 소수점 없이 입력해주세요." << endl;
            }
        }
    }

    long long vec_sum = 0;
    for (int& n : vec) {
        vec_sum += n;
    }
    
    long long num_leaf = k;
    int min_h = 1;
    while (num_leaf < vec_sum) {
        num_leaf *= k;
        min_h++;
    }

    int c = 1;
    int h = min_h;
    pair<double, int> min_exp_and_c = make_pair(DBL_MAX, 0);
    vector<pair<double, int>> min_exp_and_c_vec;
    vector<long long> v;
    while (h <= min_h + edl) {
        if (c % k != 0) {
            v.clear();
            for (int& n : vec) {
                v.push_back(c * (long long)n);
            }

            double exp = cal_exp(k, h, v);
            if (exp < min_exp_and_c.first) {
                min_exp_and_c.first = exp;
                min_exp_and_c.second = c;
            }
        }

        c++;
        if (c * vec_sum > num_leaf) {
            h++;
            num_leaf *= k;
            min_exp_and_c_vec.push_back(min_exp_and_c);
            min_exp_and_c = make_pair(DBL_MAX, 0);
        }
    }

    // debug
    double min_exp = DBL_MAX;
    int idx = 0;

    cout << fixed;
    cout.precision(11);
    for (auto& p : min_exp_and_c_vec) {
        cout << "# height = " << min_h + idx << "\t// "; 
        if (p.second == 0) {
            cout << "no case" << endl;
        }
        else {
            cout << "min E[X] = " << p.first << " // c = " << p.second << endl;
        }
        if (p.first < min_exp) {
            min_exp = p.first;
            cout << "출력합니다." << endl;
        }
        else {
            cout << "생략합니다.";
            if (p.second == 0) {
                cout << " 해당 height인 case가 없습니다.";
            }
            cout << endl;
        }
        idx++;
    }
    ////

    ofstream ofs;
    ofs.open("./result.txt");
    ofs << "## input value ##" << endl;
    ofs << "## k = " << k << " // vector = { ";
    long long sum = 0;
    for (int& n : vec) {
        ofs << n << " ";
        sum += n;
    }
    ofs << "} // extra depth limit = " << edl << " ##" << endl;

    ofs << fixed;
    ofs.precision(3);
    ofs << endl;
    ofs << "## probabilities ##" << endl;
    for (int i = 0; i < vec.size(); i++) {
        ofs << "E_" << i + 1 << "  \t: " << ( (double)vec[i] / (double)sum ) * 100 << "%" << endl;
    }
    ofs << endl;

    ofs.precision(11);

    min_exp = DBL_MAX;
    idx = 0;

    for (auto& p : min_exp_and_c_vec) {
        ofs << "=============================================" << endl;
        if (p.second == 0) {
            ofs << "# height = " << min_h + idx << "\t// no case #" << endl;
        }
        else {
            ofs << "# height = " << min_h + idx << "\t// min E[X] = " << p.first << " #" << endl;
        }
        if (p.first < min_exp) {
            ofs << "---------------------------------------------" << endl;
            min_exp = p.first;

            vector<long long> tree_vec;
            for (int& n : vec) {
                tree_vec.push_back((long long)p.second* n);
            }

            TrialRule t = TrialRule(k, min_h + idx, tree_vec, ofs);
            t.Print(ofs);
        }
        ofs << "=============================================" << endl;
        ofs << endl;
        idx++;
    }

    ofs.close();

    return 0;
}