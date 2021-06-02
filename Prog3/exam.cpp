#include <iostream>

using namespace std;
//int main(int argv, char *argc[]) int var;

int main()
{

    int B(int v)
    {
        int var = v;
        return var * var;
    }
    int A()
    {
        int var;
        return var * 2;
    }

    int var = 5;
    var = B(var);
    A();
    cout << var << endl;
}
