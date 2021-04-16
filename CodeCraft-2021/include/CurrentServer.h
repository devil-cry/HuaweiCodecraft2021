#ifndef CURRENTSERVER_H
#define CURRENTSERVER_H

#include<bits/stdc++.h>
#include "VM.h"
#include "Server.h"
#include "CurrentVM.h"
#include "FastIO.h"
using namespace FastIO;
using namespace std;

class CurrentServer
{
public:
    int core[2],mem[2],daily,id,mxc,mxm;
    vector<int> node[3];


    CurrentServer(const Server& ser,int serId)
    {
        daily=ser.daily;
        id=serId;
        core[0]=core[1]=ser.core>>1;
        mem[0]=mem[1]=ser.mem>>1;
        mxc=ser.core>>1;
        mxm=ser.mem>>1;
        node[0].clear();
        node[1].clear();
        node[2].clear();
    }

//    bool isEmpty()
//    {
//        return node[0].empty()&&node[1].empty()&&node[2].empty();
//    }
//
//    bool operator<(const CurrentServer& rhs)const{
//        if(daily==rhs.daily) return id<rhs.id;
//        return daily>rhs.daily;
//    }

//    int getVal()
//    {
//        return abs(core[0]*mxCore-core[1]*mxCore)+abs(mem[0]*mxMem-mem[1]*mxMem);
//    }

    static CurrentVM findSuitableSer(const vector<CurrentServer>& curSer,const VM& vm)
    {
        CurrentVM ret(-1,-1,"");
        int i=0,val=1e9;
        if(vm.isPair){
            for(const CurrentServer& x:curSer){
                if(x.core[0]>=vm.core/2&&x.core[1]>=vm.core/2&&x.mem[0]>=vm.mem/2&&x.mem[1]>=vm.mem/2){
                    if(val>x.core[0]+x.core[1]+x.mem[0]+x.mem[1])
                    //if(val>x.daily)
                    {
                        //val=x.daily;
                        val=x.core[0]+x.core[1]+x.mem[0]+x.mem[1];
                        ret.placeSerId=x.id;
                        ret.placeNode=2;
                    }
                }
                i++;
            }
        }else{
            for(const CurrentServer& x:curSer){
                if(x.core[0]+x.mem[0]<=x.core[1]+x.mem[1]){
                    //if(x.core[0]>=vm.core&&x.mem[0]>=vm.mem&&val>x.daily){
                    if(x.core[0]>=vm.core&&x.mem[0]>=vm.mem&&val>x.core[0]+x.mem[0]){
                        val=x.core[0]+x.mem[0];
                        //val=x.daily;
                        ret.placeSerId=x.id;
                        ret.placeNode=0;
                    }else if(x.core[1]>=vm.core&&x.mem[1]>=vm.mem&&val>x.core[1]+x.mem[1]){
                        val=x.core[1]+x.mem[1];
                        //val=x.daily;
                        ret.placeSerId=x.id;
                        ret.placeNode=1;
                    }
                }else{
                    if(x.core[1]>=vm.core&&x.mem[1]>=vm.mem&&val>x.core[1]+x.mem[1]){
                        val=x.core[1]+x.mem[1];
                        //val=x.daily;
                        ret.placeSerId=x.id;
                        ret.placeNode=1;
                    }else if(x.core[0]>=vm.core&&x.mem[0]>=vm.mem&&val>x.core[0]+x.mem[0]){
                        val=x.core[0]+x.mem[0];
                        //val=x.daily;
                        ret.placeSerId=x.id;
                        ret.placeNode=0;
                    }
                }
                i++;
            }
        }
        return ret;
    }

    static int purchaseSuitableSer(const vector<Server>& serList,const VM& vm)
    {
        int ret=0,i=0;
        if(vm.isPair){
            for(const Server &it:serList){
                if(it.core>=vm.core&&it.mem>=vm.mem){
                    return i;
                }
                i++;
            }
        }else{
            for(const Server &it:serList){

                if(it.core/2>=vm.core&&it.mem/2>=vm.mem){
                    return i;
                }
                i++;
            }
        }
        return ret;
    }

    void addVM(const VM& vm,int placeNode,int vmId)
    {
        if(vm.isPair){
            core[0]-=vm.core>>1;
            core[1]-=vm.core>>1;
            mem[0]-=vm.mem>>1;
            mem[1]-=vm.mem>>1;
        }else{
            core[placeNode]-=vm.core;
            mem[placeNode]-=vm.mem;
        }
        node[placeNode].push_back(vmId);
    }

    void delVM(const VM& vm,int placeNode,int vmId)
    {
        if(vm.isPair){
            core[0]+=vm.core>>1;
            core[1]+=vm.core>>1;
            mem[0]+=vm.mem>>1;
            mem[1]+=vm.mem>>1;
        }else{
            core[placeNode]+=vm.core;
            mem[placeNode]+=vm.mem;
        }
        for(vector<int>::iterator it=node[placeNode].begin();it!=node[placeNode].end();){
            if((*it)==vmId){
                it=node[placeNode].erase(it);
                break;
            }else it++;
        }
    }

};

#endif // CURRENTSERVER_H
