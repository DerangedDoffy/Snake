#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX_X 40
#define MAX_Y 20
#define DEFAULT_STARTING_SIZE 3

typedef struct 
{
    COORD *body;
    int size;
    int direction;
} Snake;

typedef struct 
{
    bool running;
    int score;
} Game;

Game game;
Snake snake;
COORD fruit;

void init_console(void);
void init_game(void);   

void render(void);        
void update(void);        
void handle_input(void); 

void grow(void);
void spawn_fruit(void);


void main()
{
    init_console();
    init_game();

    while (game.running)
    {
        render();
        handle_input();
        update();
        // todo: improve game loop implementation
        Sleep(30);
    }
}

void init_console(void)
{
    // seed for random
    srand(time(NULL)); 

    // remove cursor
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);

    system("cls");
}

// Sets the game up
void init_game(void)
{   
    game = (Game) { true, 0 };
    snake = (Snake) { malloc(DEFAULT_STARTING_SIZE * sizeof(COORD)), DEFAULT_STARTING_SIZE,  rand() % 4 + 1};
    for (int i = 0; i < snake.size; i++)
        snake.body[i] = (COORD) {MAX_X/2, MAX_Y/2};
    
    spawn_fruit();
}

// Draws the game
void render(void)
{
    // Clears consle 
    HANDLE Screen;
    Screen = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position={0, 0};

    SetConsoleCursorPosition(Screen, Position);

    // Scoreboard 
    printf("\n");
    printf("Score:%d", game.score);
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
            else if (j == snake.body[0].X && i == snake.body[0].Y)
                printf("@");


            // Drawing fruit
            else if (j == fruit.X && i == fruit.Y)
                printf("$");

            // Drawing snake body
            else
            {
                int x = 0;
                for (int k = 1; k < snake.size; k++)
                {
                    if (snake.body[k].X == j && snake.body[k].Y == i && x != 1){
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

// // Updates movement, score
// // Checks eat, collision, game over
void update(void)
{
    for (int i = snake.size - 1; i > 0; i--)
    {
        snake.body[i] = snake.body[i-1];
    }

    switch(snake.direction)
    {
        case 1: //w
            snake.body[0] = (COORD) {snake.body[0].X, --snake.body[0].Y};
            break;
        case 2: //a
            snake.body[0] = (COORD) {--snake.body[0].X, snake.body[0].Y};
            break;
        case 3: //s
            snake.body[0] = (COORD) {snake.body[0].X, ++snake.body[0].Y};
            break;
        case 4: //d
            snake.body[0] = (COORD) {++snake.body[0].X, snake.body[0].Y};
            break;
        default:
            break;
    }
}

// Records key presses
void handle_input(void)
{
    if (kbhit())
    {
        switch(tolower(getch()))
        {
            case 'w': 
                if (snake.direction != 3)
                    snake.direction = 1;
                break;
            case 'a':
                if (snake.direction != 4)
                    snake.direction = 2;
                break;
            case 's':
                if (snake.direction != 1)
                    snake.direction = 3;
                break;
            case 'd':
                if (snake.direction != 2)
                    snake.direction = 4;
                break;
        }
    }
}

void grow(void)
{
    COORD* new_body = realloc(snake.body, ++snake.size * sizeof(COORD));
    if (!new_body) {
        printf("Snake isn't growing (not realloced)");
        game.running = false;
        return ;
    }
    snake.body = new_body;
}

void spawn_fruit(void)
{
    int x = rand() % (MAX_X) + 1;
    int y = rand() % (MAX_Y) + 1;
    fruit = (COORD) {x, y};
}