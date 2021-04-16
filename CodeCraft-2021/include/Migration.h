#ifndef MIGRATION_H
#define MIGRATION_H

#include<bits/stdc++.h>
#include "VM.h"
#include "CurrentVM.h"
#include "CurrentServer.h"
using namespace std;

class Migration
{
public:
    int vmId,newPlaceSerId,newPlaceNode;
    Migration(int vmid,int newPid,int newPnode)
    {
        vmId=vmid;
        newPlaceSerId=newPid;
        newPlaceNode=newPnode;
    }

    static bool sortByDaily(const CurrentServer& l,const CurrentServer& r)
    {
        if(l.daily==r.daily) return l.id<r.id;
        return l.daily>r.daily;
    }

    static bool sortByCoreAndMem(const CurrentServer& l,const CurrentServer& r)
    {
        return (l.core[0]+l.mem[0]+l.core[1]+l.mem[1])>(r.core[0]+r.mem[0]+r.core[1]+r.mem[1]);
    }

    static bool sortBack(const CurrentServer& l,const CurrentServer& r)
    {
        return l.id<r.id;
    }

    static void fixCurSer(CurrentServer& cs,const VM& vm,int val)
    {
        cs.core[0]+=val*vm.core/2;cs.core[1]+=val*vm.core/2;
        cs.mem[0]+=val*vm.mem/2;cs.mem[1]+=val*vm.mem/2;
    }

    static void fixCurSer(CurrentServer& cs,const VM& vm,int pNode,int val)
    {
        cs.core[pNode]+=val*vm.core;
        cs.mem[pNode]+=val*vm.mem;
    }

    static void migration(vector<CurrentVM>& curVM,vector<CurrentServer>& curSer,
                          const vector<VM>& vmList,const vector<int>& vmLidOfvmId,
                          const int mxMigCnt,vector<Migration>& ansMig)
    {
        //return ;
        int migCnt=0,curNode,posNode,searchDeep;
        VM tempVM;
        sort(curSer.begin(),curSer.end(),sortByCoreAndMem);
        int curCore[2],curMem[2];
        bool success,rev;
        int pos;
        unsigned int tid;
        int val,tval;
        for(unsigned int i=0;i<curSer.size()&&migCnt<mxMigCnt;i++){
            if(migCnt+curSer[i].node[0].size()+curSer[i].node[1].size()+curSer[i].node[2].size()>mxMigCnt) continue;
            curCore[0]=curSer[i].mxc-curSer[i].core[0];
            curCore[1]=curSer[i].mxc-curSer[i].core[1];
            curMem[0]=curSer[i].mxm-curSer[i].mem[0];
            curMem[1]=curSer[i].mxm-curSer[i].mem[1];
            success=false;
            val=1e9;
            searchDeep=5000;
            for(unsigned int j=i+1;j<min( (unsigned int)(curSer.size()-1),(unsigned int)(i+500) );j++){
                if(j==i) continue;
                if(curCore[0]<=curSer[j].core[0]&&curMem[0]<=curSer[j].mem[0]&&curCore[1]<=curSer[j].core[1]&&curMem[1]<=curSer[j].mem[1]){
                    success=true;
                    tval=curSer[j].core[0]-curCore[0]+curSer[j].core[1]-curCore[1]+curSer[j].mem[0]-curMem[0]+curSer[j].mem[1]-curMem[1];
                    if(val>tval){
                        val=tval;
                        rev=false;
                        pos=curSer[j].id;
                        tid=j;
                    }
                }
                if(curCore[0]<=curSer[j].core[1]&&curMem[0]<=curSer[j].mem[1]&&curCore[1]<=curSer[j].core[0]&&curMem[1]<=curSer[j].mem[0]){
                    success=true;
                    tval=curSer[j].core[0]-curCore[1]+curSer[j].core[1]-curCore[0]+curSer[j].mem[0]-curMem[1]+curSer[j].mem[1]-curMem[0];
                    if(val>tval){
                        val=tval;
                        rev=true;
                        pos=curSer[j].id;
                        tid=j;
                    }
                }
            }
            if(success){
                for(int x:curSer[i].node[0]){
                    ansMig.push_back(Migration(x,pos,rev?1:0));
                    curSer[tid].node[rev?1:0].push_back(x);
                    curVM[x].placeSerId=pos;
                    curVM[x].placeNode=rev?1:0;
                }
                for(int x:curSer[i].node[1]){
                    ansMig.push_back(Migration(x,pos,rev?0:1));
                    curSer[tid].node[rev?0:1].push_back(x);
                    curVM[x].placeSerId=pos;
                    curVM[x].placeNode=rev?0:1;
                }
                for(int x:curSer[i].node[2]){
                    ansMig.push_back(Migration(x,pos,2));
                    curSer[tid].node[2].push_back(x);
                    curVM[x].placeSerId=pos;
                    curVM[x].placeNode=2;
                }
                for(int k=0;k<3;k++){
                    migCnt+=curSer[i].node[k].size();
                    curSer[i].node[k].clear();
                }
                if(rev){
                    curSer[tid].core[0]-=curCore[1];curSer[tid].core[1]-=curCore[0];
                    curSer[tid].mem[0]-=curMem[1];curSer[tid].mem[1]-=curMem[0];
                }else{
                    curSer[tid].core[0]-=curCore[0];curSer[tid].core[1]-=curCore[1];
                    curSer[tid].mem[0]-=curMem[0];curSer[tid].mem[1]-=curMem[1];
                }
                curSer[i].core[0]=curSer[i].core[1]=curSer[i].mxc;
                curSer[i].mem[0]=curSer[i].mem[1]=curSer[i].mxm;
            }
        }
        sort(curSer.begin(),curSer.end(),sortBack);
    }

//    static void migration(vector<CurrentVM>& curVM,vector<CurrentServer>& curSer,
//                          const vector<VM>& vmList,const vector<int>& vmLidOfvmId,
//                          const int mxMigCnt,vector<Migration>& ansMig)
//    {
//        int migCnt=0,curNode,posNode;//,searchDeep;
//        VM tempVM;
//        //sort(curSer.begin(),curSer.end(),sortByDaily);
//        sort(curSer.begin(),curSer.end(),sortByCoreAndMem);
//
//        bool migSuccess;
//        for(unsigned int i=0;i<curSer.size()&&migCnt<mxMigCnt;i++){
//            for(curNode=2;curNode<=2;curNode++){
//                for(vector<int>::iterator vid=curSer[i].node[curNode].begin();migCnt<mxMigCnt&&vid!=curSer[i].node[curNode].end();){
//                    tempVM=vmList[vmLidOfvmId[*vid]];
//                    //searchDeep=500;
//                    migSuccess=false;
//                    for(unsigned int j=min((unsigned int) (curSer.size()-1),(unsigned int) (i+1000));j>i;j--){
//                        //if(curSer[j].isEmpty()) continue;
//                        if(tempVM.isPair){
//                            if(curSer[j].core[0]>=tempVM.core/2 && curSer[j].core[1]>=tempVM.core/2 && curSer[j].mem[0]>=tempVM.mem/2 && curSer[j].mem[1]>=tempVM.mem/2){
//                                fixCurSer(curSer[i],tempVM,1); // mig out
//                                fixCurSer(curSer[j],tempVM,-1); // mig in
//                                curVM[*vid].placeSerId=curSer[j].id;
//                                curVM[*vid].placeNode=2;
//                                ansMig.push_back(Migration(*vid,curSer[j].id,2));
//                                curSer[j].node[curNode].push_back(*vid);
//                                vid=curSer[i].node[curNode].erase(vid);
//                                migSuccess=true;migCnt++;
//                                break;
//                            }
//                        }else{
//                            if(curSer[j].core[0]+curSer[j].mem[0]>=curSer[j].core[1]+curSer[j].mem[1])
//                                posNode=0;
//                            else
//                                posNode=1;
//                            for(int op=0;op<2;op++){
//                                if(op) posNode=1-posNode;
//                                if(curSer[j].core[posNode]>=tempVM.core&&curSer[j].mem[posNode]>=tempVM.mem){
//                                    fixCurSer(curSer[i],tempVM,curNode,1); // mig out
//                                    fixCurSer(curSer[j],tempVM,posNode,-1); // mig in
//                                    curVM[*vid].placeSerId=curSer[j].id;
//                                    curVM[*vid].placeNode=posNode;
//                                    ansMig.push_back(Migration(*vid,curSer[j].id,posNode));
//                                    curSer[j].node[posNode].push_back(*vid);
//                                    vid=curSer[i].node[curNode].erase(vid);
//                                    migSuccess=true;migCnt++;
//                                    break;
//                                }
//                            }
//                        }
//                        if(migSuccess) break;
//                    }
//                    if(!migSuccess) vid++;
//                }
//            }
//        }
//        sort(curSer.begin(),curSer.end(),sortBack);
//    }



//    {
//        int bestVal,tVal,idInVMList,migCnt=0;
//        VM tempVM;
//        for(CurrentServer& cs:curSer){
//            if(migCnt==mxMigCnt) break;
//            if(cs.core[0]+cs.mem[0]<cs.core[1]+cs.mem[1]){
//                bestVal=cs.core[0]+cs.mem[0];
//                for(vector<int>::iterator vId=cs.node[0].begin();vId!=cs.node[0].end()&&migCnt<mxMigCnt;){
//                    idInVMList=vmLidOfvmId[*vId];
//                    tempVM=vmList[idInVMList];
//                    if(cs.core[1]<tempVM.core||cs.mem[1]<tempVM.mem){
//                        vId++;
//                        continue;
//                    }
//                    tVal=min(cs.core[0]+tempVM.core+cs.mem[0]+tempVM.mem,cs.core[1]-tempVM.core+cs.mem[1]-tempVM.mem);
//                    if(bestVal<tVal){
//                        bestVal=tVal;
//                        cs.core[0]+=tempVM.core;cs.core[1]-=tempVM.core;
//                        cs.mem[0]+=tempVM.mem;cs.mem[1]-=tempVM.mem;
//                        cs.node[1].push_back(*vId);
//                        curVM[*vId].placeNode=1;
//                        ansMig.push_back(Migration(*vId,curVM[*vId].placeSerId,1));
//                        vId=cs.node[0].erase(vId);
//                        migCnt++;
//                    }else
//                        vId++;
//                }
//            }else{
//                bestVal=cs.core[1]+cs.mem[1];
//                for(vector<int>::iterator vId=cs.node[1].begin();vId!=cs.node[1].end()&&migCnt<mxMigCnt;){
//                    idInVMList=vmLidOfvmId[*vId];
//                    tempVM=vmList[idInVMList];
//                    if(cs.core[0]<tempVM.core||cs.mem[0]<tempVM.mem){
//                        vId++;
//                        continue;
//                    }
//                    tVal=min(cs.core[1]+tempVM.core+cs.mem[1]+tempVM.mem,cs.core[0]-tempVM.core+cs.mem[0]-tempVM.mem);
//                    if(bestVal<tVal){
//                        bestVal=tVal;
//                        cs.core[1]+=tempVM.core;cs.core[0]-=tempVM.core;
//                        cs.mem[1]+=tempVM.mem;cs.mem[0]-=tempVM.mem;
//                        cs.node[0].push_back(*vId);
//                        curVM[*vId].placeNode=0;
//                        ansMig.push_back(Migration(*vId,curVM[*vId].placeSerId,0));
//                        vId=cs.node[1].erase(vId);
//                        migCnt++;
//                    }else
//                        vId++;
//                }
//            }
//        }
//    }

};

#endif // MIGRATION_H
