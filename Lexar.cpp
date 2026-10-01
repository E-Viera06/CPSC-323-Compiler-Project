#include "Lexer.h"
#include <string>
#include <iostream>
using namespace std;


Lexer::Lexer() {};// Default Constructor

Lexer::Lexer(const string& filename) {
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

Token Lexer::lex() {
	/*
		DFSM will be placed into here to filter out the Tokens from the source files
	*/
};
