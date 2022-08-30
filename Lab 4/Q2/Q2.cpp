#include <iostream>
#include <fstream>

#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_set<string> non_terminals;
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

    non_terminals.insert(r[0]);

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
        for (auto el : rules)
        {
            string nt = el.first;
            vector<vector<string>> c = el.second;

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
        for (auto el : rules)
        {
            string nt = el.first;
            vector<vector<string>> c = el.second;

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

int main()
{
    ifstream inp("input.txt");

    string line;
    while (getline(inp, line))
    {
        noteRule(line);
    }

    cout << "Given Grammar : \n------------------------" << endl;
    printRules();

    leftFactorize();
    cout << "\nLeft Factorized Grammar : \n------------------------" << endl;
    printRules(true);

    inp.close();

    return 0;
}