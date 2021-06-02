#ifndef VAL_H
#define VAL_H

#include <string>
#include <bits/stdc++.h>

using namespace std;

class Val
{
    int i;
    string s;
    enum ValType
    {
        ISINT,
        ISSTR,
        ISERR
    } vt;

public:
    Val() : i(0), vt(ISERR) {}
    Val(int i) : i(i), vt(ISINT) {}
    Val(string s) : i(0), s(s), vt(ISSTR) {}
    Val(ValType vt, string errmsg) : i(0), s(errmsg), vt(ISERR) {}

    ValType getVt() const { return vt; }

    bool isErr() const { return vt == ISERR; }
    bool isInt() const { return vt == ISINT; }
    bool isStr() const { return vt == ISSTR; }

    int ValInt() const
    {
        if (isInt())
            return i;
        throw "This Val is not an Int";
    }
    string ValString() const
    {
        if (isStr())
            return s;
        throw "This Val is not a Str";
    }



    friend ostream &operator<<(ostream &out, const Val &v)
    {
        if (v.isInt())
        {
            out << v.i;
            return out;
        }
        if (v.isStr())
        {
            out << v.s;
            return out;
        }
        return out;
    }

    string GetErrMsg() const
    {
        if (isErr())
            return s;
        throw "This Val is not an Error";
    }

    Val operator+(const Val &op) const
    {
        if (isInt() && op.isInt())
            return ValInt() + op.ValInt();
        if (isStr() && op.isStr())
            return ValString() + op.ValString();
        return Val(ISERR, "Type mismatch on operands of +");
    }

    Val operator-(const Val &op) const
    {
        if (isInt() && op.isInt())
            return ValInt() - op.ValInt();
        return Val(ISERR, "Type mismatch on operands of -");
    }

    Val operator*(const Val &op) const
    {
        if (isInt() && op.isInt())
        {
            return ValInt() * op.ValInt();
        }
        else
        {
            if (isStr() && op.isInt())
            {
                if (op.ValInt() < 0)
                {
                    return Val(ISERR, "Cannot multiply string by negative int");
                }
                else
                {
                    string sInt;
                    for (int i = 0; i < op.ValInt(); i++)
                    {
                        sInt += s;
                    }
                    return sInt;
                }
            }
            else if (isInt() && op.isStr())
            {
                if (ValInt() < 0)
                {
                    return Val(ISERR, "Cannot multiply string by negative int");
                }
                else
                {
                    string intS;
                    for (int i = 0; i < ValInt(); i++)
                    {
                        intS += op.ValString();
                    }
                    return intS;
                }
            }
        }
        return Val(ISERR, "Type mismatch on operands of *");
    }

    Val operator/(const Val &op) const
    {
        if (isInt() && op.isInt())
        {
            //cout << op.ValInt();
            if (op.ValInt() == 0)
            {
                //cout << "getting here";
                return Val(ISERR, "Divide by zero error");
            }
            else
            {
                return ValInt() / op.ValInt();
            }
        }
        return Val(ISERR, "Type mismatch on operands of /");
    }

    Val operator!() const
    {
        if (isInt())
        {
            if (ValInt() >= 0)
            {
                int a = i;
                int r = 0;
                while (a > 0)
                {
                    r = (r * 10) + (a % 10);
                    a = a / 10;
                }
                return r;
            }
            else
            {
                string s = to_string(i);
                reverse(s.begin() + 1, s.end());
                return stoi(s);
            }
        }
        if (isStr())
        {
            string str = s;
            reverse(str.begin(), str.end());
            return str;
        }
        return 1;
    }
};

#endif