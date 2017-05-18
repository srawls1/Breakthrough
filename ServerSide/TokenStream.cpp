/*
    File: TokenStream.cpp
*/


#include "TokenStream.h"
#include <cctype>

//helper method to find index of punctuation in vector above
template <class T>
int index_of(vector<T> v, T target) {
  for (int i=0; i<v.size(); ++i) {
    if (v[i] == target)//return found location
      return i;
  }
  return -1;//if punctuation is not found
}

//Token constructor
Token::Token(TokenType ty, string str, int v)
: type(ty), s(str), i(v)
{}

//TokenStream constructor
TokenStream::TokenStream(istream& in)
: stream(in), charBuffer(), tokenBuffer()
{}

//gets next char from stream or buffer
char TokenStream::nextChar()
{
  char c;

  //removes char from buffer if not empty
  if (!charBuffer.empty()) {
    c = charBuffer.front();
    charBuffer.pop();
    return c;
  }

  //if no more chaers in the istream return ' '
  if (stream.eof()) {
    return ' ';
  }

  //get next char in istream
  c = stream.get();
  return c;
}

//returns whether there is more input to parse
bool TokenStream::hasNext()
{
  return !stream.eof();
}

//gets next token
Token TokenStream::next()
{
  //returns next token on buffer if not empty
  if (!tokenBuffer.empty()) {
    Token t = tokenBuffer.top();
    tokenBuffer.pop();
    return t;
  }
  
  if(stream.eof())//if no more input return NIL Token
  {
	//cerr << "TOKENSTREAM NIL 1\n";
    return Token(NILL, "", 0);//returns null token
  }
	  
	
  char c;

  //removes whitespace arround tokens
  do {
    c = nextChar();
  } while (iswspace(c));

  if (c == ';') { // eat comment
    string s;
    getline(stream, s);
    tokenBuffer.push(Token(COMMENT, s, 0));
    return Token(SYMBOL, ";", 0);
  }
  else if (c == '.')
	return Token(SYMBOL, ".", 0);
  else if (isalpha(c)) { // parse as identifier
    string s;

	//gets entire name of identifier
    do {
      s += c;
      c = nextChar();
    } while (isalnum(c) || c == '-' || c == '.');
    if (!iswspace(c)) {
      putBack(c);
    }
    return Token(IDENTIFIER, s, 0);//returns token with string value
  }
  else if (isdigit(c)) { // parse as int_lit
    string s;

	//gets all the digits
    do {
      s += c;
      c = nextChar();
    } while (isdigit(c));
    if (!iswspace(c)) {
      putBack(c);
    }
    return Token(INT_LIT, "", stoi(s));//returns token with int value
  }
  else {
	//cerr << "TOKENSTREAM NIL 2\n";
    return Token(NILL, "", 0);//returns null token

  }
}

//adds token to token buffer
void TokenStream::putBack(char c)
{
  charBuffer.push(c);
}

//adds char to char buffer
void TokenStream::putBack(Token t)
{
  if(t.type != NILL)//dont allow NIL to be put on stack
    tokenBuffer.push(t);
  //else
	  //cerr << "TOKENSTREAM NIL TRIED TO PUT BACK\n";
}
