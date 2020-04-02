all: Project1.c
	gcc -g -Wall -o project1 Project1.c
clean:
	$(RM) project1
