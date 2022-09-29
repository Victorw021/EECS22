/*********************************************************************/
/* PhotoLab.c: Assignment 2 for EECS 22, Winter 2022                 */
/*********************************************************************/
//Yuan Wang
//2022/1/24

//header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*** global definitions ***/
const int WIDTH  = 512;      /* image width */
const int HEIGHT = 288;      /* image height */
const int SLEN   =  80;      /* max. string length */

/*** function declarations ***/

/* print a menu */
void PrintMenu(void);

/* read image from a file */
int LoadImage(const char fname[SLEN], 
	      unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT], 
	      unsigned char B[WIDTH][HEIGHT]);

/* save a processed image */
int SaveImage(const char fname[SLEN], 
	      unsigned char R[WIDTH][HEIGHT], 
	      unsigned char G[WIDTH][HEIGHT], 
	      unsigned char B[WIDTH][HEIGHT]);

/* change a color image to black & white */
void BlackNWhite(unsigned char R[WIDTH][HEIGHT], 
		 unsigned char G[WIDTH][HEIGHT], 
		 unsigned char B[WIDTH][HEIGHT]);

/* reverse image color */
void Negative(unsigned char R[WIDTH][HEIGHT], 
	      unsigned char G[WIDTH][HEIGHT], 
	      unsigned char B[WIDTH][HEIGHT]);

/* color filter */
void ColorFilter(unsigned char R[WIDTH][HEIGHT], 
		 unsigned char G[WIDTH][HEIGHT],
                 unsigned char B[WIDTH][HEIGHT], 
		 int target_r, int target_g, int target_b, int threshold, 
		 int replace_r, int replace_g, int replace_b);

/* edge detection */
void Edge(unsigned char R[WIDTH][HEIGHT], 
	  unsigned char G[WIDTH][HEIGHT],
          unsigned char B[WIDTH][HEIGHT]);

/* saturated for edge */         
void saturated(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int x, int y, int r, int g, int b);

/* mirror image vertically */
void VMirror(unsigned char R[WIDTH][HEIGHT], 
	     unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT]);

/* shuffle the image */
void Shuffle(unsigned char R[WIDTH][HEIGHT], 
	     unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT]);

/* get a random number */
int Random(int max, int min);

/* add border */
void AddBorder(unsigned char R[WIDTH][HEIGHT], 
	       unsigned char G[WIDTH][HEIGHT],
               unsigned char B[WIDTH][HEIGHT], 
	       char color[SLEN], int border_width);

/* flip image vertically */
void VFlip(unsigned char R[WIDTH][HEIGHT], 
	   unsigned char G[WIDTH][HEIGHT],
           unsigned char B[WIDTH][HEIGHT]);

/* test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT], 
	      unsigned char G[WIDTH][HEIGHT], 
	      unsigned char B[WIDTH][HEIGHT]);

int main(void)
{
    /* Two dimensional arrays to hold the current image data, */
    /* one array for each color component.                    */
    unsigned char   R[WIDTH][HEIGHT];
    unsigned char   G[WIDTH][HEIGHT];
    unsigned char   B[WIDTH][HEIGHT];
	int choice;
	int target_r, target_g, target_b, threshold, replace_r, replace_g, replace_b;
	char fname[SLEN];
	int input_width;
	char color[20];
	srand(time(NULL));
	
	//loop
	while (1)
	{	
		PrintMenu();
		printf("Please make your choice: ");
		scanf(" %d", &choice);
		//go to different function based on choices user entered
		switch (choice) {
			
			case (1):
			printf("Please input the file name to load: ");
			scanf("%s", fname);
			LoadImage(fname, R, G, B); 
			break;
			
			case (2):
			SaveImage(fname, R, G, B);
			break;
			
			case (3):
			LoadImage("EngPlaza", R, G, B);
			BlackNWhite(R, G, B);
			SaveImage("bw", R, G, B);
			printf("\"Black & White\" operation is done!\n\n");
			break;
			
			case (4):
			LoadImage("EngPlaza", R, G, B);
			Negative(R, G, B);
			SaveImage("negative", R, G, B);
			printf("\"Negative\" operation is done!\n\n");
			break;
			
			case (5):
			LoadImage("EngPlaza", R, G, B);
			printf("Enter Red component for the target color: ");
			scanf("%d", &target_r);
			printf("Enter Green component for the target color: ");
			scanf("%d", &target_g);
			printf("Enter Blue component for the target color: ");
			scanf("%d", &target_b);
			printf("Enter threshold for the color difference: ");
			scanf("%d", &threshold);
			printf("Enter value for Red component in the target color: ");
			scanf("%d", &replace_r);
			printf("Enter value for Green component in the target color: ");
			scanf("%d", &replace_g);
			printf("Enter value for Blue component in the target color: ");
			scanf("%d", &replace_b);
			ColorFilter(R, G, B, target_r, target_g, target_b, threshold, replace_r, replace_g, replace_b);
			SaveImage("colorfilter", R, G, B);
			printf("\"ColorFilter\" operation is done!\n\n");
			break;
			
			case (6):
			LoadImage("EngPlaza", R, G, B);
			Edge(R, G, B);
			SaveImage("Edge", R, G, B);
			printf("\"Edge\" operation is done!\n\n");
			break;
			
			case (7):
			LoadImage("EngPlaza", R, G, B);
			Shuffle(R, G, B);
			SaveImage("shuffle", R, G, B);
			printf("\"Shuffle\" operation is done!\n\n");
			break;
			
			case (8):
			LoadImage("EngPlaza", R, G, B);
			VFlip(R, G, B);
			SaveImage("vflip", R, G, B);
			printf("\"Vflip\" operation is done!\n\n");
			break;
			
			case (9):
			LoadImage("EngPlaza", R, G, B);
			VMirror(R, G, B);
			SaveImage("vmirror", R, G, B);
			printf("\"VMirror\" operation is done!\n\n");
			break;
			
			case (10):
			printf("Enter border width: ");
			scanf("%d", &input_width);
			printf("Available border colors : black, white, red, green, blue, yellow, cyan, pink, orange\n");
			printf("Select border color from the options: ");
			scanf("%s", color);
			LoadImage("EngPlaza", R, G, B);
			AddBorder(R, G, B, color, input_width);
			SaveImage("border", R, G, B);
			printf("\"AddBorder\" operation is done!\n\n");
			break;
			
			case (11):
				AutoTest(R, G, B);
				break;
			
			case (12):
				exit(0); // end the program
			
			default:
				break;	
		}
	}

    return 0;
}

int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char Type[SLEN];
    int  Width, Height, MaxValue;
    int  x, y;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "r");
    if (!File) {
        printf("\nCannot open file \"%s\" for reading!\n", fname);
        return 1;
    }
    fscanf(File, "%79s", Type);
    if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
        printf("\nUnsupported file format!\n");
        return 2;
    }
    fscanf(File, "%d", &Width);
    if (Width != WIDTH) {
        printf("\nUnsupported image width %d!\n", Width);
        return 3;
    }
    fscanf(File, "%d", &Height);
    if (Height != HEIGHT) {
        printf("\nUnsupported image height %d!\n", Height);
        return 4;
    }
    fscanf(File, "%d", &MaxValue);
    if (MaxValue != 255) {
        printf("\nUnsupported image maximum value %d!\n", MaxValue);
        return 5;
    }
    if ('\n' != fgetc(File)) {
        printf("\nCarriage return expected!\n");
        return 6;
    }
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            R[x][y] = fgetc(File);
            G[x][y] = fgetc(File);
            B[x][y] = fgetc(File);
        }
    }
    if (ferror(File)) {
        printf("\nFile error while reading from file!\n");
        return 7;
    }
    printf("%s was read successfully!\n", fname_ext);
    fclose(File);
    return 0;
}

int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];
    char SysCmd[SLEN * 5];
    int  x, y;

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "w");
    if (!File) {
        printf("\nCannot open file \"%s\" for writing!\n", fname);
        return 1;
    }
    fprintf(File, "P6\n");
    fprintf(File, "%d %d\n", WIDTH, HEIGHT);
    fprintf(File, "255\n");

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            fputc(R[x][y], File);
            fputc(G[x][y], File);
            fputc(B[x][y], File);
        }
    }

    if (ferror(File)) {
        printf("\nFile error while writing to file!\n");
        return 2;
    }
    fclose(File);
    printf("%s was saved successfully. \n", fname_ext);

    /*
     * rename file to image.ppm, convert it to ~/public_html/<fname>.jpg
     * and make it world readable
     */
    sprintf(SysCmd, "~eecs22/bin/pnmtojpeg_hw2.tcsh %s", fname_ext);
    if (system(SysCmd) != 0) {
        printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
        return 3;
    }
    printf("%s.jpg was stored for viewing. \n", fname);

    return 0;
}

/**************************************************************/
/* Please add your function definitions here...               */
/**************************************************************/
void PrintMenu(void)
{
	printf("----------------------------------------------\n");
	printf("1: Load a PPM image\n");
	printf("2: Save the image in PPM and JPEG format\n");
	printf("3: Change the color image to Black $ White\n");	
	printf("4: Make a negative of an image\n");
	printf("5: Color filter an image\n");
 	printf("6: Sketch the edge of an image\n");
 	printf("7: Shuffle adn image\n");
 	printf("8: Flip an image vertically\n");
 	printf("9: Mirror and image vertically\n");
 	printf("10: Add Border to an image\n");
 	printf("11: Test all functions\n");
 	printf("12: Exit\n");
 	printf("----------------------------------------------\n");
}

void BlackNWhite(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	int x, y;

	for( y = 0; y < HEIGHT; y++ ) {
		for( x = 0; x < WIDTH; x++ ) {
			//all equal to (r+b+g)/3
			R[x][y] = G[x][y] = B[x][y] = (R[x][y] + G[x][y] + B[x][y]) / 3;
		}
	}
}

void Negative(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	int x, y;
	
	for( y = 0; y < HEIGHT; y++ ) {
		for( x = 0; x < WIDTH; x++ ) {
			R[x][y] = 255 - R[x][y];
			B[x][y] = 255 - B[x][y];			
			G[x][y] = 255 - G[x][y];
		}
	}
}

/* color filter */
void ColorFilter(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],unsigned char B[WIDTH][HEIGHT], int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b)
{
	int x, y;
	for( y = 0; y < HEIGHT; y++ ) {
		for( x = 0; x < WIDTH; x++ ) {
			if (((target_r - threshold) < R[x][y] && R[x][y] < (target_r + threshold)) && ((target_g - threshold) < G[x][y] && G[x][y] < (target_g + threshold)) && ((target_b - threshold) < B[x][y] && B[x][y] < (target_b + threshold))){
				R[x][y] = replace_r;
				G[x][y] = replace_g;			
				B[x][y] = replace_b;
				
				//keep it in the range of [0, 255]
				if(R[x][y] > 255)
					{ R[x][y] = 255; }
				if(R[x][y] < 0)
					{ R[x][y] = 0; }
				if(G[x][y] > 255)
					{ G[x][y] = 255; }
				if(G[x][y] < 0)
					{ G[x][y] = 0; }
				if(B[x][y] > 255)
					{ B[x][y] = 255; }
				if(B[x][y] < 0)
					{ B[x][y] = 0; }
			}
			else {
				R[x][y] = R[x][y];
				G[x][y] = G[x][y];			
				B[x][y] = B[x][y];
			}
		}
	}
	
}
		 
/* edge detection */
void Edge(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],unsigned char B[WIDTH][HEIGHT])
{
	int x, y;
	int r = 0;
	int g = 0;
	int b = 0;
	unsigned char Rtemp[WIDTH][HEIGHT];
	unsigned char Gtemp[WIDTH][HEIGHT];
	unsigned char Btemp[WIDTH][HEIGHT];
	//store into temporary variable
	for(y = 0; y < HEIGHT; y++)
	{
		for(x = 0; x < WIDTH; x++)
		{
			Rtemp[x][y] = R[x][y];
			Gtemp[x][y] = G[x][y];
			Btemp[x][y] = B[x][y];
		}
	}
	
	//sum of rgb
	for(y = 1; y < HEIGHT - 1; y++)
	{
		for(x = 1; x < WIDTH - 1; x++)
		{
			r = 8 * Rtemp[x][y] - Rtemp[x - 1][y - 1] - Rtemp[x - 1][y] - Rtemp[x - 1][y + 1] - Rtemp[x][y + 1] - Rtemp[x + 1][y + 1] - Rtemp[x + 1][y] - Rtemp[x + 1][y - 1] - Rtemp[x][y - 1];
			g = 8 * Gtemp[x][y] - Gtemp[x - 1][y - 1] - Gtemp[x - 1][y] - Gtemp[x - 1][y + 1] - Gtemp[x][y + 1] - Gtemp[x + 1][y + 1] - Gtemp[x + 1][y] - Gtemp[x + 1][y - 1] - Gtemp[x][y - 1];
			b = 8 * Btemp[x][y] - Btemp[x - 1][y - 1] - Btemp[x - 1][y] - Btemp[x - 1][y + 1] - Btemp[x][y + 1] - Btemp[x + 1][y + 1] - Btemp[x + 1][y] - Btemp[x + 1][y - 1] - Btemp[x][y - 1];
			saturated(R, G, B, r, g, b, x, y);
		}
	}
	// eight neighbouring pixel position
	for(y = 0; y < HEIGHT; y++)
	{
		for(x = 0; x < WIDTH; x++)
		{
			if(x == 0 && y == 0)
			{
				r = 3*Rtemp[x][y] - Rtemp[x + 1][y] - Rtemp[x][y - 1] - Rtemp[x + 1][y - 1];
				g = 3*Gtemp[x][y] - Gtemp[x + 1][y] - Gtemp[x][y - 1] - Gtemp[x + 1][y - 1];
				b = 3*Btemp[x][y] - Btemp[x + 1][y] - Btemp[x][y - 1] - Btemp[x + 1][y - 1];
				saturated(R, G, B, r, g, b, x, y);
			}
	
			if(x == WIDTH - 1 && y == 0)
			{
				r = 3*Rtemp[x][y] - Rtemp[x - 1][y] - Rtemp[x - 1][y - 1] - Rtemp[x][y - 1];
				g = 3*Gtemp[x][y] - Gtemp[x - 1][y] - Gtemp[x - 1][y - 1] - Gtemp[x][y - 1];
				b = 3*Btemp[x][y] - Btemp[x - 1][y] - Btemp[x - 1][y - 1] - Btemp[x][y - 1];
				saturated(R, G, B, r, g, b, x, y);
			}
	
			if(x == 0 && y == HEIGHT - 1)
			{
				r = 3*Rtemp[x][y] - Rtemp[x + 1][y] - Rtemp[x][y + 1] - Rtemp[x + 1][y + 1];
				g = 3*Gtemp[x][y] - Gtemp[x + 1][y] - Gtemp[x][y + 1] - Gtemp[x + 1][y + 1];
				b = 3*Btemp[x][y] - Btemp[x + 1][y] - Btemp[x][y + 1] - Btemp[x + 1][y + 1];
				saturated(R, G, B, r, g, b, x, y);
			}
	
			if(x == WIDTH - 1 && y == HEIGHT - 1)
			{
				r = 3*Rtemp[x][y] - Rtemp[x - 1][y] - Rtemp[x][y + 1] - Rtemp[x - 1][y + 1];
				g = 3*Gtemp[x][y] - Gtemp[x - 1][y] - Gtemp[x][y + 1] - Gtemp[x - 1][y + 1];
				b = 3*Btemp[x][y] - Btemp[x - 1][y] - Btemp[x][y + 1] - Btemp[x - 1][y + 1];
				saturated(R, G, B, r, g, b, x, y);
			}
	
			if(x == WIDTH - 1 && y != 0 && y !=  HEIGHT - 1)
			{
				r = 5*Rtemp[x][y] - Rtemp[x - 1][y - 1] - Rtemp[x][y - 1] - Rtemp[x - 1][y] - Rtemp[x - 1][y - 1] - Rtemp[x][y - 1];
				g = 5*Gtemp[x][y] - Gtemp[x - 1][y - 1] - Gtemp[x][y - 1] - Gtemp[x - 1][y] - Gtemp[x - 1][y - 1] - Gtemp[x][y - 1];
				b = 5*Btemp[x][y] - Btemp[x - 1][y - 1] - Btemp[x][y - 1] - Btemp[x - 1][y] - Btemp[x - 1][y - 1] - Btemp[x][y - 1];
				saturated(R, G, B, r, g, b, x, y);
			} 
			
			if(x != 0 && x != WIDTH - 1 && y == HEIGHT - 1)
			{
				r = 5*Rtemp[x][y] - Rtemp[x - 1][y - 1] - Rtemp[x][y - 1] - Rtemp[x + 1][y - 1] - Rtemp[x - 1][y] - Rtemp[x + 1][y];
				g = 5*Gtemp[x][y] - Gtemp[x - 1][y - 1] - Gtemp[x][y - 1] - Gtemp[x + 1][y - 1] - Gtemp[x - 1][y] - Gtemp[x + 1][y];
				b = 5*Btemp[x][y] - Btemp[x - 1][y - 1] - Btemp[x][y - 1] - Btemp[x + 1][y - 1] - Btemp[x - 1][y] - Btemp[x + 1][y];
				saturated(R, G, B, r, g, b, x, y);
			}
	
			if(x == 0 && y != 0 && y !=  HEIGHT - 1)
			{
				r = 5*Rtemp[x][y] - Rtemp[x + 1][y - 1] - Rtemp[x][y - 1] - Rtemp[x + 1][y] - Rtemp[x + 1][y - 1] - Rtemp[x][y - 1];
				g = 5*Gtemp[x][y] - Gtemp[x + 1][y - 1] - Gtemp[x][y - 1] - Gtemp[x + 1][y] - Gtemp[x + 1][y - 1] - Gtemp[x][y - 1];
				b = 5*Btemp[x][y] - Btemp[x + 1][y - 1] - Btemp[x][y - 1] - Btemp[x + 1][y] - Btemp[x + 1][y - 1] - Btemp[x][y - 1];
				saturated(R, G, B, r, g, b, x, y);
			}
	
			if(x != 0 && x != WIDTH - 1 && y == 0)
			{
				r = 5*Rtemp[x][y] - Rtemp[x - 1][y + 1] - Rtemp[x][y + 1] - Rtemp[x - 1][y] - Rtemp[x - 1][y + 1] - Rtemp[x + 1][y];
				g = 5*Gtemp[x][y] - Gtemp[x - 1][y + 1] - Gtemp[x][y + 1] - Gtemp[x - 1][y] - Gtemp[x - 1][y + 1] - Gtemp[x + 1][y];
				b = 5*Btemp[x][y] - Btemp[x - 1][y + 1] - Btemp[x][y + 1] - Btemp[x - 1][y] - Btemp[x - 1][y + 1] - Btemp[x + 1][y];
				saturated(R, G, B, r, g, b, x, y); 
			}
		}
	}
	
}

void saturated(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int r, int g, int b, int x, int y)
{
	//saturated arithmetic
	if(r > 255){
		R[x][y] = 255;
	}
	if(r < 0){
		R[x][y] = 0;
	}
	if(r <= 255 && r >=0){
		R[x][y] = r;
	}
	if(g > 255){
		G[x][y] = 255;
	}
	if(g < 0){
		G[x][y] = 0;
	}
	if(g <= 255 && g >=0){
		G[x][y] = g;
	}
	if(b > 255){
		B[x][y] = 255;
	}
	if(b < 0){
		B[x][y] = 0;
	}
	if(b <= 255 && b >=0){
		B[x][y] = b;
	}

}
			 
/* shuffle the image */
void Shuffle(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],unsigned char B[WIDTH][HEIGHT])
{
	int x, y, z;
	for(z = 0; z < 16; z++)
	{
		//get a random number
		int random = Random(15, 0); 
		for(y = 0; y < HEIGHT/4; y++)
		{
			for(x = 0; x < WIDTH/4; x++)
			{
				int swidth = WIDTH / 4; //new width of each small picture
				int sheight = HEIGHT / 4;//new height of each small picture
				int xnew = (z % 4) * swidth + x; //new x value
				int ynew = (z / 4) * sheight + y;// new y value
				int randomx = (random % 4) * swidth + x; //based on random number it generate
				int randomy = (random / 4) * sheight + y;//based on random number it generate
				int rtemp = R[xnew][ynew];
				int gtemp = G[xnew][ynew];
				int btemp = B[xnew][ynew];
			   
				R[xnew][ynew] = R[randomx][randomy];
				G[xnew][ynew] = G[randomx][randomy];
				B[xnew][ynew] = B[randomx][randomy]; 
	
				R[randomx][randomy] = rtemp;
				G[randomx][randomy] = gtemp;
				B[randomx][randomy] = btemp; 
			}
		}
	}
}

int Random(int max, int min)
{
	return (rand()%(max - min) + min);
}
			 
/* flip image vertically */
void VFlip(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],unsigned char B[WIDTH][HEIGHT])	
{
	int x, y;
	int z, w, v;
	for( y = 0; y < HEIGHT; y++ ) {
		for( x = 0; x < (WIDTH / 2); x++ ) {
			z = R[x][HEIGHT - y - 1];
			R[x][HEIGHT - y - 1] = R[x][y];
			R[x][y] = z;
			w = G[x][HEIGHT - y - 1];
			G[x][HEIGHT - y - 1] = G[x][y];
			G[x][y] = w;
			v = B[x][HEIGHT - y - 1];
			B[x][HEIGHT - y - 1] = B[x][y];
			B[x][y] = v;
		}
	}

}
		   
/* mirror image vertically */
void VMirror(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],unsigned char B[WIDTH][HEIGHT])	
{
	int x, y;
	for( y = 0; y < (HEIGHT / 2); y++ ) {
		for( x = 0; x < WIDTH; x++ ) {
			R[x][HEIGHT - y - 1] = R[x][y];
			G[x][HEIGHT - y - 1] = G[x][y];
			B[x][HEIGHT - y - 1] = B[x][y];
		}
	}

}	 

/* add border */
void AddBorder(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],unsigned char B[WIDTH][HEIGHT], char color[SLEN], int border_width)
{
	int x, y = 0;
	int r, g, b = 0;

	if(strcmp(color, "black") == 0)
	{
		r = 0;
		g = 0;
		b = 0;
	}
	else if(strcmp(color, "white") == 0)
	{
		r = 255;
		g = 255;
		b = 255;    
	}
	else if(strcmp(color, "red") == 0)
	{
		r = 255;
	}
	else if(strcmp(color, "green") == 0)
	{
		g = 255;
	}
	else if(strcmp(color, "blue") == 0)
	{
		b = 255;
	}
	else if(strcmp(color, "yellow") == 0)
	{
		r = 255;
		g = 255;
	}
	else if(strcmp(color, "cyan") == 0)
	{
		g = 255;
		b = 255;
	}
	else if(strcmp(color, "pink") == 0)
	{
		r = 255;
		g = 192;
		b = 203;
	}
	else if(strcmp(color, "orange") == 0)
	{
		r = 255;
		g = 165;
	}
	else 
	{
		return;
	}

	for(y = 0; y < HEIGHT; y++)
	{
		for(x = 0; x < WIDTH; x++)
		{
			if(x < border_width || x > (WIDTH - border_width) || y < border_width || y > (HEIGHT - border_width))
			{
				R[x][y] = r;
				G[x][y] = g;
				B[x][y] = b;
			} 
		}
	}
}

		   		   
/* test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	
	LoadImage("EngPlaza", R, G, B);
	BlackNWhite(R, G, B);
	printf("\"Black & White\" operation is done!\n");
	SaveImage("bw", R, G, B);
	printf("Black and White tested!\n\n");
	
	LoadImage("EngPlaza", R, G, B);
	Negative(R, G, B);
	printf("\"Negative\" operation is done!\n");
	SaveImage("negative", R, G, B);
	printf("Negative tested!\n\n");
	
	LoadImage("EngPlaza", R, G, B);
	ColorFilter(R, G, B, 190, 100, 150, 60, 0, 0, 255);
	printf("\"ColorFilter\" operation is done!\n");
	SaveImage("colorfilter", R, G, B);
	printf("Color Filter tested!\n\n");
	
	LoadImage("EngPlaza", R, G, B);
	Edge(R, G, B);
	printf("\"Edge\" operation is done!\n");
	SaveImage("edge", R, G, B);
	printf("Edge tested!\n\n");
	
	LoadImage("EngPlaza", R, G, B);
	Shuffle(R, G, B);
	printf("\"Shuffle\" operation is done!\n");
	SaveImage("shuffle", R, G, B);
	printf("Shuffle tested!\n\n");
	
	LoadImage("EngPlaza", R, G, B);
	VFlip(R, G, B);
	printf("\"Vflip\" operation is done!\n");
	SaveImage("vflip", R, G, B);
	printf("VFlip tested!\n\n");
	
	LoadImage("EngPlaza", R, G, B);
	VMirror(R, G, B);
	printf("\"VMirror\" operation is done!\n");
	SaveImage("vmirror", R, G, B);
	printf("VMirror tested!\n\n");
	
	LoadImage("EngPlaza", R, G, B);
	AddBorder(R, G, B, "orange" , 64);
	printf("\"AddBorder\" operation is done!\n");
	SaveImage("border", R, G, B);
	printf("AddBorder tested!\n\n");
}
		  

/* EOF */
