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

using namespace std;


int main(int argc, char* argv[]) {
	
	if (argc < 2) {
		
		cout << "Missing file's name." << endl;
		return 1;
	}
	
	ifstream fin(argv[1]);

	if (!fin) {
		cout << "File does not exist." << endl;
		return 1;
	}

	Parser(fin); 
	
	system("pause");
	fin.close();

	return 0;
}
