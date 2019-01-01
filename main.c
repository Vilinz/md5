#include <stdio.h>
#include "md5.h"

int main(int argc, char **argv) {
	
	uint8_t result[16];
	uint32_t len = 0;
	uint32_t p = 0;
	//tip for user
	//initial_msg = "2323dedeced";
	
	//printf("%d\n", len);

	if(argc != 2) {
		printf("Error command!\nPlease input with form <./a.out+<string>\n");
	}
	//initial_msg = argv[1];
	int l = strlen(argv[1]);
	char* initial_msg = (char*)malloc(l);
	memset(initial_msg, 0, l);
	initial_msg = argv[1];
	len = strlen(initial_msg);
	
	if((len*8)%512 == 448) {
		p = 512/8;
	} else {
		if((len*8)%512 < 448) {
			p = (448 - (len*8)%512)/8;
		} else {
			p = (448 + 512-((len*8)%512))/8;
		}
	}
	
	uint8_t *padded_msg = (uint8_t*)malloc(len + p + 8);
	//printf("%d\n", strlen(padded_msg));
	memset(padded_msg, 0, len + p + 8);
	//printf("%d\n", strlen(padded_msg));

	padding(initial_msg, padded_msg, p);

	md5(padded_msg, result, len + p);

	for(int i = 0; i < 16; i++) {
		printf("%02x", result[i]);
	}
	printf("\n");
	return 0;
}