#include <iostream>
#include <string>
#include <map>
#include <memory>
using namespace std;

class Node
{
public:
    string content;
    int frequency;
    shared_ptr<Node> left, right;

    Node() {}
    Node(int, char);
    Node(shared_ptr<Node>, shared_ptr<Node>);
};

class compare
{ // For priority queue
public:
    bool operator()(const shared_ptr<Node>, const shared_ptr<Node>) const;
};

shared_ptr<Node> huffman(int *);
shared_ptr<Node> input(string);

void inorderTraversal(shared_ptr<Node>, map<string, string> &, string); //root, table, code
pair<pair<int, map<string, string>>, string> encoding(const char *);

void decode(const char *, const string, int, map<string, string> &);
// filename of output, huffman encoded string, huffman encoded table