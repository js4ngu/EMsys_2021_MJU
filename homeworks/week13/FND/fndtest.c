#include "fnd.h"
#include "stdio.h"

int main(int argc , char **argv){
	int number, mode;
	printf("FND TEST\n");
	if (argc <  2) {
		doHelp();
		return 1;
	}
	
	if (argv[1][0] == '0' ) {
		fndOff();
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
    number = atoi(argv[2]);
	printf("number :%d\n",number);

    if (mode == 1) {
        fndDisp(number , 0);
    }
    
	return 0;
}
//arm-linux-gnueabi-gcc fndtest.c -lMyPeri -L.