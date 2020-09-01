#include "Grammar.h"

std::string Grammar::popNonterminal() {
	std::string result;
	while (NStack.top() != '}') {
		result.insert(result.end(), NStack.top());
		 NStack.pop();
	}
	result.insert(result.end(), NStack.top());
	 NStack.pop();

	return result;
}

void Grammar::pushRule(std::string r) {
	for (int i = r.length() - 1; i >= 0; i--)
		NStack.push(r[i]);
}

std::vector<std::string> Grammar::findRules(std::string nt) const{
	for (Rule r : rules)
		if (nt.compare(r.first) == 0)
			return r.second;
	return std::vector<std::string>();
}

bool Grammar::isDerivable(std::string w) {  //pitame se dali (q,w,x) |-* (q,@,@), kato w e na lentata, a x v NStack
	//bottom of recursion
	if (w.empty()) {
		if (NStack.empty())
			return true;
		else 
			return false;
		
	}
	//w ne e ""
	if (NStack.empty())
	return false;
	
	if (NStack.top() == '{') {  
    std::string nonterm = popNonterminal();
	std::vector<std::string> rs = findRules(nonterm);
	std::stack<char> prev = NStack;
	  for (std::string rule : rs) {
		  pushRule(rule);
		  if (isDerivable(w))  //ot novata duma x v NStack
			  return true;
		  
		  NStack = prev;                          
       }
    }
    else{ //NStack.top() e @ ili terminal
		if (NStack.top() == '@') {
			NStack.pop();

			return isDerivable(w);
		}

		if (NStack.top() == w.front()) {
			NStack.pop();

			return isDerivable(w.substr(1));
		}
		else
			return false;
	}

	return false;
}

bool Grammar::belongsToLangNStack(std::string w) {
	if (w.compare("@") == 0) { 
		std::vector<std::string> startRules = findRules(start);
		for (std::string rule : startRules)
			if (rule.compare("@") == 0)
				return true;
		return false;
	}
	else {
		while (!NStack.empty()) //clean the stack
			NStack.pop();

	for (int i = start.length()-1; i >= 0; i--)
			NStack.push(start[i]);
		return isDerivable(w);
	}

}

bool twoVars(std::string s) {
	if (s.empty())
		return false;
	else
		return (s.front() == '{') && (s.back() == '}') && (s.find("}{") != std::string::npos) && (s.find("}{") == s.rfind("}{"));
}
bool singleChar(std::string s) {
	return s.length() == 1;
}

bool Grammar::isInChomskyNormForm() {
	for (Rule r : rules) {
		std::vector<std::string> curr = r.second;
		for (std::string s : curr)
			if (!twoVars(s) && !singleChar(s))
				return false;
	}
	return true;
}

template<typename T>
bool contains(const std::vector<T>& v, const T& el) {
	for (unsigned i = 0; i < v.size(); i++)
		if (v[i] == el)
			return true;
	return false;
}

template<typename T>
std::vector<T> intersectionVect(const std::vector<T>& v1, const std::vector<T>& v2) {
	std::vector<T> result;
	for (unsigned i = 0; i < v1.size(); i++)
		if (contains(v2, v1[i]))
			result.push_back(v1[i]);
	return result;
}

template<typename T>
std::vector<T> unionVect(const std::vector<T>& v1, const std::vector<T>& v2) {
	std::vector<T> result;
	for (unsigned i = 0; i < v1.size(); i++)
		result.push_back(v1[i]);

	for (unsigned i = 0; i < v2.size(); i++)
		result.push_back(v2[i]);
	return result;
}

template<typename T>
std::vector<T> differenceVect(const std::vector<T>& v1, const std::vector<T>& v2) {
	std::vector<T> result;
	for (unsigned i = 0; i < v1.size(); i++)
		if (!contains(v2, v1[i]))
			result.push_back(v1[i]);
	return result;
}

//duma xi...xi+j-1 e s na4alo i, duljina j
std::vector<std::string> Grammar::derivableFromVars(unsigned i, unsigned j, const std::string& w) {
	if (j == 1) {
		std::vector<std::string> result;
		for (Rule r : rules) {
			std::vector<std::string> curr = r.second;
			std::string charTostr;
			charTostr.push_back(w[i - 1]);
			if (contains(curr, charTostr))
				result.push_back(r.first);
		}
		return result;
	}
	else {//j == 2 to n
		std::vector<std::string> result;
		if (i >= 1 && i <= w.length() - j + 1) {
			for (unsigned k = 1; k <= j - 1; k++) {
				std::vector<std::string> v1 = derivableFromVars(i, k, w);
				std::vector<std::string> v2 = derivableFromVars(i + k, j - k, w);

				for(std::string s1:v1)
					for (std::string s2 : v2) {
						std::string twovar = s1;
						twovar.append(s2);

						for (std::string var : vars) {
							std::vector<std::string> corrRules = findRules(var);
							if (contains(corrRules, twovar))
								result.push_back(var);
						}
					}
			}
		}
		return result;
	}
}

bool Grammar::belongsToLangCYK(std::string word) {
	if (isInChomskyNormForm()) {
		std::vector<std::string> d=derivableFromVars(1, word.length(), word);
		return contains(d, start);
	}
	else
		return belongsToLangNStack(word);
}


void unionGrammars(const Grammar& g1, const Grammar& g2, Grammar& g) {
	if (intersectionVect<std::string>(g1.vars, g2.vars).empty() && differenceVect<char>(g1.alphabet, g2.alphabet).empty() &&
		differenceVect<char>(g2.alphabet, g1.alphabet).empty()) {

		g.vars = unionVect<std::string>(g1.vars, g2.vars);
		g.start = "{stUnion}";
		g.vars.push_back(g.start);

		g.alphabet = g1.alphabet;

		g.rules = unionVect<Rule>(g1.rules, g2.rules);
		Rule r;
		r.first = g.start;
		r.second.push_back(g1.start);
		r.second.push_back(g2.start);

		g.rules.push_back(r);
	}
}
void concatenationGrammars(const Grammar& g1, const Grammar& g2, Grammar& g) {
	if (intersectionVect<std::string>(g1.vars, g2.vars).empty() && differenceVect<char>(g1.alphabet, g2.alphabet).empty() &&
		differenceVect<char>(g2.alphabet, g1.alphabet).empty()) {

		g.vars = unionVect<std::string>(g1.vars, g2.vars);
		g.start = "{stConcat}";
		g.vars.push_back(g.start);

		g.alphabet = g1.alphabet;

		g.rules = unionVect<Rule>(g1.rules, g2.rules);
		Rule r;
		r.first = g.start;

		std::string s = g1.start;
		s.append(g2.start);
		r.second.push_back(s);
		g.rules.push_back(r);
	}
}

void iterationGrammar(const Grammar& _g1, Grammar& g) {  //Sg->S_g1   S_g1->S_g1.S_g1
	g.vars = _g1.vars;
    g.start = "{stIter}";
	g.vars.push_back(g.start);

	g.alphabet = _g1.alphabet;

	Rule r1;
	r1.first = g.start;
	r1.second.push_back(_g1.start);
	g.rules.push_back(r1);

	Rule r2;
	r2.first = _g1.start;
	r2.second = _g1.findRules(_g1.start);
	std::string s = _g1.start;
	s.append(_g1.start);
	r2.second.push_back(s);
	g.rules.push_back(r2);

	std::vector<Rule> copy = _g1.rules;
	for (Rule r : copy)
		if (!(r.first == _g1.start))
			g.rules.push_back(r);

}


