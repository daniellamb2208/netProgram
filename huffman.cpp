#include "huffman.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <map>
#include <utility>

#include <bitset>
using namespace std;

Node::Node(int f, char c)
{
    this->frequency = f;
    this->content = c;
    this->left = right = nullptr;
}
Node::Node(Node *l, Node *r)
{
    this->frequency = l->frequency + r->frequency;
    this->content = l->content + r->content;

    this->left = (l->frequency < r->frequency) ? l : r;
    this->right = (l->frequency < r->frequency) ? r : l;
}

bool compare::operator()(const Node *x, const Node *y) const
{
    return x->frequency >= y->frequency;
}

Node *input(const char *filename)
{
    ifstream f(filename, ios::binary);
    int asc[256] = {};
    char byte;

    while (f.get(byte))
    {
        int index = static_cast<int>((static_cast<unsigned char>(byte)));
        asc[index]++;
    }
    f.close();

    return huffman(asc);
}

Node *huffman(int *data)
{
    priority_queue<Node *, vector<Node *>, compare> pq;
    for (auto i = 0; i < 256; i++)
    { //add node which appeared in input into my pq
        if (data[i])
        {
            Node *tmp = new Node(data[i], static_cast<char>(i));
            pq.push(tmp);
        }
    }

    while (pq.size() != 1)
    {
        Node *p = pq.top(); //Take the least frequency one
        pq.pop();
        Node *q = pq.top(); //Take the second least frequency one
        pq.pop();
        //Now merge it
        Node *tmp = new Node(p, q);
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

void inorderTraversal(Node *current, map<string, string> &table, string code = "")
{ //code is trace the tree, and append '0' when go left child, '1' for right
    if (current)
    {
        inorderTraversal(current->left, table, code + "0"); //left node put small freqeuncy and encode 0
        if (current->left == nullptr && current->right == nullptr)
            table[current->content] = code;
        inorderTraversal(current->right, table, code + "1");
    }
}

pair<pair<int, map<string, string>>, string> encoding(const char *filename)
{
    map<string, string> table; // store huffman result
    inorderTraversal(input(filename), table);
    //built the table, where input function return a ordered priority quere with the tree

    // Table information
    //for (auto &i : table)
    //    cout << i.first << " " << i.second << endl;

    ifstream f(filename, ios::in | ios::binary);
    char byte;
    string binary, result = "";
    int count = 0;

    while (f.get(byte))
    {
        binary += table[string(1, byte)];
        count++;
    }
    f.close();
    cout << "Before encoding, " << count << " bytes" << endl;

    for (int i = 0; i < (binary.length() / 8); i++)
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

    return make_pair(make_pair(padding, table), result);
}

void decode(const char *filename, const string huff, int padding, map<string, string> &table)
{
    int length = huff.length();
    string real = "";

    map<string, string> intable; //invert the table, for convenient output
    for (auto i : table)
        intable[i.second] = i.first;

    for (int i = 0; i < length; i++)
    { // decompress, take a char with compressed and turn it into 8-bit binary string
        bitset<8> byte = static_cast<int>(huff[i]);
        //cout<<byte<<endl;
        real += byte.to_string();
    }
    length = length * 8 - padding; //real length
    real = real.substr(0, length); //remove padding

    ofstream f(filename, ios::out);

    //    for (auto i : intable)
    //        cout << i.first << " " << i.second << endl;

    int j = 0; //pos
    int x = 1; //len

    while (j < length)
    {
        if (intable[real.substr(j, x)] != "")
        { //if match, skip the x pos
            f << intable[real.substr(j, x)];
            j += x;
            x = 1;
        }
        else
            x++;
    }
    f.close();
}

int main(int argc, char *argv[])
{
    cout << "Encode init" << endl;
    auto parameter = encoding(argv[1]);
    //pair<pair<int, map<string, string>>, string>
    //((padding , table) , encoded string)

    cout << "Decode init" << endl;
    decode(argv[2], parameter.second, parameter.first.first, parameter.first.second);

    return 0;
}