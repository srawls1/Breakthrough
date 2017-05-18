#include "Evaluator.h"

vector<string> splitlines(string msg)
{
	vector<string> result = vector<string>();
	stringstream ss;
	ss << msg;
	string newmsg;
	while(getline(ss,newmsg))
	{
		//cerr << "Queue:[" << newmsg << "]" << endl;
		result.push_back(newmsg+"\n");
	}
	//cerr << "SplitDone\n";
	return result;
}

bool Evaluator::cread() {

  char buf[MAX_MESSAGE];//buffer
  int length = read(fd, buf, MAX_MESSAGE-1);//read into buffer

  if (length < 0) { //if read message from pipe error
    cerr << "Dataserver Request Channel: Error reading from pipe!";
    return false;
  }

  buf[length] = '\0';//add null terminated

 // cerr << "Server reads [" << buf << "]\n";

  if (in != nullptr)//delete previous stream
	 delete in;
  if (ts != nullptr)//delete previous tokenstream
	 delete ts;

  in = new stringstream();//update stream
  *in << " " << buf << " ";

  ts = new TokenStream(*in);//update token stream

  return true;
}

bool Evaluator::cwrite(string _msg) {
  if (_msg.length() >= MAX_MESSAGE) { //check if meesage is too long
    cerr << "Message too long for Channel!\n";
    return false;
  }else if(_msg.length() == 0)
	return true;

  vector<string> msgs = splitlines(_msg);

  for(string msg : msgs)
  {

   // cerr << "Request Channel writing [" << msg << "]";

    const char * s = msg.c_str();

    if (write(fd, s, strlen(s)+1) < 0) { //write message to pipe
     cerr << "Network Request Channel: Error writing to pipe!";
     return false;
    }
    usleep(10000);//give the other side a chance to read
  }
  //cerr << "SendDone\n";
  //  cout << "(" << my_name << ") done writing." << endl;
  return true;
}


//helper methods
string Evaluator::to_lower(string in)//converts a string to lower case
{
	string out = in;
	std::transform(out.begin(), out.end(), out.begin(), ::tolower);//does to conversion to lower case
	return out;//return lower case string
}

bool Evaluator::to_lower_equal(string s1, string s2)//conpares to strings in lower case
{
	return to_lower(s1) == to_lower(s2);
}

//constructor
Evaluator::Evaluator(int file_descrip, string pass):fd(file_descrip),auth(false),password(pass)//constructor
{
	in = new stringstream();
	ts = new TokenStream(*in);
	g = nullptr;
}

//constructor
Evaluator::Evaluator(int file_descrip):fd(file_descrip),auth(false),password("letmein")//constructor
{
	in = new stringstream();
	ts = new TokenStream(*in);
	g = nullptr;
}

Evaluator::~Evaluator()//destructor
{
  if (in != nullptr)
	 delete in;
  if (ts != nullptr)
	 delete ts;
  if (g != nullptr)
	 delete g;
}

//parses symbols
bool Evaluator::par_symbol(string s)
{
	Token t = ts->next();//get token
	if (t.type == SYMBOL && t.s == s){ //check if correct symbol in token
		//cerr << "trueSY |" << t.s << "|\n";
		return true;
	}else {
		ts->putBack(t);//putback since not a match
		//cerr << "falseSY |" << t.s << "| wanted |" <<s<<"|\n";
		return false;
	}
}

//parses keywords, returns true if succeeded
bool Evaluator::par_keyword(string s)
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

//parses identifier
bool Evaluator::par_identifier()
{
	Token t = ts->next();//get token
	if(t.type == IDENTIFIER)//verify type
	{
		id_stack.push(t.s);
		//cerr << "trueINT |" << t.i << "|\n";
		return true;
	}else
	{
		//cerr << "falseINT |" << t.i << "|\n";
		ts->putBack(t);//put back ununsed token
		return false;
	}
}

//parses integers
bool Evaluator::par_int()
{
	Token t = ts->next();//get token
	if(t.type == INT_LIT)//verify type
	{
		int_stack.push(t.i);
		//cerr << "trueINT |" << t.i << "|\n";
		return true;
	}else
	{
		//cerr << "falseINT |" << t.i << "|\n";
		ts->putBack(t);//put back ununsed token
		return false;
	}
}


//main parser call, breaks it down into password, comment, command, or move
bool Evaluator::eval()
{
	bool flag;//if correct parse
	if(!auth)
	{
		cwrite("PASSWORD\n");//prompt for password
		cread();
		//cerr << "CALL PAR PASSWORD\n";
		flag =(par_exit() || par_password());//call parsers for exit or password
	}else
	{
		cread();//otherwise if auth, then parse comment, command, or move
		flag =(par_comment() || par_command() || par_move());//parse query or command
	}

	if(!flag)//if a bad parse
		cwrite("ILLEGAL\n");
	return flag;//return parse status
}

//par password
bool Evaluator::par_password()
{
	Token t = ts->next();//get token
	if(t.type == IDENTIFIER)//check if it is an identifier
	{
		if(t.s == password)//check password
		{
			auth = true;//update status
			cwrite("WELCOME\n");
			return true;
		}else
			return false;//dont have to put back, b/c of above logic and only called if !auth
	}
		return false;
}

//parses a comment
bool Evaluator::par_comment()
{
	if(par_symbol(";"))//check for ;
	{

		Token t = ts->next();//get token
		if(t.type == COMMENT)//verify type
		{
			cwrite(";"+t.s+"\n");//echo back
			return true;
		}else
		{
			ts->putBack(t);//put back unused token
			return false;
		}
	}

	return false;
}

//parse a command
bool Evaluator::par_command()
{
	return par_exit() || par_display() || par_undo() || par_human_ai() || par_ai_ai();//call these parseers
}

//parse a exit
bool Evaluator::par_exit()
{
	if(par_keyword("EXIT"))
	{
		exit = true;//flag exit signal
		cwrite("OK\n");//echo ok
		return true;
	}

	return false;
}

//parse a display
bool Evaluator::par_display()
{
	if(par_keyword("DISPLAY"))
	{
		if(g != nullptr)
			g->display();//flip display on game

		if(show)
			show = false;
		else
			show = true;

		cwrite("OK\n");//echo ok
		return true;

	}

	return false;
}

//parse a undo
bool Evaluator::par_undo()
{
	if(par_keyword("UNDO"))//check for undo
	{
		if(g->undo())//run undo on game
		{
			g->setMessage();//if allowed print to screen new board state
			cwrite("OK\n");
			cwrite(g->getMessage());
			return true;
		}else
		{
			return false;
		}

	}

	return false;
}

//setup human ai game parser
bool Evaluator::par_human_ai()
{
	if(par_keyword("HUMAN-AI"))
	{
		if(par_keyword("EASY"))
		{
			  if (g != nullptr) {//remove previous game
				delete g;
			  }

			// setup ai and game
			//cerr << "EASY\n";
			cwrite("OK\n");
			g = new Game(1,show);

			g->setMessage();//echo game state to screen
			cwrite(g->getMessage());
			return true;
		} else if (par_keyword("MEDIUM")) {

			if (g != nullptr) {//remove previous game
			  delete g;
			}

			// setup ai and game
			cwrite("OK\n");
			g = new Game(2,show);

			g->setMessage();//echo game state to screen
			cwrite(g->getMessage());
			return true;
		} else if (par_keyword("HARD")) {

			if (g != nullptr) {//remove previous game
			  delete g;
			}

			// setup ai and game
			cwrite("OK\n");
			g = new Game(3,show);

			g->setMessage();//echo game state to screen
			cwrite(g->getMessage());
			return true;
		} else
		{
			return false;
		}

	}
	return false;
}

bool Evaluator::par_address()
{
	if(par_identifier())
		return true;
	else if(par_int() && par_symbol(".") &&  par_int() && par_symbol(".") &&  par_int() && par_symbol(".") &&  par_int())
	{
		int o4 = int_stack.top();
		int_stack.pop();
		int o3 = int_stack.top();
		int_stack.pop();
		int o2 = int_stack.top();
		int_stack.pop();
		int o1 = int_stack.top();
		int_stack.pop();

		stringstream ss;
		ss << o1 << "." << o2 << "." << o3 << "." << o4;
		id_stack.push(ss.str());
	}else
		return false;


}

//parse ai-ai, not implemented yet, will be in next part
bool Evaluator::par_ai_ai()
{
	//parse input
	if(par_keyword("AI-AI") && par_address() && par_int() && par_identifier() && par_identifier() && par_identifier())
	{
		//cerr << "POP\n";
		string oppdiff = id_stack.top();//retreive data from stack
		id_stack.pop();
		//cerr << "POP2\n";
		string mydiff = id_stack.top();
		id_stack.pop();
		//cerr << "POP3\n";
		string pass  = id_stack.top();
		id_stack.pop();
		//cerr << "POP4\n";
		int port = int_stack.top();
		int_stack.pop();
		//cerr << "POP5\n";
		string server  = id_stack.top();
		id_stack.pop();
		//cerr << "POP6\n";

		//check data validity
		if(pass != password && (to_lower_equal(oppdiff, "easy") || to_lower_equal(oppdiff, "medium") || to_lower_equal(oppdiff, "hard"))
		&& (to_lower_equal(mydiff, "easy") || to_lower_equal(mydiff, "medium") || to_lower_equal(mydiff, "hard")))
		{

			return false;
		}

		NetSocket np(server, port);//create socket to new server

		SMPI s = SMPI(np.getFD(),pass,mydiff,oppdiff,show);//create evaluator & server message passing interface to play ai-ai

		if(!s.init())//initial connection and auth
		{
			return false;
		}else
			cwrite("OK\n");

		usleep(4000000);

		//int i = 10;
		while(!s.over)//play game
		{
			s.round();
			cwrite(s.getMessage());//send output to client
			if(to_lower_equal(oppdiff,"hard") || to_lower_equal(mydiff,"hard"))
				continue;
			else //if (to_lower_equal(oppdiff,"medium") || to_lower_equal(mydiff,"medium"))
				usleep(1000000);
			//else
			//sleep(1000000);// give the other end a bit of time.

		}

		return true;
	}else
		return false;
}

//parse a move & basic game play
bool Evaluator::par_move()
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
					cwrite("OK\n");
					if(!(g->is_game_over()))//check if over, then no ai move
					{
						cwrite(g->getMessage());//respond to user
						g->aiMove();
					}
				}
				cwrite(g->getMessage());//respond to user
				return true;
			} else if(par_keyword("LEFT")) {
				//cerr << "LEFT\n";
				if(g->makeMove(x, y, LEFT))//make left move
				{
					cwrite("OK\n");
					if(!(g->is_game_over()))//check if over, then no ai move
					{
						cwrite(g->getMessage());//respond to user
						g->aiMove();
					}
				}
				cwrite(g->getMessage());//respond to user
				return true;

			} else if(par_keyword("RIGHT")) {
				//cerr << "RIGHT\n";
				if(g->makeMove(x, y, RIGHT))//make right move
				{
					cwrite("OK\n");
					if(!(g->is_game_over()))//check if over, then no ai move
					{
						cwrite(g->getMessage());//respond to user
						g->aiMove();
					}
				}
				cwrite(g->getMessage());//respond to user
				return true;
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
