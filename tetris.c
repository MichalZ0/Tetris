#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <math.h>

unsigned char playfield[23][12];

void border (unsigned char playfield[23][12]);
void print (unsigned char playfield[23][12]);
void fill (unsigned char playfield[23][12]);
void createBlock (unsigned char playfield[23][12], int shape[4][2]);
void movement (unsigned char playfield[23][12], int shape[4][2]);
int keyhit();
void gameplay(unsigned char playfield[23][12], int shape[4][2]);
int blockPlacement(unsigned char playfield[23][12], int shape[4][2]);
int copyArray(unsigned char playfield[23][12], int copiedShape[4][2], int shape[4][2]);
int blocksInContact(int shape[4][2], int i, int CheckedY[4]);

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
                print(playfield);
           }
           else
            {
                printf("TUTAJ 2\n");
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

void createBlock (unsigned char playfield[23][12], int shape[4][2])
{
    for (int i = 0; i < 4; i++)
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

    int oldShapeY[4], oldShapeX[4];

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
            shape[i][1]--;
        }
    }
    if (key == 'e')
    {
        for (int i = 3; i >= 0; i--)
        {
            oldShapeX[i] = shape[i][1];
            oldShapeY[i] = shape[i][0];

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
        printf("i = %d\t", i);
        printf("x = %d\n", x);
            printf("Test warunkow\n");
            
            if (x != -1)
            {
                    if ((shape[x][0])+1 == 22)
                    {
                        exit--;
                        printf("Warunek 1\n");
                    }

                if (playfield[shape[x][0]+1][shape[x][1]] == 254)
                {
                    exit--;
                    printf("Warunek 2\n");
                }
            }
            
        
        //printf("x = %d\ty = %d ", shape[i][1], shape[i][0]);
        //printf("%d ", x);
    }
    // printf("Test 4\n");
    printf("Exit = %d\n", exit);

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
