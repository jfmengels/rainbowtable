OBJ=RainbowTable.o Table.o md5.o TestRainbow.o
CC=g++
CFLAGS=-fopenmp -ansi -I./ -g -lrt
EXE=rain

rain : $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -o $(EXE)
	
RainbowTable.o : RainbowTable.cpp
	@$(CC) $(CFLAGS) -c RainbowTable.cpp

Table.o : Table.cpp
	@$(CC) $(CFLAGS) -c Table.cpp
	
md5.o : md5.cpp
	@$(CC) $(CFLAGS) -c md5.cpp

TestRainbow.o : TestRainbow.cpp
	@$(CC) $(CFLAGS) -c TestRainbow.cpp

clean :
	@rm $(EXE) *.o
