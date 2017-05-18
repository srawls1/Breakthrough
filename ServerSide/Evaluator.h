#ifndef EVAL_H
#define EVAL_H
#include "TokenStream.h"
#include "NetSocket.h"
#include "SMPI.h"
#include "Game.h"
#include <cassert>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <algorithm>
#include <pthread.h>


class Evaluator {

private:
	const unsigned int MAX_MESSAGE = 8192;//max string 

	stringstream* in;//stringstream of input for TokenStream to parse
	TokenStream* ts;//token stream object
	Game* g;//game object
	int fd;//file descriptor
	bool auth;//if authenticated
	bool show = true;
	string password;//password of session
	stack<int> int_stack;
	stack<string> id_stack;

	bool cread();//read from fd
	bool cwrite(string _msg);//write to fd
	string to_lower(string in);//convert to lower case
	bool to_lower_equal(string s1, string s2);//compares lowercase strings

	//basic parsers
	bool par_symbol(string s);//for symbols like ;
	bool par_keyword(string s);//for keywords like exit or fwd
	bool par_int();//parses numbers, ints
	bool par_comment();//comments in the form ;*
	bool par_identifier();//parses strings
	bool par_address();//parses hostname or ip address

	bool par_command();//parses a command
	bool par_password();//parses password
	bool par_exit();//exit
	bool par_display();//if to toggle display
	bool par_undo();//undo move
	bool par_human_ai();//human ai setup
	bool par_ai_ai();//ai-ai setup, not fully implmented
	bool par_move();//a move command


public:
	bool exit = false; //if flaged to exit
	Evaluator(int file_descrip, string password);//constructors
	Evaluator(int file_descrip);
	~Evaluator();//destructor
	bool eval();//main evaluator/parser

};

typedef Evaluator Parser;

#endif
