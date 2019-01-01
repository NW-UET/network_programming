#ifndef Confirm_cpp
#define Confirm_cpp
#include<cstdint>
#include<vector>
#include"package.h"
using namespace std;
using namespace package;
void Confirm::setType(uint8_t type){
    this->type=type;
}
void Confirm::setNumOfFileConfirm(uint8_t numOfFileConfirm){
    this->numOfFileConfirm=numOfFileConfirm;
}
void Confirm::setDetailOfConfirmFiles(vector<DetailOfConfirmFiles> detailOfConfirmFiles){
    this->detailOfConfirmFiles=detailOfConfirmFiles;
}
uint8_t* Confirm::createPackage(){
    vector<uint8_t> package;
    package.push_back(type);
    package.push_back(numOfFileConfirm);
    
    
    for(DetailOfConfirmFiles e: detailOfConfirmFiles)
    {
        //set filenameLength to package
        union{
            uint16_t uint16;
            uint8_t uint8[2];
        };
        uint16=e.filenameLength;
        package.push_back(uint8[0]);
        package.push_back(uint8[1]);
        
        //set filename to package
        for(char c:e.filename)
        {
            package.push_back((uint8_t)c);
        }
        
        //set status to package
        package.push_back(e.status);
    }
    uint8_t* packageReturn = new uint8_t[package.size()];
    for(int i = 0; i < package.size(); i++)
    {
        packageReturn[i]=package.at(i);
    }
    
    
    return packageReturn;
}

#endif /* Confirm_cpp  */