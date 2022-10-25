#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <fstream>
#include <stack>

using namespace std;

class Node
{
public:
    double res;
    string val;
    string post;
    vector<Node *> children;
    stack<Node *> stk;

    Node(double v)
    {
        res = v;

        char *buf;
        size_t sz;
        sz = snprintf(NULL, 0, "%g", res);
        buf = (char *)malloc(sz + 1);

        snprintf(buf, sz + 1, "%g", res);

        val = post = buf;
    }

    Node(string val, double res, vector<Node *> nodes)
    {
        this->val = val;
        this->res = res;

        children = nodes;

        if (nodes.size() == 1)
        {
            post = nodes[0]->post;
            if (val == "-")
            {
                post += " -1 *";
            }
        }
        else
        {

            for (auto &n : nodes)
            {
                post += n->post;
                post += " ";
            }

            post += val;
        }
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

            for (auto &child : curr.second->children)
            {
                out << "\t" << curr.first << " -> " << count << "; " << endl;

                bfs.push({count++, child});
            }
        }

        out << "}";
        out.close();
    }

    void printDAG()
    {
        unordered_map<string, pair<int, Node *>> nodes;

        // traverse whole tree
        queue<pair<int, Node *>> bfs;
        int count = 0;
        bfs.push({count++, this});

        while (!bfs.empty())
        {
            pair<int, Node *> curr = bfs.front();
            bfs.pop();

            string p = curr.second->post;
            nodes[p] = curr;

            for (auto &child : curr.second->children)
            {
                bfs.push({count++, child});
            }
        }

        // make DAG
        ofstream out("DAG.dot");

        out << "digraph G {" << endl;

        out << "\tnode [shape=\"circle\"];" << endl;

        unordered_set<Node *> vis;
        vis.insert(nodes[post].second);

        bfs.push(nodes[post]);

        while (!bfs.empty())
        {
            pair<int, Node *> curr = bfs.front();
            bfs.pop();

            stk.push(curr.second);

            out << "\t" << curr.first << " [ label=\"" << curr.second->val << "\" ];" << endl;

            for (auto &child : curr.second->children)
            {
                pair<int, Node *> next = nodes[child->post];

                out << "\t" << curr.first << " -> " << next.first << "; " << endl;
                if (vis.find(next.second) == vis.end())
                {
                    vis.insert(next.second);

                    bfs.push(next);
                }
                stk.push(next.second);
            }
        }

        out << "}" << endl;

        out.close();

        // open DAG
        system("dot -Tpng DAG.dot -o DAG.png");
        system("xdg-open DAG.png");
    }

    void generate3AddressCode()
    {
        unordered_map<string, string> mp;
        int count = 0;

        while (!stk.empty())
        {
            Node *curr = stk.top();
            stk.pop();

            if (mp.find(curr->post) != mp.end())
                continue;

            if (curr->children.size() == 0)
            {
                mp[curr->post] = curr->val;
            }
            else if (curr->children.size() == 1)
            {
                mp[curr->post] = "v" + to_string(count++);
                cout << mp[curr->post] << " = " << curr->val << mp[curr->children[0]->post] << endl;
            }
            else
            {
                mp[curr->post] = "v" + to_string(count++);
                cout << mp[curr->post] << " = " << mp[curr->children[0]->post] << " " << curr->val << " " << mp[curr->children[1]->post] << endl;
            }
        }
    }
};