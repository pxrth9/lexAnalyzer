/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
#include "val.h"
using std::map;
using std::vector;

// NodeType represents all possible types
enum NodeType
{
	ERRTYPE,
	INTTYPE,
	STRTYPE
};

// a "forward declaration" for a class to hold values
class Val;

class ParseTree
{
protected:
	int linenum;
	ParseTree *left;
	ParseTree *right;

public:
	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree()
	{
		delete left;
		delete right;
	}

	int GetLineNumber() const
	{
		return linenum;
	}

	int maxDepth(ParseTree *t)
	{
		if (t == NULL)
		{
			return 0;
		}
		else
		{
			int tLeft = maxDepth(t->left);
			int tRight = maxDepth(t->right);

			if (tLeft > tRight)
			{
				return (tLeft + 1);
			}
			else
			{
				return (tRight + 1);
			}
		}
	}

	virtual bool IsIdent() const { return false; }
	virtual bool IsVar() const { return false; }
	virtual string GetId() const { return ""; }
	virtual int IsBang() const { return 0; }
	virtual bool IsLet() const { return false; }
	virtual Val Eval(map<string, Val> &syms) const = 0;

	int BangCount() const
	{
		int bang = 0;
		if (left)
		{
			bang += left->BangCount();
		}
		if (right)
		{
			bang += right->BangCount();
		}
		if (IsBang())
		{
			bang += 1;
		}
		return bang;
	}

	virtual void checkLetBeforeUse(map<string, bool> &cLet)
	{
		if (left)
		{
			left->checkLetBeforeUse(cLet);
		}
		if (right)
		{
			right->checkLetBeforeUse(cLet);
		}
		if (this->IsLet())
		{
			cLet.insert(pair<string, bool>(GetId(), false));
		}
		if (this->IsIdent())
		{
			if (cLet.count(this->GetId()))
			{
				//do nothing;
			}
			else
			{
				cout << "UNDECLARED VARIABLE " << this->GetId() << endl;
				exit(0);
			}
		}
	}
};

class StmtList : public ParseTree
{

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
	Val Eval(map<string, Val> &sys) const
	{
		left->Eval(sys);
		if (right)
		{
			right->Eval(sys);
		}
		return Val();
	}
};

class Let : public ParseTree
{
	string id;

public:
	Let(Lex &t, ParseTree *e) : ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {}

	string GetId() const { return id; }
	bool IsLet() const { return true; }
	Val Eval(map<string, Val> &sys) const
	{
		if (left)
		{
			sys[id] = left->Eval(sys);
			return left->Eval(sys);
		}
		return 0;
	}
};

class Print : public ParseTree
{
public:
	Print(int line, ParseTree *l) : ParseTree(line, l) {}
	Val Eval(map<string, Val> &sys) const
	{
		if (left)
		{
			cout << left->Eval(sys);
		}
		return Val();
	}
};

class Loop : public ParseTree
{
public:
	Loop(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
	Val Eval(map<string, Val> &sys) const
	{
		if (left->Eval(sys).isInt())
		{
			while (0 != left->Eval(sys).ValInt())
			{
				right->Eval(sys);
			}
			return left->Eval(sys);
		}
		else
		{
			throw std::string("RUNTIME ERROR at " + to_string(linenum) +": " + "Expression is not an integer");
		}
	}
};

class If : public ParseTree
{
public:
	If(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
	Val Eval(map<string, Val> &sys) const
	{
		if (left->Eval(sys).isInt())
		{
			if (0 != left->Eval(sys).ValInt())
			{
				right->Eval(sys);
			}
			return left->Eval(sys);
		}
		else
		{
			throw std::string("RUNTIME ERROR at " + to_string(linenum) +": " + "Expression is not an integer");
		}
	}
};

class PlusExpr : public ParseTree
{
public:
	PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
	Val Eval(map<string, Val> &symbols) const
	{
		auto L = left->Eval(symbols);
		if (L.isErr())
			throw std::string("RUNTIME ERROR at " + to_string(linenum) + ": " + L.GetErrMsg());
		auto R = right->Eval(symbols);
		if (R.isErr())
			throw std::string("RUNTIME ERROR at " + to_string(linenum) + ": " + R.GetErrMsg());
		auto answer = L + R;
		if (answer.isErr())
			throw std::string("RUNTIME ERROR at " + to_string(linenum) + ": " + answer.GetErrMsg());
		return answer;
	}
};

class MinusExpr : public ParseTree
{
public:
	MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
	Val Eval(map<string, Val> &symbols) const
	{
		auto L = left->Eval(symbols);
		if (L.isErr())
			throw std::string("RUNTIME ERROR at " + to_string(linenum) + ": " + L.GetErrMsg());
		auto R = right->Eval(symbols);
		if (R.isErr())
			throw std::string("RUNTIME ERROR at " + to_string(linenum) + ": " + R.GetErrMsg());
		auto answer = L - R;
		if (answer.isErr())
			throw std::string("RUNTIME ERROR at " + to_string(linenum) + ": " + answer.GetErrMsg());
		return answer;
	}
};

class TimesExpr : public ParseTree
{
public:
	TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
	Val Eval(map<string, Val> &symbols) const
	{
		auto L = left->Eval(symbols);
		if (L.isErr())
			throw std::string("RUNTIME ERROR at " + to_string(linenum) + ": " + L.GetErrMsg());
		auto R = right->Eval(symbols);
		if (R.isErr())
			throw std::string("RUNTIME ERROR at " + to_string(linenum) + ": " + R.GetErrMsg());
		auto answer = L * R;
		if (answer.isErr())
			throw std::string("RUNTIME ERROR at " + to_string(linenum) + ": " + answer.GetErrMsg());
		return answer;
	}
};

class DivideExpr : public ParseTree
{
public:
	DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
	Val Eval(map<string, Val> &symbols) const
	{
		auto L = left->Eval(symbols);
		if (L.isErr())
			throw std::string("RUNTIME ERROR at " + to_string(linenum) + ": " + L.GetErrMsg());
		auto R = right->Eval(symbols);
		if (R.isErr())
			throw std::string("RUNTIME ERROR at " + to_string(linenum) + ": " + R.GetErrMsg());
		auto answer = L / R;
		if (answer.isErr())
			throw std::string("RUNTIME ERROR at " + to_string(linenum) + ": " + answer.GetErrMsg());
		return answer;
	}
};

class BangExpr : public ParseTree
{
public:
	BangExpr(int line, ParseTree *l) : ParseTree(line, l) {}
	int IsBang() const { return 1; }
	Val Eval(map<string, Val> &sys) const
	{
		auto L = left->Eval(sys);
		auto answer = !L;
		return answer;
	}
};

class IConst : public ParseTree
{
	int val;

public:
	IConst(Lex &t) : ParseTree(t.GetLinenum())
	{
		val = stoi(t.GetLexeme());
	}
	Val Eval(map<string, Val> &sys) const
	{
		return Val(val);
	}
};

class SConst : public ParseTree
{
	string val;

public:
	SConst(Lex &t) : ParseTree(t.GetLinenum())
	{
		val = t.GetLexeme();
	}
	Val Eval(map<string, Val> &sys) const
	{
		return Val(val);
	}
};

class Ident : public ParseTree
{
	string id;

public:
	Ident(Lex &t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}

	bool IsIdent() const { return true; }
	string GetId() const { return id; }
	Val Eval(map<string, Val> &sys) const
	{
		map<string, Val>::iterator it;
		it = sys.find(id);
		while (it != sys.end())
		{
			return sys[id];
		}
		return Val();
	}
};

#endif /* PARSETREE_H_ */
