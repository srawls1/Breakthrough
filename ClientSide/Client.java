import java.io.*;
import java.net.Socket;
import java.util.Scanner;
import java.util.*;

public class Client {

    private int portNo;
    private String hostAddress;
    private String pass;
    private Socket socket;
    private BufferedReader input;
    private PrintWriter output;
    private Queue<String> queue = new LinkedList<String>();
    private int row, col;
    private byte dir;

    private boolean connected = false;
    private boolean aiai = false;
    private boolean humanai = false;


    public void setHostAddress(String address){hostAddress=address;}

    public void setPortNo(int port){portNo=port;}

    public void setPass(String password){pass=password;}

	public Client()
	{
		hostAddress="localhost";
		portNo=0;
	}

	public Client(String host, int port, String password){
		hostAddress=host;
		portNo=port;
		pass=password;
	}

	public int open(){//opens connection to server
		try
		{
			socket = new Socket(hostAddress, portNo);
			input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		}
		catch (IOException e)
		{
			return 1;//bad address or port
		}

		try
		{
			if(!getMessage().equals("PASSWORD"))
				return 2; //server new session error

			sendMessage(pass);

			if(!getMessage().equals("WELCOME"))
			{
				sendMessage("EXIT");
				return 3;//server password error
			}

			sendMessage("DISPLAY");

			if(!getMessage().equals("OK"))
				return 4;//display not supported
		}
		catch (IOException e)
		{
			return 5;//ioerror
		}
		connected = true;
		return 0;//return good
	}

	//protected void finalize() throws Throwable
	//{
	//	try {
	//		close();
	//	} finally {
	//		super.finalize();
	//	}
	//}

	public int close()//closes connection to server
	{
		if(!connected)
			return 2;//if not connected
		try
		{
			sendMessage("EXIT");

			if(!getMessage().equals("OK"))
				return 1;//if did not confirm exit
		}
		catch (IOException e)
		{
			return 5;//exception caught
		}
			connected = false;
			return 0;//return good
	}

	public int startHumanAI(int diff){//start humanai game
		try
		{
			switch(diff)//send correct game cmd
			{
				case 0: sendMessage("HUMAN-AI EASY");
					break;
				case 1: sendMessage("HUMAN-AI MEDIUM");
					break;
				case 2: sendMessage("HUMAN-AI HARD");
					break;
			}

			if(!getMessage().equals("OK"))
				return 1;//if illegal
		}
		catch (IOException e)
		{
			return 5;//exception
		}
		humanai = true;
		aiai = false;
		return 0;//good
	}

        //play humanai round
	public int roundHumanAI(String move)//0 OK, 1 White, 2 Black, 3 ILLEGAL, 5 IOException
	{
		try
		{
			sendMessage(move);//send user move

			String response = getMessage();//get ai move

			if(response.startsWith("; GAME OVER WHITE"))//if white won
				return 1;
			if(response.startsWith("; GAME OVER BLACK"))//if black won
			 	return 2;

			if(response.equals("ILLEGAL"))
				return 3;//if bad move
			else if(response.equals("OK"))
			{
				return getMove();//get ai move
			}
		}
		catch (IOException e)
		{
			return 5;//caught exception
		}

		return 0;//never used
	}

	public int undo()//0 OK, 3 ILLEGAL, 5 IOException
	{
		if (isAIAI())
			return 3;//if not aiai
		try
		{
			sendMessage("UNDO");//send undo

			String response = getMessage();

			if(response.equals("ILLEGAL"))
				return 3;//if illegal
			else if(response.equals("OK"))
			{
				return 0;//if good
			}
		}
		catch (IOException e)
		{
			return 5;//caught exception
		}

		return 1;//never used
	}



    public boolean isAIAI(){return aiai;}

        //start aiai game
	public int startAIAI(String server, int port, String pass, int mydiff, int opdiff){
		try
		{
			String cmd = "AI-AI " + server + " " + port + " " + pass + " ";//create cmd
			switch(mydiff)//add local diff
			{
				case 0: cmd += "EASY";
					break;
				case 1: cmd += "MEDIUM";
					break;
				case 2: cmd += "HARD";
					break;
			}
			cmd += " ";
			switch(opdiff)//add other diff
			{
				case 0: cmd += "EASY";
					break;
				case 1: cmd += "MEDIUM";
					break;
				case 2: cmd += "HARD";
					break;
			}

			sendMessage(cmd);//send cmd

			if(!getMessage().equals("OK"))
				return 1;//if illegal
		}
		catch (IOException e)
		{
			return 5;//if caught exception
		}

		aiai=true;
		humanai=false;
		return 0;//good
	}

	public int getMove()
	{
		try
		{
		//	System.out.println("before get message");
			String s = getMessage();
		//	System.out.println("after get message");
			if(s.startsWith("; GAME OVER WHITE"))//if white won
				return 1;
			if(s.startsWith("; GAME OVER BLACK"))//if black won
			 	return 2;

			String[] strings = s.split(" ");//parse move
			String location = strings[0].toUpperCase().trim();
			String direction = strings[1].toUpperCase().trim();

			//System.out.println(spaces[0]+"|"+spaces[1]+"|"+strings[1]);
			col = (int)(location.charAt(0))-(int)'A';//convert to int
			row = (int)'8' - (int)(location.charAt(1));

			if(direction.equals("FWD"))//parse direction
				dir = Dir.FWD;
			else if(direction.equals("LEFT"))
				dir = Dir.LEFT;
			else if(direction.equals("RIGHT"))
				dir = Dir.RIGHT;

			//getXY();

		//	System.out.println(col + " " + row + " " + dir);
		//	System.out.println("out");
			return 0;//good
		}
		catch (IOException e)
		{
			return 5;//if caught exception
		}

		//return 0;
	}

	public int getRow(){return row;}

	public int getCol(){return col;}

	public byte getDir(){return dir;}


    public void sendMessage (String cmd)throws IOException {
      output = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
	//System.out.println("Sent:["+cmd+"]");
        output.println(cmd);
        output.flush();
    }

    public String getMessage ()throws IOException {
      String message;
      message = (input.readLine().toUpperCase()).trim();
     // System.out.println("Received:["+message+"]"); 
      return message;
    }

}
