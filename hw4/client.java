import java.net.*;
import java.io.*;
import java.util.*;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import java.awt.Color;
import java.awt.event.*;

class MonitorTheGameForBoardcast extends JFrame implements Runnable {
    // Monitor the client game to show to clinet
    // use buttom represent the squre to mimic the game sheet
    static JButton button0, button1, button2, button3, button4, button5, button6, button7, button8;
    static JButton winB, loseB;
    static ImageIcon circle = new ImageIcon("images/circle.png");
    static ImageIcon fork = new ImageIcon("images/fork.png");
    static ImageIcon win = new ImageIcon("images/win.jpg");
    static ImageIcon lose = new ImageIcon("images/lose.jpeg");
    static ImageIcon bg = new ImageIcon("images/bg.png");

    void initButton() {
        // arrange the button with different argument, position, and visability
        button0 = new JButton();
        button0.setBounds(0, 0, 200, 200);
        button0.setBackground(Color.lightGray);
        button1 = new JButton();
        button1.setBounds(200, 0, 200, 200);
        button1.setBackground(Color.lightGray);
        button2 = new JButton();
        button2.setBounds(400, 0, 200, 200);
        button2.setBackground(Color.lightGray);
        button3 = new JButton();
        button3.setBounds(0, 200, 200, 200);
        button3.setBackground(Color.lightGray);
        button4 = new JButton();
        button4.setBounds(200, 200, 200, 200);
        button4.setBackground(Color.lightGray);
        button5 = new JButton();
        button5.setBounds(400, 200, 200, 200);
        button5.setBackground(Color.lightGray);
        button6 = new JButton();
        button6.setBounds(0, 400, 200, 200);
        button6.setBackground(Color.lightGray);
        button7 = new JButton();
        button7.setBounds(200, 400, 200, 200);
        button7.setBackground(Color.lightGray);
        button8 = new JButton();
        button8.setBounds(400, 400, 200, 200);
        button8.setBackground(Color.lightGray);
        // if result genetate, setVisible to true
        winB = new JButton();
        winB.setBounds(200, 600, 200, 200);
        winB.setIcon(win);
        winB.setBackground(Color.yellow);
        winB.setVisible(false);

        loseB = new JButton();
        loseB.setIcon(lose);
        loseB.setBounds(200, 600, 200, 200);
        loseB.setBackground(Color.yellow);
        loseB.setVisible(false);
    }

    MonitorTheGameForBoardcast(String m) {
        super(m);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // exit the process
        setResizable(false);
        setSize(600, 800);
        setLayout(null);
        setVisible(true); // debug
        ImageIcon image = new ImageIcon("images/icon.png");
        setIconImage(image.getImage());
        getContentPane().setBackground(Color.pink);
        initButton();
        add(button0);
        add(button1);
        add(button2);
        add(button3);
        add(button4);
        add(button5);
        add(button6);
        add(button7);
        add(button8);
        add(winB);
        add(loseB);

    }

    public void run() {
        // Monitor the game
        while (!client.gameIsReallyOver) {
            StringBuilder outputForGraphicUserInterfaceShow;
            synchronized (client.patternOfTheGame) {
                outputForGraphicUserInterfaceShow = new StringBuilder(client.patternOfTheGame);
            }
            // update the button pattern circle(O) or fork(X)

            if (outputForGraphicUserInterfaceShow.charAt(0) == '1') {
                button0.setIcon(circle);
            } else if (outputForGraphicUserInterfaceShow.charAt(0) == '2') {
                button0.setIcon(fork);
            } else {
            }
            if (outputForGraphicUserInterfaceShow.charAt(1) == '1') {
                button1.setIcon(circle);
            } else if (outputForGraphicUserInterfaceShow.charAt(1) == '2') {
                button1.setIcon(fork);
            } else {
            }
            if (outputForGraphicUserInterfaceShow.charAt(2) == '1') {
                button2.setIcon(circle);
            } else if (outputForGraphicUserInterfaceShow.charAt(2) == '2') {
                button2.setIcon(fork);
            } else {
            }
            if (outputForGraphicUserInterfaceShow.charAt(3) == '1') {
                button3.setIcon(circle);
            } else if (outputForGraphicUserInterfaceShow.charAt(3) == '2') {
                button3.setIcon(fork);
            } else {
            }
            if (outputForGraphicUserInterfaceShow.charAt(4) == '1') {
                button4.setIcon(circle);
            } else if (outputForGraphicUserInterfaceShow.charAt(4) == '2') {
                button4.setIcon(fork);
            } else {
            }
            if (outputForGraphicUserInterfaceShow.charAt(5) == '1') {
                button5.setIcon(circle);
            } else if (outputForGraphicUserInterfaceShow.charAt(5) == '2') {
                button5.setIcon(fork);
            } else {
            }
            if (outputForGraphicUserInterfaceShow.charAt(6) == '1') {
                button6.setIcon(circle);
            } else if (outputForGraphicUserInterfaceShow.charAt(6) == '2') {
                button6.setIcon(fork);
            } else {
            }
            if (outputForGraphicUserInterfaceShow.charAt(7) == '1') {
                button7.setIcon(circle);
            } else if (outputForGraphicUserInterfaceShow.charAt(7) == '2') {
                button7.setIcon(fork);
            } else {
            }
            if (outputForGraphicUserInterfaceShow.charAt(8) == '1') {
                button8.setIcon(circle);
            } else if (outputForGraphicUserInterfaceShow.charAt(8) == '2') {
                button8.setIcon(fork);
            } else {
            }
        }
        // redo the update again, avoid the last step didn't show on the monitor
        StringBuilder outputForGraphicUserInterfaceShow;
        outputForGraphicUserInterfaceShow = new StringBuilder(client.patternOfTheGame);
        // update the button pattern circle(O) or fork(X)
        if (outputForGraphicUserInterfaceShow.charAt(0) == '1') {
            button0.setIcon(circle);
        } else if (outputForGraphicUserInterfaceShow.charAt(0) == '2') {
            button0.setIcon(fork);
        } else {
        }
        if (outputForGraphicUserInterfaceShow.charAt(1) == '1') {
            button1.setIcon(circle);
        } else if (outputForGraphicUserInterfaceShow.charAt(1) == '2') {
            button1.setIcon(fork);
        } else {
        }
        if (outputForGraphicUserInterfaceShow.charAt(2) == '1') {
            button2.setIcon(circle);
        } else if (outputForGraphicUserInterfaceShow.charAt(2) == '2') {
            button2.setIcon(fork);
        } else {
        }
        if (outputForGraphicUserInterfaceShow.charAt(3) == '1') {
            button3.setIcon(circle);
        } else if (outputForGraphicUserInterfaceShow.charAt(3) == '2') {
            button3.setIcon(fork);
        } else {
        }
        if (outputForGraphicUserInterfaceShow.charAt(4) == '1') {
            button4.setIcon(circle);
        } else if (outputForGraphicUserInterfaceShow.charAt(4) == '2') {
            button4.setIcon(fork);
        } else {
        }
        if (outputForGraphicUserInterfaceShow.charAt(5) == '1') {
            button5.setIcon(circle);
        } else if (outputForGraphicUserInterfaceShow.charAt(5) == '2') {
            button5.setIcon(fork);
        } else {
        }
        if (outputForGraphicUserInterfaceShow.charAt(6) == '1') {
            button6.setIcon(circle);
        } else if (outputForGraphicUserInterfaceShow.charAt(6) == '2') {
            button6.setIcon(fork);
        } else {
        }
        if (outputForGraphicUserInterfaceShow.charAt(7) == '1') {
            button7.setIcon(circle);
        } else if (outputForGraphicUserInterfaceShow.charAt(7) == '2') {
            button7.setIcon(fork);
        } else {
        }
        if (outputForGraphicUserInterfaceShow.charAt(8) == '1') {
            button8.setIcon(circle);
        } else if (outputForGraphicUserInterfaceShow.charAt(8) == '2') {
            button8.setIcon(fork);
        } else {
        }
        // show the result of game on monitor JFrame
        if (client.winOrLose)
            winB.setVisible(true);
        else if (!client.winOrLose)
            loseB.setVisible(true);
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
        }
        this.dispose();
    }
}

public class client {
    static DataOutputStream outToServer;
    static DataInputStream inFromServer;

    static String patternOfTheGame = "000000000"; // game sheet
    static String strikOrDefend; // gaming order, first or second
    static Boolean gameIsReallyOver;
    static Boolean winOrLose; // true for win, false for lose

    static Boolean checkMoveIsIllegal(String patternOfTheGame, int index) {
        // simple check your behavoir is legal or not
        // System.out.println(patternOfTheGame + " " + index);
        if (index < 0 || index > 9)
            return true;
        if (patternOfTheGame.charAt(index) == '0')
            return false;
        return true;
    }

    public static void main(String[] args) throws Exception {
        gameIsReallyOver = false;

        try {
            Socket clientSocket = new Socket("localhost", 2208);

            outToServer = new DataOutputStream(clientSocket.getOutputStream());
            inFromServer = new DataInputStream(clientSocket.getInputStream());

            Scanner inFromUser = new Scanner(System.in);
            // Tell server who am I
            System.out.println("Please tell who you are");
            String name = inFromUser.nextLine().toString();
            outToServer.writeUTF(name);
            outToServer.flush(); // Bug
            System.out.println("Hello, " + name + ". Welcome the tic-tac-toe game, Waitng for your opponent...");
            new Thread(new MonitorTheGameForBoardcast(name)).start();

            // receive start signal
            strikOrDefend = inFromServer.readUTF();
            Boolean tie = false;
            int whichOneYouChoose;
            StringBuilder temporStoreModifiedString;
            if (strikOrDefend.equals("first")) {
                System.out.println("Game is now start, please choose your move, typing a int accroding the position.");
                System.out.println("-----------------");
                System.out.println("----0---1---2----");
                System.out.println("-----------------");
                System.out.println("----3---4---5----");
                System.out.println("-----------------");
                System.out.println("----6---7---8----");
                System.out.println("-----------------");
                int i = 0;
                while (true) {
                    // read step, (my turn)
                    i++;
                    System.out.println("Now is your turn: ");
                    whichOneYouChoose = inFromUser.nextInt();

                    while (checkMoveIsIllegal(patternOfTheGame, whichOneYouChoose)) {
                        System.out.println(
                                "Your move is illegal, there is occupied or out of range, please rechoose the location.");
                        whichOneYouChoose = inFromUser.nextInt();
                    }
                    temporStoreModifiedString = new StringBuilder(patternOfTheGame);
                    temporStoreModifiedString.setCharAt(whichOneYouChoose, '1');
                    patternOfTheGame = temporStoreModifiedString.toString();

                    System.out.println("Your move opration " + patternOfTheGame);

                    outToServer.writeUTF(patternOfTheGame);
                    outToServer.flush();
                    if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
                        System.out.println("You win");
                        patternOfTheGame = inFromServer.readUTF();
                        break;
                    }
                    if (playGroundMonitor.GameIsDraw(patternOfTheGame)) {
                        patternOfTheGame = inFromServer.readUTF();
                    }
                    if (i == 9) {
                        System.out.println("THe Game is Draw");
                        break;
                    }
                    // recieve step (opponent turn)
                    i++;
                    patternOfTheGame = inFromServer.readUTF();
                    System.out.println("Opponent's operation " + patternOfTheGame);
                    if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
                        System.out.println("Opponent win");
                        patternOfTheGame = inFromServer.readUTF();
                        System.out.println(patternOfTheGame);
                        break;
                    }
                }
            } else if (strikOrDefend.equals("second")) {
                System.out.println("Game is start, you are the defender, waiting your opponent to move first.");
                System.out.println("-----------------");
                System.out.println("----0---1---2----");
                System.out.println("-----------------");
                System.out.println("----3---4---5----");
                System.out.println("-----------------");
                System.out.println("----6---7---8----");
                System.out.println("-----------------");
                int i = 0;
                while (true) {
                    // recieve step (opponent turn)
                    i++;
                    patternOfTheGame = inFromServer.readUTF();
                    System.out.println("Opponent's operation " + patternOfTheGame);
                    if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
                        System.out.println("Opponent win");
                        patternOfTheGame = inFromServer.readUTF();
                        System.out.println(patternOfTheGame);
                        break;
                    }
                    if (i == 9) {
                        System.out.println("The game is draw");
                        break;
                    }
                    // read step, (my turn)
                    i++;
                    System.out.println("Now is your turn: ");
                    whichOneYouChoose = inFromUser.nextInt();
                    while (checkMoveIsIllegal(patternOfTheGame, whichOneYouChoose)) {
                        System.out.println(
                                "Your move is illegal, there is occupied or out of range, please rechoose the location.");
                        whichOneYouChoose = inFromUser.nextInt();
                    }
                    temporStoreModifiedString = new StringBuilder(patternOfTheGame);
                    temporStoreModifiedString.setCharAt(whichOneYouChoose, '2');
                    patternOfTheGame = temporStoreModifiedString.toString();

                    System.out.println("Your move opration " + patternOfTheGame);

                    outToServer.writeUTF(patternOfTheGame);
                    outToServer.flush();
                    if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
                        System.out.println("You win");
                        patternOfTheGame = inFromServer.readUTF();
                        break;
                    }
                    if (playGroundMonitor.GameIsDraw(patternOfTheGame)) {
                        patternOfTheGame = inFromServer.readUTF();
                    }
                }
            }
            System.out.println("Game is over");
            gameIsReallyOver = true;
            if (strikOrDefend.equals("first") && patternOfTheGame.equals("333333333"))
                winOrLose = true;
            else if (strikOrDefend.equals("second") && patternOfTheGame.equals("444444444"))
                winOrLose = true;
            else
                winOrLose = false;
            // System.out.println(patternOfTheGame + " is result");

            // tie

        } catch (Exception e) {

        }

    }
}

/*
 * @Override public void actionPerformed(ActionEvent e) {
 * 
 * if (e.getSource() == button0) { if (patternOfTheGame.charAt(0) == '0' &&
 * canMove) { button0.setIcon(icon); canMove = false; click = 0; } } if
 * (e.getSource() == button1) { if (patternOfTheGame.charAt(1) == '0' &&
 * canMove) { button1.setIcon(icon); canMove = false; click = 1; } } if
 * (e.getSource() == button2) { if (patternOfTheGame.charAt(2) == '0' &&
 * canMove) { button2.setIcon(icon); canMove = false; click = 2; } } if
 * (e.getSource() == button3) { if (patternOfTheGame.charAt(3) == '0' &&
 * canMove) { button3.setIcon(icon); canMove = false; click = 3; } } if
 * (e.getSource() == button4) { if (patternOfTheGame.charAt(4) == '0' &&
 * canMove) { button4.setIcon(icon); canMove = false; click = 4; } } if
 * (e.getSource() == button5) { if (patternOfTheGame.charAt(5) == '0' &&
 * canMove) { button5.setIcon(icon); canMove = false; click = 5; } } if
 * (e.getSource() == button6) { if (patternOfTheGame.charAt(6) == '0' &&
 * canMove) { button6.setIcon(icon); canMove = false; click = 6; } } if
 * (e.getSource() == button7) { if (patternOfTheGame.charAt(7) == '0' &&
 * canMove) { button7.setIcon(icon); canMove = false; click = 7; } } if
 * (e.getSource() == button8) { if (patternOfTheGame.charAt(8) == '0' &&
 * canMove) { button8.setIcon(icon); canMove = false; click = 8; } }
 *
 * }
 */
/*-----------------------------------------------------------------------------            
      if (strikOrDefend.equals("first")) {
          icon = circle;
      
          // step 1 (1 for myself)
      
          synchronized (canMove) {
              canMove = true;
          }
          while (canMove == true)
              ;
          System.out.println("step 1");
          StringBuilder tmp = new StringBuilder(patternOfTheGame);
          tmp.setCharAt(click, '1');
          patternOfTheGame = tmp.toString();
          outToServer.writeUTF(patternOfTheGame);
          outToServer.flush();
          System.out.println("step 1 done");
      
          // step 2
          System.out.println("step 2");
          patternOfTheGame = inFromServer.readUTF();
          update();
          System.out.println("step 2 done");
      
          // step 3 (2 for myself)
          System.out.println("step 3");
          synchronized (canMove) {
              canMove = true;
          }
          while (canMove == true)
              ;
          tmp = new StringBuilder(patternOfTheGame);
          tmp.setCharAt(click, '1');
          patternOfTheGame = tmp.toString();
          outToServer.writeUTF(patternOfTheGame);
          outToServer.flush();
      
          System.out.println("step 3 done");
      
          // step 4
          patternOfTheGame = inFromServer.readUTF();
          update();
      
          // step 5 (3 for myself)
          synchronized (canMove) {
              canMove = true;
          }
          while (canMove == true)
              ;
          tmp = new StringBuilder(patternOfTheGame);
          tmp.setCharAt(click, '1');
          patternOfTheGame = tmp.toString();
          outToServer.writeUTF(patternOfTheGame);
          outToServer.flush();
      
          // check if win every next step
          if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
              patternOfTheGame = inFromServer.readUTF();
              gameOver();
              return;
          }
      
          // step 6
          patternOfTheGame = inFromServer.readUTF();
          update();
          // check if win every next step
          if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
              patternOfTheGame = inFromServer.readUTF();
              gameOver();
              return;
          }
      
          // step 7 (4 for myself)
          synchronized (canMove) {
              canMove = true;
          }
          while (canMove == true)
              ;
          tmp = new StringBuilder(patternOfTheGame);
          tmp.setCharAt(click, '1');
          patternOfTheGame = tmp.toString();
          outToServer.writeUTF(patternOfTheGame);
          outToServer.flush();
      
          // check if win every next step
          if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
              patternOfTheGame = inFromServer.readUTF();
              gameOver();
              return;
          }
      
          // step 8
          patternOfTheGame = inFromServer.readUTF();
          update();
          // check if win every next step
          if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
              patternOfTheGame = inFromServer.readUTF();
              gameOver();
              return;
          }
      
          // step 9 (5 for myself)
          synchronized (canMove) {
              canMove = true;
          }
          while (canMove == true)
              ;
          tmp = new StringBuilder(patternOfTheGame);
          tmp.setCharAt(click, '1');
          patternOfTheGame = tmp.toString();
          outToServer.writeUTF(patternOfTheGame);
          outToServer.flush();
      
          // check if win every next step
          if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
              patternOfTheGame = inFromServer.readUTF();
              gameOver();
              return;
          }
      
          tie = true;
      
      } else if (strikOrDefend.equals("second")) {
          icon = fork;
          // step 1
          System.out.println("step 1");
          patternOfTheGame = inFromServer.readUTF();
          update();
          System.out.println("step 1 done");
      
          // step 2 (1 for myself)
          System.out.println("step 2");
      
          synchronized (canMove) {
              canMove = true;
          }
          while (canMove == true)
              ;
          StringBuilder tmp = new StringBuilder(patternOfTheGame);
          tmp.setCharAt(click, '2');
          patternOfTheGame = tmp.toString();
          outToServer.writeUTF(patternOfTheGame);
          outToServer.flush();
          System.out.println("step 2 done");
      
          // step 3
          System.out.println("step 3");
          patternOfTheGame = inFromServer.readUTF();
          update();
          System.out.println("step 3 done");
      
          // step 4 (2 for myself)
          synchronized (canMove) {
              canMove = true;
          }
          while (canMove == true)
              ;
          tmp = new StringBuilder(patternOfTheGame);
          tmp.setCharAt(click, '2');
          patternOfTheGame = tmp.toString();
          outToServer.writeUTF(patternOfTheGame);
          outToServer.flush();
      
          // step 5
          patternOfTheGame = inFromServer.readUTF();
          update();
          // check if win every next step
          if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
              patternOfTheGame = inFromServer.readUTF();
              gameOver();
              return;
          }
      
          // step 6 (3 for myself)
          synchronized (canMove) {
              canMove = true;
          }
          while (canMove == true)
              ;
          tmp = new StringBuilder(patternOfTheGame);
          tmp.setCharAt(click, '2');
          patternOfTheGame = tmp.toString();
          outToServer.writeUTF(patternOfTheGame);
          outToServer.flush();
      
          // check if win every next step
          if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
              patternOfTheGame = inFromServer.readUTF();
              gameOver();
              return;
          }
      
          // step 7
          patternOfTheGame = inFromServer.readUTF();
          update();
          // check if win every next step
          if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
              patternOfTheGame = inFromServer.readUTF();
              gameOver();
              return;
          }
          // step 8 (4 for myself)
          synchronized (canMove) {
              canMove = true;
          }
          while (canMove == true)
              ;
          tmp = new StringBuilder(patternOfTheGame);
          tmp.setCharAt(click, '2');
          patternOfTheGame = tmp.toString();
          outToServer.writeUTF(patternOfTheGame);
          outToServer.flush();
      
          // check if win every next step
          if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
              patternOfTheGame = inFromServer.readUTF();
              gameOver();
              return;
          }
          // step 9
          patternOfTheGame = inFromServer.readUTF();
          update();
          // check if win every next step
          if (playGroundMonitor.GameIsOver(patternOfTheGame)) {
              patternOfTheGame = inFromServer.readUTF();
              gameOver();
              return;
          }
          tie = true;
      }
      //-------------------------------------------------------------------------------------*/
/*
 * static void gameOver() { if (patternOfTheGame == "333333333" && strikOrDefend
 * == "strike") { System.out.println("You win!"); // win in strike } else if
 * (patternOfTheGame == "444444444" && strikOrDefend == "defend") {
 * System.out.println("You win"); // win in defend } else {
 * System.out.println("You lose"); // lose } }
 */
/*
 * 
 */
/*
 * static void update() { if (patternOfTheGame.charAt(0) != '1')
 * button0.setIcon(circle); else if (patternOfTheGame.charAt(0) != '2')
 * button0.setIcon(fork); if (patternOfTheGame.charAt(1) != '1')
 * button0.setIcon(circle); else if (patternOfTheGame.charAt(1) != '2')
 * button0.setIcon(fork); if (patternOfTheGame.charAt(2) != '1')
 * button0.setIcon(circle); else if (patternOfTheGame.charAt(2) != '2')
 * button0.setIcon(fork); if (patternOfTheGame.charAt(3) != '1')
 * button0.setIcon(circle); else if (patternOfTheGame.charAt(3) != '2')
 * button0.setIcon(fork); if (patternOfTheGame.charAt(4) != '1')
 * button0.setIcon(circle); else if (patternOfTheGame.charAt(4) != '2')
 * button0.setIcon(fork); if (patternOfTheGame.charAt(5) != '1')
 * button0.setIcon(circle); else if (patternOfTheGame.charAt(5) != '2')
 * button0.setIcon(fork); if (patternOfTheGame.charAt(6) != '1')
 * button0.setIcon(circle); else if (patternOfTheGame.charAt(6) != '2')
 * button0.setIcon(fork); if (patternOfTheGame.charAt(7) != '1')
 * button0.setIcon(circle); else if (patternOfTheGame.charAt(7) != '2')
 * button0.setIcon(fork); if (patternOfTheGame.charAt(8) != '1')
 * button0.setIcon(circle); else if (patternOfTheGame.charAt(8) != '2')
 * button0.setIcon(fork);
 * 
 * }
 */

/*
 * client() {
 * 
 * 
 * setTitle("Tic Tac Toe"); setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); //
 * exit out of application setResizable(false); setSize(600, 800);
 * setLayout(null); setVisible(false); ImageIcon image = new
 * ImageIcon("images/icon.png"); setIconImage(image.getImage());
 * getContentPane().setBackground(Color.pink);
 */

// initButton();
/*
 * button0.addActionListener(this); button1.addActionListener(this);
 * button2.addActionListener(this); button3.addActionListener(this);
 * button4.addActionListener(this); button5.addActionListener(this);
 * button6.addActionListener(this); button7.addActionListener(this);
 * button8.addActionListener(this);
 */
/*
 * 
 * 
 * }
 */