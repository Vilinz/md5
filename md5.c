#include <stdio.h>
#include "md5.h"

//table T in repeat
uint32_t T[64] ={
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 
};

uint32_t s[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5,  9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

//initail vector VI
uint32_t A= 0x67452301;
uint32_t B= 0xEFCDAB89;
uint32_t C= 0x98BADCFE;
uint32_t D= 0x10325476;

void padding(char* initial_msg, uint8_t* padded_msg, uint32_t p) {
	//padding 100000....
	size_t len = strlen(initial_msg);
	//printf("%d\n", len);
	memcpy(padded_msg, (uint8_t*)initial_msg, len);
	padded_msg[len] = 0x80;
	
	for(int i = len+1; i < len + p; i++) {
		padded_msg[i] = 0;
	}
	//printf("padded_msg %d\n", strlen(padded_msg));
	//padding k%512
	uint32_t low_tail = len*8;
	padded_msg[len + p] = (uint8_t)low_tail;
	padded_msg[len + p + 1] = (uint8_t)(low_tail >> 8);
	padded_msg[len + p + 2] = (uint8_t)(low_tail >> 16);
	padded_msg[len + p + 3] = (uint8_t)(low_tail >> 24);

	uint32_t high_tail = (len >> 29);
	padded_msg[len + p + 4] = (uint8_t)high_tail;
	padded_msg[len + p + 5] = (uint8_t)(high_tail >> 8);
	padded_msg[len + p + 6] = (uint8_t)(high_tail >> 16);
	padded_msg[len + p + 7] = (uint8_t)(high_tail >> 24);
	//printf("padded %d\n", strlen(padded_msg));
	
}

void md5(uint8_t* padded_msg, uint8_t* result, int len) {
	//int len =strlen(padded_msg);
	//printf("%d\n", len);
	int one_time = 512/8, current_use_index = 0;
	uint32_t* Y = (uint32_t*)malloc(16);
	memset(Y, 0, 16);
	uint32_t a, b, c, d, f;
	for(int i = 0; i < len; i += one_time) {
		for(int j = 0; j < 16; j++) {
			Y[j] = (uint32_t)padded_msg[i+j*4]
					|((uint32_t)padded_msg[i+j*4+1] << 8)
					|((uint32_t)padded_msg[i+j*4+2] << 16)
					|((uint32_t)padded_msg[i+j*4+3] << 24);
		}
		a = A;
		b = B;
		c = C;
		d = D;

		for(int j = 0; j < 64; j++) {
			if(j < 16) {
				f = (b & c) | ((~b) & d);
				current_use_index = j;
			} else if(j < 32) {
				f = (b & d) | (c & (~d));
				current_use_index = (1 + 5*j)%16;
			} else if(j < 48) {
				f = b ^ c ^ d;
				current_use_index = (5 + 3*j)%16;
			} else if(j < 64) {
				f = c ^ (b | (~d));
				current_use_index = (7*j)%16;
			}

			uint32_t shilt = ((a + f + Y[current_use_index] + T[j]));
			shilt = b + ((shilt << s[j]) | (shilt >> (32 - s[j])));

			uint32_t temp = d;
			d = c;
			c = b;
			b = shilt;
			a = temp;
		}
		A += a;
		B += b;
		C += c;
		D += d;
	}
	//printf("lllllll\n");

    result[0] = (uint8_t)A;
    result[1] = (uint8_t)(A >> 8);
    result[2] = (uint8_t)(A >> 16);
    result[3] = (uint8_t)(A >> 24);

    result[4] = (uint8_t)B;
    result[5] = (uint8_t)(B >> 8);
    result[6] = (uint8_t)(B >> 16);
    result[7] = (uint8_t)(B >> 24);

    result[8] = (uint8_t)C;
    result[9] = (uint8_t)(C >> 8);
    result[10] = (uint8_t)(C >> 16);
    result[11] = (uint8_t)(C >> 24);

    result[12] = (uint8_t)D;
    result[13] = (uint8_t)(D >> 8);
    result[14] = (uint8_t)(D >> 16);
    result[15] = (uint8_t)(D >> 24);
}