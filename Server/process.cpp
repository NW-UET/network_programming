#include <sqlite3.h>
#include <string>
#include <vector>
#include "message.h"
#include "process.h"
using namespace std;
const int BYTE_OF_MD5 =16;
string char_to_hexa(char a[32]){
    string temp;
    for(int i=0;i<32;i++){
        switch(a[i]){
            case 0:
                temp+="0";
                break;
            case 1:
                temp+="1";
                break;
            case 2:
                temp+="2";
                break;
            case 3:
                temp+="3";
                break;
            case 4:
                temp+="4";
                break;
            case 5:
                temp+="5";
                break;
            case 6:
                temp+="6";
                break;
            case 7:
                temp+="7";
                break;
            case 8:
                temp+="8";
                break;
            case 9:
                temp+="9";
                break;
            case 10:
                temp+="a";
                break;
            case 11:
                temp+="b";
                break;
            case 12:
                temp+="c";
                break;
            case 13:
                temp+="d";
                break;
            case 14:
                temp+="e";
                break;
            case 15:
                temp+="f";
                break;
        }
    }
    return temp;
}
vector<char> hexa_to_char(char a[32]){
    vector<char> temp;
    for(int i=0;i<32;i++){
        switch(a[i]){
            case '0':
                temp.push_back(0);
                break;
            case '1':
                temp.push_back(1);
                break;
            case '2':
                temp.push_back(2);
                break;
            case '3':
                temp.push_back(3);
                break;
            case '4':
                temp.push_back(4);
                break;
            case '5':
                temp.push_back(5);
                break;
            case '6':
                temp.push_back(6);
                break;
            case '7':
                temp.push_back(7);
                break;
            case '8':
                temp.push_back(8);
                break;
            case '9':
                temp.push_back(9);
                break;
            case 'a':
                temp.push_back(10);
                break;
            case 'b':
                temp.push_back(11);
                break;
            case 'c':
                temp.push_back(12);
                break;
            case 'd':
                temp.push_back(13);
                break;
            case 'e':
                temp.push_back(14);
                break;
            case 'f':
                temp.push_back(15);
                break;
        }
    }
    return temp;
}
void md5_16_to_md5_32(unsigned char a[16],char b[32]){
    for(int i=0;i<16;i++){
        b[i*2]=a[i]>>4;
        unsigned char temp=a[i]<<4;
        b[i*2+1]=temp>>4;
    }
}
void md5_32_to_md5_16(char a[32],unsigned char b[16]){
    for(int i=0;i<16;i++){
        unsigned char temp1=a[i*2]<<4;
        unsigned char temp2=a[i*2+1];
        b[i]=temp1|temp2;
    }
}
// using namespace std;
//(arg 4th cua exec(),so cot, mang gia tri, mang ten cot)
int callback0(void *NotUsed, int argc, char **argv, char **azColName){
   printf("select\n");
   for(int i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
int callback1(void *data, int argc, char **argv, char **azColName){
   vector<File>* dataDB=(vector<File>*) data;
   File temp;
   for(int i=0; i<argc; i++){
    //    printf("\n%s\n",azColName[i]);
       if(strcmp(azColName[i],"filename")==0){
           string t(argv[i]);
           temp.filename=t;
        //    printf("filename: %s\n",temp.filename.c_str());
       }else if(strcmp(azColName[i],"size")==0){
           temp.file_size=atoi(argv[i]);
        //    printf("size: %d\n",temp.file_size);
       }else if(strcmp(azColName[i],"md5")==0){
        //    printf("md5: %s-sizeof: %d",argv[i],sizeof(argv[i]));
           for(int j=0;j<BYTE_OF_MD5;j++){
               temp.md5[j]=argv[i][j];
            //    printf("%d ",temp.md5[j]);
           }
       }
    //    printf(".%s. = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    
   }
   dataDB->push_back(temp);
   printf("\n");
   return 0;
}
int callbackCheck(void *data, int argc, char **argv, char **azColName){
    vector<int>* idReturned=(vector<int>*) data;
    for(int i=0; i<argc; i++){
        if(strcmp(azColName[i],"id")==0){
            idReturned->push_back(atoi(argv[i]));
        }
    }
    printf("\n");
    return 0;
}
int callbackSelect(void *data, int argc, char **argv, char **azColName){
    vector<int>* numOfData=(vector<int>*)data;
    for(int i=0;i<argc;i++){
        if(strcmp(azColName[i],"id")==0){
            // printf("id= %s\n",argv[i]);
            numOfData->push_back(atoi(argv[i]));
        }
    }
    return 0;
}
int callbackSelectAll(void *data, int argc, char **argv, char **azColName){
    struct ListFilesResponse* res=(struct ListFilesResponse*)data;
    struct Filesize temp;
    for(int i=0;i<argc;i++){
        // printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        
        if(strcmp(azColName[i],"filename")==0){
            temp.filename=argv[i];
            temp.filename_length=temp.filename.length();
        }else if(strcmp(azColName[i],"size")==0){
            temp.file_size=atoi(argv[i]);
        }
    }
    res->filesize_list.push_back(temp);
    return 0;
}
int callbackSelectHosts(void *data, int argc, char **argv, char **azColName){
    struct ListHostsResponse* res=(struct ListHostsResponse*)data;
    uint32_t temp;
    for(int i=0;i<argc;i++){
        // printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        
        if(strcmp(azColName[i],"size")==0){
            res->file_size=atoi(argv[i]);
        }else if(strcmp(azColName[i],"host")==0){
            res->IP_addr_list.push_back(atoi(argv[i]));
        }
        
    }
    return 0;
}
sqlite3* connectDb(char a[]){
    sqlite3 *db;
    int rc;

    rc = sqlite3_open(a, &db);

    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }
    return db;
}
// tra ve id neu ten file trung nhung md5 khong trung, 0 trong th con lai
vector<int> selectAllFiles(sqlite3* db, int host){
    printf("select all files...Host: %d\n",host);
    string query="select * from files where host = "+ to_string(host);
    const char* sql = query.c_str();
    char *zErrMsg = 0;
    int rc;
    vector<int> idOfData;
    rc = sqlite3_exec(db,sql,callbackSelect,(void*)&idOfData,&zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Operation done successfully\n");
    }
    printf("selected all files!\n");
    // for(int e:idOfData){
    //     printf("id = %d\n",e);
    // }
    return idOfData;
}
int selectAllFiles(sqlite3* db,struct ListFilesResponse* res){
    printf("select all files...\n");
    string query="select * from files;";
    const char* sql = query.c_str();
    char *zErrMsg = 0;
    int rc;
    
    rc = sqlite3_exec(db,sql,callbackSelectAll,(void*)res,&zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Operation done successfully\n");
    }
    res->n_files=res->filesize_list.size();
    printf("selected all files!\n");
}
int selectHosts(sqlite3* db,struct Filename filename,struct ListHostsResponse* res){
    printf("select host...\n");
    string query="select * from files where filename='"+filename.filename+"';";
    const char* sql = query.c_str();
    char *zErrMsg = 0;
    int rc;
    
    
    rc = sqlite3_exec(db,sql,callbackSelectHosts,(void*)res,&zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Operation done successfully\n");
    }
    res->n_hosts=res->IP_addr_list.size();
    printf("selected host!\n");
}
/**
 * checkFiles
 * 0: co trong goi tin, co trong csdl (giong md5)
 * 1: co trong goi tin, khong co trong csdl nhung trung ten, tren host khac con file co ten giong file nay
 * 3: co trong goi tin, khong co trong csdl nhung trung ten, tren host khac khong con file ten giong file nay
 * 2: co trong goi tin, khong co trong csdl khac ten
 */
int checkFiles(sqlite3* db,struct File f,vector<int>* ids,int host){
    //code
    // vector<int> idReturned;
    printf("check files...\n");
    char md5_32[32];
    md5_16_to_md5_32(f.md5,md5_32);
    string md5=char_to_hexa(md5_32);
    // cout<<"md5: "<<md5<<endl;
    char *zErrMsg = 0;
    int rc;
    const char* sql;
    //0
    string query;
    query="select id from files where md5='"+md5+"' and host="+to_string(host)+";";
    sql = query.c_str();
    // printf("%s\n",sql);
    rc = sqlite3_exec(db,sql,callbackCheck,(void*)ids,&zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Operation done successfully\n");
    }
    if(ids->size()>0) {
        printf("checked files! Return 0\n");
        return 0;
    }
    //1
    query="select id from files where filename = '"+f.filename+"' and host="+to_string(host)+";";
    sql = query.c_str();
    vector<int> ret;
    rc = sqlite3_exec(db,sql,callbackCheck,(void*)&ret,&zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Operation done successfully\n");
    }
    if(ret.size()>0) {
        vector<int> ret;
        query="select id from files where filename='"+f.filename+"';";
        sql = query.c_str();
        rc = sqlite3_exec(db,sql,callbackCheck,(void*)&ret,&zErrMsg);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }else{
            fprintf(stdout, "Operation done successfully\n");
        }
        if(ret.size()==1){
            printf("checked files! Return 3\n");
            return 3;
        }else{
            printf("checked files! Return 1\n");
            return 1;
        }
    }
    printf("checked files! Return 2\n");
    return 2;
}
int deleteFiles(sqlite3* db,int host){
    string query="delete from files where host = "+ to_string(host);
    const char* sql = query.c_str();
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(db,sql,callback0,0,&zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Operation done successfully\n");
    }
}
int deleteFiles(sqlite3* db,int id,int host){
    string query="delete from files where id="+to_string(id)+" and host="+to_string(host)+";";
    const char* sql = query.c_str();
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(db,sql,callback0,0,&zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Operation done successfully\n");
    }
}
int insertFiles(sqlite3* db,struct File f, int host){
    string filename=f.filename;//
    uint64_t size=f.file_size;//
    // unsigned char md5_16[];
    char md5_32[32];
    md5_16_to_md5_32(f.md5,md5_32);//f.md5
    string md5=char_to_hexa(md5_32);
    char *zErrMsg = 0;
    // insert into files(filename,size,md5,host) values ('filename',1024,'123456789efab56',12345);";
    string sql ="insert into files(filename,size,md5,host) values ('"+filename+"',"+to_string(size)+",'"+md5.c_str()+"',"+to_string(host)+");";
    cout<<sql<<endl;
    int rc = sqlite3_exec(db, sql.c_str(), callback0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Records created successfully\n");
    }
}
struct FileListUpdateResponse processFileListUpdate(struct FileListUpdateRequest package, int addr){
    struct FileListUpdateResponse res;
    res.n_files=package.n_files;
    sqlite3* db=connectDb("files.db");
    char *zErrMsg = 0;
    int n=package.n_files;
    vector<File> dataDb=package.file_list;
    vector<int> idFiles=selectAllFiles(db,addr);
    vector<int> idNotDelete;
    for(struct File a: dataDb){
        Filestatus filestatus_temp;
        filestatus_temp.filename=a.filename;
        filestatus_temp.filename_length=a.filename_length;
        int kqcheck=checkFiles(db,a,&idNotDelete,addr);
        if(kqcheck==1){
            filestatus_temp.status=1;
        }else if(kqcheck==0){
            filestatus_temp.status=0;
        }else if(kqcheck==3){
            filestatus_temp.status=0;
            insertFiles(db,a,addr);
        }else{
            filestatus_temp.status=0;
            insertFiles(db,a,addr);
        }
        res.filestatus_list.push_back(filestatus_temp);
    }
    //delete
    printf("idFiles\n");
    for(int i:idFiles){
        printf("%d ",i);
    }
    printf("\n");
    printf("idNotDelete\n");
    for(int i:idNotDelete){
        printf("%d ",i);
    }
    printf("\n");
    for(int i=0;i<idFiles.size();i++){
        for(int j=0;j<idNotDelete.size();j++){
            if(idFiles.at(i)==idNotDelete.at(j)){
                idFiles.erase(idFiles.begin()+i);
                i--;
                break;
            }
        }
    }
    for(int id: idFiles){
        deleteFiles(db,id,addr);
    }

    return res;    
}

struct ListFilesResponse processListFilesRequest(struct ListFilesRequest package){
    struct ListFilesResponse res;
    sqlite3* db=connectDb("files.db");
    selectAllFiles(db,&res);
    return res;
}

struct ListHostsResponse processListHostsRequest(struct ListHostsRequest package){
    struct ListHostsResponse res;
    res.file_size=0;
    sqlite3* db=connectDb("files.db");
    struct Filename filename=package.filename;
    selectHosts(db,filename,&res);
    return res;
}