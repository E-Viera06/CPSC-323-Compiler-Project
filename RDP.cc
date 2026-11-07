#include "RDP.h"
#include <iostream>
#include <fstream>
using namespace std;

void RDP::parse() {
	if (!Tokens.empty()) {
		currentTokenindex = 0;
		currentToken = Tokens[0];
		RAT26F();
	}
	else {
		outputFile << "No tokens to parse" << endl;
		outputFile.close();
	}
};

void RDP::DisplayCurrentToken() {
	outputFile << "Token: " << currentToken.TokenType << "     Lexeme : " << currentToken.lexeme;
	outputFile << endl;
}

void RDP::AdvanceToken() {
	if (currentTokenindex < Tokens.size() - 1) {
		currentToken = Tokens[currentTokenindex + 1];
		currentTokenindex++;
	}
	else {
		outputFile << "No more tokens to advance" << endl;
	}
};

// R1 <Rat26F> -> <Opt Function Definitions>@ <Opt Declaration List> @ <Statement List>
void RDP::RAT26F() {
	DisplayCurrentToken();
	if (switch_) {
		outputFile << "<Rat26F> -> <Opt Function Definitions>@ <Opt Declaration List> @ <Statement List>" << endl;
	}
	OPTFunctionDef();
	if (currentToken.lexeme == "@") {
		AdvanceToken();
		OPTDeclarationsList();
		if (currentToken.lexeme == "@") {
			AdvanceToken();
			// StatementList();
		}
	}
};

// R2 <Opt Function Definitions> -> <Function Definitions> | <Empty>
void RDP::OPTFunctionDef() {
	DisplayCurrentToken();
	if (switch_) {
		outputFile << "<Function Definitions> | <Empty>" << endl;
	}
	if (currentToken.lexeme == "function") {
		FunctionDef();
	}
	else {
	}
};

// R3 <Function Definitions> -> <Function> <Function Definitions>
void RDP::FunctionDef() {
	if (switch_) {
		outputFile << "<Function> | <Function> <Function Definitions>" << endl;
	}
	Function();
	FunctionDefPrime();
};

// R3' <Function Definitions Prime> -> <Function Definitions> 
void RDP::FunctionDefPrime() {
	if (currentToken.lexeme == "function") {
		if (switch_) {
			DisplayCurrentToken();
			outputFile << "<Function Definitions Prime> -> <Function Definitions>" << endl;
		}
		FunctionDef();
	}
};

// R4 <Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
void RDP::Function() {
	if (switch_) {
		outputFile << "<Function> -> function <Identifier> ( <Opt Parameter List> )" << endl;
	}
	if (currentToken.lexeme == "function") {
		AdvanceToken();
		DisplayCurrentToken();
		if (currentToken.TokenType == "Identifier") {
			AdvanceToken();
			DisplayCurrentToken();
			if (currentToken.lexeme == "(") {
				AdvanceToken();
				OPTParameterList();
				if (currentToken.lexeme == ")") {
					AdvanceToken();
					OPTDeclarationsList();
					Body();
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
// R5. <Opt Parameter List> -> <Parameter List> | <Empty>
void RDP::OPTParameterList() {
	if (switch_) {
		outputFile << "<Opt Parameter List> -> <Parameter List> | <Empty>" << endl;
	}
	if (currentToken.TokenType == "Identifier") {
		ParameterList();
	}
};

// R6. <Parameter List> -> <Parameter> , <Parameter List>
void RDP::ParameterList() {
	if (switch_) {
		outputFile << "<Parameter List> -> <Parameter> , <Parameter List>" << endl;
	}
	Parameter();
	ParameterListPrime();
};

// R6' <Parameter List Prime> -> <Parameter List>
void RDP::ParameterListPrime() {
	DisplayCurrentToken();
	if (switch_) {
		outputFile << "<Parameter List Prime> -> <Parameter List>" << endl;
	}
	if (currentToken.lexeme == ",") {
		AdvanceToken();
		ParameterList();
	}
};
// R7. <Parameter> -> <IDs > <Qualifier>
void RDP::Parameter() {
	DisplayCurrentToken();
	if (switch_) {
		outputFile << "<Parameter> -> <IDs > <Qualifier>" << endl;
	}
	IDs();
	Qualifier();
};

// R8. <Qualifier> -> integer | boolean | real
void RDP::Qualifier() {
	DisplayCurrentToken();
	if (switch_) {
		outputFile << "<Qualifier> -> integer | boolean | real" << endl;
	}
	if (currentToken.lexeme == "integer" || currentToken.lexeme == "boolean" || currentToken.lexeme == "real") {
		AdvanceToken();
	}
	else {
		// Invalid qualifier
	}
};

// R9. <Body> -> { < Statement List> }
void RDP::Body() {
	if (switch_) {
		outputFile << "<Body> -> { < Statement List> }" << endl;
	}
	if (currentToken.lexeme == "{") {
		AdvanceToken();
		//StatementList();
		if (currentToken.lexeme == "}") {
			DisplayCurrentToken();
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

// R10. <Opt Declaration List> -> <Declaration List> | <Empty>
void RDP::OPTDeclarationsList() {
	DisplayCurrentToken();
	if (switch_) {
		outputFile << "<Opt Declaration List> -> <Declaration List> | <Empty>" << endl;
	}
	if (currentToken.lexeme == "integer" || currentToken.lexeme == "boolean" || currentToken.lexeme == "real") {
		DeclarationsList();
	}
};

// R11. <Declaration List> -> <<Declaration>; <Declaration List>
void RDP::DeclarationsList() {
	DisplayCurrentToken();
	if (switch_) {
		outputFile << "<Declaration List> -> <Declaration>; | <Declaration>; <Declaration List>" << endl;
	}
	Declaration();
	if (currentToken.lexeme == ";") {
		AdvanceToken();
		DeclarationsListPrime();
	}
	else {
		// Invalid declaration list
	}
};

// 11' <Declaration List Prime> -> <Declaration List>
void RDP::DeclarationsListPrime() {
	DisplayCurrentToken();
	if (switch_) {
		outputFile << "<Declaration List Prime> -> <Declaration List>" << endl;
	}
	if (currentToken.lexeme == "integer" || currentToken.lexeme == "boolean" || currentToken.lexeme == "real") {
		DeclarationsList();
	}
};

// R12. <Declaration> -> <Qualifier > <IDs>
void RDP::Declaration() {
	DisplayCurrentToken();
	if (switch_) {
		outputFile << "<Declaration> -> <Qualifier > <IDs>" << endl;
	}
	Qualifier();
	IDs();
};

// R13. <IDs> :: = <Identifier> 
void RDP::IDs() {
	if (switch_) {
		outputFile << "<IDs> -> <Identifier> | <Identifier>, <IDs>" << endl;
	}
	if (currentToken.TokenType == "Identifier") {
		AdvanceToken();
		IDsPrime();
	}
	else {
		// Invalid IDs
	}
};
// 13' <IDs Prime> -> , <IDs>
void RDP::IDsPrime() {
	if (currentToken.lexeme == ",") {
		DisplayCurrentToken();
		if (switch_) {
			outputFile << "<IDs Prime> -> , <IDs>" << endl;
		}
		AdvanceToken();
		IDs();
	}
}