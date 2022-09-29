//Advanced.c: the advanced feature in assignment 3 and 4.

#include "Advanced.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>

IMAGE *AddNoise(IMAGE *image, int n)
{
	assert(image);
	int x, y, i;
	int noise; 
	noise = (n * ImageHeight(image) * ImageWidth(image))/100;
	srand(time(NULL));

	for (i = 0; i < noise; i++) 
		{
		x = ( (double)rand()/RAND_MAX )*(ImageWidth(image)-1);
		y = ( (double)rand()/RAND_MAX )*(ImageHeight(image)-1);
		SetPixelR(image, x, y, 255);
		SetPixelG(image, x, y, 255);
		SetPixelB(image, x, y, 255);		
	}
	return image;
}

IMAGE *Sharpen(IMAGE *image)
{
	assert(image);
	int x, y;
	int temp1, temp2;
	int	tmpr = 0;
	int	tmpg = 0;
	int	tmpb = 0;
	int	i, j;
	IMAGE *temp = CreateImage(ImageWidth(image), ImageHeight(image));  

	for (j = 0; j < ImageHeight(image); j++){
		for (i = 0; i < ImageWidth(image); i++) {
			SetPixelR(temp, i, j, GetPixelR(image, i, j));
						SetPixelG(temp, i, j, GetPixelG(image, i, j));
						SetPixelB(temp, i, j, GetPixelB(image, i, j));
		}
	}

	for (j = 0; j < ImageHeight(image); j++){
		for (i = 0; i < ImageWidth(image); i++){
			for (y = -1; y <= 1; y++){
				for (x = -1; x <= 1; x++) {
					temp1 = i + x;
					temp2 = j + y;
					if (temp1 > ImageWidth(image) - 1)
						temp1 = ImageWidth(image) - 1;
					if (temp1 < 0)
						temp1 = 0;
					if (temp2 > ImageHeight(image) - 1)
						temp2 = ImageHeight(image) - 1;
					if (temp2 < 0)
						temp2 = 0;

					if ((y==0)&&(x==0))
					{
						tmpr += 9*GetPixelR(temp, temp1, temp2); 
						tmpg += 9*GetPixelG(temp, temp1, temp2);
						tmpb += 9*GetPixelB(temp, temp1, temp2); 
					}
					else
					{
						tmpr -= GetPixelR(temp, temp1, temp2); 
						tmpg -= GetPixelG(temp, temp1, temp2);
						tmpb -= GetPixelB(temp, temp1, temp2); 
					}
				}
			}
			SetPixelR(image, i, j, (tmpr>255)? 255: (tmpr<0)? 0: tmpr);
			SetPixelG(image, i, j, (tmpg>255)? 255: (tmpg<0)? 0: tmpg);
			SetPixelB(image, i, j, (tmpb>255)? 255: (tmpb<0)? 0: tmpb);
		  	tmpr = tmpg = tmpb = 0; 	
		}
	}
	DeleteImage(temp);
	return image;
}

IMAGE *Posterize(IMAGE *image, unsigned int rbit, unsigned int gbit, unsigned int bbit)
{
	assert(image);
	int i = 0;
	int j = 0;
	int r, g, b;
	unsigned char tempr1, tempg1, tempb1;
	unsigned char tempr2, tempg2, tempb2;

	//set the bit to 0
	for(j = 0; j < ImageHeight(image); j++)
	{
		for(i = 0; i < ImageWidth(image); i++)
		{
			tempr1 = GetPixelR(image, i, j);
			tempr1 &= ~(1 << (rbit - 1));
			SetPixelR(image, i, j, tempr1);

			tempg1 = GetPixelG(image, i, j);
			tempg1 &= ~(1 << (gbit - 1));
			SetPixelG(image, i, j, tempg1);

			tempb1 = GetPixelB(image, i, j);
			tempb1 &= ~(1 << (bbit - 1));
			SetPixelB(image, i, j, tempb1);

			//make the bit that less than rbit to be 1
			for(r = 0; r < rbit - 1; r++)
			{
				tempr2 = GetPixelR(image, i, j);
				tempr2 |= 1 << r;
				SetPixelR(image, i, j, tempr2); 
			}
			//make the bit that less than gbit to be 1
			for(g = 0; g < gbit - 1; g++)
			{
				tempg2 = GetPixelG(image, i, j); 
				tempg2 |= 1 << g;
				SetPixelG(image, i, j, tempg2);
			}
			//make the bit that less than bbit to be 1
			for(b = 0; b < bbit - 1; b++)
			{
				tempb2 = GetPixelB(image, i, j);
				tempb2 |= 1 << g;
				SetPixelB(image, i, j, tempb2);
			}
		}
	}
	return image;
}

IMAGE *MotionBlur(IMAGE *image, unsigned char BlurAmount)
{
	assert(image);
	int blurred, blurgreen, blurblue;
	int i, j;
	int k;

	for (i = 0; i < ImageWidth(image); i++)
			for (j = 0; j < ImageHeight(image) ; j++) 
		{
			blurred = GetPixelR(image, i, j)/2;
			blurgreen = GetPixelG(image, i, j)/2;
			blurblue = GetPixelB(image, i, j)/2;

			for (k = 1; k <= BlurAmount ; k++)
			{
				if ((i + k) < ImageWidth(image))
				{
					blurred = blurred + GetPixelR(image, i + k, j) * 0.5 / BlurAmount;
					blurgreen = blurgreen + GetPixelG(image, i + k, j) * 0.5 / BlurAmount;
					blurblue = blurblue + GetPixelB(image, i + k, j) * 0.5 / BlurAmount;
				}
			}	
			SetPixelR(image, i, j, blurred);
			SetPixelG(image, i, j, blurgreen);
			SetPixelB(image, i, j, blurblue); 
		}	
	return image;
}

IMAGE *Crop(IMAGE *image, int x, int y, int W, int H)
{
	assert(image);
	int i, j;

	if(x + W > ImageWidth(image))
	{
		W = ImageWidth(image) - x;
	} 
	if(y + H > ImageHeight(image))
	{
		H = ImageHeight(image) - y;
	}
	IMAGE *cropped = CreateImage(W, H);

	for(i = 0; i < ImageWidth(cropped); i++)
	{
		for(j = 0; j < ImageHeight(cropped); j++)
		{
			SetPixelR(cropped, i, j, GetPixelR(image, x + i, y + j));
			SetPixelG(cropped, i, j, GetPixelG(image, x + i, y + j));
			SetPixelB(cropped, i, j, GetPixelB(image, x + i, y + j));
		}
	}
	DeleteImage(image);
	return cropped;     
}

IMAGE *Resize(IMAGE *image, int percentage)
{
	assert(image);
	int i, j;
	int original_width, original_height;
	int scaledWidth;
	int scaledHeight;
	if (percentage == 100)
	{
		return image;
	}

	scaledWidth = ImageWidth(image) * (percentage / 100.00);
	scaledHeight = ImageHeight(image) * (percentage / 100.00);

	IMAGE *scaled = CreateImage(scaledWidth, scaledHeight);
	if (percentage > 100)
	{
		for (j = 0; j < ImageHeight(scaled); j++)
		{
			for (i = 0; i < ImageWidth(scaled); i++)
			{
				original_width = (int)i * ImageWidth(image) / scaledWidth; 
				original_height = (int)j * ImageHeight(image) / scaledHeight;
				
				SetPixelR(scaled, i, j, GetPixelR(image, original_width, original_height));
				SetPixelG(scaled, i, j, GetPixelG(image, original_width, original_height));
				SetPixelB(scaled, i, j, GetPixelB(image, original_width, original_height));
			}
		}         
	}
	else
	{
		for (j = 0; j < ImageHeight(scaled); j++)
		{
			for (i = 0; i < ImageWidth(scaled); i++)
			{
				original_width = (int)i * ImageWidth(image) / scaledWidth; 
				original_height = (int)j * ImageHeight(image) / scaledHeight;
				
				SetPixelR(scaled, i, j, GetPixelR(image, original_width, original_height));
				SetPixelG(scaled, i, j, GetPixelG(image, original_width, original_height));
				SetPixelB(scaled, i, j, GetPixelB(image, original_width, original_height));			}
		}

	}
	DeleteImage(image);
	return scaled;   
}

IMAGE *BrightnessAndContrast(IMAGE *image, int brightness, int contrast)
{
	assert(image);
	int i, j;
	double factor;
	int ContrastR, ContrastG, ContrastB;
	int newpR, newpG, newpB;

	factor = (double)(259 * (contrast + 255)) / (double)(255 * (259 - contrast));
	for(j = 0; j < ImageHeight(image); j++)
	{
		for(i = 0; i < ImageWidth(image); i++)
		{
			ContrastR = (int)(factor * (GetPixelR(image, i, j) - 128) + 128);
			ContrastG = (int)(factor * (GetPixelG(image, i, j) - 128) + 128);
			ContrastB = (int)(factor * (GetPixelB(image, i, j) - 128) + 128);

			newpR = ContrastR + brightness;
			newpG = ContrastG + brightness;
			newpB = ContrastB + brightness;

			if (newpR > 255)
			{
				newpR = 255;   
			}
			else if (newpR < 0)
			{
				newpR = 0;
			}

			if (newpG > 255)
			{
				newpG = 255;
			}
			else if (newpG < 0)
			{
				newpG = 0;
			}

			if (newpB > 255)
			{
				newpB = 255;
			}
			else if (newpB < 0)
			{
				newpB = 0;
			}

			SetPixelR(image, i, j, newpR);
			SetPixelG(image, i, j, newpG);
			SetPixelB(image, i, j, newpB);
		} 
	}
	return image;
}

IMAGE *Watermark(IMAGE *image, const IMAGE *watermark_image)
{
	assert(image);
	int i, j;
	int watermarkR, watermarkG, watermarkB;
	int tempr, tempg, tempb;

	for (j = 0; j < ImageHeight(image); j++)
	{
		for (i = 0; i < ImageWidth(image); i++)
		{
			watermarkR = GetPixelR(watermark_image, i % ImageWidth(watermark_image), j % ImageHeight(watermark_image));
			watermarkG = GetPixelG(watermark_image, i % ImageWidth(watermark_image), j % ImageHeight(watermark_image));
			watermarkB = GetPixelB(watermark_image, i % ImageWidth(watermark_image), j % ImageHeight(watermark_image));
			if ((watermarkR + watermarkG + watermarkB) == 0)
			{
				tempr = GetPixelR(image, i, j) * 1.45;
				if(tempr > 255)
				{
					tempr = 255;   
				}    
				SetPixelR(image, i, j, tempr);

				tempg = GetPixelG(image, i, j) * 1.45;
				if(tempg > 255)
				{
					tempg = 255;
				}
				SetPixelG(image, i, j, tempg);

				tempb = GetPixelB(image, i, j) * 1.45; 
				if(tempb > 255)
				{
					tempb = 255;
				}
				SetPixelB(image, i, j, tempb);
			}
		}
	}
	return image;    
}	