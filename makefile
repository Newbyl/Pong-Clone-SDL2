CC := g++

CFLAGS := -std=c++17 -Wall -Wextra -pedantic -ggdb -Wno-unused-variable -Wno-unused-parameter -Wunknown-pragmas -Wunused-command-line-argument -O2

LDFLAGS := -lSDL2main -lSDL2_ttf -lSDL2

PONG := pong

all : $(PONG)

$(PONG) : pong.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o : %.cpp
	clear
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -c $<
	
clean :
	rm *.o $(PONG)
	clear
	
