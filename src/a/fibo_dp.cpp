#include <iostream>
using namespace std;

long long table[60];
 
long long fibo(long long n)
{
    if (n == 0 || n == 1)
        return 1;
 
    if (table[n] != 0) {
        return table[n];
    }

    table[n] = fibo(n-1) + fibo(n-2);

    cout << "fibo(" << n << ") " << "= fibo(" << n-1 << ") + fibo(" << n-2 << ") = " << table[n] << "\n";

    return table[n];
}

int fibo1(int n)
{
    for (int i=0; i<60; i++) table[i] = 0;
    table[0] = table[1] = 1;
 
    for (int i = 2; i <= n; i++)
        table[i] = table[i-1] + table[i-2];
 
    return table[n];
}

 
int main()
{
    for (int i=0; i<60; i++) table[i] = 0;
 
    cout << "----The n is: " ;
    for (int n; (cin >> n) && (n >= 0 && n <= 60); ) {
        cout << "fibo(" << n << ") = " << fibo(n) << "\n";
        cout << "----The n is: " ;
    }
    return 0;
}
 
