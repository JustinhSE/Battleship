#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT1 2201
#define PORT2 2202
#define BUFFER_SIZE 1024
int listen_fd1, conn_fd1;
int listen_fd2, conn_fd2;
int index1 = 0, index2 = 0;
char player1_info[5][4] = {0};
char player2_info[5][4] = {0};
int **playerOneGuesses;
int **playerTwoGuesses;
int shipsRemaining1 = 5;
int shipsRemaining2 = 5;
unsigned int width = 0, height = 0;
int **board_playerOne;
int **board_playerTwo;
bool ship_remains(int **board, int num)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (board[i][j] == num)
            {
                return true;
            }
        }
    }
    return false;
}
bool piece_guessed(int **guess, int x, int y)
{
    for (int i = 0; guess[i][0] != '\0'; i++)
    {
        if (guess[i][1] == x && guess[i][2] == y)
        {
            return true;
        }
    }
    return false;
}
void query(char *text, int **guesses, int player)
{
    text[0] = 'G';
    text[1] = ' ';
    text[2] = ((player == 1 ? shipsRemaining2 : shipsRemaining1) + '0');
    text[3] = ' ';
    int arrIndex = 0;
    for (int i = 0; guesses[arrIndex][0] != '\0'; i += 6)
    {
        text[4 + i] = guesses[arrIndex][0] == 1 ? 'M' : 'H';
        text[4 + i + 1] = ' ';
        text[4 + i + 2] = (guesses[arrIndex][1] + '0');
        text[4 + i + 3] = ' ';
        text[4 + i + 4] = (guesses[arrIndex][2] + '0');
        text[4 + i + 5] = ' ';

        arrIndex++;
    }
    text[strlen(text) - 1] = '\0';
}

void memory_allocation(int height, int width)
{
    // guesses
    playerOneGuesses = malloc(1000 * sizeof(int *));
    playerTwoGuesses = malloc(1000 * sizeof(int *));
    for (int i = 0; i < 1000; i++)
    {
        playerOneGuesses[i] = malloc(3 * sizeof(int));
        playerTwoGuesses[i] = malloc(3 * sizeof(int));
    }

    // guesses

    board_playerOne = malloc(height * sizeof(int *));
    board_playerTwo = malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++)
    {
        board_playerOne[i] = malloc(width * sizeof(int));
        board_playerTwo[i] = malloc(width * sizeof(int));
    }
}
void print_boards()
{
    // boards
    printf("p1\n");
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%d ", board_playerOne[i][j]);
        }
        printf("\n");
    }

    printf("p2\n");
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%d ", board_playerTwo[i][j]);
        }
        printf("\n");
    }
}
int fill_shape(int **board, int piece_type, int rotation, int x, int y, int num)
{
    if (piece_type > 7 || piece_type < 1)
    {
        return 300;
    }
    else if (rotation > 4 || rotation < 1)
    {
        return 301;
    }
    else if (x >= 0 && y >= 0 && x < height && y < width)
    {
        if (piece_type == 1)
        {
            if (x + 1 < height && y + 1 < width)
            {
                if (board[x][y] > 0)
                {
                    return 303;
                }
                else if (board[x][y] == 0)
                {
                    board[x][y] = num;
                }
                if (board[x + 1][y] > 0)
                {
                    return 303;
                }
                if (board[x + 1][y] == 0)
                {
                    board[x + 1][y] = num;
                }
                if (board[x][y + 1] > 0)
                {
                    return 303;
                }
                if (board[x][y + 1] == 0)
                {
                    board[x][y + 1] = num;
                }
                if (board[x + 1][y + 1] > 0)
                {
                    return 303;
                }
                if (board[x + 1][y + 1] == 0)
                {
                    board[x + 1][y + 1] = num;
                }
                return 0;
            }
            else
            {
                return 302;
            }
        }
        else if (piece_type == 2)
        {
            if (rotation == 1 || rotation == 3)
            {
                if (x + 3 < height)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x + 1][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y] == 0)
                    {
                        board[x + 1][y] = num;
                    }
                    if (board[x + 2][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 2][y] == 0)
                    {
                        board[x + 2][y] = num;
                    }
                    if (board[x + 3][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 3][y] == 0)
                    {
                        board[x + 3][y] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
            if (rotation == 2 || rotation == 4)
            {
                if (y + 3 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    if (board[x][y + 2] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 2] == 0)
                    {
                        board[x][y + 2] = num;
                    }
                    if (board[x][y + 3] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 3] == 0)
                    {
                        board[x][y + 3] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
        }
        else if (piece_type == 3)
        {
            if (rotation == 1 || rotation == 3)
            {
                if (x - 1 >= 0 && y + 2 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    if (board[x - 1][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x - 1][y + 1] == 0)
                    {
                        board[x - 1][y + 1] = num;
                    }
                    if (board[x - 1][y + 2] > 0)
                    {
                        return 303;
                    }
                    if (board[x - 1][y + 2] == 0)
                    {
                        board[x - 1][y + 2] = num;
                    }
                }
                else
                {
                    return 302;
                }
            }
            if (rotation == 2 || rotation == 4)
            {
                if (x + 2 < height && y + 1 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x + 1][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y] == 0)
                    {
                        board[x + 1][y] = num;
                    }
                    if (board[x + 1][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y + 1] == 0)
                    {
                        board[x + 1][y + 1] = num;
                    }
                    if (board[x + 2][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 2][y + 1] == 0)
                    {
                        board[x + 2][y + 1] = num;
                    }
                }
                else
                {
                    return 302;
                }
            }
        }
        else if (piece_type == 4)
        {
            if (rotation == 1)
            {
                if (x + 2 < height && y + 1 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x + 1][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y] == 0)
                    {
                        board[x + 1][y] = num;
                    }
                    if (board[x + 2][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 2][y] == 0)
                    {
                        board[x + 2][y] = num;
                    }
                    if (board[x + 2][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 2][y + 1] == 0)
                    {
                        board[x + 2][y + 1] = num;
                    }
                }
                else
                {
                    return 302;
                }
            }
            if (rotation == 2)
            {
                if (x + 1 < height && y + 2 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    if (board[x][y + 2] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 2] == 0)
                    {
                        board[x][y + 2] = num;
                    }
                    if (board[x + 1][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y] == 0)
                    {
                        board[x + 1][y] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
            if (rotation == 3)
            {
                if (x + 2 < height && y + 1 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    if (board[x + 1][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y + 1] == 0)
                    {
                        board[x + 1][y + 1] = num;
                    }
                    if (board[x + 2][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 2][y + 1] == 0)
                    {
                        board[x + 2][y + 1] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
            if (rotation == 4)
            {
                if (x - 1 >= 0 && y + 2 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x - 1][y + 2] > 0)
                    {
                        return 303;
                    }
                    if (board[x - 1][y + 2] == 0)
                    {
                        board[x - 1][y + 2] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    if (board[x][y + 2] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 2] == 0)
                    {
                        board[x][y + 2] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
        }
        else if (piece_type == 5)
        {
            if (rotation == 1 || rotation == 3)
            {
                if (x + 1 < height && y + 2 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    if (board[x + 1][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y + 1] == 0)
                    {
                        board[x + 1][y + 1] = num;
                    }
                    if (board[x + 1][y + 2] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y + 2] == 0)
                    {
                        board[x + 1][y + 2] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
            if (rotation == 2 || rotation == 4)
            {
                if (x + 1 < height && x - 1 >= 0 && y + 1 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x - 1][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x - 1][y + 1] == 0)
                    {
                        board[x - 1][y + 1] = num;
                    }
                    if (board[x + 1][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y] == 0)
                    {
                        board[x + 1][y] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
        }
        else if (piece_type == 6)
        {
            if (rotation == 1)
            {
                if (x - 2 >= 0 && y + 1 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    if (board[x - 1][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x - 1][y + 1] == 0)
                    {
                        board[x - 1][y + 1] = num;
                    }
                    if (board[x - 2][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x - 2][y + 1] == 0)
                    {
                        board[x - 2][y + 1] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
            if (rotation == 2)
            {
                if (x + 1 < height && y + 2 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    if (board[x + 1][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y + 1] == 0)
                    {
                        board[x + 1][y + 1] = num;
                    }
                    if (board[x + 1][y + 2] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y + 2] == 0)
                    {
                        board[x + 1][y + 2] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
            if (rotation == 3)
            {
                if (x + 2 < height && y + 1 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x + 1][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y] == 0)
                    {
                        board[x + 1][y] = num;
                    }
                    if (board[x + 2][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 2][y] == 0)
                    {
                        board[x + 2][y] = num;
                    }
                    if (board[x + 2][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 2][y + 1] == 0)
                    {
                        board[x + 2][y + 1] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
            if (rotation == 4)
            {
                if (x + 1 < height && y + 2 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    if (board[x][y + 2] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 2] == 0)
                    {
                        board[x][y + 2] = num;
                    }
                    if (board[x + 1][y + 2] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y + 2] == 0)
                    {
                        board[x + 1][y + 2] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
        }
        else if (piece_type == 7)
        {
            if (rotation == 1)
            {
                if (x + 1 < height && y + 2 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x + 1][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y + 1] == 0)
                    {
                        board[x + 1][y + 1] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    if (board[x][y + 2] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 2] == 0)
                    {
                        board[x][y + 2] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
            if (rotation == 2)
            {
                if (x - 1 >= 0 && x + 1 < height && y + 1 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x - 1][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x - 1][y + 1] == 0)
                    {
                        board[x - 1][y + 1] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    if (board[x + 1][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y + 1] == 0)
                    {
                        board[x + 1][y + 1] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
            if (rotation == 3)
            {
                if (x - 1 >= 0 && y + 2 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 1] == 0)
                    {
                        board[x][y + 1] = num;
                    }
                    if (board[x - 1][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x - 1][y + 1] == 0)
                    {
                        board[x - 1][y + 1] = num;
                    }
                    if (board[x][y + 2] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y + 2] == 0)
                    {
                        board[x][y + 2] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
            if (rotation == 4)
            {
                if (x + 2 < height && y + 1 < width)
                {
                    if (board[x][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x][y] == 0)
                    {
                        board[x][y] = num;
                    }
                    if (board[x + 1][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y] == 0)
                    {
                        board[x + 1][y] = num;
                    }
                    if (board[x + 1][y + 1] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 1][y + 1] == 0)
                    {
                        board[x + 1][y + 1] = num;
                    }
                    if (board[x + 2][y] > 0)
                    {
                        return 303;
                    }
                    if (board[x + 2][y] == 0)
                    {
                        board[x + 2][y] = num;
                    }
                    return 0;
                }
                else
                {
                    return 302;
                }
            }
        }
    }
    else
    {
        return 302;
    }
    return 0;
}

int main()
{
    struct sockaddr_in address1;
    struct sockaddr_in address2;
    int nbytes = -1;
    int opt = 1;
    int opt2 = 1;

    int addrlen = sizeof(address1);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket 1
    if ((listen_fd1 = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // Create socket 2
    if ((listen_fd2 = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket 1 options
    if (setsockopt(listen_fd1, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))");
        exit(EXIT_FAILURE);
    }

    // Set socket 2 options
    if (setsockopt(listen_fd2, SOL_SOCKET, SO_REUSEADDR, &opt2, sizeof(opt2)))
    {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port 1
    address1.sin_family = AF_INET;
    address1.sin_addr.s_addr = INADDR_ANY;
    address1.sin_port = htons(PORT1);
    if (bind(listen_fd1, (struct sockaddr *)&address1, sizeof(address1)) < 0)
    {
        perror("[Server] bind() failed.");
        exit(EXIT_FAILURE);
    }
    // Bind socket to port 2
    address2.sin_family = AF_INET;
    address2.sin_addr.s_addr = INADDR_ANY;
    address2.sin_port = htons(PORT2);
    if (bind(listen_fd2, (struct sockaddr *)&address2, sizeof(address2)) < 0)
    {
        perror("[Server] bind() failed.");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(listen_fd1, 3) < 0)
    {
        perror("[Server] listen() failed.");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(listen_fd2, 3) < 0)
    {
        perror("[Server] listen() failed.");
        exit(EXIT_FAILURE);
    }

    printf("[Server] Running on port %d and port %d \n", PORT1, PORT2);

    // Accept incoming connection
    if ((conn_fd1 = accept(listen_fd1, (struct sockaddr *)&address1, (socklen_t *)&addrlen)) < 0)
    {
        perror("[Server] accept() failed.");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection
    if ((conn_fd2 = accept(listen_fd2, (struct sockaddr *)&address2, (socklen_t *)&addrlen)) < 0)
    {
        perror("[Server] accept() failed.");
        exit(EXIT_FAILURE);
    }

    // begin
    char *msg = malloc(10 * sizeof(char));
    char start = 1;

    // port 1
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        nbytes = read(conn_fd1, buffer, BUFFER_SIZE); // reading from client
        if (nbytes <= 0)
        {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }
        if (strcmp(buffer, "quit") == 0)
        {
            printf("[Server] Client chatter quitting...\n");
        }

        // client 1 sends a message to server

        if (buffer[0] == 'F')
        {
            send(conn_fd1, "H 0", 3, 0);
            send(conn_fd2, "H 1", 3, 0);
            close(conn_fd1);
            close(listen_fd1);
            close(conn_fd2);
            close(listen_fd2);
            return 0;
        }
        else if (buffer[0] != 'B')
        {
            msg = "E 100";
            send(conn_fd1, msg, strlen(msg), 0);
            continue;
        }
        else if (buffer[0] == 'B')
        {
            if (sscanf(buffer, "%c %d %d", &start, &width, &height) == 3)
            {
                if (buffer[10] != '\0' || width < 10 || height < 10)
                {
                    msg = "E 200";
                    send(conn_fd1, msg, strlen(msg), 0);
                    continue;
                }
                else
                {
                    send(conn_fd1, "A", 2, 0);
                    memory_allocation(height, width);
                    break;
                }
            }
            else
            {
                msg = "E 200";
                send(conn_fd1, msg, strlen(msg), 0);
                continue;
            }
        }
    }

    // port 2
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
        if (nbytes <= 0)
        {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
            continue;
        }
        if (strcmp(buffer, "quit") == 0)
        {
            printf("[Server] Client chatter quitting...\n");
            return 0;
        }
        // get player 2 to begin

        if (buffer[0] == 'F')
        {
            send(conn_fd2, "H 0", 3, 0);
            send(conn_fd1, "H 1", 3, 0);
            close(conn_fd1);
            close(listen_fd1);
            close(conn_fd2);
            close(listen_fd2);
            return 0;
        }
        else if (buffer[0] != 'B')
        {
            msg = "E 100";
            send(conn_fd2, msg, strlen(msg), 0);
            continue;
        }
        else if (buffer[0] == 'B')
        {
            if (buffer[1] != '\0')
            {
                msg = "E 200";
                send(conn_fd2, msg, strlen(msg), 0);
                continue;
            }
            else
            {
                send(conn_fd2, "A", 2, 0);
                break;
            }
        }
    }

    // initialize pieces

    // port 1
    while (1)
    {

        bool scanned_correct = false, overlap = false;
        memset(buffer, 0, BUFFER_SIZE);
        nbytes = read(conn_fd1, buffer, BUFFER_SIZE);
        if (nbytes <= 0)
        {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }

        if (strcmp(buffer, "quit") == 0)
        {
            printf("[Server] Client chatter quitting...\n");
            return 0;
        }
        if (buffer[0] == 'F')
        {
            send(conn_fd1, "H 0", 4, 0);
            send(conn_fd2, "H 1", 4, 0);
            close(conn_fd1);
            close(listen_fd1);
            close(conn_fd2);
            close(listen_fd2);
            return 0;
        }

        bool is_null = false;

        if (buffer[0] == 'I')
        {
            // skip I and \0
            char *initialized = strtok(buffer, " ");
            initialized = strtok(NULL, " ");

            int min = __INT_MAX__;
            int e = 0, err = 0; // false

            for (int i = 0; i < 5; i++)
            {
                // printf("curr @ index %d ", i);
                int piece_type = -1, piece_rotation = -1, piece_column = -1, piece_row = -1;
                if (initialized != NULL)
                {
                    // type\n");
                    piece_type = atoi(initialized);
                    initialized = strtok(NULL, " ");
                    if (initialized == NULL)
                    {
                        is_null = true;
                        break;
                    }
                }
                if (initialized != NULL)
                {

                    // rot\n");
                    piece_rotation = atoi(initialized);
                    initialized = strtok(NULL, " ");
                    if (initialized == NULL)
                    {
                        is_null = true;
                        break;
                    }
                }
                if (initialized != NULL)
                {

                    // col\n");
                    piece_column = atoi(initialized);
                    initialized = strtok(NULL, " ");
                    if (initialized == NULL)
                    {
                        is_null = true;
                        break;
                    }
                }
                if (initialized != NULL)
                {
                    // row");
                    piece_row = atoi(initialized);
                    initialized = strtok(NULL, " ");
                }
                else
                {
                    is_null = true;
                    break;
                }
                err = fill_shape(board_playerOne, piece_type, piece_rotation, piece_row, piece_column, i + 1);
                printf("err : %d\n", err);
                if (err == 0)
                {
                    player1_info[i][0] = piece_type;
                    player1_info[i][1] = piece_rotation;
                    player1_info[i][2] = piece_column;
                    player1_info[i][3] = piece_row;
                }
                else
                {
                    e = 1;
                    min = (min > err) ? err : min;
                }
            }

            if (is_null || initialized != NULL)
            {
                msg = "E 201";
                send(conn_fd1, msg, 10, 0);
                continue;
            }
            else if (e > 0)
            {
                for (int i = 0; i < 5; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        player1_info[i][j] = 0;
                    }
                }
                for(int i = 0; i < height; i++){
                    for(int j = 0 ; j < width; j++){
                        board_playerOne[i][j] = 0;
                    }
                }
                snprintf(buffer, BUFFER_SIZE, "E %d", min);
                send(conn_fd1, buffer, BUFFER_SIZE, 0);
                continue;
            }
            else
            {
                send(conn_fd1, "A", BUFFER_SIZE, 0);
                break;
            }
        }
        else
        {
            msg = "E 101";
            send(conn_fd1, msg, BUFFER_SIZE, 0);
            continue;
        }
    }

    // port 2
    while (1)
    {
        int e = 0; // false

        // iteration \n");
        bool scanned_correct = false, overlap = false;
        memset(buffer, 0, BUFFER_SIZE);
        nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
        if (nbytes <= 0)
        {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }

        if (strcmp(buffer, "quit") == 0)
        {
            printf("[Server] Client chatter quitting...\n");
            return 0;
        }
        if (buffer[0] == 'F')
        {
            send(conn_fd1, "H 1", 4, 0);
            send(conn_fd2, "H 0", 4, 0);
            close(conn_fd1);
            close(listen_fd1);
            close(conn_fd2);
            close(listen_fd2);
            return 0;
        }

        bool is_null = false;

        int min = __INT_MAX__;
        if (buffer[0] == 'I')
        {
            // skip I and \0
            char *initialized = strtok(buffer, " ");
            initialized = strtok(NULL, " ");

            int min = __INT_MAX__;

            for (int i = 0; i < 5; i++)
            {
                // curr @ index %d ", i);
                int piece_type = -1, piece_rotation = -1, piece_column = -1, piece_row = -1;
                if (initialized != NULL)
                {
                    // type\n");
                    piece_type = atoi(initialized);
                    initialized = strtok(NULL, " ");
                    if (initialized == NULL)
                    {
                        is_null = true;
                        break;
                    }
                }
                if (initialized != NULL)
                {

                    // rot\n");
                    piece_rotation = atoi(initialized);
                    initialized = strtok(NULL, " ");
                    if (initialized == NULL)
                    {
                        is_null = true;
                        break;
                    }
                }
                if (initialized != NULL)
                {

                    // col\n");
                    piece_column = atoi(initialized);
                    initialized = strtok(NULL, " ");
                    if (initialized == NULL)
                    {
                        is_null = true;
                        break;
                    }
                }
                if (initialized != NULL)
                {
                    // row");
                    piece_row = atoi(initialized);
                    initialized = strtok(NULL, " ");
                }
                else
                {
                    is_null = true;
                    break;
                }
                // called\n");
                int err = fill_shape(board_playerTwo, piece_type, piece_rotation, piece_row, piece_column, i + 1);
                printf("err : %d\n", err);
                if (err == 0)
                {
                    player2_info[i][0] = piece_type;
                    player2_info[i][1] = piece_rotation;
                    player2_info[i][2] = piece_column;
                    player2_info[i][3] = piece_row;
                }
                
                else
                {
                    e = 1;
                    min = (err > min) ? min : err;
                }
            }

            if (is_null)
            {
                // isnull \n");

                msg = "E 201";
                send(conn_fd2, msg, 10, 0);
                continue;
            }
            else if (initialized != NULL)
            {
                // init \n");

                msg = "E 201";
                send(conn_fd2, msg, 10, 0);
                continue;
            }
            else if (e > 0)
            {
                for (int i = 0; i < 5; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        player2_info[i][j] = 0;
                    }
                }
                for(int i = 0; i < height; i++){
                    for(int j = 0 ; j < width; j++){
                        board_playerTwo[i][j] = 0;
                    }
                }
                // entered \n");
                snprintf(buffer, BUFFER_SIZE, "E %d", min);
                send(conn_fd2, buffer, BUFFER_SIZE, 0);
                continue;
            }
            else
            {
                send(conn_fd2, "A", BUFFER_SIZE, 0);
                break;
            }
        }
        else
        {
            msg = "E 101";
            send(conn_fd2, msg, BUFFER_SIZE, 0);
            continue;
        }
    }

    // Shoot/Queries/Forfeit
    int shoot1X = 0, shoot1Y = 0, shoot2X = 0, shoot2Y = 0;
    char t1;
    int t2, t3, t4;
    char *text = malloc(1024);
    while (1)
    {
        // player 1
        while (1)
        {
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd1, buffer, BUFFER_SIZE); // reading from client 1
            if (nbytes <= 0)
            {
                perror("[Server] read() failed.");
                exit(EXIT_FAILURE);
            }
            if (strcmp(buffer, "quit") == 0)
            {
                printf("[Server] Client chatter quitting...\n");
            }
            // client 1 sends a message to server
            if (buffer[0] == 'F') // need help with this
            {
                send(conn_fd1, "H 0", BUFFER_SIZE, 0);
                send(conn_fd2, "H 1", BUFFER_SIZE, 0);
                close(conn_fd1);
                close(listen_fd1);
                close(conn_fd2);
                close(listen_fd2);
                return 0;
            }
            else if (buffer[0] == 'Q')
            {
                if (index1 == 0)
                {
                    send(conn_fd1, "G 5", BUFFER_SIZE, 0);
                }
                else
                {

                    query(text, playerOneGuesses, 1);
                    send(conn_fd1, text, BUFFER_SIZE, 0);
                }
                continue;
            }
            else if (buffer[0] != 'S')
            {
                send(conn_fd1, "E 102", BUFFER_SIZE, 0);
                continue;
            }

            else if (sscanf(buffer, "%c %d %d", &start, &shoot1X, &shoot1Y) == 3 && start == 'S')
            {
                if (sscanf(buffer, "%c %d %d %d", &t1, &t2, &t3, &t4) > 3)
                {
                    send(conn_fd1, "E 202", BUFFER_SIZE, 0);
                    continue;
                }

                if (shoot1X >= height || shoot1X < 0 || shoot1Y >= width || shoot1Y < 0)
                {
                    send(conn_fd1, "E 400", BUFFER_SIZE, 0);
                    continue;
                }
                else if (piece_guessed(playerOneGuesses, shoot1X, shoot1Y))
                {
                    send(conn_fd1, "E 401", BUFFER_SIZE, 0);
                    continue;
                }

                else if (board_playerTwo[shoot1X][shoot1Y] > 0)
                {
                    playerOneGuesses[index1][0] = 2;
                    playerOneGuesses[index1][1] = shoot1X;
                    playerOneGuesses[index1][2] = shoot1Y;
                    index1++;
                    int item = board_playerTwo[shoot1X][shoot1Y];
                    board_playerTwo[shoot1X][shoot1Y] = 0; // piece of ship down
                    if (ship_remains(board_playerTwo, item))
                    {
                        snprintf(buffer, BUFFER_SIZE, "R %d H", shipsRemaining2);
                        send(conn_fd1, buffer, BUFFER_SIZE, 0);
                        break; // no other other operations needed
                    }
                    else if ((shipsRemaining2 - 1) > 0)
                    {
                        shipsRemaining2--;
                        snprintf(buffer, BUFFER_SIZE, "R %d H", shipsRemaining2);
                        send(conn_fd1, buffer, BUFFER_SIZE, 0);
                        break;
                    }
                    else
                    {
                        // win
                        send(conn_fd1, "R 0 H", BUFFER_SIZE, 0);
                        send(conn_fd2, "H 0", BUFFER_SIZE, 0);
                        send(conn_fd1, "H 1", BUFFER_SIZE, 0);
                        close(conn_fd1);
                        close(listen_fd1);
                        close(conn_fd2);
                        close(listen_fd2);
                        return 0;
                    }
                }
                else if (board_playerTwo[shoot1X][shoot1Y] == 0)
                {
                    playerOneGuesses[index1][0] = 1;
                    playerOneGuesses[index1][1] = shoot1X;
                    playerOneGuesses[index1][2] = shoot1Y;
                    index1++;
                    snprintf(buffer, BUFFER_SIZE, "R %d M", shipsRemaining2);
                    send(conn_fd1, buffer, BUFFER_SIZE, 0);
                    break;
                }
                else
                {
                    // smile
                }
            }
            else
            {
                if (buffer[0] == 'S')
                {
                    send(conn_fd1, "E 202", BUFFER_SIZE, 0);
                    continue;
                }
            }
        }

        // player 2
        while (1)
        {
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd2, buffer, BUFFER_SIZE); // reading from client 1
            if (nbytes <= 0)
            {
                perror("[Server] read() failed.");
                exit(EXIT_FAILURE);
            }
            if (strcmp(buffer, "quit") == 0)
            {
                printf("[Server] Client chatter quitting...\n");
            }
            // client 1 sends a message to server
            if (buffer[0] == 'F') // need help with this
            {
                send(conn_fd2, "H 0", BUFFER_SIZE, 0);
                send(conn_fd1, "H 1", BUFFER_SIZE, 0);
                close(conn_fd1);
                close(listen_fd1);
                close(conn_fd2);
                close(listen_fd2);
                return 0;
            }
            else if (buffer[0] == 'Q')
            {

                if (index2 == 0)
                {
                    send(conn_fd2, "G 5", BUFFER_SIZE, 0);
                }
                else
                {
                    // fix
                    query(text, playerTwoGuesses, 2);
                    send(conn_fd2, text, BUFFER_SIZE, 0);
                }
                continue;
            }
            else if (buffer[0] != 'S')
            {
                send(conn_fd2, "E 102", BUFFER_SIZE, 0);
                continue;
            }

            else if (sscanf(buffer, "%c %d %d", &start, &shoot2X, &shoot2Y) == 3 && start == 'S')
            {
                if (sscanf(buffer, "%c %d %d %d", &t1, &t2, &t3, &t4) > 3)
                {
                    send(conn_fd2, "E 202", BUFFER_SIZE, 0);
                    continue;
                }

                if (shoot2X >= height || shoot2X < 0 || shoot2Y >= width || shoot2Y < 0)
                {
                    send(conn_fd2, "E 400", BUFFER_SIZE, 0);
                    continue;
                }
                else if (piece_guessed(playerTwoGuesses, shoot2X, shoot2Y))
                {
                    send(conn_fd2, "E 401", BUFFER_SIZE, 0);
                    continue;
                }

                else if (board_playerOne[shoot2X][shoot2Y] > 0)
                {
                    playerTwoGuesses[index2][0] = 2;
                    playerTwoGuesses[index2][1] = shoot2X;
                    playerTwoGuesses[index2][2] = shoot2Y;
                    index2++;
                    int item = board_playerOne[shoot2X][shoot2Y];
                    board_playerOne[shoot2X][shoot2Y] = 0; // piece of ship down
                    if (ship_remains(board_playerOne, item))
                    {
                        snprintf(buffer, BUFFER_SIZE, "R %d H", shipsRemaining1);
                        send(conn_fd2, buffer, BUFFER_SIZE, 0);
                        break; // no other other operations needed
                    }
                    else if ((shipsRemaining1 - 1) > 0)
                    {
                        shipsRemaining1--;
                        snprintf(buffer, BUFFER_SIZE, "R %d H", shipsRemaining1);
                        send(conn_fd2, buffer, BUFFER_SIZE, 0);
                        break;
                    }
                    else
                    {
                        send(conn_fd2, "R 0 H", BUFFER_SIZE, 0);
                        send(conn_fd1, "H 0", BUFFER_SIZE, 0);
                        send(conn_fd2, "H 1", BUFFER_SIZE, 0);
                        close(conn_fd1);
                        close(listen_fd1);
                        close(conn_fd2);
                        close(listen_fd2);
                        return 0;
                    }
                }
                else if (board_playerOne[shoot2X][shoot2Y] == 0)
                {
                    playerTwoGuesses[index2][0] = 1;
                    playerTwoGuesses[index2][1] = shoot2X;
                    playerTwoGuesses[index2][2] = shoot2Y;
                    index2++;
                    snprintf(buffer, BUFFER_SIZE, "R %d M", shipsRemaining1);
                    send(conn_fd2, buffer, BUFFER_SIZE, 0);
                    break;
                }
                else
                {
                    // smile
                }
            }
            else
            {
                if (buffer[0] == 'S')
                {
                    send(conn_fd2, "E 202", BUFFER_SIZE, 0);
                    continue;
                }
            }
        }
        // print_boards();
    }
}
