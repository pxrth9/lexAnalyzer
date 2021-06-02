#include "lex.h"
#include "parse.cpp"
#include "parse.h"
#include "parsetree.h"
#include <fstream>
#include "getNextToken.cpp"
//#include "prog3.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    ifstream ifile; //instantiate ifile to take files
    istream *in;    //instantiate istream to take in userinput

    if (argc == 1)
    {
        in = &cin; //take userinput if no file is given
    }
    else if (argc == 2)
    {
        ifile.open(argv[1]); //open the file
        //check if the file opens & if it doesnt print and break main
        if (!ifile.is_open())
        {
            cout << "COULD NOT OPEN " << argv[1] << endl;
            return 0;
        }
        //if it opens take it from ifstream ifile
        in = &ifile;
    }
    //if more than one file given print this
    else if (argc != 1 || argc != 2)
    {
        cout << "TOO MANY FILENAMES" << endl;
        return 0;
    }

    int lines = 0;                      //set line counter to zero
    ParseTree *prog = Prog(*in, lines); //create a object
    map<string, Val> symbols;
    map<string, bool> s;
    //if it returns zero means nothing is there, break main
    if (prog == 0)
    {
        return 0;
    }
    prog->checkLetBeforeUse(s);
    try
    {
        prog->Eval(symbols);
    }
    catch (string &e)
    {
        cout << e << endl;
    }
    //cout << "BANG COUNT: " << prog->BangCount() << endl;
    //cout << "MAX DEPTH: " << prog->maxDepth(prog) << endl;
}