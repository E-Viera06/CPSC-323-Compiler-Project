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
};

// R14. <Statement List> -> <Statement> <Statement List Prime>
void RDP::StatementList() {
	if (switch_) {
		outputFile << "<Statement List> -> <Statement> <Statement List Prime>" << endl;
	}
	Statement();
	StatementListPrime();
};
 
// R14' <Statement List Prime> -> <Statement List> | <Empty>
void RDP::StatementListPrime() { 
	if (currentToken.lexeme == "{" || currentToken.TokenType == "Identifier" || currentToken.lexeme == "if" || currentToken.lexeme == "return"
		|| currentToken.lexeme == "put" || currentToken.lexeme == "get" || currentToken.lexeme == "while") {
		if (switch_) {
			outputFile << "<Statement List Prime> -> <Statement List>" << endl;
		}
		StatementList();
	}
	else {
		if ( switch_) {
			outputFile << "<Statement List Prime> -> <Empty>" << endl;
		}
	}
};
 
// R15. <Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
void RDP::Statement() {
	if (currentToken.lexeme == "{") {
		if (switch_) {
			outputFile << "<Statement> -> <Compound>" << endl;
		}
		Compound();
	}
	else if (currentToken.TokenType == "Identifier") {
		if (switch_) {
			outputFile << "<Statement> -> <Assign>" << endl;
		}
		Assign();
	}
	else if (currentToken.lexeme == "if") {
		if (switch_) {
			outputFile << "<Statement> -> <If>" << endl;
		}
		If();
	}
	else if (currentToken.lexeme == "return") {
		if (switch_) {
			outputFile << "<Statement> -> <Return>" << endl;
		}
		Return();
	}
	else if (currentToken.lexeme == "put") {
		if (switch_) {
			outputFile << "<Statement> -> <Print>" << endl;
		}
		Print();
	}
	else if (currentToken.lexeme == "get") {
		if (switch_) {
			outputFile << "<Statement> -> <Scan>" << endl;
		}
		Scan();
	}
	else if (currentToken.lexeme == "while") {
		if (switch_) {
			outputFile << "<Statement> -> <While>" << endl;
		}
		While();
	}
	else {
		// Invalid statement
	}
};
 
// R16. <Compound> -> { <Statement List> }
void RDP::Compound() {
	if (switch_) {
		outputFile << "<Compound> -> { <Statement List> }" << endl;
	}
	if (currentToken.lexeme == "{") {
		DisplayCurrentToken();
		AdvanceToken();
		StatementList();
		if (currentToken.lexeme == "}") {
			DisplayCurrentToken();
			AdvanceToken();
		}
		else {
			// Invalid compound
		}
	}
	else {
		// Invalid compound
	}
};
 
// R17. <Assign> -> <Identifier> = <Expression> ;
void RDP::Assign() { 
	if (switch_) {
		outputFile << "<Assign> -> <Identifier> = <Expression> ;" << endl;
	}
	if (currentToken.TokenType == "Identifier") {
		DisplayCurrentToken();
		AdvanceToken();
		if (currentToken.lexeme == "=") {
			DisplayCurrentToken();
			AdvanceToken();
			Expression();
			if (currentToken.lexeme == ";") {
				DisplayCurrentToken();
				AdvanceToken();
			}
			else {
				// Invalid assign
			}
		}
		else {
			// Invalid assign
		}
	}
	else {
		// Invalid assign
	}
};
 
// R18. <If> -> if ( <Condition> ) <Statement> <If Prime>
void RDP::If() {
	if (switch_) {
		outputFile << "<If> -> if ( <Condition> ) <Statement> <If Prime>" << endl;
	}
	if (currentToken.lexeme == "if") {
		DisplayCurrentToken();
		AdvanceToken();
		if (currentToken.lexeme == "(") {
			DisplayCurrentToken();
			AdvanceToken();
			Condition();
			if (currentToken.lexeme == ")") {
				DisplayCurrentToken();
				AdvanceToken();
				Statement();
				IfPrime();
			}
			else {
				// Invalid if
			}
		}
		else {
			// Invalid if
		}
	}
	else {
		// Invalid if
	}
};
 
// R18' <If Prime> -> fi | else <Statement> fi
void RDP::IfPrime() {
	if (currentToken.lexeme == "fi") {
		DisplayCurrentToken();
		if (switch_) {
			outputFile << "<If Prime> -> fi" << endl;
		}
		AdvanceToken();
	}
	else if (currentToken.lexeme == "else") {
		DisplayCurrentToken();
		if (switch_) {
			outputFile << "<If Prime> -> else <Statement> fi" << endl;
		}
		AdvanceToken();
		Statement();
		if (currentToken.lexeme == "fi") {
			DisplayCurrentToken();
			AdvanceToken();
		}
		else {
			// Invalid if
		}
	}
	else {
		// Invalid if
	}
};
 
// R19. <Return> -> return <Return Prime>
void RDP::Return() {
	if (switch_) {
		outputFile << "<Return> -> return <Return Prime>" << endl;
	}
	if (currentToken.lexeme == "return") {
		DisplayCurrentToken();
		AdvanceToken();
		ReturnPrime();
	}
	else {
		// Invalid return
	}
};
 
// R19' <Return Prime> -> ; | <Expression> ;
void RDP::ReturnPrime() { 
	if (currentToken.lexeme == ";") { 
		DisplayCurrentToken();
		if (switch_) {
			outputFile << "<Return Prime> -> ;" << endl;
		}
		AdvanceToken();
	}
	else {
		if (switch_) {
			outputFile << "<Return Prime> -> <Expression> ;" << endl;
		}
		Expression();
		if (currentToken.lexeme == ";") {
			DisplayCurrentToken();
			AdvanceToken();
		}
		else {
			// Invalid return
		}
	}
};
 
// R20. <Print> -> put ( <Expression> ) ;
void RDP::Print() {
	if (switch_) {
		outputFile << "<Print> -> put ( <Expression> ) ;" << endl;
	}
	if (currentToken.lexeme == "put") {
		DisplayCurrentToken();
		AdvanceToken();
		if (currentToken.lexeme == "(") {
			DisplayCurrentToken();
			AdvanceToken();
			Expression();
			if (currentToken.lexeme == ")") {
				DisplayCurrentToken();
				AdvanceToken();
				if (currentToken.lexeme == ";") {
					DisplayCurrentToken();
					AdvanceToken();
				}
				else {
					// Invalid print
				}
			}
			else {
				// Invalid print
			}
		}
		else {
			// Invalid print
		}
	}
	else {
		// Invalid print
	}
};
 
// R21. <Scan> -> get ( <IDs> ) ;
void RDP::Scan() {
	if (switch_) {
		outputFile << "<Scan> -> get ( <IDs> ) ;" << endl;
	}
	if (currentToken.lexeme == "get") {
		DisplayCurrentToken();
		AdvanceToken();
		if (currentToken.lexeme == "(") {
			DisplayCurrentToken();
			AdvanceToken();
			IDs();
			if (currentToken.lexeme == ")") {
				DisplayCurrentToken();
				AdvanceToken();
				if (currentToken.lexeme == ";") {
					DisplayCurrentToken();
					AdvanceToken();
				}
				else {
					// Invalid scan
				}
			}
			else {
				// Invalid scan
			}
		}
		else {
			// Invalid scan
		}
	}
	else {
		// Invalid scan
	}
};
 
// R22. <While> -> while ( <Condition> ) <Statement> 
void RDP::While() {
	if (switch_) {
		outputFile << "<While> -> while ( <Condition> ) <Statement>" << endl;
	}
	if (currentToken.lexeme == "while") { 
		DisplayCurrentToken();
		AdvanceToken();
		if (currentToken.lexeme == "(") {
			DisplayCurrentToken();
			AdvanceToken();
			Condition();
			if (currentToken.lexeme == ")") { 
				DisplayCurrentToken();
				AdvanceToken();
				Statement();
			}
			else {
				// Invalid while
			}
		}
		else {
			// Invalid while
		}
	}
	else {
		// Invalid while
	}
};

// R23. <Condition> -> <Expression> <Relop> <Expression>
void RDP::Condition() {
    if (switch_) {
        outputFile << "<Condition> -> <Expression> <Relop> <Expression>\n";
    }
    Expression();
    Relop();
    Expression();
}

// R24. <Relop> -> == | != | > | < | <= | >=
void RDP::Relop() {

    if (currentToken.lexeme == "==" || currentToken.lexeme == "!=" || currentToken.lexeme == ">" || currentToken.lexeme == "<" 
													|| currentToken.lexeme == "<=" || currentToken.lexeme == ">=") {
        if (switch_) {
            outputFile << "<Relop> -> " << currentToken.lexeme << "\n";
        }
        DisplayCurrentToken();
        AdvanceToken();
    }
    else {

    }
}

// R25. <Expression> -> <Term> <Expression Prime>
void RDP::Expression() {
    if (switch_) {
        outputFile << "<Expression> -> <Term> <Expression Prime>\n";
    }
    Term();
    ExpressionPrime();
}

// R25' <Expression Prime> -> + <Term> <Expression Prime> | - <Term> <Expression Prime> | <Empty>
void RDP::ExpressionPrime() {
    if (currentToken.lexeme == "+") {
        if (switch_) {
            outputFile << "<Expression Prime> -> + <Term> <Expression Prime>\n";
        }
        DisplayCurrentToken();
        AdvanceToken();
        Term();
        ExpressionPrime();
    }
    else if (currentToken.lexeme == "-") {
        if (switch_) {
            outputFile << "<Expression Prime> -> - <Term> <Expression Prime>\n";
        }
        DisplayCurrentToken();
        AdvanceToken();
        Term();
        ExpressionPrime();
    }
    else {
        if (switch_) {
            outputFile << "<Expression Prime> -> <Empty>\n";
        }
    }
};

// R26. <Term> -> <Factor> <Term Prime>
void RDP::Term() {
    DisplayCurrentToken();
    if (switch_) {
        outputFile << "<Term> -> <Factor> <Term Prime>" << endl;
    }
    Factor();
    TermPrime();
};

// R26'. <Term Prime> -> * <Factor> <Term Prime> | / <Factor> <Term Prime> | <Empty>
void RDP::TermPrime() {
    if (currentToken.lexeme == "*") {
        DisplayCurrentToken();
        if (switch_) {
            outputFile << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
        }
        AdvanceToken();
        Factor();
        TermPrime();
    }
    else if (currentToken.lexeme == "/") {
        DisplayCurrentToken();
        if (switch_) {
            outputFile << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
        }
        AdvanceToken();
        Factor();
        TermPrime();
    }
};

// R27. <Factor> -> - <Primary> | <Primary>
void RDP::Factor() {
    if (currentToken.lexeme == "-") {
        DisplayCurrentToken();
        if (switch_) {
            outputFile << "<Factor> -> - <Primary>" << endl;
        }
        AdvanceToken();
        Primary();
    }
    else {
        DisplayCurrentToken();
        if (switch_) {
            outputFile << "<Factor> -> <Primary>" << endl;
        }
        Primary();
    }
};

// R28. <Primary> -> <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false
void RDP::Primary() {
    if (currentToken.TokenType == "Identifier") {
        DisplayCurrentToken();
        AdvanceToken();

        // Check if this identifier is followed by '(' for a function/procedure call
        if (currentToken.lexeme == "(") {
            if (switch_) {
                outputFile << "<Primary> -> <Identifier> ( <IDs> )" << endl;
            }
            DisplayCurrentToken();
            AdvanceToken();

            IDs();

            if (currentToken.lexeme == ")") {
                DisplayCurrentToken();
                AdvanceToken();
            }
            else {

            }
        }
        else {
            if (switch_) {
                outputFile << "<Primary> -> <Identifier>" << endl;
            }
        }
    }
    else if (currentToken.TokenType == "Integer") {
        DisplayCurrentToken();
        if (switch_) {
            outputFile << "<Primary> -> <Integer>" << endl;
        }
        AdvanceToken();
    }
    else if (currentToken.TokenType == "Real") {
        DisplayCurrentToken();
        if (switch_) {
            outputFile << "<Primary> -> <Real>" << endl;
        }
        AdvanceToken();
    }
    else if (currentToken.lexeme == "(") {
        DisplayCurrentToken();
        if (switch_) {
            outputFile << "<Primary> -> ( <Expression> )" << endl;
        }
        AdvanceToken();

        Expression();

        if (currentToken.lexeme == ")") {
            DisplayCurrentToken();
            AdvanceToken();
        }
        else {
        }
    }
    else if (currentToken.lexeme == "true" || currentToken.lexeme == "false") {
        DisplayCurrentToken();
        if (switch_) {
            outputFile << "<Primary> -> " << currentToken.lexeme << endl;
        }
        AdvanceToken();
    }
    else {
    }
};
