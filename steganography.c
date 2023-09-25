/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color* newC = (Color *)malloc(sizeof(Color));
	// 得到B的颜色
	uint8_t bVal = image->image[row * image->cols + col]->B;
	int LSB = bVal & 1;
	if(LSB == 1){
		// 白色
		newC->R = newC->G = newC->B = 255;
	}
	else{
		newC->R = newC->G = newC->B = 0;
	}
	return newC;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	// 分配内存
	Image* img = (Image*)malloc(sizeof(Image));
	img->rows = image->rows;
	img->cols = image->cols;
	Color* pixels = (Color*) malloc(sizeof(Color *) * image->rows * image->cols);
	img->image = pixels;
	for(int i = 0 ; i < image->rows; ++i){
		for(int j = 0 ; j < image->cols; ++j){
			img->image[i * img->cols + j] = evaluateOnePixel(image, i, j);
		}
	}
	return img;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	char * filename = argv[1]; // 储存文件名称
	Image* originImage = readData(filename);
	Image* decodedImage = steganography(originImage);
	// 将解码之后的图像打印在stdout里面
	writeData(decodedImage);
	// 记得释放内存
	freeImage(originImage);
	freeImage(decodedImage);
}
