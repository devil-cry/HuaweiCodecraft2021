#ifndef CURRENTVM_H
#define CURRENTVM_H

#include<bits/stdc++.h>
#include "VM.h"
using namespace std;
using namespace FastIO;


class CurrentVM
{
public:
    int placeSerId,placeNode;
    string name;

    CurrentVM(int pid,int pnode,string tname)
    {
        placeSerId=pid;
        placeNode=pnode;
        name=tname;
    }

};

#endif // CURRENTVM_H
