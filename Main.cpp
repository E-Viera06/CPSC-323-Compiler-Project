#include <iostream>
#include <string>
#include <fstream>
#include "Lexer.h"
using namespace std;


int main() {
	cout << "Printing Tokens:" << endl;
	Lexer Lexer1("TestCase1.txt");

	vector<Token> Tokens = Lexer1.lex();
	for (const auto& token : Tokens) {
		cout << "Token Type: " << token.TokenType << " Lexeme: " << token.lexeme << endl;
	}
}

/*
 Current Test: while (fahr <= upper) a = 23.00; ! this is a sample !
*/