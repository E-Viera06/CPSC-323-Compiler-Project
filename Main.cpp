#include <iostream>
#include <string>
#include <fstream>
#include "Lexer.h"
#include "RDP.h"
using namespace std;

int main() {
	cout << "Output files Generated: output1.txt, output2.txt, output3.txt" << endl;
	// Create Lexer objects for each test case
	Lexer Lexer1("TestCase1.txt");
	Lexer Lexer2("TestCase2.txt");
	Lexer Lexer3("TestCase3.txt");
	// Create output files for each test case
	ofstream outputFile1("output1.txt");
	ofstream outputFile2("output2.txt");
	ofstream outputFile3("output3.txt");

	// Write the output to the files
	outputFile1 << "Output for TestCase1.txt: " << endl;
	outputFile1 << "Token Type     Lexeme" << endl;
	vector<Token> Tokens = Lexer1.lex();
	RDP Parser1(Tokens);
	Parser1.parse();
	for (const auto& token : Tokens) {
		outputFile1 << token.TokenType << "     " << token.lexeme << endl;
	}
	outputFile1.close();

	outputFile2 << "Output for TestCase2.txt: " << endl;
	outputFile2 << "Token Type     Lexeme" << endl;
	Tokens = Lexer2.lex();
	for (const auto& token : Tokens) {
		outputFile2 << token.TokenType << "     " << token.lexeme << endl;
	}
	outputFile2.close();

	outputFile3 << "Output for TestCase3.txt: " << endl;
	outputFile3 << "Token Type     Lexeme" << endl;
	Tokens = Lexer3.lex();
	for (const auto& token : Tokens) {
		outputFile3 << token.TokenType << "     " << token.lexeme << endl;
	}
	outputFile3.close();
}