#include <iostream>
#include <math.h>

using namespace std;

/*检查可不可以放置一个新的皇后*/
bool place(int k, int *X)
{
    int i;
    i=1;
    while(i<k)
    {
        if((X[i]==X[k])||(abs(X[i]-X[k])==abs(i-k)))
            return false;
        i++;
    }
    return true;
}
/*求解问题的所有解*/
void Nqueens(int n,int *X)
{
    int k;

    X[1]=0;k=1;
    while(k>0) {
        X[k]=X[k]+1;

        while((X[k]<=n)&&(!place(k, X)))
            X[k]=X[k]+1;

        if(X[k]<=n)
            if(k==n) {
                for(int i=1;i<=n;i++)
                    cout<<X[i]<<" ";
                cout<<"\n";
            }
            else {
                k=k+1;
                X[k]=0;
            }
        else k=k-1;
    }
}

void getChar(char ichar, int &n){
    if(ichar == 'q') 
        exit(0);
    else n = (int)ichar-48;

    cout << "input value :" << n << endl;
}

int main()
{
    cout<<"|--------------Nqueens----------------|"<<endl;
    cout<<"|-------------------------------------|"<<endl<<endl;
    int n = 0;
    int *X;
    int i;
    char ichar;

    cout<<"queen num:";
    cin>>ichar;

    getChar(ichar, n);

    while(n<50)
    {
        X=new int[n];
        cout<<"solutions:"<<endl;
        Nqueens(n,X);
        cout<<"Press<q> to exit  or  input queen num:"<<endl;
        cin>>ichar;
        getChar(ichar, n);
    }
}

