#include "lex.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <map>
#include "Lex.cpp"
#include <sstream>
using namespace std;

int main(int argc, char *argv[])
{
    bool v = false;
    bool ids = false;
    bool consts = false;
    istream *in = &cin;
    ifstream file;
    //int lineNumber;
    for (int i = 1; i < argc; i++)
    {
        file.open(argv[i]);
        if (argv[i][0] == '-')
        {
            if (strcmp(argv[i], "-v") == 0)
            {
                v = true;
            }
            else if (strcmp(argv[i], "-consts") == 0)
            {
                consts = true;
            }
            else if (strcmp(argv[i], "-ids") == 0)
            {
                ids = true;
            }
            else
            {
                cout << "UNRECOGNIZED FLAG " << argv[i] << endl;
                return 0;
            }
        }
        else if (argc != i + 1)
        {
            cout << "ONLY ONE FILE NAME ALLOWED" << endl;
            return 0;
        }
        else
        {
            if (!file.is_open())
            {
                cout << "CANNOT OPEN " << argv[i] << endl;
                return 0;
            }
            in = &file;
        }
    }
    map<string, int> strings = {};
    map<int, int> integers = {};
    map<string, int> ident = {};
    map<int, string> dashV = {{0, "PRINT"}, {1, "LET"}, {2, "IF"}, {3, "LOOP"}, {4, "BEGIN"}, {5, "END"}, {6, "ID"}, {7, "INT"}, {8, "STR"}, {9, "PLUS"}, {10, "MINUS"}, {11, "STAR"}, {12, "SLASH"}, {13, "BANG"}, {14, "LPAREN"}, {15, "RPAREN"}, {16, "SC"}, {17, "ERR"}, {18, "DONE"}};
    Lex token;
    int linenumber = 0;
    int tokenCount = 0;
    while ((token = getNextToken(*in, linenumber)) != DONE && token != ERR)
    {
        if (v == true)
        {
            if (token == ID || token == STR || token == INT)
            {
                cout << dashV.find(token.GetToken())->second << "(" << token.GetLexeme() << ")" << endl;
            }
            else
            {
                cout << dashV.find(token.GetToken())->second << endl;
            }
        }
        if (ids == true)
        {
            if (token == ID)
            {
                ident.insert(pair<string, int>(token.GetLexeme(), 1));
            }
        }
        if (consts == true)
        {
            if (token == STR)
            {
                strings.insert(pair<string, int>(token.GetLexeme(), 1));
            }
            if (token == INT)
            {
                string s = token.GetLexeme();
                int i = std::stoi(s);
                integers.insert(pair<int, int>(i, 1));
            }
        }
        tokenCount++;
    }
    if (token == DONE)
    {
        if (!ident.empty())
        {
            string c = "";
            cout << "IDENTIFIERS: ";
            for (auto it = ident.begin(); it != ident.end(); it++)
            {
                cout << c << it->first;
                c = ", ";
            }
            cout << endl;
        }
        if (!strings.empty())
        {
            string c = "";
            cout << "STRINGS:" << endl;
            for (auto it = strings.begin(); it != strings.end(); it++)
            {
                cout << c << it->first << endl;
            }
        }
        if (!integers.empty())
        {
            string c = "";
            cout << "INTEGERS:" << endl;
            for (auto it = integers.begin(); it != integers.end(); it++)
            {
                cout << c << it->first << endl;
                //c = ", ";
            }
        }
    }

    if (token == DONE)
    {

        if (token.GetLinenum() == 0)
        {
            cout << "Lines: " << token.GetLinenum() << endl;
        }
        else
        {
            cout << "Lines: " << token.GetLinenum() << endl;
            cout << "Tokens: " << tokenCount << endl;
        }
    }
    if (token == ERR)
    {
        cout << "Error on line " << token.GetLinenum() + 1 << " (" << token.GetLexeme() << ")" << endl;
        return 0;
    }

    file.close();
    return 0;
}