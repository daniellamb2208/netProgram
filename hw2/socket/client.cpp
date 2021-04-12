#include "client.h"
#include "../huffman/huffman.h"

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

    while (true)
    {
        std::cout << "[student @netprog]$ ";
        getline(std::cin, cmd);

        //parse command line
        std::size_t left = 0, right = 0;
        right = cmd.find(" ");
        std::string process = cmd.substr(left, right - left);
        left = right + 1;

        if (process == "link")
        {
            std::string ip, port;
            // Parse command line
            right = cmd.find(" ", left);
            ip = cmd.substr(left, right - left);

            left = right + 1;
            right = cmd.find(" ", left);
            port = cmd.substr(left, right - left);

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
            std::cout << "Connecting to " << ip << ":" << port << std::endl;
            is_connect = 1;
            std::cout << "Connect_success" << std::endl;
        }
        else if (process == "send")
        {
            if (is_connect == 0)
            {
                std::cerr << "Send where? please tell me, link to a server first" << std::endl;
                continue;
            }
            int func = 0;
            write(fd, &func, sizeof(int));

            right = cmd.find(" ", left);
            std::string filename = cmd.substr(left, right);

            std::cout << filename << std::endl;
            auto parameter = encoding(filename.data());
            /*
            pair<pair<int, map<char, string>>, string>
                    padding, table for decoding, encoded string 
            */

            std::ofstream ofs("client.huf", std::ios::out | std::ios::binary);
            int table_quantity = 0;
            for (auto &i : parameter.first.second)
                table_quantity++;
            ofs << filename << " " << table_quantity << " " << parameter.first.first << std::endl;
            // Origin_filename, number of table, number of zero padding
            // At first line of file
            for (auto &i : parameter.first.second)
                ofs << i.second << " " << i.first << std::endl;
            // Mapping table
            ofs << parameter.second;
            // Encoded s
            ofs.close();

            fstream f("client.huf");
            f.seekg(0, f.end);
            int size = f.tellg();
            f.close();
            //std::cout << size << std::endl;
            write(fd, &size, sizeof(int));

            std::ifstream ifs("client.huf", std::ios::in | std::ios::out);

            int count = 0;
            while (true)
            {
                char x;
                ifs.get(x);
                if (ifs.eof())
                    break;
                count++;
                if (write(fd, &x, sizeof(char)) < 0)
                    err("token_tx");
            }
            ifs.close();
            //std::cout << "tx qty " << count << std::endl;
            // End of a send
            /*
            Some infomation to do
            */
        }
        else if (process == "leave")
        {
            int func = 1;
            write(fd, &func, sizeof(int));

            close(fd);
            std::cout << "Bye Bye " << std::endl;
            break;
        }
        else if (process == "--help")
        {
            std::cout << "Welcome to help page" << std::endl
                      << "Command" << std::endl
                      << " link IP PORT" << std::endl
                      << "      Built a socket and connect to target IP address" << std::endl
                      << "      If you want to send a file, you need to connect onto a server first" << std::endl
                      << std::endl
                      << " send FILENAME" << std::endl
                      << "      Please enter the exist file, or the program might crash" << std::endl
                      << "      Before transmissing the file, using Huffman encode to compress it" << std::endl
                      << "      The data which is to sent is filename, mapping table number, padding for satify every token is a byte" << std::endl
                      << "      Those data will record at client here as client.huf, and store in server there as server.huf, too" << std::endl
                      << std::endl
                      << " leave" << std::endl
                      << "      Shut down the service of client, and disconnect to server" << std::endl;
        }
        else
            std::cerr << "The command is not supported, try --help" << std::endl;
    }

    return 0;
}