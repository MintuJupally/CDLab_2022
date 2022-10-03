#include <iostream>
#include <unordered_map>
#include <vector>

#include "utils.hpp"

using namespace std;

// Class to store and represent Context Free Grammar
class Grammar
{
public:
    unordered_map<string, vector<vector<string>>> rules;
    unordered_map<string, bool> terminals;
    unordered_map<string, bool> non_terminals;
    string start;

    // initialise grammar with terminals, non terminals and start state
    void initialize(vector<string> t, vector<string> nt, string s)
    {
        start = s;
        for (auto el : t)
            terminals[el] = true;
        for (auto el : nt)
            non_terminals[el] = true;
    }

    // clear grammar definition
    void clear()
    {
        rules.clear();
        terminals.clear();
        non_terminals.clear();
        start = "";
    }

    // note terminals of the grammar
    void noteTerminals(vector<string> t)
    {
        for (int i = 0; i < t.size(); i++)
        {
            terminals[t[i]] = true;
        }
    }

    // note non terminals of the grammar
    void noteNonTerminals(vector<string> nt)
    {
        for (int i = 0; i < nt.size(); i++)
        {
            non_terminals[nt[i]] = true;
        }
    }

    // add production rule by non terminal to the grammar
    void addRule(string nt, string rule_str)
    {
        if (terminals.find(nt) != terminals.end())
        {
            throw("Cannot add rule to a terminal");
        }

        non_terminals[nt] = true;

        vector<string> rs = split(rule_str, "|");
        int pos = 0;
        for (int i = 0; i < rs.size(); i++)
        {
            rules[nt].push_back(split(trim(rs[i]), " "));
        }
    }

    // add production rule to the grammar
    void addRule(string rule)
    {
        vector<string> r = split(rule, "->");

        if (r.size() != 2)
            throw("Invalid production rule: " + rule);

        addRule(trim(r[0]), trim(r[1]));
    }

    // print grammar production rules
    void printRules(bool flag = true)
    {
        if (!flag)
            for (auto el : non_terminals)
            {
                string nt = el.first;
                vector<vector<string>> c = rules[nt];

                for (int i = 0; i < c.size(); i++)
                {
                    std::cout << nt << " -> ";
                    for (int j = 0; j < c[i].size(); j++)
                    {
                        std::cout << c[i][j] << " ";
                    }
                    std::cout << endl;
                }
            }
        else
            for (auto el : non_terminals)
            {
                string nt = el.first;
                vector<vector<string>> c = rules[nt];

                std::cout << nt << " -> ";
                for (int i = 0; i < c.size(); i++)
                {
                    for (int j = 0; j < c[i].size(); j++)
                    {
                        std::cout << c[i][j] << " ";
                    }
                    if (i != c.size() - 1)
                        std::cout << "| ";
                }
                std::cout << endl;
            }
    }
};

class GrammarHelper
{
private:
    static Grammar simplifyRules(Grammar g)
    {
        unordered_map<string, bool> vis;

        for (auto el : g.non_terminals)
        {
            string nt = el.first;
            vector<vector<string>> curr = g.rules[nt], updated;

            for (int i = 0; i < curr.size(); i++)
            {
                if (vis.find(curr[i][0]) == vis.end())
                {
                    updated.push_back(curr[i]);
                    continue;
                }

                string rnt = curr[i][0];

                for (int j = 0; j < g.rules[rnt].size(); j++)
                {
                    vector<string> upd(g.rules[rnt][j]);
                    upd.insert(upd.end(), curr[i].begin() + 1, curr[i].end());
                    updated.push_back(upd);
                }
            }

            if (curr.size() > 0)
                g.rules[nt] = updated;

            vis[nt] = true;
        }

        return g;
    }

    static Grammar eliminateLRecursion(Grammar g)
    {
        unordered_map<string, vector<vector<string>>> urules;
        vector<string> non_terminals;
        for (auto el : g.non_terminals)
            non_terminals.push_back(el.first);

        int len = g.non_terminals.size();
        for (int e = 0; e < len; e++)
        {
            string nt = non_terminals[e];
            vector<vector<string>> curr = g.rules[nt];

            bool hasLR = false;
            for (int i = 0; i < curr.size() && !hasLR; i++)
            {
                if (curr[i][0] == nt)
                    hasLR = true;
            }

            if (hasLR)
            {
                if (g.non_terminals.find(nt + "\'") == g.non_terminals.end())
                {
                    non_terminals.push_back(nt + "\'");
                    g.non_terminals[nt + "\'"] = true;
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
                urules[nt] = g.rules[nt];
            }
        }

        g.rules = urules;

        return g;
    }

    static Grammar leftFactorize(Grammar g)
    {
        vector<string> nts;
        for (auto rule : g.rules)
            nts.push_back(rule.first);

        int i = 0;
        while (i < nts.size())
        {
            string nt = nts[i];

            int j = 0;
            int len = 0;
            int last = 0, curr = 0;

            while (j < g.rules[nt].size())
            {
                if (len == 0 || last >= 1)
                {
                    for (int k = j + 1; k < g.rules[nt].size(); k++)
                    {
                        if (len < g.rules[nt][k].size())
                        {
                            if (g.rules[nt][k][len] == g.rules[nt][j][len])
                                ++curr;
                        }
                    }
                    ++len;

                    if (curr == 0 || len >= g.rules[nt][j].size() || len != 1 && last > curr)
                    {
                        if (len > 1)
                        {
                            int d = 0;
                            string newnt = nt;
                            while (g.rules.find(newnt) != g.rules.end())
                                newnt += "\'";
                            nts.push_back(newnt);
                            g.noteNonTerminals({newnt});

                            for (int r = j; r < g.rules[nt].size(); r++)
                            {
                                if (len - 1 <= g.rules[nt][r].size())
                                {
                                    vector<string> a(g.rules[nt][r].begin(), g.rules[nt][r].begin() + len - 1);
                                    vector<string> b(g.rules[nt][j].begin(), g.rules[nt][j].begin() + len - 1);

                                    if (a == b)
                                    {
                                        vector<string> vec(g.rules[nt][r].begin() + len - 1, g.rules[nt][r].end());
                                        if (vec.size() > 0)
                                            g.rules[newnt].push_back(vec);
                                        else
                                            g.rules[newnt].push_back({EPSILON});
                                    }
                                }
                            }

                            int r = j;
                            vector<string> vec(g.rules[nt][j].begin(), g.rules[nt][j].begin() + len - 1);
                            while (r < g.rules[nt].size())
                            {
                                if (len - 1 <= g.rules[nt][r].size())
                                {
                                    vector<string> a(g.rules[nt][r].begin(), g.rules[nt][r].begin() + len - 1);
                                    vector<string> b(vec.begin(), vec.begin() + len - 1);

                                    if (a == b)
                                    {
                                        g.rules[nt].erase(g.rules[nt].begin() + r);
                                        continue;
                                    }
                                }

                                ++r;
                            }

                            vec.push_back(newnt);
                            g.rules[nt].push_back(vec);

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

        return g;
    }

public:
    static Grammar eliminateLR(Grammar g)
    {
        Grammar gsimp = simplifyRules(g);
        Grammar gnoLR = eliminateLRecursion(g);

        return gnoLR;
    }

    static Grammar LFactorize(Grammar g)
    {
        return leftFactorize(g);
    }
};