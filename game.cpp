#include <iomanip>
#include <unistd.h>
#include "RandomPlayer.h"
#include "AIPlayer.h"

const char Filling = ' ';
const int Columns = 7;
const int Rows = 6;
const int Goal = 4;
const int Players = 2;

enum Status {Win, Draw, Continue, Cheating};

const char playerMarble[Players] = {'X', 'O'};

void initBoard(char board[Rows][Columns], int top[])
{
    for(int i = 0; i < Rows; i++)
        for(int j = 0; j < Columns; j++)
            board[i][j] = Filling;

    for(int i = 0; i < Columns; i++)
        top[i] = 0;

}

void displayBoard(char board[Rows][Columns])
{
    cout << endl;
    for(int i = Rows-1; i >= 0; i--) {
        cout << "-";
        for(int j = 0; j < Columns; j++)
            cout << "----";
        cout << endl;
        cout << "|";
        for(int j = 0; j < Columns; j++)
            cout << " " << board[i][j] << " |";
        cout << endl;
    }
    cout << "-";
    for(int j = 0; j < Columns; j++)
        cout << "----" ;
    cout << endl;
    cout << " ";
    for(int j = 0; j < Columns; j++)
        cout << setw(2) << j+1 << "  ";
    cout << endl;
}

// used by checkWinner
Status checkLine(char board[Rows][Columns], int row, int col, 
                 int rdelta, int cdelta, int player)
{
    int count = 0;

    while (row < Rows && row >= 0 && col < Columns && col >= 0) {
        if (board[row][col] == playerMarble[player]) {
            count++;
            if (count == Goal) return Win;
        } else {
            count = 0;
        }
        row = row + rdelta;
        col = col + cdelta;
    }
    return Continue;
}

// There may be better ways to check the winning status
// for example, start from the newly dropped peg's position
// But this will do, for now
Status checkWinner(char board[Rows][Columns], int player)
{
    Status status = Continue;

    for(int i = 0; i < Columns && status == Continue; i++) {
        status = checkLine(board, 0, i, +1, 0, player);
    }
    for(int i = 0; i < Rows && status == Continue; i++) {
        status = checkLine(board, i, 0, 0, +1, player);
    }
    for(int i=0; i < Columns && status == Continue; i++) {
        status = checkLine(board, 0, i, +1, -1, player);
    }
    for(int i=0; i < Columns && status == Continue; i++) {
        status = checkLine(board, 0, i, +1, +1, player);
    }
    for(int i=0; i < Columns && status == Continue; i++) {
        status = checkLine(board, Rows-1, i, +1, -1, player);
    }
    for(int i=0; i < Columns && status == Continue; i++) {
      status = checkLine(board, Rows-1, i, +1, +1, player);
    }

    return status;
}

Status checkDraw(int top[])
{
    Status status = Draw;

    for(int i = 0; i < Columns && status == Draw; i++) {
        if (top[i] < Rows) status = Continue;
    }

    return status;
}

// Start of main function
int main()
{
    char board[Rows][Columns];
    char tempboard[Rows][Columns];
    int top[Columns];
    int player = Players - 1;
    RANPlayer p(Rows, Columns, Goal);
    Player AI;
    Status status = Continue;
    int position = -1;
    int choice = -1;
    string name = "";

    //p = new RANPlayer(Rows, Columns, Goal);

    initBoard(board, top);
    initBoard(tempboard, top);
    AI.setName("Arti");

    cout << "\n=== Connect Four ===\n\n";

    cout << "Would you like to play against the AI [1] or have a random player play the AI [2]?" 
         << endl;
    while ((choice != 1) && (choice != 2))
    {
        cin >> choice;
    }
    if (choice == 1) //user player, choice == 2 is rand player
    {
        cout << "Please enter your name: " << endl;
        cin >> name;
    } 

    while (status == Continue) {
        sleep(.7);
        displayBoard(board);

        player = (player + 1) % Players; // rotate the players

        if (player == 0 && choice == 2) //rand player
        {
            // player makes a move based on the previous player's move position
            position = p.move(position);

            // is the move allowed?
            if (position < 0 || position >= Columns || top[position] >= Rows)
            {
                cout << "Player " << p.getName()
                     << " is cheating!\n";
                status = Cheating;
                continue;
            }
            // record the move, marble is X
            board[top[position]][position] = playerMarble[player];
            top[position]++;

            status = checkWinner(board, player);

            if (status != Win)
                status = checkDraw(top);
        } else if (player == 0 && choice == 1) //user player
        {
            cout << "Please enter a column to place your piece: " << endl;
            do
            {
                cin >> position;
            } while ((position < 1) || (position > 8));
            position--; //decrement to allow for 0-indexed board
            // record the move, marble is X
            board[top[position]][position] = playerMarble[player];
            top[position]++;

            status = checkWinner(board, player);

            if (status != Win)
                status = checkDraw(top);
        } else if (player == 1) //AI player
        {
            // player makes a move based on the previous player's move position
            position = AI.move(position);
            // is the move allowed?
            if (position < 0 || position >= Columns || top[position] >= Rows)
            {
                cout << "Player " << AI.getName()
                     << " is cheating!\n";
                status = Cheating;
                continue;
            }
            // record the move, marble is O
            board[top[position]][position] = playerMarble[player];
            top[position]++;

            status = checkWinner(board, player);

            if (status != Win)
                status = checkDraw(top);
        }  
    }

    sleep(.7);
    displayBoard(board);

    if (status == Cheating)
    {
        if (player == 1) //rand or real life player
        {
            cout << "Player " << p.getName() << " is cheating!\n";
        } else if (player == 2)
        {
            cout << "Player " << AI.getName() << " is cheating!\n";
        }
    }
       
    else if (status == Draw)
        cout << "\n  *** It is a draw! ***\n\n";
    else {
        if (player == 0 && choice == 2) //rand or real life player
        {
            cout << "\n  *** Congratulations " << p.getName() << "! ***\n";
            cout << "   *** You won the game. ***\n\n";
        } else if (player == 1)
        {
            cout << "\n  *** Congratulations AI " << AI.getName() << "! ***\n";
            cout << "   *** You won the game! ***\n\n";
        } else if (player == 0 && choice == 1)
        {
            cout << "\n  *** Congratulations " << name << "! ***\n";
            cout << "   *** You won the game! ***\n\n";
        }
    }
    return 0;
}
