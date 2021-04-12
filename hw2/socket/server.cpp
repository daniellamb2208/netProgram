#include "server.h"
#include "../huffman/huffman.h"

int main(int argc, char *argv[])
{
    int fd;

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0)
        err("socket");

    std::cout << "Socket built" << std::endl;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
        err("Bind");

    std::cout << "Bind" << std::endl;

    if (listen(fd, 5) < 0)
        err("Listen");

    std::cout << "Listening..." << std::endl;

    struct sockaddr_in client;
    int newfd;
    int cli_len = sizeof(client);

    newfd = accept(fd, (struct sockaddr *)&client, (socklen_t *)&cli_len);
    if (newfd < 0)
        err("accept");

    std::cout << "Accepted : ";

    //conver a numerical address to a string
    std::cout << "The client IP " << inet_ntoa(client.sin_addr) << " connected"
              << " via port number " << ntohs(client.sin_port)
              << "using SOCK_STREAM(TCP)" << std::endl;

    int func; //for knowing send and leave

    while (read(newfd, &func, sizeof(int))) //get function code, integer
    {
        //std::cout << "func code " << func << std::endl;
        if (func)
        {
            std::cout << "Client IP " << inet_ntoa(client.sin_addr) << " left" << std::endl;
            //I did it if client left, server terminate, too
            break;
        }

        char c;
        std::ofstream ofs("server.huf", std::ios::out | std::ios::binary);

        int size;
        read(newfd, &size, sizeof(int)); //tx file infomation, length for number of char to read
        for (auto i = 0; i < size; i++)
        {
            read(newfd, &c, sizeof(char));
            ofs.put(c);
        }
        ofs.close();

        std::ifstream ifs("server.huf", std::ios::in | std::ios::binary);
        std::string info;
        getline(ifs, info);
        char endofline;
        //ifs.get(endofline);
        //std::cout << info << std::endl;
        //filename, table_num, padding
        std::string filename;
        int num, padding;
        //std::cout << info << std::endl;
        std::size_t left = 0, right = info.find(" ");
        filename = info.substr(left, right);
        left = right + 1;
        right = info.find(" ", left);
        num = stoi(info.substr(left, right - left));
        padding = stoi(info.substr(right + 1));

        std::cout << "The client sent a file \"" << filename << "\""
                  << " with size of " << size << " bytes" << std::endl;
        std::cout << "The huffman coding data is stored in \"server.huf\"" << std::endl;
        //server.huf is supposed to be same as client.huf
        // ,which contains filename, number of table, zero padding, and encoded string
        /*std::cout << filename << std::endl
                  << num << std::endl
                  << padding << std::endl
                  << std::endl;*/

        std::map<std::string, std::string> table;
        for (auto i = 0; i < num; i++)
        {
            char x;
            std::string y;
            ifs >> y;
            ifs.get(x);
            ifs.get(x);
            table[string(1, x)] = y;
        }
        //for (auto &i : table)
        //   std::cout << i.first << " " << i.second << std::endl;

        ifs.get(endofline); // remove end of line of first line of file in order to correctly read encoded string

        std::string buffer = "";
        while (!ifs.eof())
        {
            char x;
            ifs.get(x);
            if (ifs.eof())
                break;
            buffer += x;

            if (buffer.length() > (1 << 21))
            { // unknow thing
                decode(("output_" + filename).data(), buffer, padding, table);
                buffer = "";
                std::cout << "Spilt block a time" << std::endl;
            }
        }
        //std::cout << buffer << std::endl;
        decode(("output_" + filename).data(), buffer, padding, table);

        //std::cout << "fuck" << std::endl;
    }
    close(newfd);
    close(fd);
    return 0;
}