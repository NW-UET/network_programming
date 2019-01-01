#ifndef ListUpdate_cpp
#define ListUpdate_cpp
#include<cstdint>
#include<stdio.h>
#include<vector>
#include"package.h"
using namespace std;
using namespace package;
/**
 * ListUpdate
 */
ListUpdate::ListUpdate(char* package){
    type=package[0];
    numOfFileUpdate=package[1];
    int indexNow=2;
    for(int i = 0; i < numOfFileUpdate; i++)
    {
        DetailOfUpdateFiles temp;
        union {
            uint16_t uint16;
            uint8_t uint8_1[2];
        };
        uint8_1[0]=package[indexNow++];
        uint8_1[1]=package[indexNow++];
        temp.filenameLength=uint16;

        for(int i=0;i<(int)temp.filenameLength;i++){
            temp.filename.push_back(package[indexNow++]);
        }

        union{
            uint8_t uint8_2[8];
            unsigned long lo;
        };
        for(int i=0;i<8;i++){
            uint8_2[i]=package[indexNow++];
        }
        temp.fileSize=lo;

        for(int i=0;i<16;i++){
            temp.md5[i]=package[indexNow++];
        }
        
    }
    // printf("String package: ");
    // for(int i=0;i<)
}
uint8_t  ListUpdate::getType(){
    return type;
}
uint8_t ListUpdate::getNumOfFileUpdate(){
    return numOfFileUpdate;
}
vector<DetailOfUpdateFiles> ListUpdate::getDetailOfUpdateFile(){
    return detailOfUpdateFile;
}
int8_t ListUpdate::process(){

}
void ListUpdate::printObjectPackage(){
    printf("Type: %d\nNum of file update: %d\n",type,numOfFileUpdate);
    for(DetailOfUpdateFiles e: detailOfUpdateFile){
        printf("filenameLength: %d\n",(int)e.filenameLength);
        char temp[e.filenameLength];
        printf("filename: ");
        for(int i=0;i<e.filenameLength;i++){
            temp[i]=e.filename.at(i);
        }
        printf("\nfileSize: %ld\nmd5: %s",e.fileSize,e.md5);
    }
}
#endif /* ListUpdate_cpp  */ 