#include <vector>
#include <windows.h> //included Windows.h
#include <stdio.h>
#include <iostream>


#include "Declaration.h"
#include "GraphicAPI.h"
#include "Develop.h"
#include "Sound.h"
#include "Screens.h"

using namespace std;

//int screen[playZoneH][playZoneW] = {};
int zone[playZoneH+1][playZoneW+1] = {0};
// Zone 0 = void
// Zone 1 = die
// Zone 2 = snakeHead
// Zone 3 = snakeTail
// Zone 4 = food

int score = 0;
int snakeX = (playZoneW/2), snakeY = (playZoneH/2);
int foodX = 0, foodY = 0;
int gameSpeed = 40;
int timer = 0;
int numTails = 1;

vector <int> tailX, tailY;

char direct ='S';
bool gameOver = false;
bool gameStart = false;

//----------------------------
void safeBorderLogic()
{
    // Left side
    if (snakeX == 0 && zone[snakeY][snakeX] != 1)
    {
        snakeX = playZoneW - 1;
    }
    // Right side
    if (snakeX == playZoneW && zone[snakeY][snakeX] != 1)
    {
        snakeX = 0;
    }
    // Bot side
    if (snakeY == playZoneH && zone[snakeY][snakeX] != 1)
    {
        snakeY = 1;
    }
    // Top side
    if (snakeY == 0 && zone[snakeY][snakeX] != 1)
    {
        snakeY = playZoneH - 1 ;
    }
}
//------------------------------
void getKey()
{
    char tempDir = ' ';
    if (kbhit()) //Keyboard Hit
    {
        int kbKey = getch();
        if (kbKey == 'A' || kbKey == 'a')
            tempDir = 'L';
        else
        if (kbKey == 'S' || kbKey == 's')
            tempDir = 'D';
        else
        if (kbKey == 'D' || kbKey == 'd')
            tempDir = 'R';
        else
        if (kbKey == 'W' || kbKey == 'w')
            tempDir = 'U';
        else
        if (kbKey == 0 || kbKey == 224)
        {
            char tempKB = getch();
            switch (tempKB)
            {
                case KEY_LEFT:
                    tempDir = 'L';
                    break;
                case KEY_DOWN:
                    tempDir = 'D';
                    break;
                case KEY_RIGHT:
                    tempDir = 'R';
                    break;
                case KEY_UP:
                    tempDir = 'U';
                    break;
            }

        }

        if (tempDir != ' ' && gameStart == false)
        {
            gameStart = true;
        }
    }

    if (
        (tempDir=='D' && direct!='U') ||
        (tempDir=='U' && direct!='D') ||
        (tempDir=='L' && direct!='R') ||
        (tempDir=='R' && direct!='L')
        )
    direct = tempDir;
    //cout << direct;
}

void snakeMove()
{
    switch (direct)
        {
        case 'L':
            snakeX--;
            break;
        case 'R':
            snakeX++;
            break;
        case 'U':
            snakeY--;
            break;
        case 'D':
            snakeY++;
            break;
        }
}
//Make snake
void saveTail ()
{
    tailX.insert(tailX.begin(), snakeX);
    tailY.insert(tailY.begin(), snakeY);
    //deleteTail
    while (tailX.size()>numTails || tailY.size()>numTails)
    {
        zone[tailY[tailY.size()-1]][tailX[tailX.size()-1]] = 0;
        tailX.pop_back();
        tailY.pop_back();
    }
}

void makeSnake()
{
    zone[snakeY][snakeX] = 2;
    for (int i = 0; i <= numTails-1; i++) // Tail
    {
        if (i>=1)
            zone[tailY[i]][tailX[i]] = 3;
        else
            zone[tailY[i]][tailX[i]] = 2;
    }
}

void deleteSnake()
{
    zone[snakeY][snakeX] = 0;
}
//Food Spawn
void foodSpawn()
{
    do
    {
        srand(snakeX + snakeY + foodX + foodY + GetTickCount()); //random seed {updated by HTML 31/7/17}
        foodX = 1 + rand() % (playZoneW-1);
        foodY = 1 + rand() % (playZoneH-1);
    }
    while (zone[foodY][foodX] !=0 || (foodX==snakeX && foodY==snakeY));

    zone[foodY][foodX] = 4; //make food
    //cout << "FoodX = " << foodX << " foodY = " << foodY << endl;
}
//---------------------------------
void makePlayZone()
{
    for (int i=0; i<5; i++)
        rectangle(scrX * unitLength - i, scrY * unitLength - i, (scrX + playZoneW -1 ) * unitLength + i, (scrY + playZoneH -1 ) * unitLength + i);
}
//--------------------------------
void showScoreBoard()
{
    for (int i=0; i<5; i++)
        rectangle( (scrX + playZoneW + 1)  * unitLength - i,
                   (scrY + 0) * unitLength - i,
                   (scrX + playZoneW + 7 ) * unitLength + i,
                   (scrY + 2) * unitLength + i);
}
//------------------------------
void showScoreValue()
{
    char Score[7] = {};
    string str;
    int temp = score;

    sprintf(Score,"%d",score);

    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(BOLD_FONT, HORIZ_DIR, 4);
    outtextxy((scrX + playZoneW + 3.6)  * unitLength + 10,
              (scrY + 1.2) * unitLength,
               Score);
}
//-------------------------------
void drawScreen()
{
    for (int i=1; i<playZoneH; i++)
    {
        for (int j=1; j<playZoneW; j++)
         {
            switch (zone[i][j])
            {
            case 0: // None
                drawBlock(scrX + j -1, scrY + i-1, SOLID_FILL, BLACK);
                break;
            case 1: // Danger block
                drawBlock(scrX + j-1, scrY + i-1, SOLID_FILL, RED);
                break;
            case 2: // Head
                drawBlock(scrX + j-1, scrY + i-1, SOLID_FILL, RGB(0,155,155));
                break;
            case 3: // Tail
                drawBlock(scrX + j-1, scrY + i-1, SOLID_FILL, GREEN);
                break;
            case 4: // Food
                drawBlock(scrX + j-1, scrY + i-1, SOLID_FILL, YELLOW);
                break;
            }
        }
    }
}

// Game Logical
void logic()
{
    safeBorderLogic();

    if (snakeX == foodX && snakeY == foodY)
    {
        score += 10; //increase score
        playSound(SOUND_EAT, 0);
        numTails++; //increase tails
        foodSpawn(); //spawn next food
    }

    if (zone[snakeY][snakeX] == 1 || zone[snakeY][snakeX] == 3)
    {
        playSound(SOUND_DEAD, 0);

        //Dead effect //// BLINKING SCREEN////////////////
        cleardevice();
        Sleep(100);
        drawScreen();
        makePlayZone();
        Sleep(300);
        cleardevice();
        Sleep(100);
        drawScreen();
        makePlayZone();
        Sleep(300);
        cleardevice();
        /////////////// END OF EFFECT ///////////////////

        Sleep(1000);
        gameOverScreen();
    }
}

//------------------------------
void init()
{
    makePlayZone();
    showScoreBoard();
    score = 0; //reset score
    tailX.clear(); //reset tailX
    tailY.clear(); //reset tailY
    tailX.insert(tailX.begin(),snakeX-1);
    tailY.insert(tailY.begin(),snakeY);
    foodSpawn(); //spawn first food
}
//--------------------------------
void cDraw()
{
    for (int i=0; i<playZoneH+1; i++)
    {
        for (int j=0; j<playZoneW+1; j++)
        {
            cout << zone[i][j];
        }
        cout << endl;
    }
}
void draw()
{
    getKey();

    if (gameStart && !gameOver)
    {
        //develop();    //debug only

        snakeMove();    //change head of snake
        makeSnake();    //change head on matrix
        drawScreen();   //draw screen

        //system("cls");
       // cDraw();
        showScoreValue();

        logic();
        deleteSnake();      //delete old head

        saveTail();         //save tail to Vector
        Sleep(gameSpeed);   //delay between each frame
        //cout << "SnakeX = " << snakeX << " SnakeY = " << snakeY << endl;
    }
}
