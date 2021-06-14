import java.util.*;
import java.io.*;
import java.net.*;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextArea;
import javax.swing.text.Style;

import javax.swing.Action;
import java.awt.Color;
import java.awt.event.*;
import java.awt.Image;

class WaitingRoom extends JFrame implements Runnable {
    static JTextArea windowToShowWhoIsInWaitingRoom;

    WaitingRoom() {
        super("Waiting Room");
        windowToShowWhoIsInWaitingRoom = new JTextArea();
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        add(windowToShowWhoIsInWaitingRoom);
        setVisible(true);
        setSize(300, 300);
    }

    public void run() {
        windowToShowWhoIsInWaitingRoom.append("Server is open, waiting client to connect\n");
        windowToShowWhoIsInWaitingRoom.append("Here is Waiting Room to monitor who is here\n");
        int i = 0;
        while (true) {
            int next = server.mappingNameOfPlayers.size();
            // System.out.println(server.mappingNameOfPlayers.size());
            if (next > i) {
                {
                    windowToShowWhoIsInWaitingRoom.append(server.mappingNameOfPlayers.get(i)
                            + " is connected, waiting in the waiting hall, welcone!\n");
                    i++;
                }
            }
        }
    }
}

public class server {
    public static Vector<Socket> onLineSocket = new Vector<Socket>();
    public static HashMap<Integer, Integer> scoreList = new HashMap<Integer, Integer>(); // historicalScore
    public static HashMap<Integer, String> mappingNameOfPlayers = new HashMap<Integer, String>(); // map[number] = name
    public static HashMap<Socket, Integer> mappingSocketToNumber = new HashMap<Socket, Integer>(); // map[Socket] =
                                                                                                   // number
    public static Boolean aGameStarted = false;

    public static void main(String[] args) throws Exception {
        new Thread(new WaitingRoom()).start();
        ServerSocket welcomeSocket = new ServerSocket(2208);
        System.out.println("Server is ready for accepting client connections.");
        while (true) {
            Socket connectionSocket = welcomeSocket.accept();

            scoreList.put(onLineSocket.size(), 0);
            onLineSocket.add(connectionSocket);
            // Waiting Room

            System.out.println(connectionSocket.getRemoteSocketAddress() + " is connected");

            new Thread(new clientConnected(connectionSocket)).start();

        }
    }
}

class playGroundMonitor implements Runnable {
    Vector<Socket> listOfOnlinePlayers;
    Socket strike, defend;
    String patternOfAGame;

    playGroundMonitor(Socket player1, Socket player2) {
        strike = player1;
        defend = player2;
    }

    public void run() {
        try {
            // System.out.println(server.mappingSocketToNumber.get(strike));
            // System.out.println(server.mappingSocketToNumber.get(defend));
            DataOutputStream outToClient1 = new DataOutputStream(strike.getOutputStream());
            DataOutputStream outToClient2 = new DataOutputStream(defend.getOutputStream());
            outToClient1.writeUTF("first");
            outToClient1.flush();
            outToClient2.writeUTF("second");
            outToClient2.flush();
            // notify client to start game

            DataInputStream inFromClient1 = new DataInputStream(strike.getInputStream());
            DataInputStream inFromClient2 = new DataInputStream(defend.getInputStream());

            patternOfAGame = "000000000";
            int flagOfWinOrLose = 2; // 0 for strike is win, 1 for defend is win
                                     // default draw
            while (true) {
                patternOfAGame = inFromClient1.readUTF();

                System.out.println(patternOfAGame);
                outToClient2.writeUTF(patternOfAGame);
                outToClient2.flush();
                if (GameIsOver(patternOfAGame)) {
                    flagOfWinOrLose = 0;
                    // If strike win it
                    break;
                }
                if (GameIsDraw(patternOfAGame)) {
                    break;
                }
                // transmiss step if game is not over
                patternOfAGame = inFromClient2.readUTF();

                System.out.println(patternOfAGame);
                outToClient1.writeUTF(patternOfAGame);
                outToClient1.flush();
                if (GameIsOver(patternOfAGame)) {
                    flagOfWinOrLose = 1;
                    // If defend win it
                    break;
                }
            }
            // When game over, tell client who is win
            if (flagOfWinOrLose == 0) {
                outToClient1.writeUTF("333333333");
                outToClient1.flush();
                outToClient2.writeUTF("333333333");
                outToClient2.flush();
                System.out.println("Winner is player1, who is strik.");
            } else if (flagOfWinOrLose == 1) {
                outToClient1.writeUTF("444444444");
                outToClient1.flush();
                outToClient2.writeUTF("444444444");
                outToClient2.flush();
                System.out.println("Winner is player2, who is defender.");
            } else {
                outToClient1.writeUTF("555555555");
                outToClient1.flush();
                outToClient2.writeUTF("555555555");
                outToClient2.flush();
                System.out.println("This game is draw");
            }

            // "333333333" for strike is win
            // "444444444" for defend is win
            // System.out.println("Winner is " + flagOfWinOrLose);

        } catch (Exception e) {
        }
    }

    static Boolean GameIsDraw(String p) {
        if (p.contains("0"))
            return false;
        return true;
    }

    static Boolean GameIsOver(String p) {
        if (p.charAt(0) == p.charAt(1) && p.charAt(1) == p.charAt(2) && p.charAt(0) == p.charAt(2)
                && p.charAt(0) != '0')
            return true;
        if (p.charAt(0) == p.charAt(4) && p.charAt(4) == p.charAt(8) && p.charAt(0) == p.charAt(8)
                && p.charAt(0) != '0')
            return true;
        if (p.charAt(0) == p.charAt(3) && p.charAt(3) == p.charAt(6) && p.charAt(0) == p.charAt(6)
                && p.charAt(0) != '0')
            return true;
        if (p.charAt(1) == p.charAt(4) && p.charAt(4) == p.charAt(7) && p.charAt(1) == p.charAt(7)
                && p.charAt(1) != '0')
            return true;
        if (p.charAt(2) == p.charAt(5) && p.charAt(5) == p.charAt(8) && p.charAt(2) == p.charAt(8)
                && p.charAt(2) != '0')
            return true;
        if (p.charAt(3) == p.charAt(4) && p.charAt(4) == p.charAt(5) && p.charAt(3) == p.charAt(5)
                && p.charAt(3) != '0')
            return true;
        if (p.charAt(2) == p.charAt(4) && p.charAt(4) == p.charAt(6) && p.charAt(2) == p.charAt(6)
                && p.charAt(2) != '0')
            return true;
        if (p.charAt(6) == p.charAt(7) && p.charAt(7) == p.charAt(8) && p.charAt(6) == p.charAt(8)
                && p.charAt(6) != '0')
            return true;
        return false;
    }
}

class clientConnected implements Runnable {

    Socket connection;

    clientConnected(Socket client) {
        connection = client;
    }

    public void run() {
        // System.out.println(playGroundMonitor.patternOfAGame);
        // System.out.println("Hope it's work"); // It's work
        try {
            DataInputStream inFromClient = new DataInputStream(connection.getInputStream());
            DataOutputStream outToClient = new DataOutputStream(connection.getOutputStream());

            String name = inFromClient.readUTF();
            System.out.println(name + " is online and ready to play a game");

            int myNumberOrderOfAllplayers = server.onLineSocket.size();
            server.mappingNameOfPlayers.put(myNumberOrderOfAllplayers - 1, name);
            server.mappingSocketToNumber.put(connection, myNumberOrderOfAllplayers - 1);
            // System.out.println(server.mappingNameOfPlayers.size());

            // System.out.println("Now the number of online players is " +
            // server.mappingNameOfPlayers.size());
            /*
             * for (int i = 0; i < server.mappingNameOfPlayers.size(); i++) {
             * System.out.println(server.mappingNameOfPlayers.get(i)); }
             */
            WaitingRoom.windowToShowWhoIsInWaitingRoom.append(name + " is online and waiting in the waiting room\n");

            if (server.mappingNameOfPlayers.size() % 2 == 0) {
                System.out.println("Game is now starting");
                int number = server.mappingSocketToNumber.get(connection);
                WaitingRoom.windowToShowWhoIsInWaitingRoom.append(
                        "Game between " + server.mappingNameOfPlayers.get(number - 1) + " and " + name + " is start\n");

                new Thread(new playGroundMonitor(server.onLineSocket.get(myNumberOrderOfAllplayers - 2),
                        server.onLineSocket.get(myNumberOrderOfAllplayers - 1))).start();
            }
            while (true)
                ;
        } catch (IOException e) {
        }
    }

}