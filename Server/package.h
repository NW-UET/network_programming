#ifndef package_h  
#define package_h   
#include<cstdint>
#include<vector>
using namespace std;
namespace package
{
    
    /**
     * Update files
     */
    struct
    {
        uint16_t filenameLength;
        vector<char> filename;
        unsigned long fileSize;
        unsigned char md5[16];
    } typedef DetailOfUpdateFiles;
    
    class ListUpdate{
        private:
        uint8_t type;
        uint8_t numOfFileUpdate;
        vector<DetailOfUpdateFiles> detailOfUpdateFile;
        public:
        ListUpdate(uint8_t[]);
        uint8_t getType();
        uint8_t getNumOfFileUpdate();
        vector<DetailOfUpdateFiles> getDetailOfUpdateFile();
        int8_t process();
        void printObjectPackage();
        void printStringPackage();
    };

    /**
     * Confirm files
     */
    struct 
    {
        uint16_t filenameLength;
        vector<char> filename;
        uint8_t status;
    } typedef DetailOfConfirmFiles;

    class Confirm{
        private:
        uint8_t type;
        uint8_t numOfFileConfirm;
        vector<DetailOfConfirmFiles> detailOfConfirmFiles;
        public:
        void setType(uint8_t);
        void setNumOfFileConfirm(uint8_t);
        void setDetailOfConfirmFiles(vector<DetailOfConfirmFiles>);
        uint8_t* createPackage();
        void printObjectPackage();
        void printStringPackage();
    };

    /**
     * Request list files
     */
    class ReqListFiles{
        private:
        uint8_t type;
        public:
        ReqListFiles(uint8_t[]);
        uint8_t getType();
        int8_t process();
        void printObjectPackage();
        void printStringPackage();
    };
    
    /**
     * Response list files
     */
    struct 
    {
        uint16_t filenameLength;
        vector<char> filename;
    }typedef DetailOfFiles;
    
    class ResListFiles{
        private:
        uint8_t type;
        uint8_t numOfFiles;
        vector<DetailOfFiles> detailOfFiles;
        public:
        uint8_t* createPackage();
        void setType(uint8_t);
        void setNumOfFiles(uint8_t);
        void setDetailOfFiles(vector<DetailOfFiles>);
        void printObjectPackage();
        void printStringPackage();
    };

    /**
     * Request list Hosts
     */
    class ReqListHosts{
        private:
        uint8_t type;
        DetailOfFiles detailOfFiles;
        public:
        ReqListHosts(uint8_t[]);
        uint8_t getType();
        DetailOfFiles getDetailOfFiles();
        uint8_t process();
        void printObjectPackage();
        void printStringPackage();
    };

    /**
     * Response list Hosts
     */
    class ResListHosts{
        private:
        uint8_t type;
        uint8_t numOfHosts;
        vector<uint32_t> IPAddress;
        public:
        uint8_t* createPackage();
        void setType(uint8_t);
        void setNumOfHosts(uint8_t);
        void setIPAddress(uint32_t);
        void printObjectPackage();
        void printStringPackage();
    };
} // package

#endif /* package_h  */  