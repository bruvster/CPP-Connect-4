#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <cassert>
using namespace std;

const char Space = ' ';

class RANPlayer {
    static char num;
    int Rows;
    int Columns;
    int Goal;
    string name;
    char **board;
    int *top;
    char myMarble;
    char otherMarble;

  public:
    RANPlayer (int r = 6, int c = 7, int g = 4)
    {
       Rows = r;
       Columns = c;
       Goal = g;
       name = "Random_Player_";
       name = name + num;
       num ++;
       top = new int[Columns];
       board = new char *[Rows];
       myMarble = 'X';
       otherMarble = 'O';
       for(int i = 0; i < r; i++)
          board[i] = new char[c];
       initBoard(board, top);
       time_t seconds;
       time(&seconds);
       srand((unsigned int)seconds);
    }

    ~RANPlayer()
    {
        delete [] top;
        for(int i = 0; i < Rows; i++)
            delete [] board[i];
        delete [] board;
        
    }

    void initBoard(char **board, int top[])
    {
       for(int i = 0; i < Rows; i++)
          for(int j = 0; j < Columns; j++)
             board[i][j] = Space;
       for(int i = 0; i < Columns; i++)
          top[i] = 0;
    }

    int move(int otherPlayersLastMove)
    {
       if (otherPlayersLastMove != -1) {
          board[top[otherPlayersLastMove]][otherPlayersLastMove]
                        = otherMarble;
          top[otherPlayersLastMove]++;
       }
       int count = 0;
       for (int i = 0; i < Columns; i++)
         if (top[i] < Rows) count ++;

       // count should be guaranteed a positive number
       assert(count > 0); //if not, program will terminate
       int index = rand() % count;
       for (int col = 0; col < Columns; col ++) {
          if (top[col] < Rows) index--;
          if (index < 0) {
             board[top[col]][col] = myMarble;
             top[col]++;
             return col;
          }
       }
       return -1; // impossible to reach here if main function
                  // manages things properly
    }

    string getName()
    {
        return name;
    }
};

char RANPlayer::num = 'A';