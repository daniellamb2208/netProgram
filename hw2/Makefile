CC = g++
CFLAGS = -fsanitize=address -std=c++11

all:
	$(CC) $(CFLAGS) -o server socket/server.cpp socket/server.h huffman/huffman.h huffman/huffman.cpp 
	$(CC) $(CFLAGS) -o client socket/client.cpp socket/client.h huffman/huffman.h huffman/huffman.cpp

clean:
	rm server client *.huf output_* *.o socket/*.gch huffman/*.gch