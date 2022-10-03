#include <iostream>
#include <vector>

#define EPSILON "\u2208"

using namespace std;

// split string with specified delimiter string
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

// trims whitespaces from both ends of string
string trim(string str)
{
    int i = 0;
    while (i < str.size() && (str[i] == ' ' || str[i] == '\t'))
        ++i;

    if (i == str.size())
        return "";

    str = str.substr(i);

    i = str.size() - 1;
    while (i >= 0 && (str[i] == ' ' || str[i] == '\t'))
        --i;

    if (i < 0)
        return "";

    str = str.substr(0, i + 1);

    return str;
}