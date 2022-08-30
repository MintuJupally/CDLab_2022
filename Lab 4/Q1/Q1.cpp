#include <iostream>
#include <fstream>
#include <sstream>
#include <stdarg.h>

#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

vector<string> non_terminals;
unordered_set<string> nt_exists;
unordered_map<string, vector<vector<string>>> rules;

void print(string str)
{
    cout << str << endl;
}

template <typename... Strings>
void print(string str, Strings... strings)
{
    cout << str << " ";
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
            while (pos < rs[i].size() && rs[i][pos] == '\'')
                ++pos;

            rules[r[0]].back().push_back(rs[i].substr(l, pos - l));
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
                cout << nt << "->";
                for (int j = 0; j < c[i].size(); j++)
                {
                    cout << c[i][j];
                }
                cout << endl;
            }
        }
    else
        for (auto el : non_terminals)
        {
            string nt = el;
            vector<vector<string>> c = rules[nt];

            cout << nt << "->";
            for (int i = 0; i < c.size(); i++)
            {
                for (int j = 0; j < c[i].size(); j++)
                {
                    cout << c[i][j];
                }
                if (i != c.size() - 1)
                    cout << "|";
            }
            cout << endl;
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
            // cout << nt << " => ";
            if (vis.find(curr[i][0]) == vis.end())
            {
                // cout << "nope" << endl;
                updated.push_back(curr[i]);
                continue;
            }

            // cout << "yup" << endl;
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

            urules[nt + "\'"].push_back({"\u2208"});
        }
        else
        {
            urules[nt] = rules[nt];
        }
    }

    rules = urules;
}

int main()
{
    ifstream inp("input.txt");

    string line;
    while (getline(inp, line))
    {
        noteRule(line);
    }

    cout << "Given Grammar : \n------------------------" << endl;
    printRules(true);

    simplifyRules();
    // cout << "\n------------------------" << endl;
    // printRules(true);

    eliminateLRecursion();

    cout << "\nLeft recursion removed : \n------------------------" << endl;
    printRules(true);

    inp.close();

    return 0;
}