/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2022                   */
/*                                                                   */
/* Initial author: Tim Schmidt                                       */
/*                                                                   */
/* MovieLab.c: source file for the main function                     */
/*                                                                   */
/* Please use this template for your HW5.                            */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "FileIO.h"
#include "DIPs.h"
#include "Movie.h"
#include "Constants.h"
#include "Image.h"

/* Load one movie frame from the input file */
YUVIMAGE* LoadOneFrame(const char* fname, int n,
                       unsigned int width, unsigned height);

/* Load the movie frames from the input file */
MOVIE *LoadMovie(const char *fname, int frameNum,
                 unsigned int width, unsigned height);

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVIMAGE *image, const char *fname, FILE *file);

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie);

/* Print the command-line arguments usage of the program */
void PrintUsage();

int main(int argc, char *argv[])
{
	int x = 0;
	char *fin = NULL;
	char *fout = NULL;
                MOVIE*Movieload = NULL;
	unsigned int framecount = 0;
	unsigned int width = 512;
	unsigned int height = 288;

	/* the while loop to check options in the command line */
	while (x < argc) {
		/* the input file name */
		if (strcmp(argv[x], "-i") == 0) {
			if (x < argc - 1) {
				fin = argv[x + 1];
				printf("The movie file %s has been read successfully!\n", fin);
			} /*fi*/
			else {
				printf("Missing argument for the input file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

		/* the output file name */
		if (strcmp(argv[x], "-o") == 0) {
			if (x < argc - 1) {
				fout = argv[x + 1];
				printf("output file is %s\n", fout);
			} /*fi*/
			else {
				printf("Missing argument for the output file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

		/* the number of frames */
		if (strcmp(argv[x], "-f") == 0) {
			if (x < argc - 1) {
				if (sscanf(argv[x+1], "%u", &framecount) == 0) {
				}
			} /*fi*/
			else {
				printf("Missing argument for the number of frames to load!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/
		
		/* the image dimensions option */
		if (strcmp(argv[x], "-s") == 0) {
			if (x < argc - 1) {
				if (sscanf(argv[x+1], "%ux%u", &width, &height) == 2) {
				} 
			} /*fi*/
			else {
				printf("Missing argument for image dimensions!\n");
				return 5;
			} /*esle*/
			x += 2;	
			continue;		
		} /*fi*/


		/* aging function */
		if (strcmp(argv[x], "-aging") == 0) {
			if (!Movieload) {
				Movieload = LoadMovie(fin, framecount, width, height);
				YUV2RGBMovie(Movieload);		
			}
			IENTRY *imgEntry, *nextEntry;
			ILIST *frames = Movieload->Frames;
			imgEntry = frames->First; 
			while(imgEntry) {
				nextEntry = imgEntry->Next;
				Aging(imgEntry->RGBImage);				
				imgEntry = nextEntry;
			}
			printf("Operation Aging is done!\n");
			break;
		} /*fi*/

		/* hflip function */
		if (strcmp(argv[x], "-hflip") == 0) {
			if (!Movieload) {
				Movieload = LoadMovie(fin, framecount, width, height);
				YUV2RGBMovie(Movieload);		
			}
			IENTRY *imgEntry, *nextEntry;
			ILIST *frames = Movieload->Frames;
			imgEntry = frames->First;  
			while(imgEntry) {
				nextEntry = imgEntry->Next;
				HFlip(imgEntry->RGBImage);				
				imgEntry = nextEntry;
			}
			printf("Operation HFlip is done!\n");
			break;
		} /*fi*/
		
		/* edge function */
		if (strcmp(argv[x], "-edge") == 0) {
			if (!Movieload) {
				Movieload = LoadMovie(fin, framecount, width, height);
				YUV2RGBMovie(Movieload);		
			}
			IENTRY *imgEntry, *nextEntry;
			ILIST *frames = Movieload->Frames;
			imgEntry = frames->First; 
			while(imgEntry) {
				nextEntry = imgEntry->Next;
				Edge(imgEntry->RGBImage);				
				imgEntry = nextEntry;
			}
			printf("Operation Edge is done!\n");
			break;
		} /*fi*/

		/* crop function start - end*/
		if (strcmp(argv[x], "-crop") == 0) {
			unsigned int start, end, newFNum;
			if (!Movieload) {
				Movieload = LoadMovie(fin, framecount, width, height);
				YUV2RGBMovie(Movieload);		
			}
			if (x < argc - 1) {
				if (sscanf(argv[x+1], "%u-%u", &start, &end) == 2) {
					newFNum = (end - start) + 1; 
					CropImageList(Movieload->Frames, start, end);
					printf("Operation Crop is done! New number of frames is %u.\n", newFNum);		
				} 
			}
			else {
				printf("Missing argument for start and end!\n");
				return 5;
			} 
			break;
		} /*fi*/

		/* fast function */
		if (strcmp(argv[x], "-fast") == 0) {
			unsigned int factor;
			if (!Movieload) {
				Movieload = LoadMovie(fin, framecount, width, height);
				YUV2RGBMovie(Movieload);		
			}
			if (x < argc - 1) {
				if (sscanf(argv[x], "%u", &factor) == 0) {
					FastImageList(Movieload->Frames, factor);		
				} 
			} /*fi*/
			else {
				printf("Missing argument for start and end!\n");
				return 5;
			} 
			break;
		}/*fi*/

		/* watermark function */
		if (strcmp(argv[x], "-watermark") == 0) {
			srand(time(NULL)); 
			IMAGE *ppmF;
			if (!Movieload) {
				Movieload = LoadMovie(fin, framecount, width, height);
				YUV2RGBMovie(Movieload);		
			}
			if (x < argc - 1) {
				IENTRY *imgEntry, *nextEntry;
				ILIST *frames = Movieload->Frames;
				imgEntry = frames->First; 
				ppmF = CreateImage(imgEntry->RGBImage->W, imgEntry->RGBImage->H); 
				while(imgEntry) {
					nextEntry = imgEntry->Next;
					Watermark(imgEntry->RGBImage, ppmF, rand()%width, rand()%height);
					imgEntry = nextEntry;
				}
				printf("Operation Watermark is done!\n");
			} 
			else {
				printf("Missing argument for start and end!\n");
				return 5;
			} 
			break;
		}/*fi*/
		
		/* spotlight function */
		/*if (strcmp(argv[x], "-spotlight") == 0) {
			return 0;
		}*/
		
		/* zoom function */
		if (strcmp(argv[x], "-zoom") == 0) {
			unsigned int perc = 0;
			unsigned int tmp = 0;			
			if (!Movieload) {
				Movieload = LoadMovie(fin, framecount, width, height);
				YUV2RGBMovie(Movieload);		
			}
			IENTRY *imgEntry, *nextEntry;
			ILIST *frames = Movieload->Frames;
			imgEntry = frames->First;  
			while(imgEntry) {
				nextEntry = imgEntry->Next;
				Zoom(imgEntry->RGBImage, perc);
				if (tmp == 100 && perc != 0) {
					perc-=2;				
				} else {
					perc+=2;
					tmp+=2;
				}				
				imgEntry = nextEntry;
			}
			printf("Operation Zoom is done!\n");
			break;
		} /*fi*/

		/* the help option */
		if (strcmp(argv[x], "-h") == 0) {
			PrintUsage();
			return 0;
		} /*fi*/
		
		x++;
	} /*elihw*/

	if (fin == NULL) {
		printf("Missing argument for the input file name!\n");
		PrintUsage();
		return 5;
	}
	if (fout == NULL) {
		printf("Missing argument for the output file name!\n");
		PrintUsage();
		return 5;
	}

	RGB2YUVMovie(Movieload);
	SaveMovie(fout, Movieload);
	DeleteMovie(Movieload);	

	fin = NULL;
	fout = NULL;

	return 0;
}
void PrintUsage()
{
	printf("\nUsage: MovieLab -i <file> -o <file> -f <framenum> -s <WIDTHxHEIGHT> [options]\n"
	       "Options:\n"
	       "-aging                Activate the aging filter on every movie frame\n"
	       "-hflip                Activate horizontal flip on every movie frame\n"
	       "-edge                 Activate the edge filter on every movie frame\n"
	       "-crop <start-end>     Crop the movie frames from <start> to <end>\n"
	       "-fast <factor>        Fast forward the movie by <factor>\n"
	       "-reverse              Reverse the frame order of the input movie\n"
	       "-watermark <file>     Add a watermark from <file> to every movie frame\n"
	       "-spotlight            Spotlight the fade in and fade out\n"
	       "-zoom                 Zoom in and out the input movie\n"
	       "-h                    Display this usage information\n"
	      );
}

/* Load one movie frame from the input file */
YUVIMAGE* LoadOneFrame(const char* fname, int n,
                       unsigned int width, unsigned height)
{
	FILE *file;
	unsigned int x, y;
	unsigned char c;
	YUVIMAGE* YUVimage;

	/* Check errors */
	assert(fname);
	assert(n >= 0);

	YUVimage = CreateYUVImage(width, height);
	if (YUVimage == NULL) {
		return NULL;
	}

	/* Open the input file */
	file = fopen(fname, "r");
	if (file == NULL) {
		DeleteYUVImage(YUVimage);
		return NULL;
	}

	/* Find the desired frame */
	fseek(file, 1.5 * n * width * height, SEEK_SET);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			c = fgetc(file);
			SetPixelY(YUVimage, x, y, c);
		} /*rof*/
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelU(YUVimage, x, y, c);
			SetPixelU(YUVimage, x + 1, y, c);
			SetPixelU(YUVimage, x, y + 1, c);
			SetPixelU(YUVimage, x + 1, y + 1, c);
		}
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelV(YUVimage, x, y, c);
			SetPixelV(YUVimage, x + 1, y, c);
			SetPixelV(YUVimage, x, y + 1, c);
			SetPixelV(YUVimage, x + 1, y + 1, c);
		}
	}

	/* Check errors */
	assert(ferror(file) == 0);

	/* Close the input file and return */
	fclose(file);
	file = NULL;
	return YUVimage;
}

/* Load the movie frames from the input file */
MOVIE *LoadMovie(const char *fname, int frameNum,
                 unsigned int width, unsigned height)
{
	assert(fname);
	assert(frameNum);
	assert(width);
	assert(height);	 
	MOVIE *movie = CreateMovie();
	for (int i = 0; i < frameNum; i++) {
		AppendYUVImage(movie->Frames, LoadOneFrame(fname, i, width, height));			
	}
	return movie;
}

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie)
{
	int count;
	FILE *file;
	IENTRY *curr;

	/* Open the output file */
	file = fopen(fname, "w");
	if (file == NULL) {
		return 1;
	}
	
	count = 0;
	curr = movie->Frames->First;
	while (curr != NULL) {
		SaveOneFrame(curr->YUVImage, fname, file);
		curr = curr->Next;
		count++;
	}

	fclose(file);
	file = NULL;

	printf("The movie file %s has been written successfully!\n", fname);
	printf("%d frames are written to the file %s in total.\n", count, fname);
	return 0;
}

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVIMAGE *image, const char *fname, FILE *file)
{
	int x, y;
	for (y = 0; y < image->H; y++) {
		for (x = 0; x < image->W; x++) {
			fputc(GetPixelY(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelU(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelV(image, x, y), file);
		}
	}
}

/* EOF */
