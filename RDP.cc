#include "RDP.h"
#include <iostream>
using namespace std;

Token currentToken;
size_t currentTokenindex = 0;

void RDP::parse() {
	if (!Tokens.empty()) {
		currentToken = Tokens[0];
		RAT26F(currentToken);
	}
	else {
		cout << "No tokens to parse" << endl;
	}
};

void DisplayCurrentToken() {
	cout << "Current Token: TokenType: " << currentToken.TokenType << "     Lexeme: " << currentToken.lexeme;
	cout << endl;
}

void RDP::AdvanceToken() {
	if (currentTokenindex < Tokens.size() - 1) {
		currentToken = Tokens[currentTokenindex + 1];
		currentTokenindex++;
	}
	else {
		cout << "No more tokens to advance" << endl;
	}
};

// R1 <Rat26F> ::= <Opt Function Definitions>@ <Opt Declaration List> @ <Statement List>
void RDP::RAT26F(const Token& token) {
	DisplayCurrentToken();
	if (switch_) {
		cout << "<Rat26F> ::= <Opt Function Definitions>@ <Opt Declaration List> @ <Statement List>" << endl;
	}
	OPTFunctionDef(token);
	if (token.lexeme == "@") {
		AdvanceToken();
		OPTDeclarationsList(token);
		if (token.lexeme == "@") {
			AdvanceToken();
			// StatementList(token);
		}
	}
};

// R2 <Opt Function Definitions> ::= <Function Definitions> | <Empty>
void RDP::OPTFunctionDef(const Token& token) {
	DisplayCurrentToken();
	if (switch_) {
		cout << "<Function Definitions> | <Empty>" << endl;
	}
	if (token.lexeme == "function") {
		FunctionDef(token);
	}
	else {
	}
};

// R3 <Function Definitions> ::= <Function> <Function Definitions>
void RDP::FunctionDef(const Token& token) {
	if (switch_) {
		cout << "<Function> | <Function> <Function Definitions>" << endl;
	}
	Function(token);
	FunctionDefPrime(token);
};

// R3' <Function Definitions Prime> ::= <Function Definitions> 
void RDP::FunctionDefPrime(const Token& token) {
	if (token.lexeme == "function") {
		if (switch_) {
			DisplayCurrentToken();
			cout << "<Function Definitions Prime> ::= <Function Definitions>" << endl;
		}
		FunctionDef(token);
	}
};

// R4 <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
void RDP::Function(const Token& token) {
	if (switch_) {
		cout << "<Function> ::= function <Identifier> ( <Opt Parameter List> )" << endl;
	}
	if (token.lexeme == "function") {
		AdvanceToken();
		DisplayCurrentToken();
		if (token.TokenType == "Identifier") {
			AdvanceToken();
			DisplayCurrentToken();
			if (token.lexeme == "(") {
				AdvanceToken();
				OPTParameterList(token);
				if (token.lexeme == ")") {
					AdvanceToken();
					OPTDeclarationsList(token);
					Body(token);
				}
				else {
					// Invalid function
				}
			}
			else {
				// Invalid function
			}
		}
		else {
			// Invalid function
		}
	}
};
// R5. <Opt Parameter List> ::= <Parameter List> | <Empty>
void RDP::OPTParameterList(const Token& token) {
	if (switch_) {
		cout << "<Opt Parameter List> ::= <Parameter List> | <Empty>" << endl;
	}
	if (token.TokenType == "Identifier") {
		ParameterList(token);
	}
};

// R6. <Parameter List> ::= <Parameter> , <Parameter List>
void RDP::ParameterList(const Token& token) {
	if (switch_) {
		cout << "<Parameter List> ::= <Parameter> , <Parameter List>" << endl;
	}
	Parameter(token);
	ParameterListPrime(token);
};

// R6' <Parameter List Prime> ::= <Parameter List>
void RDP::ParameterListPrime(const Token& token) {
	DisplayCurrentToken();
	if (switch_) {
		cout << "<Parameter List Prime> ::= <Parameter List>" << endl;
	}
	if (token.lexeme == ",") {
		AdvanceToken();
		ParameterList(token);
	}
};
// R7. <Parameter> ::= <IDs > <Qualifier>
void RDP::Parameter(const Token& token) {
	DisplayCurrentToken();
	if (switch_) {
		cout << "<Parameter> ::= <IDs > <Qualifier>" << endl;
	}
	IDs(token);
	Qualifier(token);
};

// R8. <Qualifier> ::= integer | boolean | real
void RDP::Qualifier(const Token& token) {
	DisplayCurrentToken();
	if (switch_) {
		cout << "<Qualifier> ::= integer | boolean | real" << endl;
	}
	if (token.lexeme == "integer" || token.lexeme == "boolean" || token.lexeme == "real") {
		AdvanceToken();
	}
	else {
		// Invalid qualifier
	}
};

// R9. <Body> ::= { < Statement List> }
void RDP::Body(const Token& token) {
	if (switch_) {
		cout << "<Body> ::= { < Statement List> }" << endl;
	}
	if (token.lexeme == "{") {
		AdvanceToken();
		//StatementList(token);
		DisplayCurrentToken();
		if (token.lexeme == "}") {
			AdvanceToken();
		}
		else {
			// Invalid body
		}
	}
	else {
		// Invalid body
	}
};

// R10. <Opt Declaration List> ::= <Declaration List> | <Empty>
void RDP::OPTDeclarationsList(const Token& token) {
	DisplayCurrentToken();
	if (switch_) {
		cout << "<Opt Declaration List> ::= <Declaration List> | <Empty>" << endl;
	}
	if (token.lexeme == "integer" || token.lexeme == "boolean" || token.lexeme == "real") {
		DeclarationsList(token);
	}
};

// R11. <Declaration List> ::= <<Declaration>; <Declaration List>
void RDP::DeclarationsList(const Token& token) {
	DisplayCurrentToken();
	if (switch_) {
		cout << "<Declaration List> ::= <Declaration>; | <Declaration>; <Declaration List>" << endl;
	}
	Declaration(token);
	if (token.lexeme == ";") {
		AdvanceToken();
		DeclarationsListPrime(token);
	}
	else {
		// Invalid declaration list
	}
};

// 11' <Declaration List Prime> ::= <Declaration List>
void RDP::DeclarationsListPrime(const Token& token) {
	DisplayCurrentToken();
	if (switch_) {
		cout << "<Declaration List Prime> ::= <Declaration List>" << endl;
	}
	if (token.lexeme == "integer" || token.lexeme == "boolean" || token.lexeme == "real") {
		DeclarationsList(token);
	}
};

// R12. <Declaration> ::= <Qualifier > <IDs>
void RDP::Declaration(const Token& token) {
	DisplayCurrentToken();
	if (switch_) {
		cout << "<Declaration> ::= <Qualifier > <IDs>" << endl;
	}
	Qualifier(token);
	IDs(token);
};

// R13. <IDs> :: = <Identifier> 
void RDP::IDs(const Token& token) {
	if (switch_) {
		cout << "<IDs> :: = <Identifier> | <Identifier>, <IDs>" << endl;
	}
	if (token.TokenType == "Identifier") {
		AdvanceToken();
		IDsPrime(token);
	}
	else {
		// Invalid IDs
	}
};
// 13' <IDs Prime> ::= , <IDs>
void RDP::IDsPrime(const Token& token) {
	if (token.lexeme == ",") {
		DisplayCurrentToken();
		if (switch_) {
			cout << "<IDs Prime> ::= , <IDs>" << endl;
		}
		AdvanceToken();
		IDs(currentToken);
	}
}