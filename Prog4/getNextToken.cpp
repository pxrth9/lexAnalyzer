#include "lex.h"
#include <iostream>

using namespace std;

Lex getNextToken(istream &in, int &linenum)
{
    enum LexState
    {
        BEGINL,
        INID,
        INSTR,
        ININT,
        COMMENT,
        BACKSLASH
    };
    LexState lexstate = BEGINL;
    string lexeme = "";
    char ch;

    while (in.get(ch))
    {
        //cout << ch;
        if (ch == '\n')
        {
            linenum++;
        }
        switch (lexstate)
        {
        case BEGINL:

            if (isspace(ch))
            {
                continue;
            }
            lexeme = ch;
            if (isalpha(ch))
            {
                lexstate = INID;
            }
            else if (ch == '"')
            {
                lexstate = INSTR;
            }
            else if (isdigit(ch))
            {
                lexstate = ININT;
            }
            else if (ch == '/')
            {
                if (in.peek() == '/')
                {

                    lexstate = COMMENT;
                }
                else
                {
                    return Lex(SLASH, lexeme, linenum);
                }
            }
            else if (ch == '+')
            {
                return Lex(PLUS, lexeme, linenum);
            }
            else if (ch == '-')
            {
                return Lex(MINUS, lexeme, linenum);
            }
            else if (ch == '*')
            {
                return Lex(STAR, lexeme, linenum);
            }
            else if (ch == '!')
            {
                return Lex(BANG, lexeme, linenum);
            }
            else if (ch == '(')
            {
                return Lex(LPAREN, lexeme, linenum);
            }
            else if (ch == ')')
            {
                return Lex(RPAREN, lexeme, linenum);
            }
            else if (ch == ';')
            {
                return Lex(SC, lexeme, linenum);
            }
            else
            {
                return Lex(ERR, lexeme, linenum);
            }
            break;
        case INID:
            if (isalpha(ch) || isdigit(ch))
            {
                lexeme += ch;
            }
            else
            {
                if (ch == '\n')
                {
                    linenum--;
                }
                in.putback(ch);
                return Lex(ID, lexeme, linenum);
            }
            if (lexeme == "let")
            {
                return Lex(LET, lexeme, linenum);
            }
            if(lexeme == "print")
            {
                return Lex(PRINT,lexeme, linenum);
            }
            if(lexeme == "if")
            {
                return Lex(IF, lexeme, linenum);
            }
            if(lexeme == "loop")
            {
                return Lex(LOOP, lexeme, linenum);
            }
            if(lexeme == "begin")
            {
                return Lex(BEGIN, lexeme, linenum);
            }
            if(lexeme == "end")
            {
                return Lex(END, lexeme, linenum);
            }
            break;
        case INSTR:
            if (ch == '\\')
            {
                if (in.peek() == 'n')
                {
                    in.get();
                    lexeme += "\n";
                }
                else
                {
                    in.get(ch);
                    lexeme = lexeme + ch;
                }
                break;
            }
            else
            {
                lexeme += ch;
            }
            if (ch == '\n')
            {
                linenum--;
                return Lex(ERR, lexeme, linenum);
            }
            else if (ch == '"')
            {
                lexeme = lexeme.substr(1, lexeme.length() - 2);
                return Lex(STR, lexeme, linenum);
            }
            break;

        case ININT:
            if (isdigit(ch))
            {
                lexeme += ch;
            }
            else if (isalpha(ch))
            {
                return Lex(ERR, lexeme, linenum);
            }
            else
            {
                if (ch == '\n')
                {
                    linenum--;
                }
                else
                {
                    in.putback(ch);
                    return Lex(INT, lexeme, linenum);
                }
            }
            break;
        case COMMENT:
            if (ch == '\n')
            {
                lexstate = BEGINL;
            }
            break;
        case BACKSLASH:

            lexstate = INSTR;
            break;
        }
    }
    //cout << endl;
    if (in.eof())
    {
        //return DONE;
        return Lex(DONE, lexeme, linenum);
    }
    else
    {
        //return ERR;
        return Lex(ERR, lexeme, linenum);
    }
}
