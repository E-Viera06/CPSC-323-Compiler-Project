#ifndef RDP_H
#define RDP_H
#include <vector>
#include <fstream>
#include "Lexer.h"

using namespace std;

class RDP {
public:
	RDP() {} // Default Constructor
	RDP(const vector<Token>& tokens, const string& filename)  {
		Tokens = tokens;
		outputFile.open(filename);
	} // Explicit Constructor with tokens vector geneterated from the lexer

	void parse(); // This will be the main function of this class
	void AdvanceToken(); // This will be used to advance the token in the vector of tokens
	void DisplayCurrentToken();
	// Rules of the grammer here 
	void RAT26F();
	void OPTFunctionDef();
	void FunctionDef();
	void FunctionDefPrime();
	void Function();
	void OPTParameterList();
	void ParameterList();
	void ParameterListPrime();
	void Parameter();
	void Qualifier();
	void Body();
	void OPTDeclarationsList();
	void DeclarationsList();
	void DeclarationsListPrime();
	void Declaration();
	void IDs();
	void IDsPrime();



private:
	ofstream outputFile; // This will be used to write the output to a file
	bool switch_ = true; // This will be used to determine if the function definitions are present or not
	vector<Token> Tokens;

	Token currentToken;
	size_t currentTokenindex = 0;
};
#endif