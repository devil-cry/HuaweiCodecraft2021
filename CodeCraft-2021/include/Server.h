#ifndef SERVER_H
#define SERVER_H

#include <bits/stdc++.h>
#include "FastIO.h"
using namespace std;
using namespace FastIO;
class Server
{
public:
    string name;
    int core,mem,cost,daily;
    void input()
    {
        reads(name);read(core);read(mem);read(cost);read(daily);
    }
    bool operator<(const Server& rhs)const{
        return daily<rhs.daily;
    }

};

#endif // SERVER_H
