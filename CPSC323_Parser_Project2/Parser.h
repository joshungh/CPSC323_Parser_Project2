#pragma once
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

#ifndef PARSER_H 
#define PARSER_H

#include "Lexer.h"
#include <string>


void term(ifstream&, TokenType&);
void factor(ifstream&, TokenType&);
void functionCall(ifstream&, TokenType&);
void ArgList(ifstream&, TokenType&);
void varList(ifstream&, TokenType&);
void statementList(ifstream&, TokenType&);
void statement(ifstream&, TokenType&);
void assign(ifstream&, TokenType&);
void read(ifstream&, TokenType&);
void write(ifstream&, TokenType&);
void If(ifstream&, TokenType&);
void While(ifstream&, TokenType&);
void Do(ifstream&, TokenType&);
void error(string, TokenType&, ifstream&);
bool accept(string, TokenType&, ifstream&);
void expect(string, TokenType&, ifstream&);
void program(ifstream&, TokenType&);
void decList(ifstream&, TokenType&);
void dec(ifstream&, TokenType&);
void condition(ifstream&, TokenType&);
void RelOp(ifstream&, TokenType&);
void expression(ifstream&, TokenType&);
void functionList(ifstream&, TokenType&);
void function(ifstream&, TokenType&);
void parameterList(ifstream&, TokenType&);
void parameter(ifstream&, TokenType&);
void type(ifstream&, TokenType&);
void Return(ifstream&, TokenType&);


 // Top down Parser - sets grammer rules from file
void Parser(ifstream &file) {
	
	TokenType curToken = lexer(file);

	// Checking program
	program(file, curToken);

}

// Error function
void error(string s, TokenType& check, ifstream &file) {
	cout << "Error: unexpected string: " << check.lexeme << ", expected " << s << endl;
}

// Checks for lemexe
bool accept(string s, TokenType& check, ifstream &file) {

	if (s.compare(check.lexeme) == 0) {
		check = lexer(file);
		return true;
	}
	else {
		return false;
	}
}

void expect(string s, TokenType& check, ifstream &file) {
	if (!accept(s, check, file)) {
		error(s, check, file);
	}
}

// Program Function
void program(ifstream& file, TokenType& token) {
	
	expect("program", token, file);

	
	if (token.token.compare("Type") == 0) {
		decList(file, token);
	}
	
	if (token.lexeme.compare("function") == 0) {
		functionList(file, token);
	}

	//  Registers token if presented
	expect("begin", token, file);

	// Looks through the statement list
	if ((!(token.lexeme.compare("end"))) == 0) {
		statementList(file, token);
	}

	// Registers token if presented
	expect("end", token, file);

	// Registers token if presented
	expect(".", token, file);

	cout << "Program => program [decList] [functionList] begin [statementList] end." << endl;
}
// Declist Function
void decList(ifstream& file, TokenType& token) {

	// Check for initial declaration 
	do {
		dec(file, token);
	} while (token.token.compare("Type") == 0);

	cout << "decList => dec {dec}" << endl;
}

// Dec Function
void dec(ifstream& file, TokenType& token) {

	// Call Type function
	type(file, token);

	// Call VarList function
	varList(file, token);

	// Registers token if presented
	expect(";", token, file);

	cout << "dec => Type varList ;" << endl;
}

// FunctionList function
void functionList(ifstream& file, TokenType& token) {
	// Check for initial function 
	do {
		function(file, token);
	} while (token.lexeme.compare("function") == 0);
	
	cout << "functionList =>function {function}" << endl;
}

// Function function
void function(ifstream& file, TokenType& token) {

	expect("function", token, file);

	// Registers the name of the function
	if (token.token.compare("Identifier") == 0)
	{
		// Registers token if presented
		token = lexer(file);
	}

	expect("(", token, file);

	// Checks if parameter list is present, registers parameterList
	if ((!token.lexeme.compare(")")) == 0) {
		parameterList(file, token);
	}
	expect(")", token, file);
	expect(":", token, file);
	type(file, token);
	expect(";", token, file);

	// While begin is not token, call decList function
	if ((!(token.lexeme.compare("begin"))) == 0) {
		decList(file, token);
	}
	expect("begin", token, file);

	// While end is not token, call statementList function
	if ((!(token.lexeme.compare("end"))) == 0) {
		statementList(file, token);
	}
	expect("end", token, file);
	cout << "function => function Ident ( [parameterList] ): Type ; [decList] begin [statementList] end " << endl;
}

// Parameter List Function
void parameterList(ifstream& file, TokenType& token) {
	// Call Parameter function
	parameter(file, token);

	// If a comma is detected, add another parameter 
	while (token.lexeme.compare(",") == 0) {
		// registers token if presented
		expect(",", token, file);
		// call the parameter function
		parameter(file, token);
	}
	cout << "parameterList => parameter {, parameter}" << endl;
}

// Parameter Function
void parameter(ifstream& file, TokenType& token) {
	// Call type function
	type(file, token);

	//  Registers token if presented
	if (token.token.compare("Identifier") == 0)
	{
		token = lexer(file);
	}

	cout << "parameter => Type Ident" << endl;
}

// Type Function
void type(ifstream& file, TokenType& token) {
	// Looks at the type
	if (token.lexeme.compare("int") == 0 || token.lexeme.compare("real") == 0 || token.lexeme.compare("string") == 0)
	{
		//  Registers token if presented
		token = lexer(file);
		
		cout << "Type => int | real | string" << endl;
	}
}

// VarList Function
void varList(ifstream& file, TokenType& token) {
	//  Registers token if presented
	if (token.token.compare("Identifier") == 0)
	{
		token = lexer(file);
	}

	// If a comma is detected then it'll continue
	while (token.lexeme.compare(",") == 0) {
		//  Registers token if presented
		token = lexer(file);
	
		if (token.token.compare("Identifier") == 0)
		{
			token = lexer(file);
		}
	}

	cout << "varList => Ident {,Ident}" << endl;
}

// Statement List Function
void statementList(ifstream& file, TokenType& token) {
	// statement function is called
	statement(file, token);

	// Calls statement function until the lexemes are Ident, read, write, if, while, do, return
	while (token.token.compare("Identifier") == 0 || token.lexeme.compare("read") == 0 || token.lexeme.compare("write") == 0 || token.lexeme.compare("if") == 0 || token.lexeme.compare("while") == 0 || token.lexeme.compare("do") == 0 || token.lexeme.compare("return") == 0) {
		statement(file, token);
	}

	cout << "statementList => statement{statement}" << endl;
}

// Statement Function
void statement(ifstream& file, TokenType& token) {
	//  Registers token if presented
	if (token.token.compare("Identifier") == 0)
	{	// Assign function is called and statement is read
		assign(file, token);
		cout << "statement => Assign" << endl;
	}
	//  Registers token if presented
	else if (token.lexeme.compare("read") == 0)
	{	// Read function is called and statement is read
		read(file, token);
		cout << "statement => Read" << endl;
	}
	//  Registers token if presented
	else if (token.lexeme.compare("write") == 0)
	{
		// Write function is called and statement is read
		write(file, token);
		cout << "statement => Write" << endl;
	}
	//  Registers token if presented
	else if (token.lexeme.compare("if") == 0)
	{	// If function is called and statement is read
		If(file, token);
		cout << "statement => If" << endl;
	}
	//  Registers token if presented
	else if (token.lexeme.compare("while") == 0)
	{	// While function is called and statement is read
		While(file, token);
		cout << "statement => While" << endl;
	}
	//  Registers token if presented
	else if (token.lexeme.compare("do") == 0)
	{	// Do function is called and statement is read
		Do(file, token);
		cout << "statement => Do" << endl;
	}
	//  Registers token if presented
	else if (token.lexeme.compare("return") == 0)
	{	// Return function is called and statement is read
		Return(file, token);
		cout << "statement => Return" << endl;
	}

}

// Assign Function
void assign(ifstream& file, TokenType& token) {
	// Registers token if presented
	if (token.token.compare("Identifier") == 0)
	{
		token = lexer(file);
	}
	expect(":=", token, file);

	// Expression function is called
	expression(file, token);
	expect(";", token, file);

	cout << "Assign => Ident := expression;" << endl;
}

// Read Function
void read(ifstream& file, TokenType& token) {

	expect("read", token, file);

	expect("(", token, file);
	// Call VarList function
	varList(file, token);

	expect(")", token, file);
	expect(";", token, file);

	cout << "Read => read ( varList ) ; " << endl;
}
// Write Function
void write(ifstream& file, TokenType& token) {
	
	expect("write", token, file);

	expect("(", token, file);

	// Search until token matches with the comma
	expression(file, token);

	while (token.lexeme.compare(",") == 0) {
		// Registers token if presented
		token = lexer(file);
		expression(file, token);
	}

	expect(")", token, file);
	expect(";", token, file);

	cout << "Write => write ( expression {, expression} ) ;" << endl;
}

// If Function
void If(ifstream& file, TokenType& token) {
	// Registers token if presented
	expect("if", token, file);
	expect("(", token, file);

	// Condition function is called
	condition(file, token);
	expect(")", token, file);
	expect("begin", token, file);

	// StatementList is called
	statementList(file, token);
	expect("end", token, file);

	if (token.lexeme.compare("elsif") == 0) {

		do {
			// Registers token if presented
			expect("elsif", token, file);

			expect("(", token, file);
			// Condition Function is called
			condition(file, token);

			expect(")", token, file);

			expect("begin", token, file);
			// StatementList Function is called
			statementList(file, token);
			expect("end", token, file);

		} while (token.lexeme.compare("elsif") == 0); // Loop continues until token is presented

	}

	if (token.lexeme.compare("else") == 0) {
		// Registers token if presented
		expect("else", token, file);
		expect("begin", token, file);
		// StatementList Function is called
		statementList(file, token);
		expect("end", token, file);
	}

	cout << "If => if ( condition ) begin statementList end { elsif ( condition ) begin statementList end } [else begin statementList end ]" << endl;
}

// While Function
void While(ifstream& file, TokenType& token) {
	// Registers token if presented
	if (token.lexeme.compare("while") == 0) {
		token = lexer(file);
	}

	expect("(", token, file);
	// Condition Function is called
	condition(file, token);

	expect(")", token, file);

	expect("begin", token, file);

	// See if statementlist exist
	if (token.token.compare("Identifier") == 0 || token.lexeme.compare("read") == 0 || token.lexeme.compare("write") == 0 || token.lexeme.compare("if") == 0 || token.lexeme.compare("while") == 0 || token.lexeme.compare("do") == 0 || token.lexeme.compare("return") == 0) {
		// StatementList function is called
		statementList(file, token);
	}

	expect("end", token, file);

	cout << "While => while ( condition ) begin [statmentList] end" << endl;
}
// Do Function
void Do(ifstream& file, TokenType& token) {

	// // Registers token if presented
	expect("do", token, file);

	// Check for statment list and call statementList
	if (token.token.compare("Identifier") == 0 || token.lexeme.compare("read") == 0 || token.lexeme.compare("write") == 0 || token.lexeme.compare("if") == 0 || token.lexeme.compare("while") == 0 || token.lexeme.compare("do") == 0 || token.lexeme.compare("return") == 0) {
		statementList(file, token);
	}

	expect("until", token, file);

	expect("(", token, file);

	// Condition function is called
	condition(file, token);

	expect(")", token, file);

	expect(";", token, file);

	cout << "do => do [statementList] until ( condition) ;" << endl;
}
// Return Function
void Return(ifstream& file, TokenType& token) {
	// Registers token if presented
	expect("return", token, file); 

	// Expression function is called
	expression(file, token);

	expect(";", token, file);

	cout << "Return => return expression ;" << endl;
}

// Condition Function
void condition(ifstream& file, TokenType& token) {

	// Expression function is called
	expression(file, token);

	// Relop function is called
	RelOp(file, token);

	// Expression function is called
	expression(file, token);

	cout << "condition => expression RelOp expression" << endl;
}

// Relop Function
void RelOp(ifstream& file, TokenType& token) {

	// Check if it is a RelOp
	if (token.token.compare("RelOp") == 0)
	{
		// Registers token if presented
		token = lexer(file);
	}

}

// Expression Function
void expression(ifstream& file, TokenType& token) {

	// Term Function is called
	term(file, token);

	//Is lexeme + or -
	if (token.lexeme.compare("+") == 0 || token.lexeme.compare("-") == 0) {
		// Registers token if presented
		token = lexer(file);

		// Term Function is called
		term(file, token);
	}

	cout << "expression => Term { (+|-) Term }" << endl;
}

// Term Function
void term(ifstream& file, TokenType& token) {

	// Call Factor function
	factor(file, token);

	// Is lexeme * or /
	if (token.lexeme.compare("*") == 0 || token.lexeme.compare("/") == 0) {
		// Registers token if presented
		token = lexer(file);

		// Call itself
		factor(file, token);
	}

	cout << "Term => Factor { (*|/) Factor } " << endl;
}

// Factor Function
void factor(ifstream& file, TokenType& token) {

	//Check if identifier, intConst, realConst, strConst
	if (token.token.compare("Identifier") == 0)
	{
		// Registers token if presented
		token = lexer(file);

		// Seach for identifiers
		if (token.lexeme.compare("(") == 0) {
			// FunctionCall Function is called
			functionCall(file, token);

			cout << "Factor => FuncCall" << endl;
		}

		else {
		
			cout << "Factor => Ident" << endl;
		}
	}
	else if (token.token.compare("IntConst") == 0)
	{
		// Registers token if presented
		token = lexer(file);
		cout << "Factor => IntConst" << endl;
	}
	else if (token.token.compare("RealConst") == 0)
	{
		// Registers token if presented
		token = lexer(file);
		cout << "Factor => RealConst" << endl;
	}
	else if (token.token.compare("StrConst") == 0)
	{
		// Registers token if presented
		token = lexer(file);
		cout << "Factor => StrConst" << endl;
	}
	else if (token.lexeme.compare("(") == 0)
	{

		expect("(", token, file);
		// Expression Function Called
		expression(file, token);
		expect(")", token, file);

		cout << "Factor => ( expression )" << endl;
	}

}

// FunctionCall Function
void functionCall(ifstream& file, TokenType& token) {
	
	expect("(", token, file);

	// Check if there is an ArgList
	if ((!token.lexeme.compare(")")) == 0) {
		// Call ArgList function if exist
		ArgList(file, token);
	}

	expect(")", token, file);

	cout << "functionCall => Ident ( [ArgList] ) " << endl;
}

// Argument List Function
void ArgList(ifstream& file, TokenType& token) {

	// Expression Function is called
	expression(file, token);

	// Look for another expression
	while (token.lexeme.compare(",") == 0) {
		expect(",", token, file);

		// Expression Function is called
		expression(file, token);
	}

	cout << "ArgList => Expr { ,Expr} " << endl;
}

#endif
