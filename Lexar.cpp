#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
// Token Record
struct Token {
	string TokenType;
	string lexeme;
};

class Lexer {
	public:
		Lexer();
		Lexer(const string& filename); // Constructor with filename
		~Lexer(); // Destructor

		// Functions for File Management
		bool openFile(const string& filename);
		void closeFile();
		
		vector<Token> lex(); // This is will be the main function of this class

	private:
		const vector<string> Keywords = { "function", "integer", "boolean", "real", "if", "else", "fi", 
										  "return", "put", "get", "while", "true", "false"};

		const vector<string> Operators = { "==", "!=", "<=", ">=", ">", "<", "=", "+", "-", "*", "/" };

		const vector<string> Separators = { "(", ")", "{", "}", "[", "]", ";", ",", "@"};

		bool isAKeyWord(const string& identifier); // Work on later, function to check if the indentifier is inside keywords 

	ifstream SourceFile;
};

#endif 
