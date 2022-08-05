#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

unsigned char playfield[23][12];

void border (unsigned char playfield[23][12]);
void print (unsigned char playfield[23][12]);
void fill (unsigned char playfield[23][12]);
void createBlock (unsigned char playfield[23][12], int size1, int size2, int shape[4][2]);
void movement (unsigned char playfield[23][12], int shape[4][2]);
int keyhit();
void gameplay(unsigned char playfield[23][12], int shape[4][2]);
void blockPlacement(unsigned char playfield[23][12], int shape[4][2]);

int main()
{
    
    int lineShape[4][2] = {{0,1}, {0,2}, {0,3}, {0,4}}, // (y, x), where y is inversed; 
        zShape[4][2] = {{0,1}, {0,2}, {1,2}, {1,3}},
        lShape[4][2] = {{1,1}, {1,2}, {1,3}, {0,1}},
        cubeShape[4][2] = {{1,1}, {1,2}, {0,1}, {0,2}},
        tShape[4][2] = {{1,1}, {1,2}, {1,3}, {0,2}},
        zInvShape[4][2] = {{1,1}, {1,2}, {0,2}, {0,3}},
        lInvShape[4][2] = {{1,1}, {1,2}, {1,3}, {0,3}};
    static int done = 0;  
    
    // creating and setting up the playfield;
    
        if (done == 0)
        {
             fill(playfield);
            border (playfield);
            done++;

        }
       
    
    
    printf("\n");

    int *randomShape[7] = {lineShape, zShape, lShape, cubeShape, tShape, zInvShape, lInvShape};
    int number = rand()%7;
    
        createBlock(playfield, 4, 2, randomShape[number]);
    
        while (1)
        {
           movement(playfield, randomShape[number]);
           print(playfield);
        }
}

void border (unsigned char playfield[23][12])
{
    for (int i = 2; i < 23; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (i < 22)
                if (j == 0 || j == 11)
                {
                    playfield[i][j] = 179; 
                }
            if (i == 22 && j > 0 && j <11)
                playfield[i][j] = 196;
                   
        }
    } 
    playfield[22][0] = 192;
    playfield[22][11] = 217;
}

void print(unsigned char playfield[23][12])
{
    for (int i = 0; i < 23; i++)
    {
            for (int j = 0; j < 12; j++)
            {
                 if (i < 22)
                    printf("%c ", playfield[i][j]);
                 if (i == 22 && j < 11)
                 {
                    printf("%c", playfield[i][j]);
                    printf("%c", 196);
                 }   
                    
            }
            if (i < 22)    
                printf("\n");
    }
    printf("%c", playfield[22][11]);
}

void fill (unsigned char playfield[23][12])
{
    for (int i = 0; i < 23; i++)
        for (int j = 0; j < 12; j++)
            playfield[i][j] = ' '; 
}

void createBlock (unsigned char playfield[23][12], int size1, int size2, int shape[4][2])
{
    for (int i = 0; i < size1; i++)  
        playfield[shape[i][0]][shape[i][1]] = 254;
}

int keyhit()
{
    time_t start = time(NULL); 
    while (time(NULL) - start < 1)
    {
       if (kbhit())
            return getch();
    }
    return -1; 
    
}

void movement (unsigned char playfield[23][12], int shape[4][2])
{
    
    char key = keyhit(); 
    

    if (key == -1 || key == 'o')
    {
        for (int i = 0; i < 4; i++)
        {
            shape[i][0]++; 
            //printf("%d %d\t%c", shape[i][0], shape[i][1], playfield[shape[i][0]][shape[i][1]]);
            
            if (playfield[shape[i][0]][shape[i][1]] == 196 || playfield[shape[i][0]][shape[i][1]] == 254)
                 main();
            
            playfield[shape[i][0]][shape[i][1]] = 254;
            playfield[shape[i][0]-1][shape[i][1]] = ' '; 
        }       
    }   
    if (key == 'a')
    {
        for (int i = 0; i < 4; i++)
        {
            shape[i][1]--;
            playfield[shape[i][0]][shape[i][1]] = 254; 
            playfield[shape[i][0]][shape[i][1]+1] = ' '; 
        }
    }
    if (key == 'e')
    {
        for (int i = 3; i >= 0; i--)
        {
            shape[i][1]++;
            playfield[shape[i][0]][shape[i][1]] = 254; 
            playfield[shape[i][0]][shape[i][1]-1] = ' '; 
        }
    }

}

void blockPlacement(unsigned char playfield[23][12], int shape[4][2])
{
    if (playfield[shape[0][0]+1][shape[0][1]] == 196)
        printf("TEST");
}