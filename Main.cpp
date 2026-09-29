#include <iostream>
#include <string>
using namespace std;
string InputString;
char CurrentChar;

void IndentifierDFSm() {
	cout << "Indentifier Found";
}

void IntAndRealDFSM() {
	cout << "Number Found";
}

void Lexer() {
	char FirstCharinToken = cin.get();
	if (isalpha(FirstCharinToken)) {
		IndentifierDFSm();
	} else if(isdigit(FirstCharinToken)) {
		IntAndRealDFSM();
	}
}

int main() {
	cout << "Enter something:";
	Lexer();
}


