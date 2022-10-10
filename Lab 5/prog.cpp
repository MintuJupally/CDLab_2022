#define DEBUG 0

#include <iostream>
#include <fstream>
#include <algorithm>

#include "colors.hpp"
#include "parser/parser.hpp"

using namespace std;

Grammar G;
Grammar G_LL;

Parser P;

void printMenu()
{
    cout << "1. Load new grammar from Grammar.txt\n";
    if (P.first.size() > 0)
        cout << "2. Test data from input.txt against Grammar\n";
    cout << "0. Exit\n----------------------------------------------\n";
}

void printFirsts(ofstream &out)
{
    out << "## FIRSTS" << endl;
    for (auto el : P.first)
    {
        string nt = el.first;

        out << nt << " => ";

        vector<string> els;

        for (auto e : el.second)
        {
            els.push_back(e.first);
        }

        sort(els.begin(), els.end());

        for (auto t : els)
        {
            out << t << " ";
        }

        out << endl;
    }
}

void printFollows(ofstream &out)
{
    out << "## FOLLOWS" << endl;

    for (auto el : P.follow)
    {
        string nt = el.first;

        out << nt << " => ";

        vector<string> els;

        for (auto e : el.second)
        {
            els.push_back(e.first);
        }

        sort(els.begin(), els.end());

        for (auto t : els)
        {
            out << t << " ";
        }

        out << endl;
    }
}

// write grammar definition into file
void writeGrammar(Grammar g)
{
    ofstream outp("output/GrammarLL.txt");
    if (outp.fail())
    {
        system("mkdir output");
        outp = ofstream("output/GrammarLL.txt");
    }

    int count = 0;
    for (auto el : g.terminals)
    {
        outp << el.first;
        if (++count != g.terminals.size())
            outp << " ";
        else
            outp << endl;
    }

    count = 0;
    for (auto el : g.non_terminals)
    {
        outp << el.first;
        if (++count != g.non_terminals.size())
            outp << " ";
        else
            outp << endl;
    }

    outp << g.start << endl;

    for (auto el : g.non_terminals)
    {
        string nt = el.first;
        vector<vector<string>> c = g.rules[nt];

        outp << nt << " -> ";
        for (int i = 0; i < c.size(); i++)
        {
            for (int j = 0; j < c[i].size(); j++)
            {
                outp << c[i][j];
                if (j < c[i].size() - 1)
                    outp << " ";
            }
            if (i != c.size() - 1)
                outp << " | ";
        }
        outp << endl;
    }

    outp.close();
}

// write first and follow into file
void writeFirstFollow()
{
    ofstream out("output/First-Follow.txt");
    if (out.fail())
    {
        system("mkdir output");
        out = ofstream("output/First-Follow.txt");
    }

    printFirsts(out);
    out << endl;

    printFollows(out);
    out << endl;

    out.close();
}

// loads grammar definition from file
void loadGrammar()
{
    G.clear();
    P.clear();

    ifstream inp("Grammar.txt");

    string terminals, non_terminals, start, line;
    getline(inp, terminals);
    getline(inp, non_terminals);
    getline(inp, start);

    G.initialize(split(terminals, " "), split(non_terminals, " "), start);

    while (getline(inp, line))
    {
        try
        {
            G.addRule(line);
        }
        catch (string err)
        {
            cout << err << endl;
        }
    }

    inp.close();

    Grammar g_noLR = GrammarHelper::eliminateLR(G);
    // cout << endl;
    // g_noLR.printRules();

    Grammar g_noLR_LF = GrammarHelper::LFactorize(g_noLR);
    // cout << endl;
    // g_noLR_LF.printRules();

    G_LL = g_noLR_LF;

    bool isLL1 = P.initialize(g_noLR_LF);

    writeGrammar(G_LL);
    if (!isLL1)
    {
        cout << red << "! Given grammar is not LL(1)" << reset << endl;
        cout << yellow << "✓ Written left factorized and left-recursion-free grammar into 'output/GrammarLL.txt'" << reset << endl;
    }
    else
        cout << yellow << "✓ Written LL(1) grammar into 'output/GrammarLL.txt'" << reset << endl;

    writeFirstFollow();
    cout << yellow << "✓ Written First and Follow of the grammar into 'output/First-Follow.txt'" << reset << endl;

    cout << yellow << "Parse Table can be checked in 'output/Parse-Table.txt'" << reset << endl;

    if (!isLL1)
        P.clear();
}

void parseInput()
{
    vector<string> inp;

    system("lex -o tokenizer/lex.yy.c tokenizer/lexa.l && gcc tokenizer/lex.yy.c -o lexa && ./lexa < input.txt > tokenizer/lexinp.txt");

    ifstream in("tokenizer/lexinp.txt");

    string token;
    while (getline(in, token))
    {
        inp.push_back(token);
    }

    bool match = P.parse(inp);

    if (match)
        cout << green << "Matches!";
    else
        cout << red << "Does not match!";
    cout << reset << endl;
}

int main()
{
    while (true)
    {
        try
        {
            printMenu();

            int choice = 0;
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice)
            {
            case 0:
                return 0;
            case 1:
                loadGrammar();
                break;
            case 2:
                if (P.first.size() > 0)
                {
                    parseInput();
                    break;
                }
            default:
                cout << "Invalid choice" << endl;
            }
        }
        catch (string err)
        {
            cout << err << endl;
        }

        cout << endl;
    }

    return 0;
}