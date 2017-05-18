//code moved to Welcome, not used anymore

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.net.InetAddress;
import javax.swing.*;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;
//handles welcome screen
public class WelcomeScreen extends JFrame {

    private JLabel connectionstatus = new JLabel();
    private Client client = new Client();
    //makes a class for panel
    public class CheckBoxPanel extends JPanel {

        public void build() {
            setSize(100, 100);
            setBorder(new BevelBorder(2, Color.RED, Color.CYAN));
            setLayout(new GridLayout(0, 1));
            setBackground(Color.RED);
            setAlignmentX(JComponent.LEFT_ALIGNMENT);

            JCheckBox EASY = new JCheckBox("EASY");
            JCheckBox MEDIUM = new JCheckBox("MEDIUM");
            JCheckBox HARD = new JCheckBox("HARD");

            add(EASY);
            add(MEDIUM);
            add(HARD);
/*
			EASY.addActionListener(this);
			MEDIUM.addActionListener(this);
			HARD.addActionListener(this);
	*/

            //setVisible(true);
        }

    }  
    //extends Button for our GUI

    public class ButtonPanel extends JPanel {
        public void build() {

            setSize(100, 100);
            setLayout(new GridLayout(0, 1));
            setBackground(Color.BLUE);

            JButton HUMAN_AI = new JButton("HUMAN-AI");
            JButton AI_AI = new JButton("AI-AI");

            add(HUMAN_AI);
            add(AI_AI);

            //setVisible(true);
        }
    }
    //text fields, extended for our GUI
    public class TextFieldPanel extends JPanel implements ActionListener {
        JButton connect;
        JOptionPane alert = new JOptionPane();
        JTextField hostAddress;
        JLabel address;
        JLabel portno;
        JTextField portNumber;
        JLabel passwd;
        JTextField password;

        public void build() {
            setSize(250, 150);
            setBackground(Color.orange);

            address = new JLabel("Host Address");
            hostAddress = new JTextField(10);
            address.setLabelFor(hostAddress);

            portno = new JLabel("Port Number");
            portNumber = new JTextField(10);
            portno.setLabelFor(portNumber);

            passwd = new JLabel("Password");
            password = new JTextField(10);
            passwd.setLabelFor(password);

            connect = new JButton("Connect");

            connect.addActionListener(this);

            //add to the Panel
            add(address);
            add(hostAddress);
            add(portno);
            add(portNumber);
            add(passwd);
            add(password);
            add(connect);

            setVisible(true);
        }

        @Override
        public void actionPerformed(ActionEvent e) {

            if ((hostAddress.getText().equals("") || portNumber.getText().equals("") || password.getText().isEmpty())) {
                alert.showMessageDialog(null, "Arguments missing");
            }

            //if (!password.getText().equalsIgnoreCase("P")) {
            //    alert.showMessageDialog(null, "Incorrect Password");
            //}

            client.setHostAddress(hostAddress.getText());
            client.setPortNo(Integer.parseInt(portNumber.getText()));
	    client.setPass(password.getText());

	    int status = client.open();

	    if(status == 0)
	    {
		connectionstatus.setText("Connected");
		MainGame mg = new MainGame(client);
	    }
	    else if(status == 1)
		alert.showMessageDialog(null, "Bad Address or Port");
	    else if (status == 2)
		alert.showMessageDialog(null, "Server Protocol Error");
	    else if (status == 3)
		alert.showMessageDialog(null, "Bad Password");
	    else if (status == 4)
		alert.showMessageDialog(null, "DISPLAY Not Supported");
	    else
		alert.showMessageDialog(null, "Unknown Communication Error");

            //after connecting to the server through Client
            //connectionstatus.setText("Connected");
            //close();
            //MainGame mg = new MainGame(client);

        }
    }

    //implements the above to a welcome screen
    //default constructor
    public WelcomeScreen() {

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(640, 500);
        setTitle("Welcome to Breakthrough!!!");

        JPanel panel = new JPanel();
        getContentPane().add(panel);

        CheckBoxPanel checkBoxPanel = new CheckBoxPanel();
        checkBoxPanel.build();
        checkBoxPanel.setLocation(400, 300);
        add(checkBoxPanel);

        ButtonPanel buttonPanel = new ButtonPanel();
        buttonPanel.build();
        buttonPanel.setBounds(10, 300, 100, 50);
        add(buttonPanel);

        TextFieldPanel textFieldPanel = new TextFieldPanel();
        textFieldPanel.build();
        textFieldPanel.setLocation(200, 60);
        add(textFieldPanel);

        add(connectionstatus);
        connectionstatus.setBounds(220, 260,80,40);

        JLabel title = new JLabel();
        add(title);
        title.setLocation(100, 10);

        setVisible(true);



    }

}
