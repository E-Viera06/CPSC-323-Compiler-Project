#include "Lexer.h"
#include <string>
#include <iostream>
using namespace std;

Lexer::Lexer() {};// Default Constructor

Lexer::Lexer(const string& filename) { // Explicit Constructor with filename
	openFile(filename);
};

bool Lexer::openFile(const string& filename) {
	if (SourceFile.is_open()) {
		SourceFile.close();
	}
	SourceFile.open(filename);
	return SourceFile.is_open();
};

void Lexer::closeFile() {
	if (SourceFile.is_open()) {
		SourceFile.close();
	}
};

bool Lexer::isAKeyWord(const string& identifier) { // This function will check if the identifier is inside the keywords vector
	for (const auto& keyword : Keywords) {
		if (identifier == keyword) {
			return true;
		}
	}
	return false;
};

int Identifier_char_col(char c) {
	if (isalpha(c)) {
		return 0; // Column 0 for letters
	}
	else if (isdigit(c)) {
		return 1; // Column 1 for digits
	}
	else if (c == '_') {
		return 2; // Column 2 for underscore
	}
	else {
		return -1; // Invalid character
	}
}

int Int_char_col(char c) {
	 if (isdigit(c)) {
		return 2; // Column 2 for digits
	}	else {
		return -1; // Invalid character
	}
}

Token IdentifierDFSM(const string& input) { // This function will use the DFSM for L(L| D | '_')* // First design of DFSM for identifiers
	int array[6][3] = {
	{ 2,6,6 },
	{ 3,4,5 },
	{ 3,4,5 },
	{ 3,4,5 },
	{ 3,4,5 },
	{ 6,6,6 },};

	int InitialState = 1;
	const int AcceptingStates[4] = {2, 3, 4, 5};
	bool isAcceptingState = false;
	for (int i = 1; i < input.length(); ++i) { // Main Loop

		int column = Identifier_char_col(input[i]);
		int state = array[InitialState][column];
		if (state == 6) {
			isAcceptingState = false;
		}
		else { isAcceptingState = true; }
	} // Need to an a check if the token is part of keyword
	if (isAcceptingState) {
		Token token;
		token.TokenType = "Identifier";
		token.lexeme = input;
		return token;
	}
	else {
		Token token;
		token.TokenType = "Invalid"; // Invalid Token
		token.lexeme = input;
		return token;
	}
};


Token IntDFSM(const string& input) { // This function will use the DFSM for d+
	int states_array[2][1] = {
	{ 2,},
	{ 2,},
	};

	int InitialState = 1, AcceptingState = 2;
	bool isAcceptingState = false;
	for (int i = 1; i < input.length(); ++i) { // Main Loop

		int column = Int_char_col(input[i]);
		int state = states_array[InitialState][column];
		if (state == 6) {
			isAcceptingState = false;
		}
		else { isAcceptingState = true; }
	}
	if (isAcceptingState) {
		Token token;
		token.TokenType = "Integer";
		token.lexeme = input;
		return token;
	}
	else {
		Token token;
		token.TokenType = "Invalid"; // Invalid Token
		token.lexeme = input;
		return token;
	}
}

Token RealDFSM(const string& input) { // This function will use the DFSM for d+.d+
}

Token Lexer::lex() {
	
	/*
		DFSM will be placed into here to filter out the Tokens from the source files
	*/
};