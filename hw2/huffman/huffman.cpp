#include "huffman.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <map>
#include <utility>
#include <bitset>
#include <memory>
// Using smart pointer to deal with memory leak
using namespace std;

Node::Node(int f, char c)
{
    this->frequency = f;
    this->content = c;
    this->left = right = nullptr;
}
Node::Node(shared_ptr<Node> l, shared_ptr<Node> r)
{
    this->frequency = l->frequency + r->frequency;
    this->content = l->content + r->content;

    this->left = (l->frequency < r->frequency) ? l : r;
    this->right = (l->frequency < r->frequency) ? r : l;
    // Maintain some order
}

bool compare::operator()(const shared_ptr<Node> x, const shared_ptr<Node> y) const
{
    return x->frequency >= y->frequency;
}

shared_ptr<Node> input(const char *filename)
{
    ifstream f(filename, ios::binary);
    int asc[256] = {}; // Init the table with all zero. for record the freq of byte
    char byte;

    while (f.get(byte))
    { // Index error fixed, signed char to unsigned char to int, to be array index correctly
        int index = static_cast<int>((static_cast<unsigned char>(byte)));
        asc[index]++;
    }
    f.close();

    return huffman(asc);
}

shared_ptr<Node> huffman(int *data)
{
    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, compare> pq;
    for (auto i = 0; i < 256; i++)
    { // Add node which appeared in input into my pq
        if (data[i])
        { // Cast into char may ambiguous, but I only need to record it with 8-bit
            shared_ptr<Node> tmp(new Node(data[i], static_cast<char>(i)));
            pq.push(tmp);
        }
    }

    while (pq.size() != 1)
    {
        shared_ptr<Node> p = pq.top(); //Take the least frequency one
        pq.pop();
        shared_ptr<Node> q = pq.top(); //Take the second least frequency one
        pq.pop();
        //Now merge it
        shared_ptr<Node> tmp(new Node(p, q));
        pq.push(tmp);
        p = q = nullptr;
        //The two merged node is BACK into pq
        //cout << p->content << " " << p->frequency << endl;
        //cout << q->content << " " << q->frequency << endl;
        //cout << endl;
    }
    //cout << pq.top()->content << " " << pq.top()->frequency << endl;
    return pq.top();
}

void inorderTraversal(shared_ptr<Node> current, map<string, string> &table, string code = "")
{ // Argument code trace the tree, append '0' when go left child, '1' for right
    if (current)
    {
        inorderTraversal(current->left, table, code + "0");
        if (current->left == nullptr && current->right == nullptr)
            table[current->content] = code;
        inorderTraversal(current->right, table, code + "1");
    }
}

pair<pair<int, map<string, string>>, string> encoding(const char *filename)
{
    map<string, string> table; // Store huffman result
    inorderTraversal(input(filename), table);
    // Built the table, with where input function return a ordered priority quere with the tree

    // Table information
    //for (auto &i : table)
    //    cout << i.first << " " << i.second << endl;

    ifstream f(filename, ios::in | ios::binary);
    if (!f.is_open())
    {
        //WARNING: It doesn't handle file DNE;
        cerr << "File doesn't exist" << endl;
        exit(1);
    }
    char byte;
    string binary = "", result = "";
    int count = 0;
    // Reread the file to encode it
    while (f.get(byte))
    {
        binary += table[string(1, byte)];
        count++;
    }
    f.close();
    cout << "Before encoding, " << count << " bytes" << endl;

    for (int i = 0; i < int(binary.length() / 8); i++)
        // take every 8-bit binary string to int to char, and append it to encoded string
        result += static_cast<char>(stoi(binary.substr(i * 8, 8), nullptr, 2));

    int padding = 8 - binary.length() % 8;
    //8 - (15 % 8) = 1, if length of binary_string is 15, need to padding `1` zero(s)
    if (8 - padding)
    { //if divisible by 8, not satisfy the last special case condition
        //example "10101" + "000" means stoi("10101") << 3
        char byte_last = stoi(binary.substr(binary.length() / 8 * 8), nullptr, 2) << padding;
        result += byte_last;
    }
    cout << "After encoding, " << result.length() << " bytes" << endl;
    //cout << "Some compress ratio " << result.length() / count << endl;

    return make_pair(make_pair(padding, table), result);
}

void decode(const char *filename, const string huff, int padding, map<string, string> &table)
{
    int length = huff.length();
    string real = "";

    map<string, string> intable; // Invert the table, for convenient output
    for (auto i : table)
        intable[i.second] = i.first;

    for (int i = 0; i < length; i++)
    { // Decompress, take a char with compressed and turn it into 8-bit binary string
        bitset<8> byte = static_cast<int>(huff[i]);
        //cout<<byte<<endl;
        // WARNING: Maybe crush, if the string cannot afford the hole data
        real += byte.to_string();
    }
    length = length * 8 - padding; // Original length
    real = real.substr(0, length); // Remove padding

    ofstream f(filename, ios::out | ios::binary | ios::app);

    //for (auto i : intable)
    //    cout << i.first << " " << i.second << endl;

    int j = 0; //pos
    int x = 1; //len
    //std::cout << length << std::endl;

    while (j < length)
    {
        if (intable[real.substr(j, x)] != "")
        { // Match the pattern, skip the x pos
            f << intable[real.substr(j, x)];
            j += x;
            x = 1;
        }
        else
            x++;
    }

    f.close();
}
/*
int main(int argc, char *argv[])
{
    cout << "Encode init" << endl;
    auto parameter = encoding(argv[1]);
    //pair<pair<int, map<string, string>>, string>
    //((padding , table) , encoded string)

    cout << "Decode init" << endl;
    decode(argv[2], parameter.second, parameter.first.first, parameter.first.second);

    return 0;
}*/