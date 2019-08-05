#include <cassert>
#include <cctype>
#include <iostream>
#include <stack>
#include <string>
#include "Set.h"  // with ItemType being a type alias for char
using namespace std;

bool validInfix(string exp) {

	stack<char> pstack;
	stack<char> ostack;
	ostack.push('!');

	for (int i = 0; i < exp.length(); i++) {
		const char ch = exp[i];
		const char top = ostack.top();

		switch (ch) {
		case ' ':
			break;
		case '(':
			if (isalpha(top) || top == ')')
				return false;
			ostack.push(ch);
			pstack.push(ch);
			break;
		case ')':
			if (!isalpha(top) && top != ')')
				return false;
			ostack.push(ch);
			if (pstack.empty())
				return false;
			pstack.pop();
			break;
		case '|':
		case '&':
			if (!isalpha(top) && top != ')')
				return false;
			ostack.push(ch);
			break;
		case '!':
			if (isalpha(top) || top == ')')
				return false;
			ostack.push(ch);
			break;
		default:
			if (!isalpha(ch) || !islower(ch) || isalpha(top) || top == ')')
				return false;
			ostack.push(ch);
		}
	}

	while (ostack.top() == ')')
		ostack.pop();
	if (!isalpha(ostack.top()))
		return false;

	return pstack.empty();
}

int precedence(char ch) {
	switch (ch) {
	case '!':
		return 3;
	case '&':
		return 2;
	case '|':
		return 1;
	default:
		return 4;
	}
}

int evaluate(string infix, const Set& trueValues, const Set& falseValues,
	string& postfix, bool& result) {
	// checking if infix is valid
	if (!validInfix(infix))
		return 1;


	// checking if an operand doesnt exist within truevalues/falsevalues or exists within both
	for (int i = 0; i < infix.length(); i++) {
		if (isalpha(infix[i])) {
			if (!trueValues.contains(infix[i]) && !falseValues.contains(infix[i]))
				return 2;
			if (trueValues.contains(infix[i]) && falseValues.contains(infix[i]))
				return 3;	
		}
	}

	// creating postfix expression
	postfix = "";
	stack<char> ostack;
	for (int i = 0; i < infix.length(); i++) {
		switch (infix[i]) {
		case ' ':
			break;
		case '(':
			ostack.push('(');
			break;
		case ')':;
			while (ostack.top() != '(') {
				postfix += ostack.top();
				ostack.pop();
			}
			ostack.pop();
			break;
		case '|':
		case '&':
		case '!':
			while (!ostack.empty() && ostack.top() != '(' && precedence(infix[i]) <= precedence(ostack.top())) {
				postfix += ostack.top();
				ostack.pop();
			}
			ostack.push(infix[i]);
			break;
		default:
			if (!trueValues.contains(infix[i]) && !falseValues.contains(infix[i]))
				return 1;
			ostack.push(infix[i]);
			break;
		}
	}

	while (!ostack.empty()) {
		postfix += ostack.top();
		ostack.pop();
	}

	// evaluating postfix expression
	stack<bool> evalstack;

	for (int i = 0; i < postfix.length(); i++) {
		char ch = postfix[i];
		if (isalpha(ch)) {
			if (trueValues.contains(ch))
				evalstack.push(true);
			else
				evalstack.push(false);
		}
		else {
			char op2 = evalstack.top();
			evalstack.pop();
			char op1 = evalstack.top();
			evalstack.pop();

			if (ch == '|') {
				evalstack.push(op1 || op2);
			}
			else if (ch == '&') {
				evalstack.push(op1 && op2);
			}
			else {
				evalstack.push(op1);
				evalstack.push(!op2);
			}
		}
	}
	result = evalstack.top();
	return 0;
}

int main()
{
	string trueChars = "tywz";
	string falseChars = "fnx";
	Set trues;
	Set falses;
	for (int k = 0; k < trueChars.size(); k++)
		trues.insert(trueChars[k]);
	for (int k = 0; k < falseChars.size(); k++)
		falses.insert(falseChars[k]);

	string pf;
	bool answer;
	assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" &&  answer);
	assert(evaluate("y|", trues, falses, pf, answer) == 1);
	assert(evaluate("n t", trues, falses, pf, answer) == 1);
	assert(evaluate("nt", trues, falses, pf, answer) == 1);
	assert(evaluate("()", trues, falses, pf, answer) == 1);
	assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
	assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
	assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
	assert(evaluate("", trues, falses, pf, answer) == 1);
	assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
		&& pf == "ff!tn&&|" && !answer);
	assert(evaluate(" x  ", trues, falses, pf, answer) == 0 && pf == "x" && !answer);
	trues.insert('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 3);
	falses.erase('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 0 && pf == "x"  &&  answer);
	trues.erase('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 2);
	falses.insert('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" && !answer);
	cout << "Passed all tests" << endl;
}