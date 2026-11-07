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
	vector<Token> Tokens2 = Lexer2.lex();
	vector<Token> Tokens3 = Lexer3.lex();

	RDP Parser1(Tokens, "output1.txt");
	Parser1.parse();

	
}