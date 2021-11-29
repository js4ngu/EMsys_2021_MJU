#include "textLCD.h"
#include "stdio.h"

int main(int argc , char ***argv){
    int mode, line_number;
    char str[];
	printf("LCD TEST\n");
	if (argc <  3) {
		doHelp();
		return 1;
	}
	
	if (argv[1][0] == '0' ) {
		textLCD_off();
		return 0; 
	}
    else if(argv[1][0] == '1') {
        mode = 1;
    }
	else {
		doHelp();
		perror("option error \n");
		return 1;
	}
    line_number = atoi(argv[2]);
    str = atoi(argv[2]);
	printf("line_number :%d\n",line_number);

    if (mode == 1) {
        fndDisp(line_number , str);
    }
    
	return 0;
}
//arm-linux-gnueabi-gcc fndtest.c -lMyPeri -L.