//  LAB ASSIGNMENT - 2
//  — — — — — — — — — — — — — — — — — — — — — — — — — — — — — —
// | In this assignment, you will work on implementing a       |
// | lexical analyzer in your preferred programming language.  |
// | We will consider some basic constructs (e.g., if-else     |
// | statements), understand what are the different tokens     |
// | (and patterns for the tokens) in the considered language/ |
// | constructs. Implement a lexical analyzer (using any       |
// | programming language) for the considered tokens/patterns. |
// |                                                           |
// | Your program should take a statement as input and return  |
// | the sequence of tokens as output.                         |
//  — — — — — — — — — — — — — — — — — — — — — — — — — — — — — —

#define DEBUG 0

#include <iostream>

using namespace std;

bool isDigit(char ch)
{
    return (ch >= '0' && ch <= '9');
}

bool isLetter(char ch)
{
    return (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z');
}

bool isRelopChar(char ch)
{
    return (ch == '>' || ch == '<' || ch == '=');
}

bool isWhitespace(char ch)
{
    return (ch == ' ' || ch == '\t' || ch == '\n');
}

string stripWhitespaces(string str, int &p)
{
    while (p < str.size() && isWhitespace(str[p]))
        ++p;

    return "";
}

string matchKeywordOrId(string str, int &p)
{
    const string keywords[] = {"if", "then", "else"};
    for (auto word : keywords)
    {
        if (word.size() + p <= str.size())
        {
            if (str.substr(p, word.size()) == word && (p + word.size() == str.size() || isWhitespace(str[p + word.size()])))
            {
                string ans = "( ";
                ans += word;
                ans += " )\n";
                p += word.size();

                return ans;
            }
        }
    }

    int init = p++;
    while (p < str.size() && (isDigit(str[p]) || isLetter(str[p])))
        ++p;

    string res = "( id, ";
    res += str.substr(init, p - init);
    res += " )\n";

    return res;
}

string matchNumber(string str, int &p)
{
    int init = p;
    while (p < str.size() && isDigit(str[p]))
        ++p;

    if (p < str.size())
    {
        if (str[p] == '.')
        {
            ++p;
            while (p < str.size() && isDigit(str[p]))
                ++p;
        }

        if (str[p] == 'E')
        {
            ++p;
            if (p < str.size() && (str[p] == '+' || str[p] == '-'))

                ++p;

            while (p < str.size() && isDigit(str[p]))
                ++p;
        }
    }

    string res = "( number, ";
    res += str.substr(init, p - init);
    res += " )\n";

    return res;
}

string matchRelop(string str, int &p)
{
    string res = "( relop, ";
    if (str[p] == '<')
    {
        ++p;
        if (str[p] == '=')
        {
            ++p;
            res += "<= )\n";
        }
        else if (str[p] == '>')
        {
            ++p;
            res += "<> )\n";
        }
        else
            res += "< )\n";
    }
    else if (str[p] == '=')
    {
        ++p;
        res += "= )\n";
    }
    else if (str[p] == '>')
    {
        ++p;
        if (str[p] == '=')
        {
            ++p;
            res += ">= )\n";
        }
        else
            res += "> )\n";
    }

    return res;
}

string analyze(string str, int &p)
{
    if (p >= str.size())
        return "";

    char first = str[p];
    if (isWhitespace(first))
    {
        return stripWhitespaces(str, p);
    }
    else if (isLetter(first))
    {
        return matchKeywordOrId(str, p);
    }
    else if (isDigit(first))
    {
        return matchNumber(str, p);
    }
    else if (isRelopChar(first))
    {
        return matchRelop(str, p);
    }

    return "";
}

int main()
{
    cout << "Enter any statement for Lexical analysis:\n";
    string inp_stmt;
    getline(cin, inp_stmt);

    int pos = 0;
    while (pos < inp_stmt.size())
    {
        cout << analyze(inp_stmt, pos);
    }
}