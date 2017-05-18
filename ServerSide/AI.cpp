#include "AI.h"
#include <stdlib.h>
#include <climits>
//will be used to call other ai based on difficulty
void AI::ai_move(Board& b){
	switch (difficulty) {
		case 1: // easy difficulty
			simple_ai_move(b);
			break;
		case 2: // medium difficulty
			medium_ai_move(b);
			break;
		case 3:
			hard_ai_move(b);
			break;
	}
}

//random ai move algorithm
void AI::simple_ai_move(Board& b){
	vector<Loc> locs = b.listMoves(side);//get possible moves

	if(locs.size() < 1)//never should be true
	{
		moveMade = "; AI Passed on Turn";
		return;
	}

	int ran_loc =rand()% (locs.size());//select move

	b.movePiece(locs[ran_loc].x,locs[ran_loc].y, locs[ran_loc].d, side);//make the move

	stringstream ss;
	ss << "" << (char)('A'+locs[ran_loc].x) << (char)('8'-locs[ran_loc].y);//place corridinates of move in moveMade
	moveMade = ss.str();

	//add direction to move made
	switch(locs[ran_loc].d)
	{
		case(0): moveMade += " FWD\n"; break;
		case(1): moveMade += " RIGHT\n"; break;
		case(2): moveMade += " LEFT\n"; break;
		default: moveMade += " NONE\n"; break;//should never be used
	}
}

//medium difficulty ai algorithm
void AI::medium_ai_move(Board& b) {
    /*vector <Loc> locs = b.listLocs(side);
		vector <Loc> tempLocs;

    if (locs.size()<1) {
        return;
        moveMade = "; AI Passed on Turn";
    }

    for (Loc possible : locs) {
        MinMaxTree trees = MinMaxTree(b,side,3,false);
        Loc temploc = trees.get_optimal_move();
        tempLocs.push_back(temploc);
    }

    int ran_loc = rand()%(tempLocs.size());

	  b.movePiece(tempLocs[ran_loc].x, tempLocs[ran_loc].y, tempLocs[ran_loc].d,side);*/
		Loc optimalmove = get_optimal_move(b, side, 3, false);
		b.movePiece(optimalmove.x, optimalmove.y, optimalmove.d, side);

    stringstream ss;
    ss << "" << (char)('A'+optimalmove.x) << (char)('8'-optimalmove.y);//place corridinates of move in moveMade
    moveMade = ss.str();
    switch(optimalmove.d)
    {
        case(0): moveMade += " FWD\n"; break;
        case(1): moveMade += " RIGHT\n"; break;
        case(2): moveMade += " LEFT\n"; break;
        default: moveMade += " NONE\n"; break;//should never be used
    }
}

//hard difficulty ai algorithm
void AI::hard_ai_move(Board& b) {
		/*vector <Loc> locs = b.listLocs(side);

    if (locs.size()<1) {
      return;
    	moveMade = "; AI Passed on Turn";
    }

    MinMaxTree* tree = new MinMaxTree(b,side,6,true);
    Loc tempLoc = tree->get_optimal_move();


    b.movePiece(tempLoc.x, tempLoc.y, tempLoc.d,side);*/
		Loc optimalmove = get_optimal_move(b, side, 6, true);
		b.movePiece(optimalmove.x, optimalmove.y, optimalmove.d, side);

    stringstream ss;
    ss << "" << (char)('A'+optimalmove.x) << (char)('8'-optimalmove.y);//place corridinates of move in moveMade
    moveMade = ss.str();

    switch(optimalmove.d)
    {
        case(0): moveMade += " FWD\n"; break;
        case(1): moveMade += " RIGHT\n"; break;
        case(2): moveMade += " LEFT\n"; break;
        default: moveMade += " NONE\n"; break;//should never be used
    }
}

//returns moveMade to allow for printing over session
string AI::getMove()
{
	return moveMade;
}
