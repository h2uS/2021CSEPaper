#include <iostream>
#include <string>
#include <sstream>
#include <vector>

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

pair<int, double> CalExpectedVal(int k, vector<int>& vec)
{
    if (k <= 1) {
        cerr << "error : k <= 1" << endl;
        return make_pair(0, -1.0);
    }

    int sum = 0;
    //int caseNum = 1;
    for (int& n : vec) {
        //cout << "# of case " << caseNum << " : " << n << endl;
        //caseNum++;
        if (n < 0) {
            cerr << "error : negative # of case" << endl;
            return make_pair(0, -1.0);
        }
        sum += n;
    }

    if (sum <= 1) {
        cerr << "error : sum <= 1" << endl;
        return make_pair(0, -1.0);
    }

    int depth = 1;
    int numOfLeaves = k;
    while (numOfLeaves < sum) {
        numOfLeaves *= k;
        depth++;
    }

    vec.push_back(numOfLeaves - sum);

    int numerator = 0;
    for (int& n : vec) {
        int q = n;
        int d = depth;
        int p = 1;
        while (q != 0) {
            numerator += d * (q % k) * p;
            q /= k;
            d -= 1;
            p *= k;
        }
    }

    vec.pop_back();

    return make_pair(depth, (double)numerator / (double)sum);
}

int main()
{
    const int k = 6;
    vector<int> vec = { 5, 20, 75 };
    const int limitLevel = 2;

    //for (int& n : vec) {
    //    cout << n << " ";
    //}
    //cout << endl;

    int g = gcd(vec[0], vec[1]);
    for (int i = 2; i < vec.size(); i++) {
        g = gcd(g, vec[i]);
        if (g == 1) {
            break;
        }
    }

    for (int& n : vec) {
        n /= g;
    }

    cout << "k : " << k << " / ";
    cout << "vector : ";
    for (int& n : vec) {
        cout << n << " ";
    }
    cout << " / extra depth limit : " << limitLevel << endl;

    int sum = 0;
    for (int& n : vec) {
        sum += n;
    }

    int numOfLeaves = k;
    while (numOfLeaves < sum) {
        numOfLeaves *= k;
    }

    for (int i = 0; i < limitLevel; i++) {
        numOfLeaves *= k;
    }

    double minExp = DBL_MAX;
    int minExpIndex = 0;
    int lastIndex = 0;
    vector<pair<int, int>> depthAndMaxIndex;
    int d = 0;

    for (int i = 1; i * sum <= numOfLeaves; i++) {
        if (i % k == 0) {
            continue;
        }

        vector<int> v;
        for (int& n : vec) {
            v.push_back(i * n);
        }
        pair<int, double> depthAndExp = CalExpectedVal(k, v);
        if (depthAndExp.first > d) {
            if (d != 0) {
                depthAndMaxIndex.push_back(make_pair(d, lastIndex));
            }
            d = depthAndExp.first;
            cout << "######## height = " << d << " ########" << endl;
        }
        cout << "c = " << i << " ) E[X] = " << depthAndExp.second << endl;
        if (depthAndExp.second < minExp) {
            minExp = depthAndExp.second;
            minExpIndex = i;
        }
        lastIndex = i;
    }
    depthAndMaxIndex.push_back(make_pair(d, lastIndex));

    cout << endl;

    cout << "min E[X] = " << minExp << endl;
    cout << "min E[X] when c = " << minExpIndex << endl;

    for (auto& p : depthAndMaxIndex) {
        cout << "height = " << p.first << "\t| " << "~ c = " << p.second << endl;
    }

    return 0;
}