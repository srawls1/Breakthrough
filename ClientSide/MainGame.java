import java.util.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
//this is the main game class, runs the game
public class MainGame extends JFrame implements Runnable, ActionListener {

  private Board b;
  private Client c;
  //constructor given client
  public MainGame(Client c) {
    this.c = c;
    b = new Board(c);

    //set the frame size and position to the center of the screen
    setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
    setSize(400, 450);
    setTitle("Welcome to Breakthrough!!!");
	setLocationRelativeTo(null);

    //enable undo button if the game is not AI-AI
    if(!c.isAIAI())
    {
      JButton undoButton = new JButton("Undo");
      undoButton.addActionListener(this);
      add(undoButton, BorderLayout.PAGE_END);
    }

    addMouseListener(b);
    add(b);

    //add newly created board and set the frame visible
    setVisible(true);
  }
  //runs the board if aiai
  public void run()
  {
    b.run();
  }

  //undo button process
  public void actionPerformed(ActionEvent e) {
    if (b.isOver())
      return;
    if (c.undo() == 0)
      b.undo();
  }

  //main used for testing
 // public static void main(String[] args) {
   // Client c = new Client("linux2",10050,"p");
   // c.open();
   // c.startHumanAI(3);
   // MainGame mg = new MainGame(c);
 // }
}
