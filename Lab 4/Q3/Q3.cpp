#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <queue>

#define EPSILON "\u2208"

using namespace std;

vector<string> non_terminals;
unordered_set<string> nt_exists;
unordered_map<string, vector<vector<string>>> rules;

string word;
int pos = 0;
int count = 0;

class Node
{
public:
    int ind;
    string label;
    vector<Node *> links;

    Node(int i, string l)
    {
        ind = i;
        label = l;
    }
};

void print(string str)
{
    std::cout << str << endl;
}

template <typename... Strings>
void print(string str, Strings... strings)
{
    std::cout << str << " ";
    print(strings...);
}

template <class... Rest>
void error(int code, Rest... rest)
{
    print(rest...);

    exit(code);
}

vector<string> split(string str, string p)
{
    vector<string> pieces;
    int l = 0, i = 0;
    while (i < str.size())
    {
        if (i + p.size() <= str.size() && str.substr(i, p.size()) == p)
        {
            pieces.push_back(str.substr(l, i - l));
            l = i + p.size();
            i = l;

            if (i == str.size())
            {
                pieces.push_back({});
                ++l;
            }
        }
        else
            ++i;
    }

    if (l <= i)
        pieces.push_back(str.substr(l, i - l));

    return pieces;
}

void noteRule(string rule)
{
    vector<string> r = split(rule, "->");

    if (r.size() != 2)
        error(-1, "Invalid production rule:", rule);

    if (nt_exists.find(r[0]) == nt_exists.end())
    {
        non_terminals.push_back(r[0]);
        nt_exists.insert(r[0]);
    }

    vector<string> rs = split(r[1], "|");
    int pos = 0;
    for (int i = 0; i < rs.size(); i++)
    {
        rules[r[0]].push_back({});

        int pos = 0;
        while (pos < rs[i].size())
        {
            int l = pos++;
            if (l + 1 < rs[i].size() && rs[i][l] == 'i' && rs[i][l + 1] == 'd')
            {
                rules[r[0]].back().push_back("id");
                pos += 1;
            }
            else if (l + 2 < rs[i].size() && (int)(rs[i][l]) == -30 && (int)(rs[i][l + 1]) == -120 && (int)(rs[i][l + 2]) == -120)
            {
                rules[r[0]].back().push_back(EPSILON);
                pos += 2;
            }
            else
            {
                while (pos < rs[i].size() && rs[i][pos] == '\'')
                    ++pos;
                rules[r[0]].back().push_back(rs[i].substr(l, pos - l));
            }
        }
    }
}

void printRules(bool flag = false)
{
    if (!flag)
        for (auto el : non_terminals)
        {
            string nt = el;
            vector<vector<string>> c = rules[nt];

            for (int i = 0; i < c.size(); i++)
            {
                std::cout << nt << "->";
                for (int j = 0; j < c[i].size(); j++)
                {
                    std::cout << c[i][j];
                }
                std::cout << endl;
            }
        }
    else
        for (auto el : non_terminals)
        {
            string nt = el;
            vector<vector<string>> c = rules[nt];

            std::cout << nt << "->";
            for (int i = 0; i < c.size(); i++)
            {
                for (int j = 0; j < c[i].size(); j++)
                {
                    std::cout << c[i][j];
                }
                if (i != c.size() - 1)
                    std::cout << "|";
            }
            std::cout << endl;
        }
}

void simplifyRules()
{
    unordered_map<string, bool> vis;

    for (auto el : non_terminals)
    {
        string nt = el;
        vector<vector<string>> curr = rules[nt], updated;

        for (int i = 0; i < curr.size(); i++)
        {
            if (vis.find(curr[i][0]) == vis.end())
            {
                updated.push_back(curr[i]);
                continue;
            }

            string rnt = curr[i][0];

            for (int j = 0; j < rules[rnt].size(); j++)
            {
                vector<string> upd(rules[rnt][j]);
                upd.insert(upd.end(), curr[i].begin() + 1, curr[i].end());
                updated.push_back(upd);
            }
        }

        if (curr.size() > 0)
            rules[nt] = updated;

        vis[nt] = true;
    }
}

void eliminateLRecursion()
{
    unordered_map<string, vector<vector<string>>> urules;

    int len = non_terminals.size();
    for (int e = 0; e < len; e++)
    {
        string nt = non_terminals[e];
        vector<vector<string>> curr = rules[nt];

        bool hasLR = false;
        for (int i = 0; i < curr.size() && !hasLR; i++)
        {
            if (curr[i][0] == nt)
                hasLR = true;
        }

        if (hasLR)
        {
            if (nt_exists.find(nt + "\'") == nt_exists.end())
            {
                non_terminals.push_back(nt + "\'");
                nt_exists.insert(nt + "\'");
            }

            for (int i = 0; i < curr.size(); i++)
            {
                if (curr[i][0] == nt)
                {
                    hasLR = true;

                    vector<string> vec(curr[i].begin() + 1, curr[i].end());
                    vec.push_back(nt + "\'");

                    urules[nt + "\'"].push_back(vec);
                }
                else
                {
                    vector<string> vec = curr[i];
                    vec.push_back(nt + "\'");

                    urules[nt].push_back(vec);
                }
            }

            urules[nt + "\'"].push_back({EPSILON});
        }
        else
        {
            urules[nt] = rules[nt];
        }
    }

    rules = urules;
}

void leftFactorize()
{
    vector<string> nts;
    for (auto rule : rules)
        nts.push_back(rule.first);

    int i = 0;
    while (i < nts.size())
    {
        string nt = nts[i];

        int j = 0;
        int len = 0;
        int last = 0, curr = 0;

        while (j < rules[nt].size())
        {
            if (len == 0 || last >= 1)
            {
                for (int k = j + 1; k < rules[nt].size(); k++)
                {
                    if (len < rules[nt][k].size())
                    {
                        vector<string> a(rules[nt][k].begin(), rules[nt][k].begin() + len + 1);
                        vector<string> b(rules[nt][j].begin(), rules[nt][j].begin() + len + 1);

                        if (rules[nt][k][len] == rules[nt][j][len])
                            ++curr;
                    }
                }
                ++len;

                if (curr == 0 || len >= rules[nt][j].size() || len != 1 && last > curr)
                {
                    if (len > 1)
                    {
                        int d = 0;
                        string newnt = nt;
                        while (rules.find(newnt) != rules.end())
                            newnt += "\'";
                        nts.push_back(newnt);

                        for (int r = j; r < rules[nt].size(); r++)
                        {
                            if (len <= rules[nt][r].size())
                            {
                                vector<string> a(rules[nt][r].begin(), rules[nt][r].begin() + len - 1);
                                vector<string> b(rules[nt][j].begin(), rules[nt][j].begin() + len - 1);

                                if (a == b)
                                {
                                    vector<string> vec(rules[nt][r].begin() + len - 1, rules[nt][r].end());
                                    rules[newnt].push_back(vec);
                                }
                            }
                        }

                        int r = j;
                        vector<string> vec(rules[nt][j].begin(), rules[nt][j].begin() + len - 1);
                        while (r < rules[nt].size())
                        {
                            if (len <= rules[nt][r].size())
                            {
                                vector<string> a(rules[nt][r].begin(), rules[nt][r].begin() + len - 1);
                                vector<string> b(vec.begin(), vec.begin() + len - 1);

                                if (a == b)
                                {
                                    rules[nt].erase(rules[nt].begin() + r);
                                    continue;
                                }
                            }

                            ++r;
                        }

                        vec.push_back(newnt);
                        rules[nt].push_back(vec);

                        j = len = last = curr = 0;
                    }
                    else
                    {
                        ++j;
                    }
                }
                else
                {
                    last = curr;
                    curr = 0;
                }
            }
            else
            {
                len = last = 0;
                ++j;
            }
        }

        ++i;
    }
}

Node *parse(string str)
{
    Node *newnode = new Node(++count, str);
    // cout << str << " -> " << endl;

    for (int i = 0; i < rules[str].size(); i++)
    {
        int j = 0;
        vector<Node *> vec;
        for (j = 0; j < rules[str][i].size(); j++)
        {
            string c = rules[str][i][j];

            if (rules.find(c) != rules.end())
            {
                Node *p = parse(c);
                if (p == NULL)
                    break;
                else
                    vec.push_back(p);
            }
            else if (c == "id")
            {
                int l = pos;
                if (isalpha(word[l]))
                    ++l;
                else
                {
                    break;
                }

                while (isalpha(word[l]) || isdigit(word[l]))
                    ++l;
                pos = l;
                // cout << "pos became " << pos << endl;

                Node *idnode = new Node(++count, "id");
                vec.push_back(idnode);
            }
            else if (c[0] == word[pos] || c == EPSILON)
            {
                if (c != EPSILON)
                {
                    ++pos;
                    // cout << "pos became " << pos << endl;
                }
                Node *cnode = new Node(++count, c);
                vec.push_back(cnode);
            }
            else
            {
                break;
            }
        }

        for (int k = 0; k < rules[str][i].size(); k++)
        {
            string c = rules[str][i][k];
            // cout << c;
        }
        // cout << " ~ pos : " << pos << endl;

        // cout << "i - " << i << ", j - " << j << " : " << str << endl;

        if (j == rules[str][i].size())
        {
            // cout << "yup" << endl;
            newnode->links = vec;
            return newnode;
        }
    }

    // cout << "Gone" << endl;

    return NULL;
}

void drawTree(Node *root)
{
    if (root == NULL)
        return;

    ofstream dot_file("graph.dot");

    dot_file << "digraph G {\n";
    dot_file << "node [shape=\"circle\"]\n";

    queue<Node *> bfs;
    dot_file << "\t" << root->ind << " [label=\"" << root->label << "\"]\n";
    bfs.push(root);

    while (!bfs.empty())
    {
        Node *curr = bfs.front();
        bfs.pop();

        for (int i = 0; i < curr->links.size(); i++)
        {
            dot_file << "\t" << curr->links[i]->ind << " [label=\"" << curr->links[i]->label << "\"]\n";
            dot_file << "\t" << curr->ind << " -> " << curr->links[i]->ind << "\n";
            bfs.push(curr->links[i]);
        }
    }
    dot_file << "}\n";

    dot_file.close();
}

int main()
{
    ifstream inp("input.txt");

    string start, line;
    getline(inp, start);

    while (getline(inp, line))
    {
        noteRule(line);
    }

    if (false)
    {
        std::cout << "Given Grammar : \n------------------------" << endl;
        printRules(true);

        simplifyRules();
        std::cout << "\n------------------------" << endl;
        printRules(true);

        eliminateLRecursion();

        std::cout << "\nLeft recursion removed : \n------------------------" << endl;
        printRules(true);

        leftFactorize();
        std::cout << "\nLeft Factorized Grammar : \n------------------------" << endl;
        printRules(true);
    }

    int choice = 1;

    while (true)
    {
        std::cout << "\n1. Choose 1 to test words over grammar\n2. Choose 0 to exit\n------------------------\nChoice: ";
        cin >> choice;

        if (!choice)
            break;

        std::cout << "Enter your word : ";
        cin >> word;
        pos = count = 0;

        Node *root = parse(start);
        // cout << "pos - " << pos << endl;
        if (root == NULL || pos != word.size())
            std::cout << "Does not match the grammar" << endl;
        else
        {
            cout << "Matches!\n";
            drawTree(root);

            std::system("dot -Tpng graph.dot -o graph.png");
            std::cout << "✅ Derivation tree generated\n";

            std::system("xdg-open graph.png");
        }
    }

    inp.close();

    return 0;
}