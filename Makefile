CC=gcc  #compiler
TARGET= simulation

all:
	$(CC) asn3.c list.o -o $(TARGET)
valgrind:
	


clean:
	rm $(TARGET)
