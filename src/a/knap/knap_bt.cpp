#include "iostream"
using namespace std;


/**
 *
 * 【算法复杂度分析】：计算上界需要O(n)时间，
 *  在最坏情况下有O(pow(2,n))个右儿子结点需要计算上界，
 *  故解0-1背包问题的回溯算法所需的计算时间为O(n*pow(2,n))。 
 *
 */
class Knap
{
    friend int Knapsack(int p[], int w[], int c, int n);
    private:
    int Bound(int i);
    void Backtrack(int i);
    int c;   //背包容量
    int n;   //物品数
    int *w;   //物品重量数组
    int *p;   //物品价值数组
    int cw;   //当前重量
    int cp;   //当前价值
    int bestp;   //当前最优价值
};
void Knap::Backtrack(int i)
{
    if(i > n - 1)   //到达叶子节点
    {
        bestp = cp;
        return;
    }
    if(cw + w[i] <= c)   //进入左子树
    {
        cw += w[i];
        cp += p[i];
        Backtrack(i + 1);
        cw -= w[i];
        cp -= p[i];
    }
    if(Bound(i + 1) > bestp)   //进入右子树
    {
        Backtrack(i + 1);
    }
}
int Knap::Bound(int i)
{
    //计算节点所相应价值的上界
    int cleft = c - cw;   //剩余容量
    int b = cp;
    //以物品单位重量价值递减顺序装入物品
    while(i < n && w[i] <= cleft)
    {
        cleft -= w[i];
        b += p[i];
        i++;
    }
    if(i <= n)
    {
        b += p[i] * cleft / w[i];
    }
    return b;
}
class Object
{
    friend int Knapsack(int p[], int w[], int c, int n);
    public:
    int operator <= (Object a) const
    {
        return (d >= a.d);
    }
    private:
    int ID;
    float d;   //单位重量价值
};
int Knapsack(int p[], int w[], int c, int n)
{
    //为Knap::Backtrack初始化
    int W = 0;
    int P = 0;
    int i, j, max;
    Object *Q = new Object[n];
    Object qtmp;
    for(i = 0;i < n;i++)
    {
        Q[i].ID = i;
        Q[i].d = 1.0 * p[i] / w[i];
        W += w[i];
        P += p[i];
    }
    if(W <= c)
    {
        return P;   //装入所有物品
    }
    //依物品单位重量价值排序
    for(i = 0;i < n;i++)
    {
        max = i;
        for(j = i + 1;j < n;j++)
        {
            if(Q[max].d < Q[j].d)
            {
                max = j;
            }
        }
        qtmp = Q[i];
        Q[i] = Q[max];
        Q[max] = qtmp;
    }
    Knap K;
    K.p = new int[n];
    K.w = new int[n];
    for(i = 0;i < n;i++)
    {
        K.p[i] = p[Q[i].ID];
        K.w[i] = w[Q[i].ID];
    }
    K.cp = 0;
    K.cw = 0;
    K.c = c;
    K.n = n;
    K.bestp = 0;
    //回溯搜索
    K.Backtrack(0);
    delete[] Q;
    delete[] K.w;
    delete[] K.p;
    return K.bestp;
}
#define n 50   //物品的数量
//物体重量、收益、背包容量
int weight[n], profit[n], contain;
//从文件中读取背包信息
int read_infor()
{
    FILE *fp;
    int i;
    if ((fp=fopen("knapsack.txt","r"))==NULL)
    {  
        printf("The file is not found!");
        return 0;
    }
    //读取物体收益信息
    for(i = 0;i < n;i++)
    {
        fscanf(fp, "%d", &profit[i]);
    }
    //读取物体重量信息，计算物体的单位重量价值
    for(i = 0;i < n;i++)
    {
        fscanf(fp, "%d", &weight[i]);
    }
    //读取背包容量
    fscanf(fp, "%d", &contain);
    fclose(fp);
    return 1;
}

int main()
{
    int result;
    if(read_infor())
    {
        result = Knapsack(profit, weight, contain, n);
        printf("The maximum profit is: %d.", result);
    }
    scanf("%d", &result);
}


