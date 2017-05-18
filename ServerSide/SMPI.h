#ifndef SMPI_H
#define SMPI_H
#include "TokenStream.h"
#include "NetSocket.h"
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


class SMPI {

private:
	const unsigned int MAX_MESSAGE = 8192;//max string 

	stringstream* in;//stringstream of input for TokenStream to parse
	TokenStream* ts;//token stream object
	Game* g;//game object
	int fd;//file descriptor
	//NetSocket* ns;
	string password;//password of session
	string diff;
	string otherAIMove;
	string message;

	bool cread();//read from fd
	bool cwrite(string _msg);//write to fd
	string to_lower(string in);//convert to lower case
	string to_upper(string in);//convert to upper case
	bool to_lower_equal(string s1, string s2);//compares lowercase strings
	bool par_keyword(string key);
	bool par_move();
	
	
public:
	SMPI(int file, string pass, string mydiff, string oppdiff, bool show);//constructors
	~SMPI();//destructor
	bool init();
	bool round();
	string getMessage();	

	bool over = false;

};
#endif