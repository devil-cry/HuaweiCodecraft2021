#ifndef FASTIO_H
#define FASTIO_H

#include <bits/stdc++.h>
using namespace std;
namespace FastIO
{
    template<typename T>inline void read(T &x)
    {
        x=0;bool f=0;char ch=getchar();
        while(ch<'0'||ch>'9')(ch=='-')&&(f=1),ch=getchar();
        while(ch>='0'&&ch<='9')x=(x<<1)+(x<<3)+(ch^48),ch=getchar();f&&(x=-x);
    }
    template<typename T>inline void out(T x)
    {
        (x<0)&&(putchar('-'),x=-x);
        if(x>9)out(x/10);putchar(x%10+'0');
    }

    // Start Here
    inline void reads(string &x)
    {
        x="";char ch=getchar();
        while(ch=='('||ch==')'||ch==','||ch==' '||ch=='\n') ch=getchar();
        while(ch!='('&&ch!=')'&&ch!=','&&ch!=' '&&ch!='\n') x.push_back(ch),ch=getchar();
    }

    inline void outs(const string& x)
    {
        for(auto& ch:x) putchar(ch);
    }

    #define fi first
    #define se second
    #define pb push_back
    #define bug(x) cout<<"-----"<<x<<endl
    #define all(x) (x).begin(),(x).end()
    typedef long long ll;
    typedef pair<int,int> pii;
    const int maxSerCnt=100;
    const int maxOperator=1e5+7;
    ll mod=998244353;
//    ll mod=1e9+7;

};

#endif // FASTIO_H
