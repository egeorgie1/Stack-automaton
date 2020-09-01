
#include "Grammar.h"

int main() {
	std::cout << "Test NStack:" << std::endl;
	std::cout << "G1:" << std::endl;
	//S->aSb|ab , start=S, L(G)=a^nb^n, n>=1
	std::vector<char> al;
	std::vector<std::string> v;
	std::vector<Rule> rss;
	std::string s;

	al.push_back('a');
	al.push_back('b');
	s = "{s}";
	v.push_back(s);
	Rule r1;
	r1.first = s;
	r1.second.push_back("a{s}b");
	r1.second.push_back("ab");
	rss.push_back(r1);

	Grammar g1(al, v, rss, s);

	std::cout << std::boolalpha << g1.belongsToLangNStack("aabb") << std::endl; //true
	std::cout << std::boolalpha << g1.belongsToLangNStack("ab") << std::endl;   //true
	std::cout << std::boolalpha << g1.belongsToLangNStack("aab") << std::endl;  //false
	std::cout << std::boolalpha << g1.belongsToLangNStack("aabbb") << std::endl;  //false
	std::cout << std::boolalpha << g1.belongsToLangNStack("@") << std::endl;    //false
	std::cout << std::boolalpha << g1.belongsToLangNStack("baba") << std::endl;  //false
	std::cout << std::endl;

	std::cout << "G2:" << std::endl;
	//A->aA|aB   B->bB|b, start=A, L(G)=a^nb^m, m,n >=1
	std::vector<char> al2;
	std::vector<std::string> v2;
	std::vector<Rule> rss2;
	std::string s2;

	al2.push_back('a');
	al2.push_back('b');
	s2 = "{A}";
	v2.push_back(s2);
	v2.push_back("{B}");
	Rule r21;
	r21.first = s2;
	r21.second.push_back("a{A}");
	r21.second.push_back("a{B}");
	rss2.push_back(r21);
	Rule r22;
	r22.first = "{B}";
	r22.second.push_back("b");
	r22.second.push_back("b{B}");
	rss2.push_back(r22);

	Grammar g2(al2, v2, rss2, s2);
	std::cout << std::boolalpha << g2.belongsToLangNStack("aabb") << std::endl; //true
	std::cout << std::boolalpha << g2.belongsToLangNStack("ab") << std::endl;   //true
	std::cout << std::boolalpha << g2.belongsToLangNStack("aab") << std::endl;  //true
	std::cout << std::boolalpha << g2.belongsToLangNStack("aabbb") << std::endl;  //true
	std::cout << std::boolalpha << g2.belongsToLangNStack("@") << std::endl;    //false
	std::cout << std::boolalpha << g2.belongsToLangNStack("baba") << std::endl;  //false
	std::cout << std::endl;

	std::cout << "Test Chomsky Normal Form:" << std::endl;
	//g1 in Chomsky Normal Form
	// S->AxU|AxBx  U->SBx  Ax->a  Bx->b
	std::vector<char> al3;
	std::vector<std::string> v3;
	std::vector<Rule> rss3;
	std::string s3;

	al3.push_back('a');
	al3.push_back('b');
	s3 = "{S}";
	v3.push_back(s3);
	v3.push_back("{Ax}");
	v3.push_back("{Bx}");
	v3.push_back("{U}");

	Rule r31;
	r31.first = s3;
	r31.second.push_back("{Ax}{U}");
	r31.second.push_back("{Ax}{Bx}");
	rss3.push_back(r31);

	Rule r32;
	r32.first = "{U}";
	r32.second.push_back("{S}{Bx}");
	rss3.push_back(r32);

	Rule r33;
	r33.first = "{Ax}";
	r33.second.push_back("a");
	rss3.push_back(r33);

	Rule r34;
	r34.first = "{Bx}";
	r34.second.push_back("b");
	rss3.push_back(r34);

	Grammar g3(al3, v3, rss3, s3);
	std::cout << std::boolalpha << g3.isInChomskyNormForm() << std::endl; //true
	std::cout<< std::endl;

	std::cout << "Test CYK:" << std::endl;
	std::cout << std::boolalpha << g3.belongsToLangCYK("aabb") << std::endl; //true
	std::cout << std::boolalpha << g3.belongsToLangCYK("ab") << std::endl;   //true
	std::cout << std::boolalpha << g3.belongsToLangCYK("aab") << std::endl;  //false
	std::cout << std::boolalpha << g3.belongsToLangCYK("aabbb") << std::endl;  //false
	std::cout << std::boolalpha << g3.belongsToLangCYK("baba") << std::endl;  //false
	std::cout << std::endl;

	std::cout << "Test union:" << std::endl;
	Grammar gU;
	unionGrammars(g2, g3, gU);
	std::cout << std::boolalpha << gU.belongsToLangNStack("aabbb") << std::endl;  //true, ot g2
	std::cout << std::boolalpha << gU.belongsToLangCYK("aabb") << std::endl; //true, ot g3
	std::cout << std::boolalpha << gU.belongsToLangNStack("baba") << std::endl;  //false
	std::cout << std::endl;

	std::cout << "Test concatenation:" << std::endl;
	Grammar gC;
	concatenationGrammars(g2, g3, gC);
	std::cout << std::boolalpha << gC.belongsToLangNStack("aabbbaabb") << std::endl; //true, g2.g3
	std::cout << std::boolalpha << gC.belongsToLangNStack("aabbaabbb") << std::endl; //false, g3.g2
	std::cout << std::boolalpha << gC.belongsToLangNStack("baba") << std::endl;  //false
	std::cout << std::endl;

	std::cout << "Test iteration:" << std::endl;
	Grammar gI;
	//We modify g1, because the start variable can not appear on the right side of the rules when we iterate!
	//S->X  X->aXb|ab
	std::vector<char> alNew;
	std::vector<std::string> vNew;
	std::vector<Rule> rssNew;
	std::string sNew;

	alNew.push_back('a');
	alNew.push_back('b');
	sNew = "{st}";
	vNew.push_back(sNew);
	vNew.push_back("{X}");

	Rule r1New;
	r1New.first = sNew;
	r1New.second.push_back("{X}");
	rssNew.push_back(r1New);

	Rule r2New;
	r2New.first = "{X}";
	r2New.second.push_back("a{X}b");
	r2New.second.push_back("ab");
	rssNew.push_back(r2New);

	Grammar g1New(alNew, vNew, rssNew, sNew);

	iterationGrammar(g1New, gI);
	std::cout << std::boolalpha << gI.belongsToLangNStack("ab") << std::endl; //true g1
	std::cout << std::boolalpha << gI.belongsToLangNStack("aabb") << std::endl; //true g1
	std::cout << std::boolalpha <<gI.belongsToLangNStack("abab") << std::endl; //true g1.g1
	std::cout << std::boolalpha << gI.belongsToLangNStack("aabbab") << std::endl; //true g1.g1
	std::cout << std::boolalpha << gI.belongsToLangNStack("aabbabab") << std::endl; //true g1.g1.g1
	
	return 0;
}