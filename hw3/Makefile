CC = g++
CFLAGS = -fsanitize=address -std=c++11

all:
	$(CC) $(CCFLAGS) -o server server.cpp -lpthread -W
	$(CC) $(CCFLAGS) -o client client.cpp -lpthread -W
clean:
	rm server client