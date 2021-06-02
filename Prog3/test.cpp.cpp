#include <iostream>
#include <bits/stdc++.h>

using namespace std;

string str = "HELLO";
string getLexeme()
{
    return str;
}

int main()
{
    
    int n = 1;
    if (n == 1)
    {
        string rT = getLexeme();
        reverse(rT.begin(), rT.end());
        cout << rT;
    }
}
