OBJ=RainbowTable.o Table.o md5.o testRainbow.o
CC=g++
CFLAGS=-ansi -I./ -g
EXE=rain

rain : $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -o $(EXE)
	
RainbowTable.o : RainbowTable.cpp
	@$(CC) $(CFLAGS) -c RainbowTable.cpp

Table.o : Table.cpp
	@$(CC) $(CFLAGS) -c Table.cpp
	
md5.o : md5.cpp
	@$(CC) $(CFLAGS) -c md5.cpp

testRainbow.o : testRainbow.cpp
	@$(CC) $(CFLAGS) -c testRainbow.cpp

clean :
	@rm $(EXE) *.o
