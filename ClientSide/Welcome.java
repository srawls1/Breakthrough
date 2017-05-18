import com.sun.jndi.cosnaming.IiopUrl;

import java.lang.Thread;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;
//loads the welcome screen
public class Welcome extends JFrame implements ActionListener{

    JOptionPane alert = new JOptionPane();
    private JLabel connectionstatus = new JLabel();
    private int diffBlack = -1;
    private int diffWhite = -1;
    private int type = 0; //1 HUMANAI, 2 AIAI


    public Welcome() {
        //set the defaults for the frame
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(640, 480);
        setTitle("Welcome to Breakthrough");
        setLocationRelativeTo(null); //position to the center of screen
        getContentPane().setLayout(null); //override the Flowlayout

        //panel to store text fiedls for login information
        JPanel panel1 = new JPanel();
        panel1.setBounds(170, 70, 300, 100);
        panel1.setLayout(new GridLayout(0,2));
        panel1.setBorder(BorderFactory.createTitledBorder(BorderFactory.createBevelBorder(2),"Log-in Information",TitledBorder.CENTER,TitledBorder.CENTER));

        //text fields and their labels instantiation and adding it to panel 1
        JLabel HostAddress = new JLabel("Host Address");
        final JTextField Address = new JTextField(15);
        Address.addActionListener(this);
        HostAddress.setLabelFor(Address);

        JLabel portno = new JLabel("Port Number");
        final JTextField portNo = new JTextField(15);
        portno.setLabelFor(portNo);

        JLabel passwd = new JLabel("Password");
        final JTextField password = new JTextField(15);
        passwd.setLabelFor(password);

        panel1.add(HostAddress);
        panel1.add(Address);
        panel1.add(portno);
        panel1.add(portNo);
        panel1.add(passwd);
        panel1.add(password);
        add(panel1);
        panel1.setVisible(true);

        //text fields and their labels instantiation for AI-AI game and and adding it to panel5
        JPanel panel5 = new JPanel();
        panel5.setBounds(170, 170, 300, 100);
        panel5.setLayout(new GridLayout(0,2));
        panel5.setBorder(BorderFactory.createTitledBorder(BorderFactory.createBevelBorder(2),"AI-AI Second Server Information",TitledBorder.CENTER,TitledBorder.CENTER));

        JLabel HostAddress2 = new JLabel("Host Address");
        final JTextField Address2 = new JTextField(15);
        Address2.addActionListener(this);
        HostAddress2.setLabelFor(Address2);

        JLabel portno2 = new JLabel("Port Number");
        final JTextField portNo2 = new JTextField(15);
        portno2.setLabelFor(portNo2);

        JLabel passwd2 = new JLabel("Password");
        final JTextField password2 = new JTextField(15);
        passwd2.setLabelFor(password2);

        panel5.add(HostAddress2);
        panel5.add(Address2);
        panel5.add(portno2);
        panel5.add(portNo2);
        panel5.add(passwd2);
        panel5.add(password2);
        add(panel5);
        panel5.setVisible(true);

        //radio buttons to select game-type and adding it to panel2
        JPanel panel2 = new JPanel();
        panel2.setBounds(90, 280, 150, 100);
        panel2.setLayout(new GridLayout(0, 1));
        panel2.setBorder(BorderFactory.createTitledBorder(BorderFactory.createBevelBorder(2), "Select AI", TitledBorder.CENTER, TitledBorder.CENTER));

        final JRadioButton humanAI = new JRadioButton("HUMAN AI");
        final JRadioButton aiai = new JRadioButton("AI-AI");

        ButtonGroup aigroup = new ButtonGroup();
        aigroup.add(humanAI);
        aigroup.add(aiai);

        panel2.add(humanAI);
        panel2.add(aiai);
        add(panel2);

        panel2.setVisible(true);

        //radio buttons to select difficulty level and adding it to panel3
        JPanel panel3 = new JPanel();
        panel3.setBounds(260, 280, 150, 100);
        panel3.setLayout(new GridLayout(0, 1));
        panel3.setBorder(BorderFactory.createTitledBorder(BorderFactory.createBevelBorder(2), "BLACK Difficulty Level", TitledBorder.CENTER,TitledBorder.CENTER));

        final JRadioButton easy1 = new JRadioButton("Easy");
        easy1.addActionListener(this);
        final JRadioButton medium1 = new JRadioButton("Medium");
        medium1.addActionListener(this);
        final JRadioButton hard1 = new JRadioButton("Hard");
        hard1.addActionListener(this);

        ButtonGroup difficultygroup1 = new ButtonGroup();
        difficultygroup1.add(easy1);
        difficultygroup1.add(medium1);
        difficultygroup1.add(hard1);

        panel3.add(easy1);
        panel3.add(medium1);
        panel3.add(hard1);
        add(panel3);
        panel3.setVisible(true);

        //radio buttons to select difficulty level for AI-AI game and adding it to panel4
        JPanel panel4 = new JPanel();
        panel4.setBounds(425, 280, 170, 100);
        panel4.setLayout(new GridLayout(0, 1));
        panel4.setBorder(BorderFactory.createTitledBorder(BorderFactory.createBevelBorder(2), "WHITE Difficulty (AIAI Only)", TitledBorder.CENTER,TitledBorder.CENTER));

        final JRadioButton easy2 = new JRadioButton("Easy");
        easy2.addActionListener(this);
        final JRadioButton medium2 = new JRadioButton("Medium");
        medium2.addActionListener(this);
        final JRadioButton hard2 = new JRadioButton("Hard");
        hard2.addActionListener(this);

        ButtonGroup difficultygroup2 = new ButtonGroup();
        difficultygroup2.add(easy2);
        difficultygroup2.add(medium2);
        difficultygroup2.add(hard2);

        panel4.add(easy2);
        panel4.add(medium2);
        panel4.add(hard2);
        add(panel4);
        panel4.setVisible(true);

        connectionstatus.setBounds(290, 470, 80, 20);
        add(connectionstatus);

        //A button to start connecting to server and starting the game
        final JButton play = new JButton("Connect & Play");
        play.setBounds(280, 390, 170, 30);
        add(play);
        setVisible(true);

        //action listerner method for play button
        play.addActionListener(new ActionListener() {
                                   @Override
                                   public void actionPerformed(ActionEvent e) {

                                       if (e.getSource().equals(play)) { //if the button is pressed

                                           //chek the fields first if they are empty
                                           if ((Address.getText().equals("") || portNo.getText().equals("") || password.getText().isEmpty())) {
                                               alert.showMessageDialog(null, "Arguments missing: Server Settings");
                                               return;
                                           }

                                           //start a client and get all those parameters needed to start connectinf client to server
											Client client = new Client();

                                           client.setHostAddress(Address.getText());
                                           client.setPortNo(Integer.parseInt(portNo.getText()));
                                           client.setPass(password.getText());

                                            //check the gametype and set the property of client as requested
                                           if (easy1.isSelected()) {
                                               diffBlack = 0;
                                              // System.out.println("Easy");
                                           } else if (medium1.isSelected()){
                                               diffBlack = 1;
                                              // System.out.println("Medium");
                                           } else if (hard1.isSelected()){
                                               diffBlack = 2;
                                              // System.out.println("Hard");
                                           } else {
                                               diffBlack = -1;
						                    alert.showMessageDialog(null, "Arguments missing: BLACK Difficulty");
                                               return;
                                           }


                                           //check the AI type and set the property of client as requested
                                           if (humanAI.isSelected()){
                                              // System.out.println("AI: Human");
                                               type = 1;
                                           }
                                           //if AI-AI selected, start a different server and get fields from the next lof in panel
                                           else if (aiai.isSelected()) {
                                              // System.out.println("AI:AI");
                                               type = 2;

                                               if ((Address2.getText().equals("") || portNo2.getText().equals("") || password2.getText().isEmpty())) {
                                                   alert.showMessageDialog(null, "Arguments missing: Second Server Settings");
                                                   return;
                                               }
                                           
                                               if (easy1.isSelected()) {
                                                   diffWhite = 0;
                                                   //System.out.println("Easy");
                                               } else if (medium1.isSelected()){
                                                   diffWhite = 1;
                                                   //System.out.println("Medium");
                                               } else if (hard1.isSelected()){
                                                   diffWhite = 2;
                                                  // System.out.println("Hard");
                                               } else {
                                                   diffWhite = -1;
						                        alert.showMessageDialog(null, "Arguments missing: WHITE Difficulty");
                                                   return;
                                               }
                                           }else {
                                               type = 0;
								            alert.showMessageDialog(null, "Arguments missing: Game Type");
                                               return;
                                           }
						 
                                           //check the connection status
                                           int status = client.open();

                                           if(status == 0)
                                           {
                                               //connectionstatus.setText("Connected");
                                               if (type == 1)
                                                   status = client.startHumanAI(diffBlack);
                                               else if (type == 2)
                                                   status = client.startAIAI(Address2.getText(),Integer.parseInt(portNo2.getText()),password2.getText(),diffWhite,diffBlack);

                                               if (status == 0 && type == 1) {
                                                  MainGame mg = new MainGame(client);
                                               }else if(status == 0 && type == 2 ) {
                                                  Thread t = new Thread(new MainGame(client));
                                                  t.start();
                                               }else if (status == 1)
                                                  alert.showMessageDialog(null, "Failed to Start Game");
                                               else
                                                   alert.showMessageDialog(null, "Unknown Communication Error");

                                               
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


                                          // System.out.println(Address.getText());
                                          // System.out.println(portNo.getText());
                                          // System.out.println(password.getText());

                                       }
                                   }
                               }
                //play.setActionCommand("Play");
        );
    }

public void actionPerformed(ActionEvent e){


}
}
