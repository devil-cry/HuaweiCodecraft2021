#ifndef ANSWER_H
#define ANSWER_H

#include<bits/stdc++.h>
#include "VM.h"
#include "Server.h"
#include "CurrentVM.h"
#include "Migration.h"
#include "CurrentServer.h"
#include "FastIO.h"
using namespace std;
using namespace FastIO;

class Answer
{
public:
    vector<pii> place;
    vector<int> buy,buyToday,buyLast;
    set<int> buyType;
    vector<Migration> mig;
    queue<int> q[maxSerCnt];
    vector<int> serId;
    int tempSerCnt;

    void init()
    {
        for(int i=0;i<maxSerCnt;i++) buyToday.push_back(0),buyLast.push_back(0);
        serId.resize(maxOperator);
        tempSerCnt=0;
    }

    void adjustServerId()   // transform my SerId order to output Id order
    {
        for(auto &x:buy){
            while(!q[x].empty()){
                serId[q[x].front()]=tempSerCnt++;
                q[x].pop();
            }
        }
    }

    void printPurchaseInfo(const vector<Server>& serList)
    {
        printf("(purchase, %d)\n",buyType.size());
        for(auto &x:buy)if(buyType.count(x))
        {
            printf("(");outs(serList[x].name);
            printf(", %d)\n",buyToday[x]-buyLast[x]);
            buyLast[x]=buyToday[x];
            buyType.erase(x);
        }
        buy.clear();
    }

    void printMigrationInfo(const vector<int>& vmId2out)
    {
        printf("(migration, %d)\n",mig.size());
        for(Migration &x:mig){
            if(x.newPlaceNode==2)   //double node
                printf("(%d, %d)\n",vmId2out[x.vmId],serId[x.newPlaceSerId]);
            else    // single node
                printf("(%d, %d, %c)\n",vmId2out[x.vmId],serId[x.newPlaceSerId],'A'+x.newPlaceNode);
        }
        mig.clear();
    }

    void printPlaceInfo()
    {
        for(pii &x:place){
            if(x.second==2)
                printf("(%d)\n",serId[x.first]);
            else
                printf("(%d, %c)\n",serId[x.first],'A'+x.second);
        }
        place.clear();
    }

};

#endif // ANSWER_H
