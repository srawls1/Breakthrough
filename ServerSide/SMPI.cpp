#include "SMPI.h"

bool SMPI::cread() {

  char buf[MAX_MESSAGE];//buffer
  int length = read(fd, buf, MAX_MESSAGE-1);//read into buffer

  if (length < 0) { //if read message from pipe error
    cerr << "Dataserver Request Channel: Error reading from pipe!";
    return false;
  }

  buf[length] = '\0';//add null terminated

  //cerr << "SMPI Server reads [" << buf << "]\n";

  if (in != nullptr)//delete previous stream
	 delete in;
  if (ts != nullptr)//delete previous tokenstream
	 delete ts;

  in = new stringstream();//update stream
  *in << " " << buf << " ";

  ts = new TokenStream(*in);//update token stream

  return true;
}

bool SMPI::cwrite(string _msg) {

  if (_msg.length() >= MAX_MESSAGE) { //check if meesage is too long
    cerr << "Message too long for Channel!\n";
    return false;
  }else if(_msg.length() == 0)
	return true;

  //  cout << "Request Channel writing [" << _msg << "]";

  const char * s = _msg.c_str();

  if (write(fd, s, strlen(s)+1) < 0) { //write message to pipe
   cerr << "Network Request Channel: Error writing to pipe!";
   return false;
  }

  //  cout << "(" << my_name << ") done writing." << endl;
  return true;
}


//helper methods
string SMPI::to_lower(string in)//converts a string to lower case
{
	string out = in;
	std::transform(out.begin(), out.end(), out.begin(), ::tolower);//does to conversion to lower case
	return out;//return lower case string
}

string SMPI::to_upper(string in)//converts a string to lower case
{
	string out = in;
	std::transform(out.begin(), out.end(), out.begin(), ::toupper);//does to conversion to lower case
	return out;//return lower case string
}

bool SMPI::to_lower_equal(string s1, string s2)//conpares to strings in lower case
{
	return to_lower(s1) == to_lower(s2);
}

//constructor
SMPI::SMPI(int file, string pass, string mydiff, string oppdiff, bool show):fd(file),password(pass),diff(oppdiff)//constructor
{
	//cerr << "CREATE SMPI\n";
	//cerr << "CREATE Socket\n";
	//ns = new NetSocket(serv, port);
	//cerr << "get FS\n";
	//fd = ns->getFD();
	//cerr << "RESET STREAM\n";
	in = new stringstream();//create stream & tokenstream to read in input
	ts = new TokenStream(*in);
	//cerr << "CREATE GAME\n";
	if(to_lower_equal(mydiff,"easy"))//create new game
		g = new Game(1,WHITE,show);
	else if(to_lower_equal(mydiff,"medium"))
		g = new Game(2,WHITE,show);
	else if(to_lower_equal(mydiff,"hard"))
		g = new Game(3,WHITE,show);
}

SMPI::~SMPI()
{
	cwrite("EXIT");//send exit command
	
	usleep(10000);

	cread();
	if(!par_keyword("OK"))
		cerr << "Other Side Server failed to confirm exit\n";

	if(in != nullptr)//destroy pointer objects
		delete in;

	if(ts != nullptr)
		delete ts;

	if(g != nullptr)
		delete g;

	//if(ns != nullptr)
	//	delete ns;
}

//starts connection
bool SMPI::init()
{
	//cerr << "SMPI INIT\n";
	if(g == nullptr)//clear game
		return false;

	if(fd < 0)//check if fd is valid
		return false;

	//cerr << "SMPI Pass\n";
	cread();
	if(!par_keyword("PASSWORD"))
		return false;

	cwrite(password);//auth to secondary server

	//cerr << "SMPI Welcome\n";
	cread();
	if(!par_keyword("WELCOME"))
	{
		cwrite("EXIT");
		return false;
	}

	//cerr << "SMPI DISPLAY\n";
	cwrite("DISPLAY");//turn off board output

	cread();
	if(!par_keyword("OK"))
		return false;

	//cerr << "SMPI HUMAN-AI " << diff << "\n";
	cwrite("HUMAN-AI "+ diff);//send game info

	cread();
	if(!par_keyword("OK"))
		return false;

	return true;
}

//round of game play
bool SMPI::round()
{
	message = "";
	bool flag = true;//bad other ai move
	bool white_won = false;
	if(!(g->is_game_over()))//make left move
	{
		g->aiMove();//whie
		if(!(g->is_game_over()))//check if over, then no ai move
		{
			message += g->getMessage();//get first ai board
			//cerr << g->getAIMove() << endl;
			cwrite(g->getAIMove());//send white move to other server

			cread();
			if(!par_keyword("OK"))//check for ok
				return false;

			cread();
			flag = par_move();//parse its move
			message += otherAIMove + "\n";
		}else
		{
			flag = true;
			white_won=true;
		}
	}
	if(!white_won)//special case modification if white wins
		g->setMessage();//respond to user
	message += g->getMessage();//respond to user
	over = g->is_game_over();
	return flag;
}


//return output
string SMPI::getMessage()
{
	return message;
}

//parses keywords, returns true if succeeded
bool SMPI::par_keyword(string s)
{
	Token t = ts->next();//get token
	if (t.type == IDENTIFIER && to_lower_equal(t.s,s)){//check if identifier matches lowercase
		//cerr << "trueK |" << t.s << "|\n";
		return true;
	}else {
		//cerr << "falseK |" << t.s << "|wanted |" <<s<<"|\n";
		ts->putBack(t);//putback since failure
		return false;
	}
}


//parse a move & basic game play
bool SMPI::par_move()
{

	Token t = ts->next();//get token
	if(t.type == IDENTIFIER)//verify type
	{
		//cerr << "move |" << t.s << "|\n";
		if((t.s).length() == 2)//check length
		{
			char c = char((to_lower(t.s))[0]);
			char i = char((to_lower(t.s))[1]);
			//cerr << "move [" << c << "][" << i << "]\n";

			//check for valid options
			if(!((c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g' || c == 'h') &&
			(i == '1' || i == '2' || i == '3' || i == '4' || i == '5' || i == '6' || i == '7' || i == '8')))
			{
				ts->putBack(t);
				return false;
			}

			int x = c - 'a';
			int y = '8' - i;
			//cerr << "move [" << x << "][" << y << "]\n";

			if(par_keyword("FWD")) {
				//cerr << "FWD\n";
				if(g->makeMove(x, y, FWD))//make fwd move
				{
					otherAIMove = to_upper(t.s) + " FWD";
					return true;
				}else
				{
					otherAIMove = "; Secondary AI made a Invalid move";
					return false;
				}
			} else if(par_keyword("LEFT")) {
				//cerr << "LEFT\n";
				if(g->makeMove(x, y, LEFT))//make left move
				{
					otherAIMove = to_upper(t.s) + " LEFT";
					return true;
				}else
				{
					otherAIMove = "; Secondary AI made a Invalid move";
					return false;
				}

			} else if(par_keyword("RIGHT")) {
				//cerr << "RIGHT\n";
				if(g->makeMove(x, y, RIGHT))//make right move
				{
					otherAIMove = to_upper(t.s) + " RIGHT";
					return true;
				}else
				{
					otherAIMove = "; Secondary AI made a Invalid move";
					return false;
				}
			}else
			{
				ts->putBack(t);
				return false;
			}
		}
		else
		{
			ts->putBack(t);
			return false;
		}
	}

	ts->putBack(t);//put back unused token
	return false;
}
