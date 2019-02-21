
PROG = stucs
OBJS = main.o LinkedList.o Student.o Hash.o SkipList.o
SRCS = main.c LinkedList.c Student.c Hash.c SkipList.c
HEADER = Student.h SkipList.h LinkedList.h Hash.h

$(PROG): $(OBJS)
	gcc -g $(OBJS) -o $(PROG)


main.o: main.c
	gcc -g -c main.c
	
LinkedList.o:
	gcc -g -c LinkedList.c
	
SkipList.o:
	gcc -g -c SkipList.c
	
Student.o:
	gcc -g -c Student.c
	
Hash.o:
	gcc -g -c Hash.c

clear:
	rm $(OBJS) $(PROG)
