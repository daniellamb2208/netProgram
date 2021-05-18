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
#include <pthread.h>
#include <stdarg.h>
#define BUFF_LEN 512
#define err(x)     \
    {              \
        perror(x); \
        exit(1);   \
    }

#define MSG "What time is it?\n"

int errexit(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}
void getmsg(int fd)
{
    //std::cout << "create a thread to receive server forward message" << std::endl;
    while (true)
    {
        int argc;
        read(fd, &argc, sizeof(int));
        //std::cout << std::endl << "received argc is " << argc << std::endl;

        // argc is for dealing with message receive from server
        // 1 for normal communication
        // 2 for off the line reminder
        // 3 for offline msg
        // 4 for not exist
        // 5 for online reminder
        // 6 for remind chatter is offline, will pending the message
        if (argc == 1)
        {
            char name[BUFF_LEN] = {0};
            char msg[BUFF_LEN] = {0};
            read(fd, name, BUFF_LEN);
            read(fd, msg, BUFF_LEN);
            std::cout << name << " says " << msg << std::endl;
        }
        else if (argc == 2)
        {
            char name[BUFF_LEN] = {0};
            read(fd, name, BUFF_LEN);
            std::cout << "User " << name << " is off-line" << std::endl;
        }
        else if (argc == 3)
        {
            char name[BUFF_LEN] = {0};
            char msg[BUFF_LEN] = {0};
            time_t when;
            read(fd, name, BUFF_LEN);
            read(fd, msg, BUFF_LEN);
            read(fd, &when, sizeof(time_t));

            struct tm *timeinfo;
            timeinfo = localtime(&when);
            std::cout << name << " left a message to you when " << asctime(timeinfo) << msg << std::endl;
        }
        else if (argc == 4)
        {
            char name[BUFF_LEN] = {0};
            read(fd, name, BUFF_LEN);
            std::cerr << name << " is not a existing user" << std::endl;
        }
        else if (argc == 5)
        {
            char name[BUFF_LEN] = {0};
            read(fd, name, BUFF_LEN);
            std::cout << name << " is online" << std::endl;
        }
        else if (argc == 6)
        {
            char name[BUFF_LEN] = {0};
            read(fd, name, BUFF_LEN);
            std::cout << name << " is offline, the message will pending until he/she is back" << std::endl;
        }
        else
            std::cerr << "Something error with server, received a strange opcode" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    int fd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    std::string cmd;

    int is_connect = 0; //1 for test, default to 0
    std::cout << "Hello user" << std::endl;
    time_t raw_time;
    struct tm *timeinfo;

    time(&raw_time);
    timeinfo = localtime(&raw_time);
    std::cout << "Now is " << asctime(timeinfo) << std::endl;
    std::cout << "--help for more desciption" << std::endl;
    std::cout << "> ";
    pthread_t th;
    pthread_attr_t ta;
    pthread_attr_init(&ta);
    pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);

    while (true)
    {
        std::cin >> cmd;
        //getline(std::cin, cmd);

        //parse command line
        /*std::size_t left = 0, right = 0;
        right = cmd.find(" ");
        std::string process = cmd.substr(left, right - left);
        left = right + 1;*/

        if (cmd == "connect")
        {
            std::string ip, port, name;
            std::cin >> ip >> port >> name;
            //std::cout << ip << " " << port << " " << name << std::endl;

            fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (fd < 0)
                err("socket");

            server_addr.sin_addr.s_addr = inet_addr(ip.data());
            //inet_addr, convert "1.2.3.4" to integert, big endian, suit for network addr

            if (server_addr.sin_addr.s_addr == (in_addr_t)-1)
                err("inet_addr");

            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(atoi(port.data()));
            //host to network, short
            /*
                h host byte order
                n network byte order
                l long, for convert IP addr
                s short, for convert port number
            */

            if (connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
                err("connect");
            write(fd, name.data(), BUFF_LEN); // declare who you are
            std::cout << "Connecting to " << ip << ":" << port << std::endl;
            is_connect = 1;
            std::cout << "Connect succeed, You have logged in as " << name << std::endl;

            /* create a slave thread to monitor the concurrency of all slave threads */
            if (pthread_create(&th, &ta, (void *(*)(void *))getmsg, (void *)fd) < 0)
                errexit("ppthread_create(prstats): % s\n ", strerror(errno));
            //std::cout << name << " is ready for chatting" << std::endl;
        }
        else if (cmd == "chat")
        {
            if (is_connect == 0)
            {
                std::cerr << "Link to server first" << std::endl
                          << "usage: --help" << std::endl;
                continue;
            }
            std::string friend1, friend2, msg;
            // Parse command line
            char space;
            int op;
            std::cin >> friend1;
            std::cin.ignore();
            if (std::cin.peek() == 34)
            {
                friend2 = "";
                getline(std::cin, msg);
                op = 2; // unicast
            }
            else
            {
                std::cin >> friend2;
                getline(std::cin, msg);
                op = 3; // multicast
            }
            //std::cout << "client op " << op << std::endl;

            friend1.push_back(0);
            friend2.push_back(0);
            msg.push_back(0);
            //std::cout << "friend1 " << friend1 << std::endl;
            //std::cout << "friend2 " << friend2 << std::endl;
            //std::cout << "message " << msg << std::endl;

            //std::cout << "send to " << friend1 << std::endl;
            write(fd, &op, sizeof(int)); // tell server how many argument I'd like to send

            if (op == 2)
            {
                //std::cout << "unicast " << std::endl;
                write(fd, friend1.data(), BUFF_LEN);
                write(fd, msg.data(), BUFF_LEN);
                //std::cout << "data sent" << std::endl;
            }
            else if (op == 3)
            { //multicast
                //std::cout << "multicast " << std::endl;
                write(fd, friend1.data(), BUFF_LEN);
                write(fd, friend2.data(), BUFF_LEN);
                write(fd, msg.data(), BUFF_LEN);
                //std::cout << "data sent" << std::endl;
            }
        }
        else if (cmd == "bye")
        {
            int shut = 0;
            write(fd, &shut, sizeof(int));

            close(fd);
            std::cout << "Bye Bye " << std::endl;
            break;
        }
        else if (cmd == "--help")
        {
            std::cout << "Welcome to help page" << std::endl
                      << "Command" << std::endl
                      << " connect [IP] [PORT] [your login name]" << std::endl
                      << "      Built a socket and connect to target IP address" << std::endl
                      << "      If you want to chat with others, you need to connect onto a server first with your nickname" << std::endl
                      << std::endl
                      << " chat [friend1] [friend2](optional) [message]" << std::endl
                      << "      You should type \"\" to contain message, \" begin is to recognize message string" << std::endl
                      << "          eg: chat daniel peter \"hi, how r u\"" << std::endl
                      << std::endl
                      << " bye" << std::endl
                      << "      Shut down the service of client, and disconnect to server" << std::endl;
        }
        else
            std::cerr << "The command is not supported, try --help" << std::endl;
    }

    return 0;
}