#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <wchar.h>
#include <sys/time.h>


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

unsigned char playfield[23][12];

void border (unsigned char playfield[23][12]);
void print (unsigned char playfield[23][12], char *ColorShape[25], int number);
void fill (unsigned char playfield[23][12]);
void createBlock (unsigned char playfield[23][12], int shape[4][2]);
void movement (unsigned char playfield[23][12], int shape[4][2], int colorShape[4][2], int p, int z);
int keyhit(char rejectKey, time_t *current);
void gameplay(unsigned char playfield[23][12], int shape[4][2]);
int blockPlacement(unsigned char playfield[23][12], int shape[4][2]);
int copyArray(unsigned char playfield[23][12], int copiedShape[4][2], int shape[4][2]);
int blocksInContact(int shape[4][2], int i, int CheckedY[4]);
int blocksCleared(unsigned char playfield[23][12], int shape[4][2], int p);
void BlocksClearedFull(unsigned char playfield[23][12]); 

int ShapesToColor[100][3], q = 0, NumberOfColor[50];
char* ColorShape[7] = {ANSI_COLOR_BLUE, ANSI_COLOR_RED, ANSI_COLOR_CYAN, ANSI_COLOR_YELLOW, ANSI_COLOR_GREEN, ANSI_COLOR_MAGENTA, ANSI_COLOR_RED};
static int done = 0;

struct timeval stop, start, now;
unsigned curr = 0, end = 600000;

int main()
{

     printf("I'm here!\n" );
     
      HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return 0;
    }
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    DWORD dwOriginalOutMode = 0;
    DWORD dwOriginalInMode = 0;
    if (!GetConsoleMode(hOut, &dwOriginalOutMode))
    {
        return 0;
    }
    if (!GetConsoleMode(hIn, &dwOriginalInMode))
    {
        return 0;
    }

    DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

    DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
    if (!SetConsoleMode(hOut, dwOutMode))
    {
        // we failed to set both modes, try to step down mode gracefully.
        dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
        if (!SetConsoleMode(hOut, dwOutMode))
        {
            // Failed to set any VT mode, can't do anything here.
            return -1;
        }
    }

    DWORD dwInMode = dwOriginalInMode | dwRequestedInModes;
    if (!SetConsoleMode(hIn, dwInMode))
    {
        // Failed to set VT input mode, can't do anything here.
        return -1;
    }





    int lineShape[4][2] = {{0,1}, {0,2}, {0,3}, {0,4}}, // (y, x), where y is inversed;
        zShape[4][2] = {{0,1}, {0,2}, {1,2}, {1,3}},
        lShape[4][2] = {{0,1}, {1,1}, {1,2}, {1,3}},
        cubeShape[4][2] = {{0,1}, {0,2}, {1,1}, {1,2}},
        tShape[4][2] = {{0,2}, {1,1}, {1,2}, {1,3}},
        zInvShape[4][2] = {{0,2}, {0,3}, {1,1}, {1,2}},
        lInvShape[4][2] = {{0,3}, {1,1}, {1,2}, {1,3}};

    int shape[4][2];

    for (int i = 0; i < 100; i++)
    {

            ShapesToColor[i][0] = -1;
            ShapesToColor[i][1] = -1;
            ShapesToColor[i][2] = 2;

    }

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
    int p = 0, z = p + 4;





        copyArray(playfield, randomShape[number], shape);
        createBlock(playfield, shape);

        while (1)
        {
           
           printf("Przechodze tu");
           if (blockPlacement(playfield, shape))
           {
                
                printf("TUTAJ 1\n");
                movement(playfield, shape, ShapesToColor, p, z);
                system("cls"); 
                
                // for (int i = 0; i < 100; i++)
                // {
                //     printf("Shapes to Color X: %d\tShapes to Color Y: %d\t Shapes to Color Z = %d\n", ShapesToColor[i][1], ShapesToColor[i][0], ShapesToColor[i][2]);
                // }

                    print(playfield, ColorShape, number); 
                printf("P: %d", p); 
           }
           else
            {
                p = blocksCleared(playfield, shape, p);
                z = p + 4;
                
                number = rand()%7;


                    ShapesToColor[z][2] = number;
                    ShapesToColor[z+1][2] = number;
                    ShapesToColor[z+2][2] = number;
                    ShapesToColor[z+3][2] = number;


                q++;
                copyArray(playfield, randomShape[number], shape);
                createBlock(playfield, shape);
                //p = p + 4;
                

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

void print(unsigned char playfield[23][12], char *ColorShape[25], int number)
{
    for (int i = 0; i < 23; i++) // prints y
    {
            for (int j = 0; j < 12; j++) // prints x
            {

                 if (i < 22)
                 {
                        int switchBit = 1;

                        for (int k = 0; k < 100; k++)
                        {

                                if (i == ShapesToColor[k][0] && j == ShapesToColor[k][1])
                                {
                                    printf(ColorShape[ShapesToColor[k][2]]); 
                                    printf("%c ", playfield[i][j]);
                                    printf(ANSI_COLOR_RESET);
                                    switchBit = 0;
                                }

                        }

                        if (switchBit)
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

    printf("%d %d %d", playfield[1][9], playfield[1][10], playfield[1][11]); 
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

int keyhit(char rejectKey, time_t *current)
{ 
    // if (curr > end)
    // {
    //     gettimeofday(&start, NULL);
    //     gettimeofday(&now, NULL); 

    //     curr = (now.tv_sec - start.tv_sec) * 1000000 + now.tv_usec - start.tv_usec; 

    // }    
   
    char key;

    while (curr < end)
    { 
      
       if (key = kbhit())
       {
            return getch();
       }    
       gettimeofday(&now, NULL);
       curr = (now.tv_sec - start.tv_sec) * 1000000 + now.tv_usec - start.tv_usec;
    }
    return -1;

}

void movement (unsigned char playfield[23][12], int shape[4][2], int colorShape[4][2], int p, int z)
{
    static char rejectKey = 'o';
    static int blockRejected = 0; 
    int CheckThis = 0; 
    
    time_t current;

    if (curr > end)
    {
         gettimeofday(&start, NULL);
        gettimeofday(&now, NULL); 
        curr = 0;

    }   
   
    char key = keyhit(rejectKey, &current);

    printf("Current: %lf\n", current); 
    printf("Key: %c", key);

    int GoToY;


    int a=0, b=0;

    int oldShapeY[4], oldShapeX[4], leftMost[4] = {-1, -1, -1, -1}, rightMost[4] = {-1, -1, -1, -1}, leftX = shape[0][1], rightX = shape[0][1];

    if (key == 'r')
    {
        
        system("cls"); 
        done = 0;
        main(); 
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
            
            if (playfield[shape[i][0]][shape[i][1]-1] == 179 )
                CheckThis = 1;

            //printf("Shape[i][1] przed petla: %d\n", shape[i][1]); 
            

            shape[i][1]--;


        }
    }
    if (key == 'e')
    {
        for (int i = 3; i >= 0; i--)
        {

            oldShapeX[i] = shape[i][1];
            oldShapeY[i] = shape[i][0];
            
            if (playfield[shape[i][0]][shape[i][1]+1] == 179)
                CheckThis = 1;

           

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

            if (playfield[shape[i][0]][shape[i][1]] == 179 )
                CheckThis = 1; 

        }
    }


    if (key == ' ')
    {
         int lowestY[4]= {-1, -1, -1, -1}, highestY= -1, distance[4]={25,25,25,25}, finalDistance, YUsed[4] = {0,1,2,3};

        int ShapeValues[4][2]; 
        copyArray(playfield, shape, ShapeValues); 

        for (int j = 3; j >= 0; j--)
        {
            int y = blocksInContact(shape, j, lowestY);
            YUsed[y] = -1;


            for (int i = 3; i >=0; i--)
                printf("lowestY: %d\n", lowestY[i]);

            printf("block Y value: %d\n", y );

            if (y != -1)
            {
                int GoToWhileLoop = 0; 
                int BlockesChecked = 0;
                    


                oldShapeX[y] = shape[y][1];
                oldShapeY[y] = shape[y][0];


                printf("Shape[y][0] before increment: %d\n", shape[y][0]);
                printf("Old Shape Y value: %d\n", oldShapeY[j]);
                    
                    
                while ((playfield[shape[y][0]+1][shape[y][1]] != 254 || GoToWhileLoop > 0) && shape[y][0] < 21)
                {
                    printf("Shape[y][0] in loop: %d\n", shape[y][0]);
                    shape[y][0]++;
                }


                printf("Shape[y][0] value after increment: %d\n", shape[y][0]);
                distance[j] = shape[y][0] - oldShapeY[y];
                shape[y][0] = oldShapeY[y];
                GoToWhileLoop = 0; 

                for (int g = 3; g >= 0; g--)
                {
                     for (int h = 0; h < 4; h++)
                     {
                        
                        if (shape[g][0]+distance[j] == shape[h][0])
                        {
                            
                            GoToWhileLoop++; 
                            
                        }    
                     }                                                 
                    //printf("shape[y][0] = %d\ndistance = %d\n shape with distance = %d", shape[y][0], distance[j], shape[y][0]+distance[j]);
                    if ((playfield[shape[g][0]+distance[j]][shape[g][1]] != 254 || GoToWhileLoop > 0) && shape[g][0]+distance[j] < 22)
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

        //printf("Sprawdzamy czy obiekt sie naklada z innym\n"); 
        for (int i = 0; i < 4; i++)
        {  

            int ShapesNotColliding = 0;
            if (playfield[shape[i][0]][shape[i][1]] == 254)
            { 
                for (int j = 0; j < 4; j++)
                {
                    //printf("shape[%d][1] = %d, oldShapeX = %d\n", i, shape[i][1], oldShapeX[j]); 
                    if (shape[i][1] != oldShapeX[j])
                    {
                        printf("True\n"); 
                        ShapesNotColliding++; 
                    }
                }

                if (ShapesNotColliding == 4)
                    CheckThis = 1; 
            }
            
        }

        for (int i = 0; i < 4 && CheckThis == 1; i++)
        {
            shape[i][0] = oldShapeY[i]; 
            shape[i][1] = oldShapeX[i]; 
            gettimeofday(&now, NULL); 
            curr = (now.tv_sec - start.tv_sec) * 1000000 + now.tv_usec - start.tv_usec;
            if (i == 3)
                return; 
        }


            for (int i = 0; i < 4; i++)
            {
                playfield[oldShapeY[i]][oldShapeX[i]] = ' ';
            }
            


            for (int i = 0; i < 4; i++)
                playfield[shape[i][0]][shape[i][1]] = 254;




            for (int g = 0; g < 4 && p < z; g++, p++)
            {
                ShapesToColor[p][0] = shape[g][0];
                ShapesToColor[p][1] = shape[g][1];

            }

    gettimeofday(&now, NULL); 

        //blockRejected = 0; 

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

int blocksCleared(unsigned char playfield[23][12], int shape[4][2], int p)
{
    int ClearedBlocks[4] = {-1, -1, -1, -1}, LineCleared = 0;
    
        int i = 21;         
        for (i; i > 0; i--)
        {
            int blocksInY=0;
            for (int j = 1; j < 11; j++)
            {
                if (playfield[i][j] == 254)
                    blocksInY++;
            }
            
            if (blocksInY == 10)
            {
                for (int j = i; j > 0; j--)
                {
                    for (int g = 1; g < 11; g++)
                        playfield[j][g] = playfield[j-1][g]; 
                }       
        
            }

            
            for (int k = 0; k < 99 && blocksInY == 10; k++)
            {
                    
                    if (i == ShapesToColor[k][0])
                    {
                        for (int g = k; g < 99; g++)
                        {
                            ShapesToColor[g][0] = ShapesToColor[g+1][0]; 
                            ShapesToColor[g][1] = ShapesToColor[g+1][1];
                            ShapesToColor[g][2] = ShapesToColor[g+1][2];
                            //printf("K before decr: %d\n", k); 
                            //sleep(1); 
                            k--; 
                            //printf("Print k var: %d\n", k); 
                        }
                    }

            }




            for (int h = 0; h < 99 && blocksInY == 10; h++)
            {
                if (ShapesToColor[h][0] < i && ShapesToColor[h][0] != -1)
                    ShapesToColor[h][0]++; 
            }
            if (blocksInY == 10)
            {
                i = i + 1;
                LineCleared+=10;  

            }    
            printf("I wynosi: %d", i); 
        }

        for (int i = 0; i < 99 && ShapesToColor[i][0] != -1 && LineCleared > 0; i++)
        {
            p = i+1; 
        }
    
        printf("Line Cleared: %d\n", LineCleared);
        printf("P wynosi: %d\n", p);  

        if (LineCleared > 0)
            return p;
        return p+4;
}

