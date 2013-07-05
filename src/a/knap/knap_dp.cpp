#include <fstream>
#include <iostream>
#include <vector> 

using namespace std;

vector<int> weight, profit;

void readinfo(const char* filename, int &c, int &n) {
    ifstream ifs(filename);
    ifs >> c >> n;
    for(int i=0; i<n; i++) {
        int t1,t2;
        ifs >> t1 >> t2;
        weight.push_back(t1);
        profit.push_back(t2);
    }
}

int knapsack(int m, int n, vector<int> w, vector<int> p) {
    int c[10][100];
    for(int i=0;i<10;i++) for(int j=0;j<100;j++) c[i][j]=0;

    for(int i=1; i<n+1; i++)
        for(int j=1; j<m+1; j++) {
            if(w[i]<=j) {
                if( p[i] + c[i-1][j-w[i]] > c[i-1][j])
                    c[i][j] = p[i] + c[i-1][j - w[i]];
                else
                    c[i][j]=c[i-1][j];
            }
            else c[i][j]=c[i-1][j];
        }
    return(c[n][m]);
}

int main() {

    int c, n;
    readinfo("knapsack.txt", c, n);

    cout << knapsack(c, n, weight, profit) << endl;

    return 0;
}

