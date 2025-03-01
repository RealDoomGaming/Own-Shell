CC = gcc
CFLAGS = -Wall -Wextra -g
OBJECTS = ./code/main.o ./code/utils.o ./code/echo.o ./code/read.o ./code/directory.o ./code/makedirectory.o ./code/touchFile.o

shell: $(OBJECTS)
	$(CC) $(CFLAGS) -o shell $(OBJECTS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

echo.o: echo.c echo.h utils.h
	$(CC) $(CFLAGS) -c echo.c

read.o: read.c read.h utils.h
	$(CC) $(CFLAGS) -c read.c

directory.o: directory.c directory.h utils.h
	$(CC) $(CFLAGS) -c directory.c

makedirectory.o: makedirectory.c makedirectory.h utils.h 
	$(CC) $(CFLAGS) -c makedirectory.c 

touchFile.o: touchFile.c touchFile.h utils.h 
	$(CC) $(CFLAGS) -c touchFile.c

clean:
	rm -f shell $(OBJECTS)
