import java.io.IOException;
import java.util.Stack;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
//draws and updates the actual board to gameplay
public class Board extends JPanel implements MouseListener, Runnable {

  //private byte[][] spaces;
  private Stack<byte[][]> states;
  private int winner;
  private Client c;
  private int xSelected;
  private int ySelected;
  private boolean over;
  //constructor
  public Board(Client c) {
    winner = Space.EMPTY;
    this.c = c;
    xSelected = -1; // value of -1 indicates that no tile is selected
    ySelected = -1;
    byte[][] spaces = new byte[8][8];//set up new spaces array
    for (int x=0; x<8; ++x) {
      int y=0;
      for (; y<2; ++y) {
        spaces[y][x] = Space.BLACK;
      }
      for (; y<6; ++y) {
        spaces[y][x] = Space.EMPTY;
      }
      for (; y<8; ++y) {
        spaces[y][x] = Space.WHITE;
      }
    }
    states = new Stack<byte[][]>();
    states.push(spaces);
  }
  //main function
  public void run()
  {
	byte turn = Space.BLACK;
    while(true)
	{
	    //rotate turn
		if(turn == Space.WHITE)
		  turn = Space.BLACK;
	    else
		  turn = Space.WHITE;

	    int m = c.getMove();//get move
		if(m == 0)//if no winner yet
		{
		  makeMove(c.getCol(),c.getRow(),c.getDir(),turn);//make move
		  repaint();
	    }

		if(m == 1)//white won
	    {
	      //System.out.println("WHITE WON");
	      winner = Space.WHITE;
              over = true;
              repaint();
	      break;
	    }

	    if(m == 2)//black won
	    {
	      //System.out.println("BLACK WON");
	      winner = Space.BLACK;
              over = true;
		  repaint();
	      break;
		}
	}
    close();
  }

  public boolean isOver(){return over;}

  public void close()//close client on game winner
  {
    if(over)
      c.close();
  }

  //check if black won, because of java io blocking
  public boolean checkForWinner()
  {
     int bcount = 0, wcount = 0;
     for (int yy=0; yy<8; ++yy) { // count current state pices
      for (int xx=0; xx<8; ++xx) {
        if((states.peek())[yy][xx] == Space.WHITE)
	  bcount++;
	else if((states.peek())[yy][xx] == Space.BLACK)
          wcount++;
      }
    }

   if(bcount == 0 || wcount == 0)
	return true;//if somebody ran out of pices

    for (int xx=0; xx<8; ++xx) {
        if((states.peek())[7][xx] == Space.BLACK)
	  return true;//if black probably won
    }
   return false;//if black did not win
   }
 

  //makes a move on the board, changes the matricies
  public void makeMove(int x, int y, byte d, byte side) {
    byte[][] newBoard = new byte[8][8];
    byte[][] spaces = states.peek();
    for (int yy=0; yy<8; ++yy) { // copy current state into new state
      for (int xx=0; xx<8; ++xx) {
        newBoard[yy][xx] = spaces[yy][xx];
      }
    }
    //System.out.println("Created newBoard:");
    //print();
    //byte[][] newBoard = spaces;

    //System.out.println("|("+x+","+y+")|"+side);
    if (side == Space.WHITE) {//if white move
      //System.out.println("White move");
      switch (d) {
        case Dir.FWD:
          newBoard[y-1][x] = newBoard[y][x];
          newBoard[y][x] = Space.EMPTY;
          break;
        case Dir.LEFT:
          newBoard[y-1][x-1] = newBoard[y][x];
          newBoard[y][x] = Space.EMPTY;
          break;
        case Dir.RIGHT:
          newBoard[y-1][x+1] = newBoard[y][x];
          newBoard[y][x] = Space.EMPTY;
          break;
        default:
          System.out.println("You dun goofed.(1)"); // should never happen
      }
    }
    else if (side == Space.BLACK) {//if black move
      //System.out.println("Black move");
      switch (d) {
        case Dir.FWD:
          newBoard[y+1][x] = newBoard[y][x];
          newBoard[y][x] = Space.EMPTY;
          break;
        case Dir.LEFT:
          newBoard[y+1][x+1] = newBoard[y][x];
          newBoard[y][x] = Space.EMPTY;
          break;
        case Dir.RIGHT:
          newBoard[y+1][x-1] = newBoard[y][x];
          newBoard[y][x] = Space.EMPTY;
          break;
        default:
          System.out.println("You dun goofed.(2)"); // should never happen
      }
    }
    else {
      System.out.println("You dun goofed.(3)"); // Trying to move a piece that isn't there
    }
    //System.out.println("Updated newBoard:");
    states.push(newBoard);//add new board state
    //spaces = newBoard;
    //print();
  }
  //redraw function, called everytime something is changed
  public void paintComponent(Graphics g) {
	super.paintComponent(g);
  byte[][] spaces = states.peek();
	//System.out.println("updating board");
    // draw board
    for (int x=0; x<8; ++x) {
      for (int y=0; y<8; ++y) {

        if ((x+y)%2==0) {
          if (x == xSelected && y == ySelected)
            g.setColor(Pallette.DARK_SELECTED);
          else
            g.setColor(Pallette.DARK_TILE);
        } else {
          if (x == xSelected && y == ySelected)
            g.setColor(Pallette.LIGHT_SELECTED);
          else
            g.setColor(Pallette.LIGHT_TILE);
        }
        g.fillRect(x*50, y*50, 50, 50);
        if (spaces[y][x] == Space.WHITE) {
          g.setColor(Pallette.LIGHT_PIECE);
          g.fillOval(x*50+5, y*50+5, 40, 40);
        } else if (spaces[y][x] == Space.BLACK) {
          g.setColor(Pallette.DARK_PIECE);
          g.fillOval(x*50+5, y*50+5, 40, 40);
        }
      }
    }

    //display winner
    if (winner == Space.WHITE) {
      g.setColor(Color.RED);
      g.setFont(new Font("Arial", Font.BOLD, 32));
      g.drawString("White wins!", 150, 200);
    } else if (winner == Space.BLACK) {
      g.setColor(Color.RED);
      g.setFont(new Font("Arial", Font.BOLD, 32));
      g.drawString("Black wins!", 150, 200);
    }
  }

  public void undo() {
    //System.out.println("First pop");
    states.pop();
    //print();
    //System.out.println("Second pop");
    states.pop();
    //print();
    //System.out.println("Result");
    //spaces = states.peek();
    //print();
    repaint();
  }

  //for debug
  public void print() {
    for (byte[][] spaces : states) {
      for (byte[] row : spaces) {
        for (byte space : row) {
          if (space == Space.EMPTY)
            System.out.print('_');
          else if (space == Space.BLACK)
            System.out.print('X');
          else if (space == Space.WHITE)
            System.out.print('O');
        }
        System.out.println();
      }
      System.out.println();
    }
  }

  public void mouseClicked(MouseEvent e) {}
  public void mouseEntered(MouseEvent e) {}
  public void mouseExited(MouseEvent e) {}
  //handles clickes
  public void mousePressed(MouseEvent e) {
    if(c.isAIAI() == true)//disable if aiai
       return;
    if(over)//disable if over
      return;

	if(e.getX() > 400 || e.getY() > 400) { // if you click out of bounds,reset
		xSelected = -1;
		ySelected = -1;
		repaint();
		return;
	}
    //System.out.println(e.getX()+" " +xSelected+ " " +e.getY() + " " + ySelected);

    if (xSelected == -1) { // ySelected is also -1
      //select tile
      xSelected = e.getX()/50;
      ySelected = e.getY()/50;
    } else {
      //make move from selected tile to newly clicked tile
      int x = e.getX()/50;
      int y = e.getY()/50; // subtract 25 from y to compensate for title bar(removed not needed)
      String message = "" + (char)((int)'A'+xSelected) + (char)((int)'8'-ySelected);
      byte d;
      if (x == xSelected && y == ySelected-1) { //forward
        d = Dir.FWD;
        message += " FWD";
      } else if (x == xSelected-1 && y == ySelected-1) { //left
        d = Dir.LEFT;
        message += " LEFT";
      } else if (x == xSelected+1 && y == ySelected-1) { //right
        d = Dir.RIGHT;
        message += " RIGHT";
      } else {
        message = "";
	xSelected = -1;
	ySelected = -1;
	repaint();
        return;
      }

      // send message to server
      /*try {
        String response = c.sendClientMessage(message);
        System.out.println(message);
        System.out.println(response);
      } catch (IOException ex) {
        System.out.println("Lost connection to server");
      }*/
      int response = c.roundHumanAI(message);
      /*String s;
      try {
        c.sendMessage(message);
        s = c.getMessage();
      } catch (IOException ex) {
        System.out.println("Server connection interrupted");
        return;
      }
      if (s.equals("ILLEGAL")) {
        xSelected = x;
        ySelected = y;
        repaint();
        return;
      }

      makeMove(x, y, d, Space.WHITE);
      repaint();

      int oppMove = c.getMove();
      if (oppMove == 1) {
        winner = Space.WHITE;
      } else if (oppMove == 2) {
        winner = Space.BLACK;
      } else if (oppMove == 0) {
        int row = c.getRow();
        int column = c.getCol();
        Dir dir = c.getDir();
        makeMove(column, row, dir, Space.BLACK);
      }*/

      if(response == 0)//get ai move
      {
        makeMove(xSelected,ySelected,d,Space.WHITE);
        makeMove(c.getCol(),c.getRow(),c.getDir(),Space.BLACK);
        //		System.out.println(c.getRow() + " " + c.getCol());
      }
      else if(response == 1)//white wins
      {
        makeMove(xSelected,ySelected,d,Space.WHITE);
        winner = Space.WHITE;
        over = true;
        close();
      }
      else if(response == 2)//black wins
      {
        //makeMove(xSelected,ySelected,d,Space.WHITE);
        winner = Space.BLACK;
        over = true;
        close();
      }
      else if(response == 3)//illegal
      {
		  System.out.println("illegal move");
      }
      // unselect previously selected tile
      xSelected = -1;
      ySelected = -1;
    }
	   // System.out.println("Trying to repaint");

     if(checkForWinner())
     {
       int who = c.getMove();//get winner

     if(who == 1)//white wins
      {
        //makeMove(xSelected,ySelected,d,Space.WHITE);
        winner = Space.WHITE;
        over = true;
        close();
      }
      else if(who == 2)//black wins
      {
        //makeMove(xSelected,ySelected,d,Space.WHITE);
        winner = Space.BLACK;
        over = true;
        close();
      }
     }
      repaint();
  }

  public void mouseReleased(MouseEvent e) {}
}
