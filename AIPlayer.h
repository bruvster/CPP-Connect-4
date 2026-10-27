/**
 * @file AIPlayer.h
 * @author Mia Murray, 655671493, VIU
 * @version 1.0.0
 * @date October 26th, 2023
 *
 * Defines the functions required for an AI search agent and creates its Player class
 * Made specifically for the connect 4 game
 */

#ifndef __PLAYER_HEADER__
#define __PLAYER_HEADER__

#include <iostream>
#include <string>
using namespace std;


class Player {
    private:
    string name = "";
    int Rows = 0;
    int Columns = 0;
    int Goal = 0;
    char **board;
    char **tempboard; //used for simulating moves
    int *top;
    char myMarble;
    char otherMarble;

    public:
        // get information of the board and the goal number
        Player( int r = 6, int c = 7, int g = 4, string n = "player")
        {
            Rows = r;
            Columns = c;
            Goal = g;
            name = n;
            top = new int[Columns];
            board = new char *[Rows];
            tempboard = new char *[Rows];
            myMarble = 'O';
            otherMarble = 'X';
            for(int i = 0; i < r; i++)
                board[i] = new char[c];
            initBoard(board, top);
            for (int j = 0; j < r; j++)
               tempboard[j] = new char[c];
            initBoard(tempboard, top);
        }

        ~Player()
        {
            delete[] top;
            for (int i = 0; i < Rows; i++)
                delete[] board[i];
            delete[] board;
            for (int i = 0; i < Rows; i++)
                delete[] tempboard[i];
            delete[] tempboard;
        }

        void initBoard(char **board, int top[]);
        void setBoard(char **board, char **copyboard);

        int move(int otherPlayersLastMove);
        int evaluation(char **state);
        int minimax(char **state, int depth, int alpha, int beta, bool ismax);
       
        bool checkLine(char **board, int row, int col, int rdelta, int cdelta, char piece, int goalnum);
        bool isWon(char **board, char piece);
        bool checkDraw(int top[]);

        string getName();
        void setName(string n);
};

#endif