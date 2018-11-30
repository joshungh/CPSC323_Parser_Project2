/*
Parser_Project2

Joshua Ungheanu
Derek Dorr
Adam Weesner

CPSC 323 Fall 2018
Shohrat Geldiyev

*********************************************
Writing a parser using the Lexer Analyzer.
The parser will analyze the syntax written.
*********************************************/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Parser.h"
#include "Lexer.h"
#include <string>

using namespace std;


int main() {
	string fileName;
	cin >> fileName;

	ifstream fin;
	fin.open(fileName);

	cout << "TOKEN" << "\t\t" << "LEXEME" << "\n";
	cout << "----------------------------------------\n";
	string word;
	while (!fin.eof()) {
		//cout << "\n" << lexer(fin).token << " \t" << lexer(fin).lexeme << "\n\n";
		Parser(fin);
	}
	//fin.close();
	//fin.open(fileName);
	//Parser(fin);




	system("pause");
	fin.close();

	return 0;
}
