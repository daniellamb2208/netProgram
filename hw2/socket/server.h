#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <utility>
#include <unistd.h>
#include <memory>

#define port 2208
#define BUFF_LEN 1 << 9
#define err(x)     \
    {              \
        perror(x); \
        exit(1);   \
    }
