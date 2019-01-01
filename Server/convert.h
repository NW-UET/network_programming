#if !defined(conrt_h)
#define convert_h
#include<cstdint>
#include<vector>
using namespace std;
/**
 * uint16 to uint8[]
 */
void uint16_to_uint8(uint16_t,uint8_t*);
/**
 * uint8[] to uint16
 */
void uint8_to_uint16(uint8_t*,uint16_t);
/**
 * u long to uint8[]
 */
void ulong_to_uint8(unsigned long, uint8_t*);
/**
 * uint8[] to u long
 */
void uint8_to_ulong(uint8_t*,unsigned long);
/**
 * u char[16] to uint8[]
 */
void uchar_to_uint8(unsigned char*,uint8_t*);
/**
 * uint8[] to u char[16]
 */
void uint8_to_uchar(uint8_t*,unsigned char*);
/**
 * char[] to uint8[]
 */
void char_to_uint8(char* a, uint8_t* b);
/**
 * vector<char> to uint8[]
 */ 
void vectorchar_to_uint8(vector<char>,uint8_t*);
/**
 * uint8[] to vector<char>
 */
void uint8_to_vectorchar(uint8_t*,int,vector<char>);
/**
 * 
 */
#endif // convert_h
