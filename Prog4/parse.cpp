/*
 * parse.cpp
 */

#include "parse.h"
#include <iostream>
#include <bits/stdc++.h>
#include "val.h"
using namespace std;

namespace Parser
{
bool pushed_back = false;
Lex pushed_token;

static Lex GetNextToken(istream &in, int &line)
{
	if (pushed_back)
	{
		pushed_back = false;
		return pushed_token;
	}
	return getNextToken(in, line);
}

static void PushBackToken(Lex &t)
{
	if (pushed_back)
	{
		abort();
	}
	pushed_back = true;
	pushed_token = t;
}

} // namespace Parser

static int error_count = 0;

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

ParseTree *Prog(istream &in, int &line)
{
	ParseTree *sl = Slist(in, line);

	if (sl == 0)
	{
		ParseError(line, "No statements in program");
		return 0;
	}
	if (error_count)
	{
		return 0;
	}

	return sl;
}

// Slist is a Statement followed by a Statement List
ParseTree *Slist(istream &in, int &line)
{

	ParseTree *s = Stmt(in, line);
	if (s == 0)
	{
		return 0;
	}
	Lex sc = Parser::GetNextToken(in, line);
	if (sc != SC)
	{
		ParseError(line, "Missing semicolon after statement");
		return 0;
	}
	return new StmtList(s, Slist(in, line));
}

ParseTree *Stmt(istream &in, int &line)
{
	ParseTree *parse;
	Lex t = Parser::GetNextToken(in, line);
	if (t == IF)
	{
		parse = IfStmt(in, line);
	}
	else if (t == PRINT)
	{
		parse = PrintStmt(in, line);
	}
	else if (t == LET)
	{
		parse = LetStmt(in, line);
	}
	else if (t == LOOP)
	{
		parse = LoopStmt(in, line);
	}
	else if (t == DONE)
	{
		return 0;
	}
	else if (t == END)
	{
		Parser::PushBackToken(t);
		return 0;
	}
	else
	{
		ParseError(line, "Invalid Token");
		return 0;
	}

	return parse;
}

ParseTree *LetStmt(istream &in, int &line)
{
	Lex t = Parser::GetNextToken(in, line);
	if (t != ID)
	{
		ParseError(line, "ID not found!");
		return 0;
	}
	ParseTree *l = Expr(in, line);

	return new Let(t, l);
}

ParseTree *PrintStmt(istream &in, int &line)
{
	ParseTree *parse = Expr(in, line);
	if (parse == 0)
	{
		ParseError(line, "Missing expression after operator");
		return 0;
	}
	return new Print(line, parse);
}

ParseTree *LoopStmt(istream &in, int &line)
{
	int l = line;
	ParseTree *exp = Expr(in, line);
	if (exp == 0)
	{
		ParseError(line, "Expression not found!");
		return 0;
	}

	Lex beg = Parser::GetNextToken(in, line);
	if (beg != BEGIN)
	{
		ParseError(line, "BEGIN was not found!");
		return 0;
	}
	ParseTree *sli = Slist(in, line);
	if (sli == 0)
	{
		ParseError(line, "Slist not found!");
		return 0;
	}
	Lex don = Parser::GetNextToken(in, line);
	if (don != END)
	{
		ParseError(line, "END was not found");
		return 0;
	}
	return new Loop(l, exp, sli);
}

ParseTree *IfStmt(istream &in, int &line)
{
	int l = line;
	ParseTree *exp = Expr(in, line);
	if (exp == 0)
	{
		ParseError(line, "Expression not found!");
		return 0;
	}
	Lex beg = Parser::GetNextToken(in, line);
	if (beg != BEGIN)
	{
		ParseError(line, "BEGIN was not found!");
		return 0;
	}
	ParseTree *sli = Slist(in, line);
	if (sli == 0)
	{
		ParseError(line, "Slist not found!");
		return 0;
	}
	Lex don = Parser::GetNextToken(in, line);
	if (don != END)
	{
		ParseError(line, "END was not found");
		return 0;
	}
	return new If(l, exp, sli);
}

ParseTree *Expr(istream &in, int &line)
{
	ParseTree *t1 = Prod(in, line);
	if (t1 == 0)
	{
		return 0;
	}

	while (true)
	{
		Lex t = Parser::GetNextToken(in, line);

		if (t != PLUS && t != MINUS)
		{
			Parser::PushBackToken(t);
			return t1;
		}

		ParseTree *t2 = Prod(in, line);
		if (t2 == 0)
		{
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if (t == PLUS)
			t1 = new PlusExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new MinusExpr(t.GetLinenum(), t1, t2);
	}
}

ParseTree *Prod(istream &in, int &line)
{
	ParseTree *re = Rev(in, line);
	if (re == 0)
	{
		return 0;
	}

	while (true)
	{
		Lex t = Parser::GetNextToken(in, line);

		if (t != STAR && t != SLASH)
		{
			Parser::PushBackToken(t);
			return re;
		}

		ParseTree *revv = Rev(in, line);
		if (revv == 0)
		{
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if (t == STAR)
			re = new TimesExpr(t.GetLinenum(), re, revv);
		else
			re = new DivideExpr(t.GetLinenum(), re, revv);
	}
}

ParseTree *Rev(istream &in, int &line)
{
	Lex t = Parser::GetNextToken(in, line);
	if (t == BANG)
	{
		ParseTree *re = Rev(in, line);
		if (re == 0)
		{
			ParseError(line, "Cant Do Rev");
			return 0;
		}
		return new BangExpr(line, re);
	}
	Parser::PushBackToken(t);
	ParseTree *prm = Primary(in, line);
	if (prm == 0)
	{
		ParseError(line, "Primary Not Found");
	}
	return prm;
}

ParseTree *Primary(istream &in, int &line)
{
	Lex t = Parser::GetNextToken(in, line);

	if (t == ID)
	{
		return new Ident(t);
	}
	else if (t == INT)
	{
		return new IConst(t);
	}
	else if (t == STR)
	{
		return new SConst(t);
	}
	else if (t == LPAREN)
	{
		ParseTree *ex = Expr(in, line);
		if (ex == 0)
		{
			ParseError(line, "Missing expression after (");
			return 0;
		}
		if (Parser::GetNextToken(in, line) == RPAREN)
			return ex;

		ParseError(line, "Missing ) after expression");
		return 0;
	}

	ParseError(line, "Primary expected");
	return 0;
}
