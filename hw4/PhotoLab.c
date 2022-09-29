//PhotoLab.c
//Assignment 4
//Yuan Wang
//2022/2/21

#include <stdio.h>
#include "Constants.h"
#include "Advanced.h"
#include "DIPs.h"
#include "FileIO.h"
#include "Image.h"
#include "Test.h"

void PrintMenu();

int main() {

	#ifdef DEBUG
	int AutoTest(void);
	#else

	int ChoiceByUser;           /* user input option */
	char fname[SLEN];           /* input file name */
	char colorOption[SLEN];

	PrintMenu();
	printf("Please make your choice: ");
	scanf("%d", &ChoiceByUser);
	
	IMAGE *rc = NULL;
	IMAGE *image = NULL;      
	IMAGE *watermark = NULL;
 
	/* ColorFilter() parameters */
	int target_r, target_g, target_b, threshold;
	double factor_r, factor_g, factor_b;
	/* AddBorder() parameter */
	int border_width;
	/* Noise() parameter */
	int n;
	/* Posterization() parameter */
	unsigned int rbit, gbit, bbit;
	/* Blur() parameter */
	int bluramount;
	/* Crop() parameter */
	int cropx, cropy, cropW, cropH;
	/* Resizing() parameter */
	int percentage;
	/* BrightnessAndContrast() parameters */
	int brightness, contrast;

	while (ChoiceByUser != 20) {
		if (ChoiceByUser == 1) {
			if(image)
			{
				DeleteImage(image);
			}
			printf("Please input the file name to load: ");
			scanf("%s", fname);
			image = LoadImage(fname);
			if(image)
			{
				rc = SUCCESS;
			}
		}
		/* menu item 2 - 18 requires image is loaded first */
		else if (ChoiceByUser >= 2 && ChoiceByUser < 19) {
			if (rc != SUCCESS)   {
				printf("No image to process!\n");
			}
			/* now image is loaded */
			else {
				switch(ChoiceByUser) {
					case 2:
						printf("Please input the file name to save: ");
						scanf("%s", fname);
						SaveImage(fname, image);
						break;
					case 3:
						BlackNWhite(image);
						printf("\"Black & White\" operation is done!\n");
						break;
					case 4:
						Negative(image);
						printf("\"Negative\" operation is done!\n");
						break;
					case 5:
						printf("Enter Red   component for the target color: ");
						scanf("%d", &target_r);
						printf("Enter Green component for the target color: ");
						scanf("%d", &target_g);
						printf("Enter Blue  component for the target color: ");
						scanf("%d", &target_b);
						printf("Enter threshold for the color difference: ");
						scanf("%d", &threshold);
						printf("Enter value for Red component in the target color: ");
						scanf("%lf", &factor_r);
						printf("Enter value for Green component in the target color: ");
						scanf("%lf", &factor_g);
						printf("Enter value for Blue  component in the target color: ");
						scanf("%lf", &factor_b);

						ColorFilter(image, target_r, target_g, target_b, threshold, factor_r, factor_g, factor_b);
						printf("\"Color Filter\" operation is done!\n");
						break;
					case 6:
						Edge(image);
						printf("\"Edge\" operation is done!\n");
						break;
					case 7:
						Shuffle(image);
						printf("\"Shuffle\" operation is done!\n");
						break;
					case 8:
						VFlip(image);
						printf("\"VFlip\" operation is done!\n");
						break;
					case 9:
						VMirror(image);
						printf("\"Vertically Mirror\" operation is done!\n");
						break;
					case 10:
						printf("Enter border width:");
						scanf("%d", &border_width);
						printf("Available border colors : black, white, red, green, blue, yellow, cyan, pink, orange\n");
						printf("Select border color from the options: ");
						scanf("%s",colorOption);
						AddBorder(image, colorOption, border_width);
						printf("\"Border\" operation is done!\n");
						break;
					case 11:
						printf("Please input noise percentage: ");
						scanf("%d", &n);
						AddNoise(image, n);
						printf("\"Noise\" operation is done!\n");
						break;
					case 12:
						Sharpen(image);
						printf("\"Sharpen\" operation is done!\n");
						break;
					case 13:
						printf("Enter the number of posterization bits for R channel (1 to 8):  ");
						scanf("%d", &rbit);
						printf("Enter the number of posterization bits for G channel (1 to 8):  ");
						scanf("%d", &gbit);
						printf("Enter the number of posterization bits for B channel (1 to 8):  ");
						scanf("%d", &bbit);
						Posterize(image, rbit, gbit, bbit);
						printf("\"Posterize\" operation is done!\n");
						break;
					case 14:
						printf("Please input blur amount: ");
						scanf("%d", &bluramount);
						MotionBlur(image, bluramount);
						printf("\"Blur\" operation is done!\n");
						break;
					case 15:
						printf("Please enter the X offset value: ");
						scanf("%d", &cropx);
						printf("Please enter the Y offset value: ");
						scanf("%d", &cropy);
						printf("Please enter the crop width: ");
						scanf("%d", &cropW);
						printf("Please enter the crop height: ");
						scanf("%d", &cropH);
						image = Crop(image, cropx, cropy, cropW, cropH); 
						printf("\"Crop\" operation is done!\n");
						break;
					case 16:
						printf("Please input the resizing percentage (integer between 1 - 500): ");
						scanf("%d", &percentage);
						image = Resize(image, percentage);
						printf("\"Resize\" operation is done!\n");
						break;
					case 17:
						printf("Please input the brightness level (integer between -255 - 255): ");
						scanf("%d", &brightness);
						printf("Please input the contrast level (integer between -255 - 255): ");
						scanf("%d", &contrast);
						BrightnessAndContrast(image, brightness, contrast);
						printf("\"Brightness and Contrast Adjumustment\" operation is done!\n");
						break;
					case 18:
						watermark = LoadImage("watermark_template");
						if(watermark)
						{
							Watermark(image, watermark);
							printf("\"Watermark\" operation is done!\n");
							DeleteImage(watermark);
						}
						else
						{
							printf("watermark could not be loaded");
						}
						break;
					default:
						break;
				}
			}
		}
		else if (ChoiceByUser == 19) {
			int AutoTest(void);
			printf("AutoTest finished successfully.\n");
			/*rc = SUCCESS;*/
		}
		else {
			printf("Invalid selection!\n");
		}

		/* Process finished, waiting for another input */
		PrintMenu();
		printf("Please make your choice: ");
		scanf("%d", &ChoiceByUser);
	}
	if(image)
	{
		DeleteImage(image);
	}

	printf("You exit the program.\n");
	#endif
	return 0;
}

void PrintMenu() {
	printf("\n----------------------------\n");
	printf(" 1: Load a PPM image\n");
	printf(" 2: Save an image in PPM and JPEG format\n");
	printf(" 3: Change a color image to Black & White\n");
	printf(" 4: Make a negative of an image\n");
	printf(" 5: Color filter an image\n");
	printf(" 6: Sketch the edge of an image\n");
	printf(" 7: Shuffle an image\n");
	printf(" 8: Flip an image vertically\n");
	printf(" 9: Mirror an image vertically\n");
	printf("10: Add border to an image\n");
	printf("11: Add Noise to an image\n");
	printf("12: Sharpen the image\n");
	printf("13: Posterize the image\n");
	printf("14: Blur the image\n");
	printf("15: Crop an image\n");
	printf("16: Resize an image\n");
	printf("17: Adjust the Brightness and Contrast of an image\n");
	printf("18: Add Watermark to an image\n");
	printf("19: Test all functions\n");
	printf("20: Exit\n");
}