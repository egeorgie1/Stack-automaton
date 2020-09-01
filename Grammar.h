#pragma once
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <string>
#include <utility>
#include <stack>
#include <iostream>

using Rule = std::pair <std::string, std::vector < std::string>>;

class Grammar {
private:

	std::vector<char> alphabet;  //terminals
	std::vector<std::string> vars;  //nonterminals
	std::vector<Rule> rules;      //rules
	std::string start;   //start variable

	std::stack<char> NStack; //pushdown automaton

	bool isDerivable(std::string);
	std::string popNonterminal();
	void pushRule(std::string r);
	std::vector<std::string> findRules (std::string nt) const; //returns the rules, corresponding to the given nonterminal
	std::vector<std::string> derivableFromVars(unsigned i, unsigned j, const std::string& w);

public:
	
    Grammar(std::vector<char> _alphabet= std::vector<char>(), std::vector<std::string> _vars= std::vector<std::string>(),
		std::vector<Rule> _rules= std::vector<Rule>(), std::string _start= std::string()) :alphabet(_alphabet), vars(_vars), rules(_rules), start(_start)
	    {}
	
	bool belongsToLangNStack(std::string);
	bool isInChomskyNormForm();
	bool belongsToLangCYK(std::string);
	friend void unionGrammars(const Grammar& g1, const Grammar& g2, Grammar& g);
	friend void concatenationGrammars(const Grammar& g1, const Grammar& g2, Grammar& g);
	friend void iterationGrammar(const Grammar& g1, Grammar& g);
	friend std::istream& operator>>(std::istream&, Grammar&);
};

bool twoVars(std::string s);
bool singleChar(std::string s);

#endif // GRAMMAR_H
