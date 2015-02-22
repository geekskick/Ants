//
//  main.c
//  Ants
//  BIO 2000 round 1
//
//  Created by Patrick Mintram on 20/02/2015.
//  Copyright (c) 2015 Patrick Mintram. All rights reserved.
//

#include <stdio.h>

//grid size
#define HEIGHT 11
#define WIDTH  11

//Gridsqure fillers
#define WHITE '.'
#define BLACK '*'

//Boolean terms
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

//co-ordinates
struct XY {
    int X;
    int Y;
};

//ant information
struct ant{
    struct XY real;
    struct XY temp;
    struct XY user;
    char dir;
    char symbol;
    char alive;
};

//gridsquare filler colours
struct fills{
    char old;
    char new;
};

//Function prototpyes
struct ant enterAnt(char letter);
void initGrid(char  grid[][WIDTH]);
void dispGrid(char grid[][WIDTH]);
int getMoves(char *exit_flag);
void updateUserXY(struct ant *ant);
void makeMove(struct ant *ant, char grid[][WIDTH]);
void showAntPos(struct ant *ant, char grid[][WIDTH]);
void moveDir(struct ant *ant);
void turnLeft(struct ant *ant);
void dispUserXY(struct ant *ant1,struct ant *ant2,char grid[][WIDTH]);
void isDeadTest(struct ant *ant);
void removedMessage(struct ant *ant);
void endShow(char grid[][WIDTH], struct ant *ant1, struct ant *ant2);


int main(int argc, const char * argv[]) {

    //create ants and their pointers
    struct ant ant1= enterAnt('1'), ant2 = enterAnt('2'), *pAnt1 = &ant1, *pAnt2=&ant2;

    char grid[HEIGHT][WIDTH],exit_flag = FALSE;

    // initialise grid to all dots (WHITE)
    initGrid(grid);
    
    while(1){
    //get amount of moves user wants to see
    int moves = getMoves(&exit_flag);
        
        if (exit_flag){
            break;
        }
    
    // makes the desired moves
    for (int i = 1; i <=moves;i++){
        makeMove(pAnt1,grid);
        makeMove(pAnt2,grid);
    }
    
    //show the grid and end messages
    endShow(grid, pAnt1,pAnt2);
    
    }
    
    //puts ant symbol/number in grid to show where it is at finish
    showAntPos(pAnt1,grid);
    showAntPos(pAnt2,grid);
    return 0;
}

void endShow(char grid[][WIDTH], struct ant *ant1, struct ant *ant2){
    //shows the grid
    dispGrid(grid);
    
    //shows the ant co-ordinates
    dispUserXY(ant1,ant2,grid);
    
    //says if ant is dead
    removedMessage(ant1);
    removedMessage(ant2);
}

void dispUserXY(struct ant *ant1,struct ant *ant2,char grid[][WIDTH]){
    //shows the ant co-ords in (x,y) with indexing from 1
    printf("USER:\r\nAnt 1 X: %i Y: %i Dir: %c\r\nAnt 2 X: %i Y: %i Dir: %c\r\n",ant1->user.X,ant1->user.Y,ant1->dir,ant2->user.X,ant2->user.Y,ant2->dir);
    
    //shows the ant co-ords in (x,y) with indexing from 0
    printf("ARRAY:\r\nAnt 1 X: %i Y: %i Dir: %c\r\nAnt 2 X: %i Y: %i Dir: %c\r\n",ant1->real.X,ant1->real.Y, ant1->dir,ant2->real.X,ant2->real.Y, ant2->dir);
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
    // change the 'user' co-ordinates to reflect this
    updateUserXY(ant);
}

void turnLeft(struct ant *ant){
    
    //chages direction to the left
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
    
    //changes direction to the right
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

void isDeadTest(struct ant *ant){
    
    //checks ant is still in the grid, if not kill it
    if(ant->real.Y >=HEIGHT || ant->real.Y <0 || ant->real.X >=HEIGHT || ant->real.X <0){
        ant->alive=FALSE;
    }
}

void removedMessage(struct ant *ant){
    
    //if the ant is dead, tell the user
    if (!ant->alive){
        printf("Ant %c is DEAD!\r\n",ant->symbol);
    }
}

void makeMove(struct ant *ant, char grid[][WIDTH]){
    
    struct fills fill;
    
    if(ant->alive){
    //moves the ant forward one square based on direction of travel
    moveDir(ant); //already an address so no need to use &
    }
    
    //check that the ant is on the grid still
    isDeadTest(ant);
    
    //if ant is on the grid do the colour and direction change
    if(ant->alive){
    
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
    
}

void showAntPos(struct ant *ant, char grid[][WIDTH]){
    //insert the ant's symbol to show where it is in the grid only if it's alive
    
    if(ant->alive){
        grid[ant->real.Y][ant->real.X] = ant->symbol;
    }
}

void updateUserXY(struct ant *ant){
    //makes 'array' X and Y match the users interpretation of x and y
    ant->user.Y = ant->real.Y +1;
    ant->user.X = ant->real.X +1;
}

int getMoves(char *exit_flag){
    
    //get the number of moves
    int moves = 0;

    fflush(stdin); // flush input buffer to prevent errors
    
    printf("See the result afer how many moves?\r\n");
    scanf("%i",&moves);
    
    if (moves == -1){
        *exit_flag = TRUE;
    }
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
    
    //use real X and Y for the array indexing
    ant.real.X = ant.user.X-1;
    ant.real.Y = ant.user.Y-1;
    
    //do other ant stuff
    ant.symbol = letter;
    ant.alive =TRUE;
    
    return ant;
}
