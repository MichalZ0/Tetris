#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <math.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

unsigned char playfield[23][12];

void border (unsigned char playfield[23][12]);
void print (unsigned char playfield[23][12], int shape[4][2]);
void fill (unsigned char playfield[23][12]);
void createBlock (unsigned char playfield[23][12], int shape[4][2]);
void movement (unsigned char playfield[23][12], int shape[4][2]);
int keyhit(char rejectKey);
void gameplay(unsigned char playfield[23][12], int shape[4][2]);
int blockPlacement(unsigned char playfield[23][12], int shape[4][2]);
int copyArray(unsigned char playfield[23][12], int copiedShape[4][2], int shape[4][2]);
int blocksInContact(int shape[4][2], int i, int CheckedY[4]);
int blocksCleared(unsigned char playfield[23][12], int shape[4][2]); 


int main()
{

    int lineShape[4][2] = {{0,1}, {0,2}, {0,3}, {0,4}}, // (y, x), where y is inversed;
        zShape[4][2] = {{0,1}, {0,2}, {1,2}, {1,3}},
        lShape[4][2] = {{0,1}, {1,1}, {1,2}, {1,3}},
        cubeShape[4][2] = {{0,1}, {0,2}, {1,1}, {1,2}},
        tShape[4][2] = {{0,2}, {1,1}, {1,2}, {1,3}},
        zInvShape[4][2] = {{0,2}, {0,3}, {1,1}, {1,2}},
        lInvShape[4][2] = {{0,3}, {1,1}, {1,2}, {1,3}};
    static int done = 0;

    int shape[4][2];

    // creating and setting up the playfield;

        if (done == 0)
        {
             fill(playfield);
            border (playfield);
            done++;

        }



    printf("\n");

    int *randomShape[7] = {lineShape, zShape, lShape, cubeShape, tShape, zInvShape, lInvShape};
    srand(time(NULL));
    int number = rand()%7;


        copyArray(playfield, randomShape[number], shape);
        createBlock(playfield, shape);

        while (1)
        {
           printf("Przechodze tu");
           if (blockPlacement(playfield, shape))
           {
                printf("TUTAJ 1\n");
                movement(playfield, shape);
                print(playfield, shape);
           }
           else
            {
                blocksCleared(playfield, shape); 
                number = rand()%7;
                copyArray(playfield, randomShape[number], shape);
                createBlock(playfield, shape);
            }
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

void print(unsigned char playfield[23][12], int shape[4][2])
{
    for (int i = 0; i < 23; i++)
    {
            for (int j = 0; j < 12; j++)
            {
                 if (i < 22 )
                 {
                    if (i == 33 && j == 33)
                        printf(ANSI_COLOR_RED "%c ", playfield[i][j], ANSI_COLOR_RESET);   
                    else
                        printf("%c ", playfield[i][j]);
                 }

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

void createBlock (unsigned char playfield[23][12], int shape[4][2])
{
    for (int i = 0; i < 4; i++)
        playfield[shape[i][0]][shape[i][1]] = 254;
}

int keyhit(char rejectKey)
{
    time_t start = time(NULL);
    char key;
    while (time(NULL) - start < 1)
    {                                                                           
       if (key = kbhit() && key != rejectKey)
            return getch();
    }
    return -1;

}

void movement (unsigned char playfield[23][12], int shape[4][2])
{
    static char rejectKey = 'o'; 
    char key = keyhit(rejectKey);
    int GoToY;

    
    int a=0, b=0; 

    int oldShapeY[4], oldShapeX[4], leftMost[4] = {-1, -1, -1, -1}, rightMost[4] = {-1, -1, -1, -1}, leftX = shape[0][1], rightX = shape[0][1];
    
    for (int i = 1; i < 4; i++)
    {
          if (rightX < shape[i][1])
            rightX = shape[i][1];

            if (leftX > shape[i][1])  
                leftX = shape[i][1];    
    }

   for (int i = 0; i < 4; i++)
   {
        if (rightX == shape[i][1])
            rightMost[i] = shape[i][0];

        if (leftX == shape[i][1])
            leftMost[i] = shape[i][0];
   }

    for (int i = 0; i< 4; i++)
    {
        printf("LeftX : %d, RightX : %d\n", leftX, rightX);
        printf("Left Most: %d\tRight Most: %d\n\n", leftMost[i], rightMost[i]);
    }


    if (key == -1 || key == 'o')
    {
        for (int i = 3; i >= 0; i--)
        {
                oldShapeY[i] = shape[i][0];
                oldShapeX[i] = shape[i][1];

                

                shape[i][0]++;
        }
    }
    if (key == 'a')
    {
        for (int i = 0; i < 4; i++)
        {
                   
            oldShapeX[i] = shape[i][1];
            oldShapeY[i] = shape[i][0];

            if (playfield[leftMost[i]][leftX-1] == 254 || playfield[leftMost[i]][leftX-1] == 179)
            {
                rejectKey = 'a'; 
                a = 1;
            }    
                 

            shape[i][1]--;
            
            
        }
    }
    if (key == 'e')
    {
        for (int i = 3; i >= 0; i--)
        {
        
            oldShapeX[i] = shape[i][1];
            oldShapeY[i] = shape[i][0];

            if (playfield[rightMost[i]][rightX+1] == 254 || playfield[rightMost[i]][rightX+1] == 179)
             {
               rejectKey = 'e'; 
               b = 1;
             }  
                
    
            shape[i][1]++;

        }
    }

    if (key == ',')
    {
        int pivot[2];
        pivot[0] = shape[2][0];
        pivot[1] = shape[2][1];

        for (int i = 3; i >= 0; i--)
        {
            float xnew = cos(1.57079633) * (shape[i][1]-pivot[1]) - sin(1.57079633) * (shape[i][0] - pivot[0]) + pivot[1];
            float ynew = sin(1.57079633) * (shape[i][1]-pivot[1]) + cos(1.57079633) * (shape[i][0] - pivot[0]) + pivot[0];

            oldShapeX[i] = shape[i][1];
            oldShapeY[i] = shape[i][0];

            shape[i][0] = ynew;
            shape[i][1] = xnew;

        }
    }

    
    if (key == ' ')
    {
         int lowestY[4]= {-1, -1, -1, -1}, highestY= -1, distance[4]={25,25,25,25}, finalDistance, YUsed[4] = {0,1,2,3};  


        for (int j = 3; j >= 0; j--)
        {
            int y = blocksInContact(shape, j, lowestY); 
            YUsed[y] = -1; 


            for (int i = 3; i >=0; i--)
                printf("lowestY: %d\n", lowestY[i]);

            printf("block Y value: %d\n", y ); 
            
            if (y != -1)
            {
                int BlockesChecked = 0; 
                
                oldShapeX[y] = shape[y][1];
                oldShapeY[y] = shape[y][0];


                printf("Shape[y][0] before increment: %d\n", shape[y][0]); 
                printf("Old Shape Y value: %d\n", oldShapeY[j]); 
                while (playfield[shape[y][0]+1][shape[y][1]] != 254 && shape[y][0] < 21 )
                {
                    printf("Shape[y][0] in loop: %d\n", shape[y][0]); 
                    shape[y][0]++;    
                }     
                
                  
                printf("Shape[y][0] value after increment: %d\n", shape[y][0]);
                distance[j] = shape[y][0] - oldShapeY[y];
                shape[y][0] = oldShapeY[y];

                for (int g = 3; g >= 0; g--)
                {
                    //printf("shape[y][0] = %d\ndistance = %d\n shape with distance = %d", shape[y][0], distance[j], shape[y][0]+distance[j]);
                    if (playfield[shape[g][0]+distance[j]][shape[g][1]] != 254 && shape[g][0]+distance[j] < 22)
                     {
                        printf("X shape with distance: %d\tY shape with distance: %d\n", shape[g][1], shape[g][0] + distance[j]);  
                        BlockesChecked++;
                        //printf("Blockes checked: %d\n", BlockesChecked); 
                     } 
                } 

                printf("Blockes Checked: %d\n", BlockesChecked); 
                printf("Distance: %d\n\n", distance[j]); 

                if (BlockesChecked < 4)
                        distance[j] = 100; 

                if (BlockesChecked == 4)
                    finalDistance = distance[j]; 
                

            
            }
                                   
         }
         printf("Final Distance: %d", finalDistance); 

         for (int i = 3; i >= 0; i--)
         { 
            printf("DIstance[%d]=%d\n", i, distance[i]); 
         }   

         for (int i = 3; i >= 0; i--)
         {
            if (finalDistance > distance[i])
            {
                
                printf("Final distance in loop: %d\tdistance in loop\n", finalDistance, distance[i]); 
                finalDistance = distance[i];
                  

            }   
             printf("Loop test\n"); 
         }

         printf("Final distance after loop: %d\n", finalDistance); 

         for (int i = 3; i >=0; i--)
         {
            if (YUsed[i] != -1)
            {
                oldShapeX[YUsed[i]] = shape[YUsed[i]][1]; 
                oldShapeY[YUsed[i]] = shape[YUsed[i]][0]; 
            }
         }
         
         for (int i = 3; i >= 0; i--)
         {
            shape[i][0]+= finalDistance;
         }

         for (int i = 3; i >= 0; i--)
        {
            printf("OldShapeX : %d\tOldShapeY : %d\n", oldShapeX[i], oldShapeY[i]);
            printf("Shape X: %d\tShape Y: %d\n", shape[i][1], shape[i][0]); 
        }


    }

    printf("a = %d b = %d", a, b);

            if (a == 1 || b == 1)
            {
                for (int i = 0; i < 4; i++)
                {
                    shape[i][0] = oldShapeY[i];
                    shape[i][1] = oldShapeX[i];
                    
                }
                return;
            }

            for (int i = 0; i < 4; i++)
            {
                playfield[oldShapeY[i]][oldShapeX[i]] = ' ';
            }


            for (int i = 0; i < 4; i++)
                playfield[shape[i][0]][shape[i][1]] = 254;

}

int blockPlacement(unsigned char playfield[23][12], int shape[4][2])
{
    int exit = 1, j = 3, numbersOfShapesInContact[4] = {-1, -1, -1, -1};
    for (int i = 3; i >= 0; i--)
    {   
        int x = blocksInContact(shape, i, numbersOfShapesInContact);
        printf("Number of block: %d\n", x); 
            
            if (x != -1)
            {
                    if ((shape[x][0])+1 == 22)
                    {
                        exit--;
                        
                    }

                if (playfield[shape[x][0]+1][shape[x][1]] == 254)
                {
                    exit--;
                }
            }
            
        
    }

    if (exit == 1)
        return 1;
    if (exit < 1)
         return 0;
}

int copyArray(unsigned char playfield[23][12], int copiedShape[4][2], int shape[4][2])
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 2; j++)
            shape[i][j] = copiedShape[i][j];
}

int blocksInContact(int shape[4][2], int i, int CheckedY[4])
{

    int LowestY = i;

    for (int k = 3; k >= 0; k--)
    {
        if (i == CheckedY[k])
            return -1;
    }

    for (int j = i-1; j >= 0; j--)
    {
        if (shape[i][1] == shape[j][1])
        {
            if (shape[LowestY][0] < shape[j][0])
            {
                 LowestY = j;
            }  
            CheckedY[j+1] = j;
            
        }
    }    
    return LowestY;  
}

int blocksCleared(unsigned char playfield[23][12], int shape[4][2])
{
    int ClearedBlocks[4] = {-1, -1, -1, -1}; 
    for (int i = 0; i < 4; i++)
    {
        
        int blocksInY=0;
        for (int j = 1; j < 11; j++)
        {
            if (playfield[shape[i][0]][j] == 254)
                blocksInY++; 
        }
        if (blocksInY == 10)
        {
            ClearedBlocks[i] = shape[i][0];
        }                       
    }

    for (int g = 0; g < 3; g++)
    {
        for (int j = g + 1; j < 4; j++)
        {
            if (ClearedBlocks[g] == ClearedBlocks[j])
                ClearedBlocks[j] = -1; 
        }
    }
    
    for (int y = 0; y < 4; y++)
    {
        for (int x = ClearedBlocks[y]; x > 0; x--)
        {
            for (int z = 1; z < 11; z++)
            {
                if (ClearedBlocks[y] != -1)
                {
                    playfield[x][z] = playfield[x-1][z];
                }
            }
        }
    }        
        for (int x = 0; x < 4; x++)
                    printf("%d", ClearedBlocks[x]);


}
        
