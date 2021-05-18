#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
//extern int errno;
#include <stdarg.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/signal.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <string>
#include <arpa/inet.h>
using namespace std;
#define QLEN 32 /* maximum connection queue length */
#define BUFSIZE 512
#define INTERVAL 5 /* secs */
struct
{
    pthread_mutex_t st_mutex;
    unsigned int st_concount;
    unsigned int st_contotal;
    unsigned long st_contime;
    unsigned long st_bytecount;
} stats;

struct User
{
    pthread_mutex_t st_mutex;
    int regist; //for check if registed
    std::string name;
    int socket; //socket file descriptor

    /*std::string message; //for send
    std::string towho;
    */
    time_t login_time;

    int online; // check if online now

    int offline;       // check the user is exist and is not online now and there is a message pending to sent to
                       // check the flag when log in, if set, send a pending message directly
    std::string who;   // who left the message
    std::string ofmsg; //for offline message
    time_t oftime;     //the time when offline message left
    //time(&raw_time);

    //struct tm *timeinfo;
    //timeinfo = localtime(&raw_time);
    //std::cout << "Now is " << asctime(timeinfo) << std::endl;
};

struct
{
    struct User user[10];
    int num; //default to 0, if num > 3, cannot log as new user anymore
} record;

//void prstats(void);
//int TCPechod(int fd);

int errexit(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}
/* port base, for non-root servers */
unsigned short portbase = 0;

int passivesock(const char *service, const char *transport, int qlen);

int passiveTCP(const char *service, int qlen)
{
    return passivesock(service, "tcp", qlen);
}

/*---------------------------------------------------------------
* passivesock: Allocate & bind a server socket using TCP or UDP
* [Arguments]
* service: Service associated with the desired port
* transport: Transport protocol to use ("tcp" or "udp")
* qlen: Maximum server request queue length
*---------------------------------------------------------------
*/
int passivesock(const char *service, const char *transport, int qlen)
{
    struct servent *pse;    // pointer to service information entry
    struct protoent *ppe;   // pointer to protocol information entry
    struct sockaddr_in sin; // Internet endpoint address
    int s, type;            // socket descriptor & socket type

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    /* Map service name to port number */
    if (pse = getservbyname(service, transport))
        sin.sin_port = htons(ntohs((unsigned short)pse->s_port) + portbase);
    else if ((sin.sin_port = htons((unsigned short)atoi(service))) == 0)
        errexit("can't get \"%s\" service entry\n", service);

    //std::cout << ntohs(sin.sin_port) << std::endl;

    /* Map protocol name to protocol number */
    if ((ppe = getprotobyname(transport)) == 0)
        errexit("can't get \"%s\" protocol entry\n", transport);

    /* Use protocol to choose a socket type */
    if (strcmp(transport, "udp") == 0)
        type = SOCK_DGRAM;
    else
        type = SOCK_STREAM;
    /* Allocate a socket */
    if ((s = socket(PF_INET, type, ppe->p_proto)) < 0)
        errexit("can't create socket: %s\n", strerror(errno));
    /* Bind the socket */
    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        errexit("can't bind to %s port: %d\n", service, strerror(errno));
    if (type == SOCK_STREAM && listen(s, qlen) < 0)
        errexit("can't listen on %s port: %s\n", service, strerror(errno));
    return s;
}
/* --------------------------------------------------------------
* prstats: Print server statistical data
* --------------------------------------------------------------
*/
void prstats(void)
{
    return; //block output info
    time_t now;
    while (1)
    {
        sleep(INTERVAL);
        pthread_mutex_lock(&stats.st_mutex);
        now = time(0);
        printf("--- %s", ctime(&now));
        printf("%-32s: %u\n", "Current connections", stats.st_concount);
        printf("%-32s: %u\n", "Completed connections", stats.st_contotal);
        if (stats.st_contotal)
        {
            printf("%-32s: %.2f (secs)\n", "Average complete connection time", (float)stats.st_contime / (float)stats.st_contotal);
            printf("%-32s: %.2f\n", "Average byte count", (float)stats.st_bytecount / (float)(stats.st_contotal + stats.st_concount));
        }
        printf("%-32s: %lu\n\n", "Total byte count", stats.st_bytecount);
        pthread_mutex_unlock(&stats.st_mutex);
    }
}

int notexist(std::string is_there)
{
    for (int i = 0; i < 10; i++)
    {
        if (record.user[i].name == is_there)
            return 0;
    }
    return 1; // means not exist
}

void welcome(int ssock)
{
    time_t start;
    char buf[BUFSIZE] = {0};
    int cc;
    start = time(0);

    read(ssock, buf, BUFSIZE); //declare name in first tx
    std::cout << buf << " is connected, its fd is " << ssock << std::endl;
    for (int i = 0; i < 10; i++)
    {
        if (record.user[i].name == buf || record.user[i].regist == 0)
        {
            pthread_mutex_lock(&record.user[i].st_mutex);
            if (record.user[i].offline == 1)
            {               //forward the offline message directly, if there is.
                int om = 3; //offline message forwarding
                write(ssock, &om, sizeof(int));
                write(ssock, record.user[i].who.data(), BUFSIZE); //who left the message
                write(ssock, record.user[i].ofmsg.data(), BUFSIZE);
                write(ssock, &record.user[i].oftime, sizeof(time_t));
            }
            // init the online client and regist its information
            record.user[i].name = buf;
            record.user[i].regist = 1;
            record.user[i].socket = ssock;
            record.user[i].online = 1;
            record.user[i].login_time = start;
            record.user[i].offline = 0;
            //std::cout << "occupied " << i << std::endl;
            pthread_mutex_unlock(&record.user[i].st_mutex);

            for (int j = 0; j < 10; j++)
            {
                pthread_mutex_lock(&record.user[j].st_mutex);
                if (record.user[j].online)
                {
                    int on = 5; //online reminder
                    write(record.user[j].socket, &on, sizeof(int));
                    write(record.user[j].socket, buf, BUFSIZE);
                }
                pthread_mutex_unlock(&record.user[j].st_mutex);
            }
            break;
        }
    }
    // initailized
    //std::cout << "recorded" << std::endl;

    pthread_mutex_lock(&stats.st_mutex);
    stats.st_concount++;
    pthread_mutex_unlock(&stats.st_mutex);
    //link 127.0.0.1 2208 [name]
    //  link 127.0.0.1 2208 is handled by client
    //  here is handle [name]
    //  above is register and init a client with name

    //chat [somebody] "message"
    //  chat is handled by client function
    //  message recieced by read
    //  ,and send to somebody who is online
    int op;
    //std::cout << buf << " is ready for talking" << std::endl;
    std::cout << buf << " is ready" << std::endl;
    while (true)
    {
        read(ssock, &op, sizeof(int)); // how many argument to be received
        std::cout << "server received opcode " << op << " from " << buf << std::endl;

        if (op == 0) // zero for closing connection
        {
            std::cout << buf << " connection is closed" << std::endl;
            close(ssock);
            for (int i = 0; i < 10; i++)
            {
                pthread_mutex_lock(&record.user[i].st_mutex);
                if (record.user[i].name == buf)
                {
                    record.user[i].online = 0;
                    pthread_mutex_unlock(&record.user[i].st_mutex); // before break, unlock first

                    for (int j = 0; j < 10; j++)
                    {
                        pthread_mutex_lock(&record.user[j].st_mutex);
                        if (record.user[j].regist == 1 && record.user[j].online == 1)
                        { //send offline message to whom is online
                            int rmd = 2;
                            write(record.user[j].socket, &rmd, sizeof(rmd));
                            write(record.user[j].socket, buf, BUFSIZE);
                        }
                        pthread_mutex_unlock(&record.user[j].st_mutex);
                    }

                    pthread_mutex_lock(&stats.st_mutex);
                    stats.st_contime += time(0) - start;
                    stats.st_concount--;
                    stats.st_contotal++;
                    pthread_mutex_unlock(&stats.st_mutex);
                    return;
                }
                pthread_mutex_unlock(&record.user[i].st_mutex);
            }
            break;
        }
        else if (op == 1)
        {
            //error
        }
        else if (op == 2) // unicast
        {                 // directly tx message to receiver, without storing
            std::cout << std::endl
                      << "Now is unicasting" << std::endl;

            char rcvr[BUFSIZE] = {0}; // receiver
            char msg[BUFSIZE] = {0};

            if (read(ssock, rcvr, BUFSIZE) < 0)
                std::cerr << "receiver is lost" << std::endl;
            if (read(ssock, msg, BUFSIZE) < 0)
                std::cerr << "message is lost" << std::endl;

            std::cout << "receiver is " << rcvr << " " << strlen(rcvr) << std::endl;
            std::cout << buf << " says " << msg << " " << strlen(msg) << std::endl;

            if (notexist(std::string(rcvr)))
            {
                int dne = 4;
                write(ssock, &dne, sizeof(int));
                write(ssock, rcvr, BUFSIZE);
                std::cerr << "User " << rcvr << " is not exist" << std::endl;
                continue;
            }

            for (int i = 0; i < 10; i++)
            {
                //std::cout << "testing " << i << std::endl;

                if (record.user[i].name == std::string(rcvr))
                {
                    pthread_mutex_lock(&record.user[i].st_mutex);
                    //std::cout << "receiver is exist" << std::endl;

                    if (record.user[i].online)
                    { // [who] [message]
                        //std::cout << "receiver is online" << std::endl;
                        int a = 1;
                        write(record.user[i].socket, &a, sizeof(int));
                        write(record.user[i].socket, buf, BUFSIZE);
                        write(record.user[i].socket, msg, BUFSIZE);
                        pthread_mutex_unlock(&record.user[i].st_mutex);
                        break;
                    }
                    else if (record.user[i].online == 0)
                    {
                        record.user[i].offline = 1; // mean someone left message
                        record.user[i].who = std::string(buf);
                        record.user[i].oftime = time(0);
                        record.user[i].ofmsg = std::string(msg);

                        int ofmsg = 6; // for tell the receiver is not online, will pending the message
                        write(ssock, &ofmsg, sizeof(int));
                        write(ssock, record.user[i].name.data(), BUFSIZE);
                    }
                    pthread_mutex_unlock(&record.user[i].st_mutex);
                }
            }
            std::cout << "Finished unicast" << std::endl
                      << std::endl;
        }
        else if (op == 3) // multicast among 3 client, 1 to 2
        {
            std::cout << std::endl
                      << "This is multicast" << std::endl;
            char rcvr1[BUFSIZE] = {0}; // receiver
            char rcvr2[BUFSIZE] = {0};
            char msg[BUFSIZE] = {0};

            //std::cout << "fuck..." << std::endl;
            if (read(ssock, rcvr1, BUFSIZE) < 0)
                std::cerr << "receiver1 is lost" << std::endl;
            if (read(ssock, rcvr2, BUFSIZE) < 0)
                std::cerr << "receiver2 is lost" << std::endl;
            if (read(ssock, msg, BUFSIZE) < 0)
                std::cerr << "message is lost" << std::endl;

            std::cout << "receiver1 is " << rcvr1 << std::endl;
            std::cout << "receiver2 is " << rcvr2 << std::endl;
            std::cout << buf << " says " << msg << std::endl;

            if (notexist(std::string(rcvr1)))
            {
                int dne = 4;
                write(ssock, &dne, sizeof(int));
                write(ssock, rcvr1, BUFSIZE);
                std::cerr << "user " << rcvr1 << " is not exist" << std::endl;
            }
            else
            {
                for (int i = 0; i < 10; i++)
                {
                    //std::cout << "testing " << i << std::endl;

                    if (record.user[i].name == std::string(rcvr1))
                    {
                        pthread_mutex_lock(&record.user[i].st_mutex);
                        //std::cout << "receiver is exist" << std::endl;

                        if (record.user[i].online)
                        { // [who] [message]
                            //std::cout << "receiver is online" << std::endl;
                            int a = 1;
                            write(record.user[i].socket, &a, sizeof(int));
                            write(record.user[i].socket, buf, BUFSIZE);
                            write(record.user[i].socket, msg, BUFSIZE);
                            pthread_mutex_unlock(&record.user[i].st_mutex);
                            break;
                        }
                        else if (record.user[i].online == 0)
                        {
                            record.user[i].offline = 1; // mean someone left message
                            record.user[i].who = std::string(buf);
                            record.user[i].oftime = time(0);
                            record.user[i].ofmsg = std::string(msg);

                            int ofmsg = 6; // for tell the receiver is not online, will pending the message
                            write(ssock, &ofmsg, sizeof(int));
                            write(ssock, record.user[i].name.data(), BUFSIZE);
                        }
                        pthread_mutex_unlock(&record.user[i].st_mutex);
                    }
                }
            }
            if (notexist(std::string(rcvr2)))
            {
                int dne = 4;
                write(ssock, &dne, sizeof(int));
                write(ssock, rcvr2, BUFSIZE);
                std::cerr << "user " << rcvr2 << " is not exist" << std::endl;
            }
            else
            {
                for (int i = 0; i < 10; i++)
                {
                    //std::cout << "testing " << i << std::endl;

                    if (record.user[i].name == std::string(rcvr2))
                    {
                        pthread_mutex_lock(&record.user[i].st_mutex);
                        //std::cout << "receiver is exist" << std::endl;

                        if (record.user[i].online)
                        { // [who] [message]
                            //std::cout << "receiver is online" << std::endl;
                            int a = 1;
                            write(record.user[i].socket, &a, sizeof(int));
                            write(record.user[i].socket, buf, BUFSIZE);
                            write(record.user[i].socket, msg, BUFSIZE);
                            pthread_mutex_unlock(&record.user[i].st_mutex);
                            break;
                        }
                        else if (record.user[i].online == 0)
                        {
                            record.user[i].offline = 1; // mean someone left message
                            record.user[i].who = std::string(buf);
                            record.user[i].oftime = time(0);
                            record.user[i].ofmsg = std::string(msg);

                            int ofmsg = 6; // for tell the receiver is not online, will pending the message
                            write(ssock, &ofmsg, sizeof(int));
                            write(ssock, record.user[i].name.data(), BUFSIZE);
                        }
                        pthread_mutex_unlock(&record.user[i].st_mutex);
                    }
                }
            }
            std::cout << "Finished multicast" << std::endl
                      << std::endl;
        }
    }
}

int main()
{
    pthread_t th;
    pthread_attr_t ta;
    struct sockaddr_in fsin;                    // the address of a client
    unsigned int alen;                          // length of client’s address
    int ssock;                                  // slave server socket
    int msock = passivesock("2208", "tcp", 32); // master server socket
    // Port 2208
    // Protocol TCP
    //std::cout << m;

    pthread_attr_init(&ta);
    pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);
    pthread_mutex_init(&stats.st_mutex, 0);
    /* create a slave thread to monitor the concurrency of all slave threads */
    if (pthread_create(&th, &ta, (void *(*)(void *))prstats, 0) < 0)
        errexit("ppthread_create(prstats): % s\n ", strerror(errno));

    for (int i = 0; i < 10; i++)
    { //init user mutex and set name to empty string
        pthread_mutex_init(&record.user[i].st_mutex, 0);
        record.user[i].name = "";
        record.user[i].regist = 0;
    }
    record.num = 0;

    while (1)
    {
        alen = sizeof(fsin);
        ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
        //std::cout << "accept client " << ssock << std::endl;

        std::cout << "The client IP " << inet_ntoa(fsin.sin_addr)
                  << " connected via port number " << ntohs(fsin.sin_port)
                  << " using SOCK_STREAM(TCP)" << std::endl;

        if (ssock < 0)
        {
            if (errno == EINTR)
                continue;
            errexit("accept: % s\n", strerror(errno));
        }
        record.num++;
        /*if (record.num > 3)
        {
            std::cerr << "The server is fully loaded" << std::endl;
            continue;
        }*/
        if (pthread_create(&th, &ta, (void *(*)(void *))welcome, (void *)ssock) < 0)
            errexit("pthread_create: %s\n", strerror(errno));
        //        if (pthread_create(&th, &ta, (void *(*)(void *))TCPechod, (void *)ssock) < 0)
        //            errexit("pthread_create: %s\n", strerror(errno));
        //std::cout << "A thread created" << std::endl;
    }

    close(msock);
}