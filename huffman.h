#include <iostream>
#include <string>
#include <map>
using namespace std;

class Node
{
public:
    string content;
    int frequency;
    Node *left, *right;

    Node() {}
    Node(int, char);
    Node(Node *, Node *);
};

class compare
{
public:
    bool operator()(const Node *, const Node *) const;
};

Node *huffman(int *);
Node *input(string);

void inorderTraversal(Node *, map<string, string> &, string); //root, table, code
pair<pair<int, map<string, string>>, string> encoding(const string);

void decode(const string, int, map<string, string> &);
//output the result into file named "output.txt"