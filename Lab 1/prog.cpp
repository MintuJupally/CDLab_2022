//  LAB ASSIGNMENT - 1
//  — — — — — — — — — — — — — — — — — — — — — — — — — — — — — —
// | Write a program to generate NFA from a given Regular      |
// | Expression. Consider some alphabet with 2-3 elements.     |
// |                                                           |
// | Given any Regular Expression over the alphabet as input,  |
// | your program should generate the corresponding NFA.       |
//  — — — — — — — — — — — — — — — — — — — — — — — — — — — — — —

#define DEBUG 0

#include <algorithm>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <stack>
#include <stdlib.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

vector<char> alphabet; // Vector of alphabet
string regexp = "";    // Current regular expression
string m_regexp = "";  // Current expanded regular expression

unordered_map<char, int> precedence = {{'(', 4}, {')', 4}, {'*', 3}, {'.', 2}, {'+', 1}, {'|', 1}};

char black[] = "\033[0;30m";
char red[] = "\033[0;31m";
char green[] = "\033[0;32m";
char yellow[] = "\033[0;33m";
char blue[] = "\033[0;34m";
char purple[] = "\033[0;35m";
char cyan[] = "\033[0;36m";
char white[] = "\033[0;37m";
char reset[] = "\033[0m";

class Node
{
public:
    int type; // 0 - start state, 1 - accept state, -1 - normal states
    int state;
    unordered_map<char, vector<Node *>> transitions;

    Node(int typ = -1)
    {
        type = typ;
    }

    void setType(int typ)
    {
        if (typ < -1 || typ > 1)
        {
            cout << "Invalid Node Type\n";
            return;
        }

        type = typ;
    }

    void addTransition(char alp, Node *dest)
    {
        transitions[alp].push_back(dest);
    }
};

class NFA
{
public:
    Node *start;
    vector<Node *> accept;

    NFA(char alp)
    {
        Node *start = new Node(0), *end = new Node(1);

        start->addTransition(alp, end);

        this->start = start;
        this->accept.push_back(end);
    }
};

bool isAlphabet(char ch)
{
    return find(alphabet.begin(), alphabet.end(), ch) != alphabet.end();
}

bool isOperator(char ch)
{
    return (ch == '(' || ch == ')' || ch == '*' || ch == '.' || ch == '|' || ch == '+');
}

bool isValidRegExp(string exp)
{
#if DEBUG == 1
    cout << exp << endl;
#endif

    // Alphabet check
    for (int i = 0; i < exp.size(); i++)
    {
        if (isOperator(exp[i]) || isAlphabet(exp[i]))
            continue;

        return false;
    }

    // Regular expression validation
    stack<char> op_stack;
    stack<string> exp_stack;

    for (int i = 0; i < exp.size(); i++)
    {
        const char curr = exp[i];

        if (curr == '*')
        {
            if (exp_stack.empty())
                return false;

            string a = exp_stack.top();
            exp_stack.pop();

            exp_stack.push(a + '*');
        }
        else if (isOperator(curr))
        {
            if (curr == '(' || (curr != ')' && op_stack.empty()))
            {
                op_stack.push(curr);
            }
            else if (curr == ')')
            {
                if (op_stack.empty())
                    return false;

                while (!op_stack.empty() && op_stack.top() != '(')
                {
                    const char op = op_stack.top();
                    op_stack.pop();

                    if (exp_stack.size() < 2)
                    {
                        return false;
                    }

                    string b = exp_stack.top();
                    exp_stack.pop();

                    string a = exp_stack.top();
                    exp_stack.pop();

                    exp_stack.push((a + op) + b);
                }

                if (op_stack.empty())
                    return false;

                op_stack.pop();
            }
            else if (precedence[curr] <= precedence[op_stack.top()])
            {
                while (!op_stack.empty() && precedence[curr] <= precedence[op_stack.top()])
                {
                    const char op = op_stack.top();
                    if (op == '(')
                        break;

                    op_stack.pop();

                    if (exp_stack.size() < 2)
                    {
                        return false;
                    }

                    string b = exp_stack.top();
                    exp_stack.pop();

                    string a = exp_stack.top();
                    exp_stack.pop();

                    exp_stack.push((a + op) + b);
                }

                op_stack.push(curr);
            }
            else
            {
                op_stack.push(curr);
            }
        }
        else
        {
            string curr_st = "";
            curr_st += curr;
            exp_stack.push(curr_st);
        }
    }

    while (!op_stack.empty())
    {
        const char op = op_stack.top();
        op_stack.pop();

        if (exp_stack.size() < 2)
        {
            return false;
        }

        string b = exp_stack.top();
        exp_stack.pop();

        string a = exp_stack.top();
        exp_stack.pop();

        exp_stack.push((a + op) + b);
    }

    if (exp_stack.size() != 1)
    {
        return false;
    }

    return true;
}

void readAlphabet()
{
    cout << "Enter your alphabet (space separated) : \n";

    string inp_alphabet;
    getline(cin, inp_alphabet);

    unordered_set<char> alphabet_set; // Discards duplicates in alphabet if any

    for (int i = 0; i < inp_alphabet.size(); i++)
    {
        if (inp_alphabet[i] != ' ')
        {
            alphabet_set.insert(inp_alphabet[i]);
        }
    }

    alphabet.insert(alphabet.end(), alphabet_set.begin(), alphabet_set.end());

#if DEBUG == 1
    cout << "YOUR ALPHABET\n";
    for (int i = 0; i < alphabet.size(); i++)
        cout << alphabet[i] << endl;
#endif
}

void readRegExp()
{
    cout << "Enter a regular expression : \n";

    string input = "";
    getline(cin, input);
    getline(cin, input);

    string regexp_pre1;
    string regexp_pre;

    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == ' ')
            continue;

        regexp_pre1 += input[i];
    }

    if (regexp_pre1.size() > 0)
        regexp_pre = regexp_pre1[0];
    for (int i = 1; i < regexp_pre1.size(); i++)
    {
        if (((regexp_pre1[i] == '(') && (regexp_pre1[i - 1] != '(')) || (!isOperator(regexp_pre1[i]) && (!isOperator(regexp_pre1[i - 1]) || regexp_pre1[i - 1] == ')' || regexp_pre1[i - 1] == '*')))
        {
            regexp_pre += ".";
        }
        regexp_pre += regexp_pre1[i];
    }

    if (regexp_pre.size() > 0)
    {
        if (isValidRegExp(regexp_pre))
        {
            regexp = regexp_pre1;
            m_regexp = regexp_pre;
        }
        else
            cout << red << "Invalid regular expression!\n"
                 << reset;
    }
    else
    {
        cout << red << "Could not read any expression!\n"
             << reset;
    }
}

int readOperation()
{
    cout << "\n------------------------------------------------------\n";
    if (regexp.size() > 0)
    {
        cout << "Curent regular expression : " << regexp;

#if DEBUG == 1
        cout << " ~ " << m_regexp;
#endif
        cout << endl;
    }
    cout << "Choose an operation to perform : \n";
    if (regexp.size() > 0)
        cout << "1. Generate NFA for the current regular expresssion\n";
    cout << "2. Enter ";
    if (regexp.size() > 0)
        cout << "new";
    else
        cout << "a";
    cout << " regular expression\n";
    cout << "0. Terminate the program\n";
    cout << "------------------------------------------------------\n";

    int input;
    cin >> input;

    return input;
}

NFA *perform(NFA *a, char op, NFA *b = NULL)
{
    NFA *res;
    switch (op)
    {
    case '*':
    {
        Node *new_state = new Node(0);
        a->start->setType(-1);
        new_state->addTransition('\0', a->start);
        a->start = new_state;

        for (auto node : a->accept)
        {
            node->addTransition('\0', a->start);
        }
        return a;
    }
    break;
    case '+':
    case '|':
    {
        Node *new_state = new Node(0);
        a->start->setType(-1);
        b->start->setType(-1);

        new_state->addTransition('\0', a->start);
        new_state->addTransition('\0', b->start);

        a->start = new_state;

        Node *new_accept_state = new Node(1);
        for (auto node : a->accept)
        {
            node->setType(-1);
            node->addTransition('\0', new_accept_state);
        }
        for (auto node : b->accept)
        {
            node->setType(-1);
            node->addTransition('\0', new_accept_state);
        }

        a->accept = {new_accept_state};

        return a;
    }
    break;
    case '.':
    {
        for (auto node : a->accept)
        {
            node->addTransition('\0', b->start);
            node->setType(-1);
        }
        b->start->setType(-1);

        a->accept = b->accept;

        return a;
    }
    break;
    default:
        cout << "Invalid operation \"" << op << "\" on NFAs!\n";
        exit(-1);
        break;
    }

    return res;
}

void visualizeNFA(NFA *root)
{
    unordered_map<Node *, int> vis;
    queue<Node *> bfs;

    int counter = 0;

    bfs.push(root->start);
    root->start->state = counter++;

    ofstream dotFile("output.dot");

    dotFile << "digraph G {\n";

    dotFile << "\trankdir=\"LR\";\n\
    node [shape=\"circle\"];\n\
	start [shape=\"plaintext\"];\n\n\
	start->q0;\n";

    while (!bfs.empty())
    {
        Node *curr = bfs.front();
        bfs.pop();

        if (vis[curr] == 2)
            continue;

        for (auto it : curr->transitions)
        {
            for (auto node : it.second)
            {
                if (vis[node] == 0)
                {
                    node->state = counter++;
                    bfs.push(node);
                    vis[node] = 1;
                }

                if (curr->type == 1)
                {
                    dotFile << "\tq" << curr->state << " [shape=doublecircle];\n";
                }
                if (node->type == 1)
                {
                    dotFile << "\tq" << node->state << " [shape=doublecircle];\n";
                }

                dotFile << "\tq" << curr->state << " -> q" << node->state << " [ label=\" ";
                if (it.first == '\0')
                    dotFile << "&epsilon;";
                else
                    dotFile << it.first;
                dotFile << " \" ];\n";
            }
        }

        vis[curr] = 2;
    }

    dotFile << "}\n";

    dotFile.close();

    system("dot -Tpng output.dot -o graph.png");
    cout << green << "✅ NFA graph generated\n"
         << reset;

    cout << yellow << "Opening image ... \n"
         << reset;
    system("xdg-open graph.png");
    cout << green << "Graph opened\n"
         << reset;
}

void generateNFA() // Generate NFA from current regular expression
{
    stack<char> op_stack;
    stack<NFA *> nfa_stack;

    for (int i = 0; i < m_regexp.size(); i++)
    {
        const char curr = m_regexp[i];
        if (curr == '*')
        {
            NFA *a = nfa_stack.top();
            nfa_stack.pop();

            nfa_stack.push(perform(a, curr));
        }
        else if (isOperator(curr))
        {
            if (curr == '(' || op_stack.empty())
            {
                op_stack.push(curr);
            }
            else if (curr == ')')
            {
                while (op_stack.top() != '(')
                {
                    const char op = op_stack.top();
                    op_stack.pop();

                    if (nfa_stack.size() < 2)
                    {
                        cout << "ERROR1: Need atleast two NFAs in nfa_stack to perform operation\n";
                        exit(0);
                    }

                    NFA *b = nfa_stack.top();
                    nfa_stack.pop();

                    NFA *a = nfa_stack.top();
                    nfa_stack.pop();

                    nfa_stack.push(perform(a, op, b));
                }

                op_stack.pop();
            }
            else if (precedence[curr] <= precedence[op_stack.top()])
            {
                while (!op_stack.empty() && precedence[curr] <= precedence[op_stack.top()])
                {
                    const char op = op_stack.top();
                    if (op == '(')
                        break;

                    op_stack.pop();

                    if (nfa_stack.size() < 2)
                    {
                        cout << "ERROR2: Need atleast two NFAs in nfa_stack to perform operation\n";
                        exit(0);
                    }

                    NFA *b = nfa_stack.top();
                    nfa_stack.pop();

                    NFA *a = nfa_stack.top();
                    nfa_stack.pop();

                    nfa_stack.push(perform(a, op, b));
                }

                op_stack.push(curr);
            }
            else
            {
                op_stack.push(curr);
            }
        }
        else
            nfa_stack.push(new NFA(curr));
    }

    while (!op_stack.empty())
    {
        const char op = op_stack.top();
        op_stack.pop();

        if (nfa_stack.size() < 2)
        {
            cout << "ERROR3: Need atleast two NFAs in nfa_stack to perform operation\n";
            exit(0);
        }

        NFA *b = nfa_stack.top();
        nfa_stack.pop();

        NFA *a = nfa_stack.top();
        nfa_stack.pop();

        nfa_stack.push(perform(a, op, b));
    }

    visualizeNFA(nfa_stack.top());
}

int main()
{
    // NFA a('a'), b('b');

    // cout << "a+b -------------------------\n";
    // visualizeNFA(perform(&a, '+', &b));

    // return 0;

    readAlphabet(); // Reading input and extracting alphabet

    while (true)
    {
        int choice = readOperation();

        switch (choice)
        {
        case 2:
            readRegExp(); // Reading regular expression from input
            break;
        case 0:
            return 0;
        case 1:
            if (regexp.size() > 0)
            {
                generateNFA();
                break;
            }
            else
                cout << "No regular expression provided\n";
        default:
            cout << "Invalid choice!\n";
            break;
        }
    }

    return 0;
}