#ifndef RDP_H
#define RDP_H
#include <vector>
#include "Lexer.h"
using namespace std;

class RDP {
public:
	RDP() {} // Default Constructor
	RDP(const vector<Token>& tokens) : Tokens(tokens) {} // Explicit Constructor with tokens vector geneterated from the lexer

	void parse(); // This will be the main function of this class
	void AdvanceToken(); // This will be used to advance the token in the vector of tokens

	// Rules of the grammer here 
	void RAT26F(const Token& token);
	void OPTFunctionDef(const Token& token);
	void FunctionDef(const Token& token);
	void FunctionDefPrime(const Token& token);
	void Function(const Token& token);
	void OPTParameterList(const Token& token);
	void ParameterList(const Token& token);
	void ParameterListPrime(const Token& token);
	void Parameter(const Token& token);
	void Qualifier(const Token& token);
	void Body(const Token& token);
	void OPTDeclarationsList(const Token& token);
	void DeclarationsList(const Token& token);
	void DeclarationsListPrime(const Token& token);
	void Declaration(const Token& token);
	void IDs(const Token& token);
	void IDsPrime(const Token& token);



private:
	bool switch_ = true; // This will be used to determine if the function definitions are present or not
	vector<Token> Tokens;
};
#endif