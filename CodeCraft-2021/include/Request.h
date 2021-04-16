#ifndef REQUEST_H
#define REQUEST_H

#include<bits/stdc++.h>
#include "FastIO.h"
using namespace std;
using namespace FastIO;

class Request
{
public:
    int opCnt;
    string opType;
    queue<string> addVmName;
    queue<int> addVmId;
    queue<int> delVmId;

    void input()
    {
        opType.clear();
        while(!addVmName.empty()) addVmName.pop();
        while(!addVmId.empty()) addVmId.pop();
        while(!delVmId.empty()) delVmId.pop();

        read(opCnt);
        string s;
        for(int i=0,t;i<opCnt;i++){
            reads(s);
            opType.push_back(s[0]);
            if(s[0]=='a'){
                reads(s),read(t);
                addVmName.push(s);
                addVmId.push(t);
            }else{
                read(t);
                delVmId.push(t);
            }
        }
    }

};

#endif // REQUEST_H
