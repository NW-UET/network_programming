#if !defined(convert_cpp)
#define convert_cpp
#include<cstdint>
#include<vector>
using namespace std;
/**
 * uint16 to uint8[]
 */ 
void uint16_to_uint8(uint16_t a,uint8_t* b){
    union{
        uint16_t uint16;
        uint8_t uint8[2];
    };
    uint16=a;
    
    for(int i = 0; i < 2; i++)
    {
        b[i]=uint8[i];
    }
    
}
/**
 * uint8[] to uint16
 */
void uint8_to_uint16(uint8_t* a,uint16_t b){
    union{
        uint16_t uint16;
        uint8_t uint8[2];
    };
    for(int i=0;i<2;i++){
        uint8[i]=a[i];
    }
    b=uint16;
}
/**
 * u long to uint8[]
 */
void ulong_to_uint8(unsigned long a, uint8_t* b){
    union{
        unsigned long x;
        uint8_t y[8];
    };
    x=a;
    
    for(int i = 0; i < 8; i++)
    {
        b[i]=y[i];
    }
    
}
/**
 * uint8[] to u long
 */
void uint8_to_ulong(uint8_t* a,unsigned long b){
    union{
        unsigned long x;
        uint8_t y[8];
    };
    
    for(int i = 0; i < 8; i++)
    {
        y[i]=a[i];
    }
    b=x;
    
}
/**
 * u char[16] to uint8[]
 */
void uchar_to_uint8(unsigned char* a,uint8_t* b){
    
    for(int i = 0; i < 16; i++)
    {
        b[i]=(uint8_t)a[i];
    }    
}
/**
 * uint8[] to u char[16]
 */
void uint8_to_uchar(uint8_t* a,unsigned char* b){
    for(int i = 0; i < 16; i++)
    {
        b[i]=(unsigned char)a[i];
    }   
}
/**
 * char[] to uint8[]
 */
void char_to_uint8(char* a, uint8_t* b){
    for(int i = 0; i < 16; i++)
    {
        b[i]=(char)a[i];
    } 
}

/**
 * vector<char> to uint8[]
 */ 
void vectorchar_to_uint8(vector<char> a,uint8_t* b){
    for(int i = 0; i < a.size(); i++)
    {
        b[i]=(uint8_t)a.at(i);
    } 
}
/**
 * uint8[] to vector<char>
 */
void uint8_to_vectorchar(uint8_t* a,int size, vector<char> b){
    for(int i = 0; i < size; i++)
    {
        b.push_back((char)a[i]);
    } 
}


#endif // convert_cpp