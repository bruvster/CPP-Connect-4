/**
 * @file AIPlayer.cpp
 * @author Mia Murray, 655671493, VIU
 * @version 1.0.0
 * @date October 26th, 2023
 *
 * Implements the connect 4 playing AI agent. Includes a minimax funcion that includes
 * alpha-beta pruning. Also implements an evaluation function to rank an AI's potential moves.
 */

#include <iostream>
#include <string>
#include <cassert>
#include "AIPlayer.h"
using namespace std;

void Player::initBoard(char **board, int top[])
{
    for (int i = 0; i < Rows; i++)
        for (int j = 0; j < Columns; j++)
            board[i][j] = ' ';
    for (int i = 0; i < Columns; i++)
        top[i] = 0;
}

void Player::setBoard(char **board, char **copyboard)
{
    for (int x = 0; x < Rows; x++)
    {
        for (int y = 0; y < Columns; y++)
        {
            copyboard[x][y] = board[x][y];
        }
    }
}
// return your next move, which column to drop your marble.
// otherPlayersLastMove will be -1 if you are to move first
// and this is your first move.
int Player::move(int otherPlayersLastMove)
{
    if (otherPlayersLastMove != -1)
    {
        board[top[otherPlayersLastMove]][otherPlayersLastMove] = otherMarble;
        top[otherPlayersLastMove]++;
    }
    // make a copy of the board so we can evaluate all the possible moves
    setBoard(board, tempboard);
    int count = 0;
    for (int i = 0; i < Columns; i++)
        if (top[i] < Rows)
            count++;
    // count should be guaranteed a positive number
    assert(count > 0); // if not, program will terminate
    int placement = -1;
    int best = -10000;
    for (int col = 0; col < Columns; col++)
    { // we can place here if below condition is met
        if (top[col] < Rows)
        {
            tempboard[top[col]][col] = myMarble;
            top[col]++;
            // for this assignment supposed to be depth of 3 moves (6-ply) and depth 6
            int minmax = minimax(tempboard, 6, -1000, 1000, true);
            if (minmax > best)
            {
                best = minmax; // find the highest minmax evaluation
                placement = col;
            }
            top[col]--;
            setBoard(board, tempboard); // reset board back to before move was made
        }
    }
    board[top[placement]][placement] = myMarble;
    top[placement]++;
    return placement;
}

int Player::evaluation(char **state)
{
    int eval = 0;
    // below variables are used to store last otherMarble position
    int otherlastrow = -1;
    int otherlastcol = -1;
    // positive favours myMarble, negative favours otherMarble
    for (int r = 0; r < Rows; r++)
    {
        for (int c = 0; c < Columns; c++)
        {
            if (state[r][c] == myMarble)
            { // if otherMarble almost has a connect 4 (has a connect 2), increase eval
                // in that same column so we are more likely to block them off
                if (checkLine(state, 0, c, 1, 0, otherMarble, 2))
                {
                    eval = eval + 25;
                }
                else if (checkLine(state, r, 0, 0, 1, otherMarble, 2))
                {
		    eval = eval + 25;
                }
                if (c - 1 >= 0)
                {
                    if (state[r][c - 1] == myMarble)
                        eval++;
                }
                else
                {
                    eval--;
                }
                if (c + 1 < Columns)
                {
                    if (state[r][c + 1] == myMarble)
                        eval++;
                }
                else
                {
                    eval--;
                }
                if (r - 1 >= 0)
                {
                    if (state[r - 1][c] == myMarble)
                        eval++;
                }
                else
                {
                    eval--;
                }
                if (r + 1 < Rows)
                {
                    if (state[r + 1][c] == myMarble)
                        eval++;
                }
                else
                {
                    eval--;
                }
                // now check the diagonals
                if ((r - 1 >= 0) && (c - 1 >= 0))
                {
                    if (state[r - 1][c - 1] == myMarble)
                        eval++;
                }
                else
                {
                    eval--;
                }
                if ((r + 1 < Rows) && (c + 1 < Columns))
                {
                    if (state[r + 1][c + 1] == myMarble)
                        eval++;
                }
                else
                {
                    eval--;
                }
                if ((r - 1 >= 0) && (c + 1 < Rows))
                {
                    if (state[r - 1][c + 1] == myMarble)
                        eval++;
                }
                else
                {
                    eval--;
                }
                if ((r + 1 < Rows) && (c - 1 >= 0))
                {
                    if (state[r + 1][c - 1] == myMarble)
                        eval++;
                }
                else
                {
                    eval--;
                }
                eval++;
            }
            else if (state[r][c] == otherMarble)
            { // if otherMarble was found at an ajdacent position before (same column)
                if (((otherlastrow - 1 == r) && (otherlastcol == c)) || ((otherlastrow + 1 == r) && (otherlastcol == c)))
                {
                    eval = eval - 2;
                }
                else if (((otherlastcol - 1 == c) && (otherlastrow == r)) || ((otherlastcol + 1 == c) && (otherlastrow == r)))
                {
                    eval = eval - 3;
                }
                otherlastrow = r;
                otherlastcol = c;
                eval--;
            }
        }
    }
    return eval;
}

int Player::minimax(char **state, int depth, int alpha, int beta, bool ismax)
{
    int maxEval = 0; // used to hold maximum value seen in evaluation function
    int minEval = 0; // used to store the minimum value seen in the evaluation function
    int eval = 0;    // used to store evaluation function result
    // are we at a leaf node?
    if (depth == 0)
    {
        if (isWon(state, myMarble))
        { // my win
            return 1000;
        }
        else if (isWon(state, otherMarble))
        { // other win
            return -1000;
        }
        else if (checkDraw(top))
        { // it must be a draw state
            return 0;
        }
        else
        { // its not a complete board, assign evaluation
            return evaluation(state);
        }
    }
    // searching for best move to make
    if (ismax == true) // maximizing player
    {
        maxEval = -1000;
        // for each child of current state
        for (int i = 0; i < Columns; i++)
        {
            if (top[i] < Rows)
            {
                state[top[i]][i] = myMarble;
                eval = minimax(state, depth - 1, alpha, beta, false);
                state[top[i]][i] = ' ';
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha)
                {
                    break; // prune
                }
            }
        }
        return maxEval;
    }
    else // minimizing player
    {
        minEval = 1000;
        for (int i = 0; i < Columns; i++)
        {
            if (top[i] < Rows)
            {
                state[top[i]][i] = otherMarble;
                eval = minimax(state, depth - 1, alpha, beta, true);
                state[top[i]][i] = ' ';
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha)
                {
                    break; // prune
                }
            }
        }
        return minEval;
    }
}

bool Player::checkLine(char **board, int row, int col, int rdelta, int cdelta, char piece, int goalnum)
{
    int count = 0;
    while (row < Rows && row >= 0 && col < Columns && col >= 0)
    {
        if (board[row][col] == piece)
        {
            count++;
            if (count == goalnum)
                return true;
        }
        else
        {
            count = 0;
        }
        row = row + rdelta;
        col = col + cdelta;
    }
    return false;
}

bool Player::isWon(char **board, char piece)
{
    bool win = false;
    for (int i = 0; i < Columns && win == false; i++)
    {
        win = checkLine(board, 0, i, +1, 0, piece, Goal);
    }
    for (int i = 0; i < Rows && win == false; i++)
    {
        win = checkLine(board, i, 0, 0, +1, piece, Goal);
    }
    for (int i = 0; i < Columns && win == false; i++)
    {
        win = checkLine(board, 0, i, +1, -1, piece, Goal);
    }
    for (int i = 0; i < Columns && win == false; i++)
    {
        win = checkLine(board, 0, i, +1, +1, piece, Goal);
    }
    for (int i = 0; i < Columns && win == false; i++)
    {
        win = checkLine(board, Rows - 1, i, +1, -1, piece, Goal);
    }
    for (int i = 0; i < Columns && win == false; i++)
    {
        win = checkLine(board, Rows - 1, i, +1, +1, piece, Goal);
    }
    return win;
}

bool Player::checkDraw(int top[])
{
    bool status = true;
    for (int i = 0; i < Columns && status == true; i++)
    {
        if (top[i] < Rows)
            status = false;
    }
    return status;
}

// return your agent's name or your own name
string Player::getName()
{
    return name;
}

void Player::setName(string n)
{
    name = n;
}
