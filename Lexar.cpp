#include "Lexer.h"
#include <string>
#include <iostream>
#include <cctype>
#include <cstdio>

using namespace std;

Lexer::Lexer() {};// Default Constructor

Lexer::~Lexer() {};

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
		return 0; // Column 1 for letters
	}
	else if (isdigit(c)) {
		return 1; // Column 2 for digits
	}
	else if (c == '_') {
		return 2; // Column 3 for underscore
	}
	else return -1; // Invalid character
}

Token IdentifierDFSM(const string& input) { // This function will use the DFSM for L(L| D | '_')* // First design of DFSM for identifiers
	int array[6][3] = {
	{ 2,6,6 },
	{ 3,4,5 },
	{ 3,4,5 },
	{ 3,4,5 },
	{ 3,4,5 },
	{ 6,6,6 },};

	int current_state = 0;
	const int AcceptingStates[4] = {1, 2, 3, 4};
	for (int i = 0; i < input.length(); ++i) { // Main Loop
		
		int column = Identifier_char_col(input[i]);
		if (column == -1) { // Invalid character
			current_state = 5; // Set to invalid state
			break;
		}
		current_state = (array[current_state][column]-1);

		if (current_state == 5) {
			break;}	
	} // Need to an a check if the token is part of keyword

	bool isAcceptingState = (current_state >= 1 && current_state <= 4);

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

int Int_char_col(char c) {
	if (isdigit(c)) {
		return 0; // Column 1 for digits
	}
	else
		return -1; // invalid character
}

Token IntDFSM(const string& input) { // This function will use the DFSM for d+
	int states_array[2][1] = {
							{2},
							{2}, }; // 1 = 2 because column 0 is the only column for digits and 1 is the only accepting state

	int Current_State = 0, AcceptingState = 1;
	for (int i = 0; i < input.length(); ++i) { // Main Loop

		int column = Int_char_col(input[i]);
		if (column == -1) { // Invalid character
			Current_State = -1; // Set to invalid state
			break;
		}
		Current_State = (states_array[Current_State][column] - 1);
	}
	bool isAcceptingState = (Current_State == AcceptingState);

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

int RealNum_char_col(char c) {
	if (isdigit(c)) {
		return 0; // Column 0 for digits
	}
	else if (c == '.') {
		return 1; // Column 1 for decimal point
	}
	else {
		return -1; // Invalid character
	}
}

Token RealNumDFSM(const string& input) { // This function will use the DFSM for d+.d+
	int state_array[5][2] = {
		{2,3}, // 0 = state 1, 1 = state 2, 2 = state 3, 3 = state 4, 4 = state 5
		{2,3},
		{3,5},
		{4,5},
		{5,5},}, AcceptingState = 2, CurrentState = 0;
	for (int i = 0; i < input.length(); ++i) { // Main Loop

		int column = RealNum_char_col(input[i]);
		if (column == -1) { // Invalid character
			CurrentState = 4; // Set to invalid state
			break;
		}
		CurrentState = (state_array[CurrentState][column] - 1);
	}
	bool isAcceptingState = (CurrentState == AcceptingState);
	if (isAcceptingState) {
		Token token;
		token.TokenType = "RealNumber";
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

vector<Token> Lexer::lex() {
	vector<Token> tokens;

	if (!SourceFile.is_open()) { //checks for files to open
		return tokens;
	}

	while (true) {

		int next = SourceFile.peek(); //end of file check
		if (next == EOF) {
			break;
		}
		char c = static_cast<char>(next);

		if (isspace(static_cast<unsigned char>(c))) { //checks for space
			SourceFile.get();
			continue;
		}

		if (c == '!') { //checks for comments
			SourceFile.get();
			if (SourceFile.peek() == '=') {
				SourceFile.unget();
			}
			else {
				int ch = SourceFile.get();
				while (ch != EOF && ch != '!') {
					ch = SourceFile.get();
				}
				if (ch == EOF) {
					Token token;
					token.TokenType = "Invalid";
					token.lexeme = "unterminated comment";
					tokens.push_back(token);
				}
				continue;
			}
		}

		if (isalpha(static_cast<unsigned char>(c))) { //checks for alphabet characters
			string word = "";
			while (true) {
				int n = SourceFile.peek();
				if (n == EOF) {
					break;
				}
				unsigned char uc = static_cast<unsigned char>(n);
				if (isalpha(uc) || isdigit(uc) || n == '_') {
					word += static_cast<char>(SourceFile.get());
				}
				else {
					break;
				}

			}
			Token token = IdentifierDFSM(word);

			if (isAKeyWord(word)) {
				token.TokenType = "Keyword";
			}

			tokens.push_back(token);
			continue;
		}


		if (isdigit(static_cast<unsigned char>(c)) || c == '.') { // checks for real number
			string number = "";
			bool isReal = false;

			while (isdigit(SourceFile.peek())) {
				number += static_cast<char>(SourceFile.get());
			}

			if (SourceFile.peek() == '.') {
				SourceFile.get();
				if (isdigit(SourceFile.peek())) {
					isReal = true;
					number += '.';
					while (isdigit(SourceFile.peek())) {
						number += static_cast<char>(SourceFile.get());
					}
				}
				else if (number.empty()) {
					isReal = true;
					number = ".";
				}
				else {
					SourceFile.unget();
				}
			}

			Token token;
			if (isReal) {
				token = RealNumDFSM(number);
			}
			else {
				token = IntDFSM(number);
			}
			tokens.push_back(token);
			continue;
		}
		//unfinished. intended for operators and seperators
		SourceFile.get();
		Token token;
		token.TokenType = "";
		token.lexeme = string(1, c);
		tokens.push_back(token);

	}

	return tokens;
};
