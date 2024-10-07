#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20

void rnum(int *num)
{
    srand(time(NULL));
    *num = (rand() % 7) + 1;
}

void printscreen(char board[BOARD_HEIGHT][BOARD_WIDTH])
{
    
    printf("\033[%d;%dH", 1, 1);

    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (board[i][j] == '0')
            {
                printf("%c", ' ');
            }
            else
            {

                printf("%c", board[i][j]);
            }
        }
        printf("\n");
    }
}
int check_collision(char board[BOARD_HEIGHT][BOARD_WIDTH], char block[4][4], char block2[3][3], char block3[2][2], int blockHeight, int blockWidth, int x, int y, int random)
{
    for (int i = 0; i < blockHeight; i++)
    {
        for (int j = 0; j < blockWidth; j++)
        {
            if (random == 1)
            {
                if (block[i][j] != '0')
                {
                    int boardX = x + i;
                    int boardY = y + j;
                    // Check if we are out of bounds or if there's a block in the board
                    if (boardX + 1 >= BOARD_HEIGHT || boardY < 0 || boardY >= BOARD_WIDTH || board[boardX + 1][boardY] != '0')
                    {
                        return 1; // Collision detected
                    }
                }
            }
            else if (random == 2)
            {
                if (block3[i][j] != '0')
                {
                    int boardX = x + i;
                    int boardY = y + j;
                    // Check if we are out of bounds or if there's a block in the board
                    if (boardX + 1 >= BOARD_HEIGHT || boardY < 0 || boardY >= BOARD_WIDTH || board[boardX + 1][boardY] != '0')
                    {
                        return 1; // Collision detected
                    }
                }
            }

            else
            {
                if (block2[i][j] != '0')
                {
                    int boardX = x + i;
                    int boardY = y + j;
                    // Check if we are out of bounds or if there's a block in the board
                    if (boardX + 1 >= BOARD_HEIGHT || boardY < 0 || boardY >= BOARD_WIDTH || board[boardX + 1][boardY] != '0')
                    {
                        return 1; // Collision detected
                    }
                }
            }
        }
    }
    return 0; // No collision
}

int gamestatus(char board[BOARD_HEIGHT][BOARD_WIDTH])
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        if (board[0][i] != '0')
        {
            return 1;
        }
    }
    return 0;
}
void move_block(char board[BOARD_HEIGHT][BOARD_WIDTH], int *blockX, int *blockY, int blockWidth, int random, char AA[4][4], char BB[3][3], char O[2][2])
{
    // Check for left, right, and down arrow keys, as well as rotation key ('R')
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        if (*blockY > 0 && check_collision(board, AA, BB, O, blockWidth, blockWidth, *blockX, *blockY - 1, random) == 0)
        {
            *blockY -= 1; // Move block left
        }
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        if (*blockY + blockWidth < BOARD_WIDTH && check_collision(board, AA, BB, O, blockWidth, blockWidth, *blockX, *blockY + 1, random) == 0)
        {
            *blockY += 1; // Move block right
        }
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        if (*blockX + blockWidth < BOARD_HEIGHT && check_collision(board, AA, BB, O, blockWidth, blockWidth, *blockX + 1, *blockY, random) == 0)
        {
            *blockX += 1; // Move block down faster
        }
    }
    else if (GetAsyncKeyState('R') & 0x8000)
    {
        // Rotate block (example with 3x3 blocks)
        // Simple rotation by 90 degrees (clockwise)
        char temp[3][3];
        if (random != 1 && random != 2) // Example for 3x3 blocks
        {
            // Rotate the block
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    temp[i][j] = BB[2 - j][i];
                }
            }

            // Check if the rotated block fits in the current position
            if (check_collision(board, AA, temp, O, 3, 3, *blockX, *blockY, random) == 0)
            {
                // Update the block with the rotated version
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        BB[i][j] = temp[i][j];
                    }
                }
            }
        }
        char temp1[4][4];
        if (random == 1)
        {
            // Rotate the block
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    temp1[i][j] = AA[4 - j - 1][i];
                    // temp[i][j] = BB[2 - j][i];
                }
            }

            // Check if the rotated block fits in the current position
            if (check_collision(board, AA, temp, O, 3, 3, *blockX, *blockY, random) == 0)
            {
                // Update the block with the rotated version
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        // BB[i][j] = temp[i][j];
                        AA[i][j] = temp1[i][j];
                    }
                }
            }
        }
    }
}

void check_clear_lines(char board[BOARD_HEIGHT][BOARD_WIDTH])
{
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        int full_line = 1; // Assume the row is full initially

        // Check if the current row is full
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (board[i][j] == '0') // If there's an empty cell, the line is not full
            {
                full_line = 0;
                break;
            }
        }

        // If the line is full, clear it
        if (full_line)
        {
            // Shift all rows above down by one
            for (int k = i; k > 0; k--)
            {
                for (int j = 0; j < BOARD_WIDTH; j++)
                {
                    board[k][j] = board[k - 1][j];
                }
            }

            // Set the top row to empty (all '0')
            for (int j = 0; j < BOARD_WIDTH; j++)
            {
                board[0][j] = '0';
            }

            // Since we've cleared a line, recheck the same row index (i) after shifting
            i--;
        }
    }
}

int main(int argc, char const *argv[])
{
    char board[20][20] = {
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'}};
    // creating the block
    char I[4][4] = {
        {'1', '0', '0', '0'},
        {'1', '0', '0', '0'},
        {'1', '0', '0', '0'},
        {'1', '0', '0', '0'}};
    char O[2][2] = {
        {'2', '2'},
        {'2', '2'}};
    char T[3][3] = {
        {'3', '3', '3'},
        {'0', '3', '0'},
        {'0', '0', '0'}};
    char L[3][3] = {
        {'4', '0', '0'},
        {'4', '0', '0'},
        {'4', '4', '0'}};

    char J[3][3] = {
        {'0', '5', '0'},
        {'0', '5', '0'},
        {'5', '5', '0'}};

    char S[3][3] = {
        {'0', '6', '6'},
        {'6', '6', '0'},
        {'0', '0', '0'}};

    char Z[3][3] = {
        {'7', '7', '0'},
        {'0', '7', '7'},
        {'0', '0', '0'}};

start:

    int random_number;
    rnum(&random_number);
    // Assume we set the block to a position (x=0, y=8)
    int blockX = 0;
    int blockY = 8;

    // adding twisting blocks
    char arrowkey;
    char AA[4][4] = {
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'}};
    char BB[3][3] = {
        {'0', '0', '0'},
        {'0', '0', '0'},
        {'0', '0', '0'}};
    char CC[2][2] = {
        {'0', '0'},
        {'0', '0'}};
    // memory
    //  gravity and intigration of blocks in the board

    switch (random_number) // intigration
    {
    case 1:
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                AA[i][j] = I[i][j];
            }
        }
        break;
    case 3:
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                BB[i][j] = T[i][j];
            }
        }
        break;
    case 4:
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                BB[i][j] = L[i][j];
            }
        }
        break;
    case 5:
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                BB[i][j] = J[i][j];
            }
        }
        break;
    case 6:
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                BB[i][j] = S[i][j];
            }
        }
        break;
    case 7:
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                BB[i][j] = Z[i][j];
            }
        }
        break;

    default:
        break;
    }

    printscreen(board);
    for (int x = 0; x < 20; x++)
    {

        // move down
        if (random_number == 1)
        {
            if (check_collision(board, AA, BB, O, 4, 4, blockX, blockY, random_number) == 0)

            {
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if (AA[i][j] != '0')
                        {
                            board[blockX + i][blockY + j] = AA[i][j];
                        }
                    }
                }
                printscreen(board);

                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if (AA[i][j] != '0')
                        {
                            board[blockX + i][blockY + j] = '0';
                        }
                    }
                }

                blockX++;
            }
            else
            {
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if (AA[i][j] != '0')
                        {
                            board[blockX + i][blockY + j] = AA[i][j];
                        }
                    }
                }
            }
        }
        else if (random_number == 2)
        {
            if (check_collision(board, AA, BB, O, 2, 2, blockX, blockY, random_number) == 0)

            {
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        if (O[i][j] != '0')
                        {
                            board[blockX + i][blockY + j] = O[i][j];
                        }
                    }
                }
                printscreen(board);

                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        if (O[i][j] != '0')
                        {
                            board[blockX + i][blockY + j] = '0';
                        }
                    }
                }
                blockX++;
            }
            else
            {
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        if (O[i][j] != '0')
                        {
                            board[blockX + i][blockY + j] = O[i][j];
                        }
                    }
                }
            }
        }
        else
        {
            if (check_collision(board, AA, BB, O, 3, 3, blockX, blockY, random_number) == 0)
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (BB[i][j] != '0')
                        {
                            board[blockX + i][blockY + j] = BB[i][j];
                        }
                    }
                }
                printscreen(board);

                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (BB[i][j] != '0')
                        {
                            board[blockX + i][blockY + j] = '0';
                        }
                    }
                }
                blockX++;
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (BB[i][j] != '0')
                        {
                            board[blockX + i][blockY + j] = BB[i][j];
                        }
                    }
                }
            }
        }
        {

            move_block(board, &blockX, &blockY, 3, random_number, AA, BB, O);
            check_clear_lines(board);
        }

        printf("\033[%d;%dH", 0, 0);
        Sleep(500);
    }

    if (gamestatus(board) == 0)
    {
        goto start;
    }
    else
    {
        printf("\n");
        printf("\n");
        printf("\n");
        printf("####################\n");
        printf("#    GAME OVER!    #\n");
        printf("####################\n");
        printf("\n");
        printf("\n");
        printf("\n");
    }

    return 0;
}
