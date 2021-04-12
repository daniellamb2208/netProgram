#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utility>

#define BUFF_LEN 1 << 9
#define err(x)     \
    {              \
        perror(x); \
        exit(1);   \
    }

#define MSG "What time is it?\n"