#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iomanip>

#include "grammar.hpp"

using namespace std;

class Parser
{
private:
    unordered_map<string, int> state;

    void calculateFirst(string s)
    {
        // if first is already calculated
        if (first.find(s) != first.end())
            return;

        for (int i = 0; i < G.rules[s].size(); i++)
        {
            for (int j = 0; j < G.rules[s][i].size(); i++)
            {
                string curr = G.rules[s][i][j];
                // if terminal or epsilon, need not go further in production rule
                if (curr == EPSILON || G.terminals.find(curr) != G.terminals.end())
                {
                    first[s][curr] = true;
                    break;
                }

                calculateFirst(curr);

                bool flag = false;
                for (auto el : first[curr])
                {
                    string c = el.first;
                    if (c == EPSILON)
                        flag = true;
                    else
                    {
                        first[s][c] = true;
                    }
                }

                if (!flag)
                    break;
            }
        }
    }

    void evaluateFirsts()
    {
        for (auto el : G.non_terminals)
        {
            string nt = el.first;
            calculateFirst(nt);
        }
    }

    void calculateFollow(string s)
    {
        // cout << s << " => " << endl;

        if (follow.find(s) != follow.end())
            return;

        state[s] = 1;
        if (s == G.start)
        {
            follow[s]["$"] = true;
            // cout << " 1 ---------- " << s << " "
            //  << "$" << endl;
        }

        for (auto r : G.rules)
        {
            string nt = r.first;
            for (int i = 0; i < r.second.size(); i++)
            {
                // cout << s << " - " << nt << " :\t";
                for (int j = 0; j < r.second[i].size(); j++)
                {
                    // cout << r.second[i][j] << " ";
                }
                // cout << endl;

                int len = r.second[i].size();
                int pos = 0;

                // iterating until s is found
                while (pos < len)
                {
                    string curr = r.second[i][pos];
                    if (curr == s)
                        break;
                    ++pos;
                }

                if (pos == len)
                    continue;

                // cout << "\t" << s << " ~ " << pos << " " << len << endl;

                ++pos;

                // iterating through terminals and non terminals following the 's' match in the production rule
                while (pos < len)
                {
                    string curr = r.second[i][pos];
                    // cout << "\t" << len << " : " << pos << " - " << curr << endl;
                    if (G.terminals.find(curr) != G.terminals.end())
                    {
                        follow[s][curr] = true;
                        // cout << " 2 ---------- " << s << " "
                        //  << curr << endl;
                        break;
                    }

                    bool flag = false;

                    for (auto el : first[curr])
                    {
                        // cout << " 3 ---------- " << s << " "
                        //  << el.first << endl;
                        if (el.first == EPSILON)
                            flag = true;
                        else
                        {
                            follow[s][el.first] = true;
                        }
                    }

                    if (!flag)
                        break;

                    ++pos;
                }

                if (pos < len)
                    continue;

                // cout << "\t" << r.second[i][pos - 1] << endl;

                if (nt != s)
                {
                    if (state[nt] != 1)
                        calculateFollow(nt);

                    for (auto el : follow[nt])
                    {
                        follow[s][el.first] = true;
                        // cout << " 4 ---------- " << s << " "
                        //      << el.first << endl;
                    }
                }
            }
        }

        state[s] = 2;
        // cout << "\t\t------------------------------\n\t\t" << s << " - DONE - ";
        // for (auto el : follow[s])
        // cout << el.first << " ";
        // cout << endl;
    }

    void evaluateFollows()
    {
        for (auto el : G.non_terminals)
        {
            string nt = el.first;
            calculateFollow(nt);
        }
    }

    void printTable()
    {
        int n = G.non_terminals.size() + 1; // num of rows
        int m = G.terminals.size() + 1;     // num of columns

        vector<vector<vector<string>>> tab(n, vector<vector<string>>(m));

        tab[0][0].push_back(" ");

        vector<string> r, c;
        for (auto el : G.non_terminals)
        {
            r.push_back(el.first);
        }

        for (auto el : G.terminals)
        {
            c.push_back(el.first);
        }

        sort(r.begin(), r.end());
        sort(c.begin(), c.end());
        c.push_back("$");

        // row head and column head cells index vs terminal/non-terminal map
        vector<string> rh(n), ch(m);
        for (int i = 0; i < n - 1; i++)
        {
            rh[i + 1] = r[i];
            tab[i + 1][0].push_back(r[i]);
        }
        for (int i = 0; i < m - 1; i++)
        {
            ch[i + 1] = c[i];
            tab[0][i + 1].push_back(c[i]);
        }

        vector<int> roh(n, 1), cow(m, 1);

        for (int i = 0; i < G.non_terminals.size(); i++)
        {
            if (cow[0] < r[i].size())
                cow[0] = r[i].size();
        }

        for (int i = 0; i < G.terminals.size(); i++)
        {
            if (cow[i + 1] < c[i].size())
                cow[i + 1] = c[i].size();
        }

        // calculating row heights and col widths
        for (int i = 1; i < n; i++)
        {
            for (int j = 1; j < m; j++)
            {
                int count = 0;

                for (auto &el : table[rh[i]][ch[j]])
                {
                    string nt = el.first;
                    for (auto &rule : el.second)
                    {
                        ++count;
                        string s = " ";
                        s += nt;
                        s += " => ";

                        for (auto &ltr : rule)
                        {
                            s += ltr;
                            s += " ";
                        }

                        tab[i][j].push_back(s);

                        // if (s.substr(s.size() - 4, 3) == EPSILON)
                        // {
                        //     if (s.size() - 2 > cow[j])
                        //         cow[j] = s.size() - 2;
                        // }
                        // else
                        if (s.size() > cow[j])
                            cow[j] = s.size();
                    }
                }

                if (count > roh[i])
                    roh[i] = count;
            }
        }

        // writing table to file
        ofstream out("output/Parse-Table.txt");

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < roh[j]; j++)
            {
                for (int k = 0; k < m; k++)
                {

                    if (j < tab[i][k].size())
                    {
                        string curr = tab[i][k][j];
                        if (curr.size() > 4 && curr.substr(curr.size() - 4, 3) == EPSILON)
                            out << "|" << setw(cow[k] + 2) << left << tab[i][k][j];
                        else
                            out << "|" << setw(cow[k]) << left << tab[i][k][j];
                    }
                    else
                    {
                        out << "|" << setw(cow[k]) << " ";
                    }
                }

                out << "|" << endl;
                for (int col = 0; col < m; col++)
                {
                    out << "+";
                    for (int x = 0; x < cow[col]; x++)
                        out << "-";
                }
                out << "+" << endl;
            }
        }

        out.close();
    }

    void tabulate()
    {
        for (auto nti : G.non_terminals)
        {
            string nt = nti.first;
            for (auto &rule : G.rules[nt])
            {
                vector<string> req; // required terminals from first (or FOLLOW) where the production rule has to be added in the table
                unordered_set<string> vis;

                bool hasEpsilon = false;
                for (int i = 0; i < rule.size(); i++)
                {
                    hasEpsilon = false;
                    if (G.terminals.find(rule[i]) != G.terminals.end())
                    {
                        if (vis.find(rule[i]) == vis.end())
                        {
                            req.push_back(rule[i]);
                            vis.insert(rule[i]);
                        }
                        break;
                    }

                    if (rule[i] == EPSILON)
                        hasEpsilon = true;
                    else
                        for (auto ch : first[rule[i]])
                        {
                            string curr = ch.first;
                            if (curr == EPSILON)
                            {
                                hasEpsilon = true;
                            }
                            else if (vis.find(curr) == vis.end())
                            {
                                req.push_back(curr);
                                vis.insert(curr);
                            }
                        }

                    if (!hasEpsilon)
                        break;
                }

                if (hasEpsilon)
                {
                    // production rule can derive epsilon, hence the terminals in follow must also be considered
                    for (auto ti : follow[nt])
                    {
                        string t = ti.first;
                        req.push_back(t);
                    }
                }

                for (auto t : req)
                {
                    table[nt][t][nt].push_back(rule);
                }
            }
        }

        printTable();
    }

public:
    unordered_map<string, unordered_map<string, bool>> first;
    unordered_map<string, unordered_map<string, bool>> follow;
    unordered_map<string, unordered_map<string, unordered_map<string, vector<vector<string>>>>> table;
    Grammar G;

    // initialize parser with given grammar
    void initialize(Grammar g)
    {
        G = g;

        evaluateFirsts();
        evaluateFollows();
        tabulate();
    }

    bool parse(vector<string> tokens)
    {
        stack<string> stk;
        stk.push("$");
        stk.push(G.start);

        tokens.push_back("$");

        int mlen = 0;
        for (int i = 0; i < tokens.size(); i++)
        {
            if (mlen < tokens[i].size())
                mlen = tokens[i].size();
        }

        int pos = 0;
        while (pos < tokens.size() && !stk.empty())
        {
            // cout << right << setw(5) << pos;
            // cout << " - " << setw(mlen + 5) << left << tokens[pos];

            string top = stk.top();

            stack temp = stk;
            string s = "";
            while (!temp.empty())
            {
                s = temp.top() + s;
                s = " " + s;
                temp.pop();
            }
            // cout << s << "\t";

            if (G.terminals.find(top) != G.terminals.end() || top == "$")
            {
                if (top == tokens[pos])
                {
                    // cout << "match" << endl;
                    stk.pop();
                    ++pos;
                    continue;
                }
                else
                {
                    return false;
                }
            }

            vector<vector<string>> cell = table[top][tokens[pos]][top];

            if (cell.size() == 0)
                return false;

            vector<string> rule = cell[0];

            stk.pop();

            for (int i = rule.size() - 1; i >= 0; i--)
            {
                if (rule[i] != EPSILON)
                    stk.push(rule[i]);
            }

            // cout << endl;
        }

        return pos >= tokens.size() && stk.empty();
    }
};