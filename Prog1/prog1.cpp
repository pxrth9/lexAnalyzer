#include <iostream>
#include <string>
#include <cctype>
#include <string.h>
#include <fstream>
#include <ctime>
#include <map>
#include <sstream>

using namespace std;

void dict()
{
        int alpha = 97;
        int alpha1[26];
        int alphaSize = 26; // setting 26 to alphaSize
        for (int i = 0; i < 26; i++)
        {
                alpha1[i] = i;
        }

        srand(time(NULL));
        for (int i = 0; i < 26; i++)
        {
                int l = rand() % alphaSize;

                if (char(alpha) == char(alpha1[l] + 'a'))
                {
                        l--;
                }
                cout << char(alpha) << char(alpha1[l] + 'a') << endl;
                alpha1[l] = alpha1[alphaSize - 1];
                alphaSize--;
                alpha++;
        }
}

void rot13(string enMsg)
{
        string msg = "";
        for (int i = 0; i < enMsg.length(); i++)
        {

                if (isalpha(enMsg[i]))
                {
                        if ((enMsg[i] > 96 && enMsg[i] < 123) || (enMsg[i] > 64 && enMsg[i] < 91))
                        {
                                if ((enMsg[i] < 110 && enMsg[i] > 96) || (enMsg[i] < 78))
                                {
                                        msg += enMsg[i] + 13;
                                }
                                else
                                {
                                        msg += enMsg[i] - 13;
                                }
                        }
                }
                else
                {
                        msg += enMsg[i];
                }
        }
        cout << msg;
}

int main(int argc, char *argv[])
{
        string alpha = "abcdefghijklmnopqrstuvwxyz";
        string msg;
        if (argc == 1)
        {
                cout << "MISSING COMMAND" << endl;
                exit(1);
        }

        else if (strcmp(argv[1], "-r") == 0 && strcmp(argv[1], "-g") == 0)
        {
                cout << argv[1] << " NOT A VALID COMMAND" << endl;
        }
        else if (((strcmp(argv[1], "-e") == 0)) || (strcmp(argv[1], "-d") == 0))
        {
                if (argc < 3)
                {
                        cout << " NO DICTIONARY GIVEN" << endl;
                        //exit(0);
                }
                if (argc == 3)
                {
                        ifstream infile;
                        infile.open(argv[2]);
                        if (infile.fail())
                        {
                                cout << argv[2] << " DICTIONARY COULD NOT BE OPENED" << endl;
                                exit(0);
                        }
                }
                if (argc == 4)
                {
                        string msginfile = "";
                        map<char, char> map1;
                        map<char, char> map2;
                        ifstream infile;
                        ifstream dictfile;
                        string infilemsg;
                        dictfile.open(argv[2]);

                        while (dictfile >> msginfile)
                        {

                                if (msginfile[0] = msginfile[0 + 1])
                                {
                                        cout << msginfile[0];
                                }
                                map1[msginfile[0]] = msginfile[1];
                                map2[msginfile[1]] = msginfile[0];
                        }

                        infile.open(argv[3]);
                        {
                                if (infile.fail())
                                {
                                        cout << argv[3] << " DICTIONARY COULD NOT BE OPENED" << endl;
                                }
                                getline(infile, infilemsg, char(0));
                                if (infile.is_open())
                                {
                                        if (((strcmp(argv[1], "-e") == 0)))
                                        {
                                                for (int i = 0; i < infilemsg.size(); i++)
                                                {
                                                        char c = infilemsg[i];
                                                        if (isalpha(c))
                                                        {
                                                                for (map<char, char>::iterator it = map1.begin(); it != map1.end(); ++it)
                                                                {
                                                                        if (isupper(infilemsg[i]))
                                                                        {
                                                                                c = tolower(infilemsg[i]);
                                                                                if (c == it->first)
                                                                                {
                                                                                        cout << char(toupper(it->second));
                                                                                }
                                                                        }
                                                                        else if (c == it->first)
                                                                        {
                                                                                cout << it->second;
                                                                        }
                                                                }
                                                        }
                                                        else
                                                        {
                                                                cout << infilemsg[i];
                                                        }
                                                }
                                        }
                                        else if (((strcmp(argv[1], "-d") == 0)))
                                        {
                                                for (int i = 0; i < infilemsg.size(); i++)
                                                {
                                                        char c = infilemsg[i];
                                                        if (isalpha(c))
                                                        {
                                                                for (map<char, char>::iterator it = map2.begin(); it != map2.end(); ++it)
                                                                {
                                                                        if (isupper(infilemsg[i]))
                                                                        {
                                                                                c = tolower(infilemsg[i]);
                                                                                if (c == it->first)
                                                                                {
                                                                                        cout << char(toupper(it->second));
                                                                                }
                                                                        }
                                                                        else if (c == it->first)
                                                                        {
                                                                                cout << it->second;
                                                                        }
                                                                }
                                                        }
                                                        else
                                                        {
                                                                cout << infilemsg[i];
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }

        else if (strcmp(argv[1], "-r") == 0)
        {
                if (argc == 3)
                {
                        ifstream infile;
                        infile.open(argv[2]);
                        if (infile.fail())
                        {
                                cout << "FILE COULD NOT BE OPENED";
                                exit(0);
                        }
                        if (infile.is_open())
                        {
                                getline(infile, msg, char(0));
                                infile.close();
                        }
                        rot13(msg);
                }
                else
                {
                        getline(cin, msg, char(0));
                        rot13(msg);
                }
        }

        else if (strcmp(argv[1], "-g") == 0)
        {
                dict();
        }
}
