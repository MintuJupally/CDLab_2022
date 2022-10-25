#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

class Node
{
public:
    double res;
    string val;
    vector<Node *> children;

    Node(double v)
    {
        res = v;

        char *buf;
        size_t sz;
        sz = snprintf(NULL, 0, "%g", res);
        buf = (char *)malloc(sz + 1);

        snprintf(buf, sz + 1, "%g", res);

        val = buf;
    }

    Node(string val, double res, vector<Node *> nodes)
    {
        this->val = val;
        this->res = res;

        children = nodes;
    }

    void printTree()
    {
        ofstream out("AST.dot");
        out << "digraph G {" << endl;

        out << "\tnode [shape=\"circle\"];" << endl;

        queue<pair<int, Node *>> bfs;

        int count = 0;
        bfs.push({count++, this});

        while (!bfs.empty())
        {
            pair<int, Node *> curr = bfs.front();
            bfs.pop();

            out << "\t" << curr.first << " [ label=\"" << curr.second->val << "\" ];" << endl;

            if (curr.second->children.size() == 0)
                continue;

            for (auto &child : curr.second->children)
            {
                out << "\t" << curr.first << " -> " << count << "; " << endl;

                bfs.push({count++, child});
            }
        }

        out << "}";
        out.close();
    }
};