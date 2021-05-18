CC = g++
CFLAGS = -fsanitize=address -std=c++11

all:
	$(CC) $(CCFLAGS) -o server server.cpp -lpthread
	$(CC) $(CCFLAGS) -o client client.cpp -lpthread
clean:
	rm server client