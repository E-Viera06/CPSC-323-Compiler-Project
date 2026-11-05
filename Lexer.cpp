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
bool Lexer::isAOperator(const string& input) {
	for (const auto& op : Operators) {
		if (input == op) return true;
	}
	return false;
}
bool Lexer::isSeparator(const string& input) {
	for (const auto& sp : Separators) {
		if (input == sp) return true;
	}
	return false;
}
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
	}

	bool isAcceptingState = (current_state >= 1 && current_state <= 4);
	if (isAcceptingState) {
		Token token;
		token.TokenType = "Identifier";
		token.lexeme = input;
		return token;
	}
	else {
		Token token;
		token.TokenType = "Invalid Token"; // Invalid Token
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
		token.TokenType = "Invalid Token"; // Invalid Token
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
		{4,5},
		{4,5},
		{5,5},}, AcceptingState = 3, CurrentState = 0;
	for (int i = 0; i < input.length(); ++i) { // Main Loop

		int column = RealNum_char_col(input[i]);
		if (column == -1) { // Invalid character
			CurrentState = 4; // Set to invalid state
			break;
		}
		CurrentState = (state_array[CurrentState][column] - 1); // Update the current state based on the transition table
	}
	bool isAcceptingState = (CurrentState == AcceptingState);
	if (isAcceptingState) {
		Token token;
		token.TokenType = "Real";
		token.lexeme = input;
		return token;
	}
	else {
		Token token;
		token.TokenType = "Invalid Token"; // Invalid Token
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

		if (c == '!') { //checks for comments in order to igore them
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
		// Identifier Handling and Keyword Handling
		if (isalpha(static_cast<unsigned char>(c)) || c == '_') { // checks for alphabet characters or underscore
			string word = "";
			while (true) {
				int n = SourceFile.peek(); // Peek at the next character over without consuming it
				if (n == EOF) { // If we reach the end of the file, break out of the loop
					break;
				}
				char nc = static_cast<char>(n);

				// Should stop reading when reaching whitespace, operators, or separators
				string str(1, nc);
				if (isspace(static_cast<unsigned char>(nc)) || isAOperator(str) || isSeparator(str)) {
					break;
				}

				word += static_cast<char>(SourceFile.get()); // Consume the character and add it to the word
			}

			Token token = IdentifierDFSM(word);

			if (token.TokenType == "Identifier" && isAKeyWord(word)) { // If the token is an identifier and also a keyword, change its type to "Keyword"
				token.TokenType = "Keyword";
			}

			tokens.push_back(token);
			continue;
		}

		// Int and Real Number Handling
		if (isdigit(static_cast<unsigned char>(c)) || c == '.') { // checks for digits or decimal point
			string number = "";

			while (true) {
				int n = SourceFile.peek(); // Peek at the next character without consuming it
				if (n == EOF) break; // If we reach the end of the file, break out of the loop

				char nc = static_cast<char>(n); // Convert the peeked character to char
				string str(1, nc); // Create a string from the character for operator and separator checks

				// Stop consuming at whitespace, operators, or separators
				if (isspace(static_cast<unsigned char>(nc)) || isAOperator(str) || isSeparator(str)) {
					break;
				}

				number += static_cast<char>(SourceFile.get()); // Consume the character and add it to the number string
			}

			Token token;
			// if a dot is present, send it to the RealNumDFSM, otherwise send it to the IntDFSM
			if (number.find('.') != string::npos) {
				token = RealNumDFSM(number);
			}
			else {
				token = IntDFSM(number);
			}

			tokens.push_back(token);
			continue;
		}
		// Operator and Separator Checks
		SourceFile.get(); // Consume the character since it's not part of an identifier or number
		string singleChar(1, c); // Create a string for the single character operator or separator
		int nextchar = SourceFile.peek();
		if (nextchar != EOF) { // Check for double character operators if it not the end of the file
			string doubleChar = singleChar + static_cast<char>(nextchar); // Create a string for the double character operator
			// Simple if statement to check if the double character operator is valid, if it is, consume the next character and add the token to the vector
			if (isAOperator(doubleChar)) {
				SourceFile.get();
				Token token;
				token.TokenType = "Operator";
				token.lexeme = doubleChar;
				tokens.push_back(token);
				continue;
			}
		}
		if (isAOperator(singleChar)) {
			Token token;
			token.TokenType = "Operator";
			token.lexeme = singleChar;
			tokens.push_back(token);
			continue;
		}
		if (isSeparator(singleChar)) {
			Token token;
			token.TokenType = "Separator";
			token.lexeme = singleChar;
			tokens.push_back(token);
			continue;
		}
		Token token;
		token.TokenType = "Invalid Token";
		token.lexeme = singleChar;
		tokens.push_back(token);
	}
	return tokens;
};
