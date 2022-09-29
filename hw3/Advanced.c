//Advanced
//add more function to the program
//Yuan Wang
//2/7/2022

#include "Constants.h"
#include "Advanced.h"

void Noise(int percentage, unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	int n = (percentage * WIDTH * HEIGHT) / 100;
	int x, y;
	int i;

	srand(0);

	for (i = 0; i < n/2; i++) {
		x = rand() % WIDTH;
		y = rand() % HEIGHT;
		R[x][y] = 255;
		G[x][y] = 255;		
		B[x][y] = 255;
	}
	for (i = 0; i < n/2; i++) {
		x = rand() % WIDTH;
		y = rand() % HEIGHT;
		R[x][y] = 0;
		G[x][y] = 0;		
		B[x][y] = 0;
	}
}

void Sharpen(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) 
{
    int R_temp[WIDTH][HEIGHT];
    int G_temp[WIDTH][HEIGHT];
    int B_temp[WIDTH][HEIGHT];
    int i,j = 0;
        //store the variable into the temp variable
    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < HEIGHT; j++) {
            R_temp[i][j] = R[i][j];
            B_temp[i][j] = B[i][j];
            G_temp[i][j] = G[i][j]; 
        }
    }
    for (i = 1; i < WIDTH-1; i++) {
        for (j = 1; j < HEIGHT-1; j++) {
            int r = 9*R_temp[i][j]-R_temp[i-1][j-1]-R_temp[i][j-1]-R_temp[i+1][j-1]-R_temp[i-1][j]-R_temp[i+1][j]-R_temp[i-1][j+1]-R_temp[i][j+1]-R_temp[i+1][j+1];
            int g = 9*G_temp[i][j]-G_temp[i-1][j-1]-G_temp[i][j-1]-G_temp[i+1][j-1]-G_temp[i-1][j]-G_temp[i+1][j]-G_temp[i-1][j+1]-G_temp[i][j+1]-G_temp[i+1][j+1];
            int b = 9*B_temp[i][j]-B_temp[i-1][j-1]-B_temp[i][j-1]-B_temp[i+1][j-1]-B_temp[i-1][j]-B_temp[i+1][j]-B_temp[i-1][j+1]-B_temp[i][j+1]-B_temp[i+1][j+1];
            if (r < 0) {
                r = 0;
            } else if (r > 255) {
                r = 255;
            }
            if (g < 0) {
                g = 0;
            } else if (g > 255) {
                g = 255;
            }
            if (b < 0) {
                b = 0;
            } else if (b > 255) {
                b = 255;
            }
            R[i][j] = r;
            G[i][j] = g; 
            B[i][j] = b;
        }
    }
}

void Posterize(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], unsigned int rbit, unsigned int gbit, unsigned int bbit)
{
	int x, y = 0;
	int r, g, b;
	for(y = 0; y < HEIGHT; y++)
	{
		for(x = 0; x < WIDTH; x++)
		{
			//set the bit to zero
			R[x][y] &= ~(1 << (rbit - 1));
			G[x][y] &= ~(1 << (gbit - 1));
			B[x][y] &= ~(1 << (bbit - 1));
			
			//make the bit that less than rbit to be 1
			for(r = 0; r < rbit - 1; r++)
			{
				R[x][y] |= 1 << r;     
			}
			for(g = 0; g < gbit - 1; g++)
			{
				G[x][y] |= 1 << g;
			}
			for(b = 0; b < bbit - 1; b++)
			{
				B[x][y] |= 1 << b;
			}
		}   
	}
}

void MotionBlur(int BlurAmount, unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	int y;
	int x;
	int b;
	int blurred;
	int blurgreen;
	int blurblue;
	for (y = 0; y < HEIGHT; y++){
    		for (x = 0; x < WIDTH; x++) {
        		for(b = 1; b < BlurAmount && b + x < WIDTH; b++){
            			blurred += R[x+b][y];
            			blurgreen += G[x+b][y];
            			blurblue += B[x+b][y];
        		}
        			blurred = blurred/(b > 0 ? b : 1);
        			blurgreen = blurgreen/(b > 0 ? b : 1);
        			blurblue = blurblue/(b > 0 ? b : 1);

       				R[x][y] = R[x][y] / 2 + blurred / 2;
       				G[x][y] = G[x][y] / 2 + blurgreen / 2;
        			B[x][y] = B[x][y] / 2 + blurblue / 2;
    		}
	}
}
