#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <time.h>

#define MAX_X 40 // 0 -> 40
#define MAX_Y 20 // 0 -> 20
#define DEFAULT_STARTING_SIZE 10

// Global variables
bool running;
COORD *snake;
int size = DEFAULT_STARTING_SIZE;
COORD fruit;
int direction;
int score;

/*
    snake 
    {
        body COORD[]
        direction int
        size int
    }
*/


// Function prototypes
void init_game(void);   
void grow(void);
void fruit_spawn(void);
void draw(void);        
void input(void);       
void update(void);        

void main()
{
    srand(time(NULL)); 
    system("cls");

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);

    init_game();
    
    // Main game loop
    int count = 0;
    while (running)
    {
        draw();
        input();
        update();
        Sleep(30);
        // count++;
        // if (count > 5)
        //     running = false;
    }
}

// Spawns fruit 
void fruit_spawn(void)
{
    int x = rand() % (MAX_X) + 1;
    int y = rand() % (MAX_Y) + 1;
    fruit = (COORD) {x, y};
}

// Sets the game up
void init_game(void)
{   
    running = true;
    // create snake
    snake = malloc(DEFAULT_STARTING_SIZE * sizeof(COORD));
    for (int i = 0; i < size; i++)
    {
        snake[i] = (COORD) {MAX_X/2, MAX_Y/2};
    }

    direction = 1; 
    score = 0;

    // Fruit spawn
    fruit_spawn();
}

void grow(void)
{
    COORD* new_snake = realloc(snake, ++size * sizeof(COORD));
    if (!new_snake) {
        printf("Snake isn't growing");
        running = false;
        return ;
    }
    snake = new_snake;
}

// Draws the game
void draw(void)
{
    // Clears consle 
    HANDLE Screen;
    Screen = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position={0, 0};

    SetConsoleCursorPosition(Screen, Position);

    // Scoreboard 
    printf("\n");
    printf("Score:%d", score);
    printf("\n");

    // Draw top wall
    for (int i = 0; i < MAX_X + 2; i++)
        printf("-");
    printf("\n");

    // Draw side walls, snake, & fruit
    for (int i = 0; i < MAX_Y + 2; i++) {
        for (int j = 0; j < MAX_X + 2; j++) {

            // Creating side wallsdddw
            if (j == 0 || j == MAX_X + 1)
                printf("#");
            
            // Drawing snake head
            else if (j == snake[0].X && i == snake[0].Y)
                printf("@");


            // Drawing fruit
            else if (j == fruit.X && i == fruit.Y)
                printf("$");

            // Drawing snake body
            else
            {
                int x = 0;
                for (int k = 1; k < size; k++)
                {
                    if (snake[k].X == j && snake[k].Y == i && x != 1){
                        printf("o");
                        x = 1;
                    }
                }
                if (x != 1)
                    printf(" ");

            }
        }
        printf("\n");
    }

    // Draw bottom wall
    for (int i = 0; i < MAX_X + 2; i++)
        printf("-");
    printf("\n");
}

// Records key presses
void input(void)
{
    if (kbhit())
    {
        switch(tolower(getch()))
        {
            case 'w': 
                if (direction != 3)
                    direction = 1;
                break;
            case 'a':
                if (direction != 4)
                    direction = 2;
                break;
            case 's':
                if (direction != 1)
                    direction = 3;
                break;
            case 'd':
                if (direction != 2)
                    direction = 4;
                break;
        }
    }
}

// // Updates movement, score
// // Checks eat, collision, game over
void update(void)
{
    for (int i = size - 1; i > 0; i--)
    {
        snake[i] = snake[i-1];
    }

    switch(direction)
    {
        case 1: //w
            snake[0] = (COORD) {snake[0].X, --snake[0].Y};
            break;
        case 2: //a
            snake[0] = (COORD) {--snake[0].X, snake[0].Y};
            break;
        case 3: //s
            snake[0] = (COORD) {snake[0].X, ++snake[0].Y};
            break;
        case 4: //d
            snake[0] = (COORD) {++snake[0].X, snake[0].Y};
            break;
        default:
            break;
    }
}