#ifndef _MD5_H_
#define _MD5_H_
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//do padding 
void padding(char* initial_msg, uint8_t* padded_msg, uint32_t p);
//repeat
void md5(uint8_t* padded_msg, uint8_t* result, int len);

#endif