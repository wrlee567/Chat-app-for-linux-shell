CC=gcc  #compiler
TARGET= sim

all:
	$(CC) asn3.c list.o -o $(TARGET)
valgrind:
	


clean:
	rm $(TARGET)
