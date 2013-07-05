#include <string.h>
#include <iostream>

using namespace std;

void combine3(int m, int r)
{
    int *a = new int[r];
    memset(a, 0, r * sizeof(int));

    a[0] = 1;
    int i = 0;

    while (1) {
        // 达到规模，输出结果
        if (a[i] - i <= m - r + 1) {
            if (i >= r - 1) {
                for (int t = 0; t < r; t++) {
                    cout << *(a + t) << "  ";
                }
                cout << endl;

                a[i]++;
                continue;
            }
        }

        // 满足条件，因而扩大规模
        if (a[i] < m && i < r) {
            i++;
        }    
        else {
            if (i == 0)  {// 退出
                break;
            }
            else if (i > 0) { 
                a[i] = a[i - 1] + 1;
                a[--i]++;    // 回溯
            }   
        }

        // 组合数字肯定有序
        if (a[i] <= a[i - 1] && i > 0) {
            a[i] = a[i - 1] + 1;
        }
    }

    delete a;
}

int main()
{
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int b[8] = {0};

    int m = 6;
    int r = 4;

    cout << "m=" << m << ",  r=" << r << endl << endl;
    combine3(m, r);

    return 0;
}

