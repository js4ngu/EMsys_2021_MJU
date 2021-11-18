final.elf : 1.o 2.o main.o myProject.h
	gcc 1.o 2.o main.o -o final.elf

1.o : 1.c myProject.h
	gcc -c 1.c -o 1.o

2.o : 2.c myProject.h
	gcc -c 2.c -o 2.o

main.o : main.c myProject.h
	gcc -c main.c -o main.o

clean :
	rm -rf final.elf 1.o 2.o main.o