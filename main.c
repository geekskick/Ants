//
//  main.c
//  Ants
//  BIO 2000 round 1
//
//  Created by Patrick Mintram on 20/02/2015.
//  Copyright (c) 2015 Patrick Mintram. All rights reserved.
//

#include <stdio.h>

#define HEIGHT 11
#define WIDTH  11
#define WHITE '.'
#define BLACK '*'
#define TRUE 1
#define FALSE 0

// macros do stuff, used for debugging in this case
// size of the array in bytes/size of elements in bytes = elements
#define N_COLS(x) (sizeof(x[0])/sizeof(x[0][0]))
#define N_ROWS(x) (sizeof(x)/sizeof(x[0]))
#define N_ELEMS(x) (sizeof(x)/sizeof(x[0]))

//display columns and rows
#define sizeShow(x) { \
            printf("Number of columns in grid is %lu\r\n", N_COLS(grid)); \
            printf("Number of rows in grid is %lu\r\n", N_ROWS(grid));\
}

struct XY {
    int X;
    int Y;
};

struct ant{
    struct XY real;
    struct XY temp;
    struct XY user;
    char dir;
    char symbol;
    char alive;
};

struct fills{
    char old;
    char new;
};

//Function prototpyes
struct ant enterAnt(char letter);
void initGrid(char  grid[][WIDTH]);
void dispGrid(char grid[][WIDTH]);
int getMoves(void);
void updateUserXY(struct ant *ant);
void makeMove(struct ant *ant, char grid[][WIDTH]);
void showAntPos(struct ant *ant, char grid[][WIDTH]);
void moveDir(struct ant *ant);
void turnLeft(struct ant *ant);
void dispUserXY(struct ant *ant1,struct ant *ant2,char grid[][WIDTH]);


//int counter = 0;

int main(int argc, const char * argv[]) {

    //create ants
    struct ant ant1= enterAnt('1'), ant2 = enterAnt('2'), *pAnt1 = &ant1, *pAnt2=&ant2;
    
    //colours of grid squares
    //struct fills fill ={WHITE,BLACK},*pFill = &fill;
    
    //creat grid
    char grid[HEIGHT][WIDTH];
    
    //get amount of moves user wants to see
    int moves = getMoves();

    // initialise grid to all dots
    initGrid(grid);
    
    for (int i = 1; i <=moves;i++){
        makeMove(pAnt1,grid);
        makeMove(pAnt2,grid);
    }
    
    dispGrid(grid);
    dispUserXY(pAnt1,pAnt2,grid);
    return 0;
}

void dispUserXY(struct ant *ant1,struct ant *ant2,char grid[][WIDTH]){
    printf("USER:\r\nAnt 1 X: %i Y: %i\r\nAnt 2 X: %i Y: %i\r\n",ant1->user.X,ant1->user.Y,ant2->user.X,ant2->user.Y);
    printf("ARRAY:\r\nAnt 1 X: %i Y: %i\r\nAnt 2 X: %i Y: %i\r\n",ant1->real.X,ant1->real.Y,ant2->real.X,ant2->real.Y);
    //printf("COUNTER: %i\r\n", counter);

}

void moveDir(struct ant *ant){
    //make new ant co-ordinates from the direction
    switch(ant->dir){
        case 'T':
            ant->real.Y+=1;
            break;
        case 'B':
            ant->real.Y -=1;
            break;
        case 'R':
            ant->real.X+=1;
            break;
        case 'L':
            ant->real.X -=1;
            break;
    }

    updateUserXY(ant);
}

void turnLeft(struct ant *ant){
    switch(ant->dir){
        case 'T':
            ant->dir = 'L';
            break;
        case 'L':
            ant->dir = 'B';
            break;
        case 'B':
            ant->dir = 'R';
            break;
        case 'R':
            ant->dir = 'T';
            break;
    }
}

void turnRight(struct ant *ant){
    switch(ant->dir){
        case 'T':
            ant->dir = 'R';
            break;
        case 'R':
            ant->dir = 'B';
            break;
        case 'B':
            ant->dir = 'L';
            break;
        case 'L':
            ant->dir = 'T';
            break;
    }
}


void makeMove(struct ant *ant, char grid[][WIDTH]){
    
    struct fills fill;
    
    //moves the ant forward one square based on direction of travel
    moveDir(ant); //already an address so no need to use &

    //create old and new filler colours
    fill.old = grid[ant->real.Y][ant->real.X];
    if (fill.old == WHITE){
        fill.new = BLACK;
    }
    else {
        fill.new = WHITE;
    }
    
    //change the ant direction then change colour of square
    
    if(fill.old == BLACK){
        //if it was a black square turn the ant left
        grid[ant->real.Y][ant->real.X] = fill.new;
        turnLeft(ant);
    }

    else if (fill.old == WHITE){
        //if it was a white square turn the ant right
        grid[ant->real.Y][ant->real.X] = fill.new;
        turnRight(ant);
    }
    
    else{
        printf("Fill error");
        getchar();
    
    }
    
}

void showAntPos(struct ant *ant, char grid[][WIDTH]){
    //insert the ant's symbol to show where it is in the grid
    grid[ant->real.Y][ant->real.X] = ant->symbol;
}

void updateUserXY(struct ant *ant){
    //makes 'array' X and Y match the users interpretation of x and y
    ant->user.Y = ant->real.Y +1;
    ant->user.X = ant->real.X +1;
}

int getMoves(void){
    //get the number of moves
    int moves = 0;
    fflush(stdin);
    printf("See the result afer how many moves?\r\n");
    scanf("%i",&moves);
    return moves;
}

void initGrid(char grid[][WIDTH]){
    
    //initialise grid to dots
    for(int i=0;i<HEIGHT; i++){
        for (int j = 0; j <WIDTH; j++){
            grid[i][j] = '.';
        }
    }
    
}

void dispGrid(char grid[][WIDTH]){

    //display grid by looping through columns and rows
    for(int i = HEIGHT-1; i>=0; i--){
        for (int j = 0; j <WIDTH; j++){
            printf("%c",grid[i][j]);
        }
        
        //Display line number
        printf("   Line %i\r\n",i+1);
    }

    //Seperator between grids
    printf("===========\r\n");
    
}

struct ant enterAnt(char letter){
    struct ant ant;
    
    //ensures a valid user input
    do {
    printf("Enter Ant details in the following format: X Y Direction \r\n");
    printf("Directions: T B L R\r\n");
    scanf("%i %i %c",&ant.user.X, &ant.user.Y, &ant.dir);
    } while( (ant.dir!= 'T' && ant.dir!='B' && ant.dir!='L' && ant.dir!='R') || (ant.user.X>WIDTH && ant.user.X <1)|| (ant.user.Y>HEIGHT && ant.user.Y <1));
    
    //flush input buffer to prevent errors in future functions
    fflush(stdin);
    
    //use realX and Y for the array indexing
    ant.real.X = ant.user.X-1;
    ant.real.Y = ant.user.Y-1;
    ant.symbol = letter;
    ant.alive =TRUE;
    return ant;
}
