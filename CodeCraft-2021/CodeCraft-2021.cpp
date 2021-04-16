#include <bits/stdc++.h>
#include "VM.h"
#include "Server.h"
#include "FastIO.h"
#include "CurrentVM.h"
#include "CurrentServer.h"
#include "Request.h"
#include "Answer.h"
using namespace FastIO;
using namespace std;


int N,M,T,K;
vector<Server> serList;
vector<VM> vmList;

int vmIdCnt,sumVM;
int serIdCnt;
map<int,int> vmOut2Id;
vector<int> vmId2out(maxOperator);
vector<int> vmLidOfvmId(maxOperator);
map<string,int> vmName2Id;

vector<CurrentVM> curVM;
vector<CurrentServer> curSer;

Answer ans;


void init()
{
    read(N);
    serList.resize(N);
    for(int i=0;i<N;i++)
        serList[i].input();
    sort(all(serList));
    read(M);
    vmList.resize(M);
    string s;
    for(int i=0;i<M;i++){
        reads(s);
        vmName2Id[s]=i;
        vmList[i].input();
    }
    read(T),read(K);
    ans.init();
}


void solve()
{
    Request reqArray[K];
    int vmId,tempVmId,purchaseSerId;
    string vmName;
    VM tempVM;
    Request req;
    CurrentVM tempCurVM(-1,-1,"");

    for(int i=0;i<K-1;i++){
        reqArray[i].input();
    }
    int today=K-1;    // scroll array
    while(today<T+K-1){
        if(today<T) reqArray[today%K].input();
        req=reqArray[(today+1)%K];

        Migration::migration(curVM,curSer,vmList,vmLidOfvmId,sumVM*3/100,ans.mig);

        for(int i=0;i<req.opCnt;i++){
            if(req.opType[i]=='a'){  // add a new VM
                vmName=req.addVmName.front();req.addVmName.pop();
                vmId=req.addVmId.front();req.addVmId.pop();
                // transform vmId(int32) to my vmIdCnt(id<maxOperator)
                vmId2out[vmIdCnt] = vmId;
                vmOut2Id[vmId] = vmIdCnt;
                // copy a VM model from VM list
                vmLidOfvmId[vmIdCnt]=vmName2Id[vmName];
                tempVM = vmList[vmLidOfvmId[vmIdCnt]];
                // judge whether the VM can be placed in existed Server
                tempCurVM = CurrentServer::findSuitableSer(curSer,tempVM);
                tempCurVM.name=vmName;
                if(tempCurVM.placeSerId==-1){  //purchase a new Server and place VM in it
                    purchaseSerId=CurrentServer::purchaseSuitableSer(serList,tempVM);
                    curSer.push_back(CurrentServer(serList[purchaseSerId],serIdCnt));
                    ans.buy.push_back(purchaseSerId);
                    ans.buyToday[purchaseSerId]++;
                    ans.buyType.insert(purchaseSerId);
                    ans.q[purchaseSerId].push(serIdCnt);
                    tempCurVM.placeSerId=serIdCnt++;
                    tempCurVM.placeNode=tempVM.isPair?2:0;
                }
                curVM.push_back(tempCurVM);
                curSer[tempCurVM.placeSerId].addVM(tempVM,tempCurVM.placeNode,vmIdCnt);
                ans.place.push_back(pii(tempCurVM.placeSerId,tempCurVM.placeNode));
                sumVM++;
                vmIdCnt++;
            }else{  // delete VM
                vmId=req.delVmId.front();req.delVmId.pop();
                tempVmId=vmOut2Id[vmId];
                tempCurVM=curVM[tempVmId];
                tempVM=vmList[vmName2Id[tempCurVM.name]];
                curSer[tempCurVM.placeSerId].delVM(tempVM,tempCurVM.placeNode,tempVmId);
                sumVM--;
            }
        }
        ans.adjustServerId();
        ans.printPurchaseInfo(serList);
        ans.printMigrationInfo(vmId2out);
        ans.printPlaceInfo();
        if(today<T) fflush(stdout);
        today++;
    }
}

int main()
{
    init();
    solve();

    return 0;
}

/**
2
(NV603, 92, 324, 53800, 500)
(NV604, 128, 512, 87800, 800)
2
(c3.large.4, 2, 8, 0)
(c3.8xlarge.2, 32, 64, 1)
3 2
2
(add, c3.large.4, 5)
(add, c3.large.4, 0)
2
(del, 0)
(add, c3.8xlarge.2, 1)
3
(add, c3.large.4, 2)
(del, 1)
(del, 2)

rm devilcry_v1.1.zip
rm -rf CodeCraft-2021/include/
cp -r ../../../v1.1/include/ CodeCraft-2021/include
bash CodeCraft_zip.sh
mv CodeCraft-2021.zip devilcry_v1.1.zip

*/
