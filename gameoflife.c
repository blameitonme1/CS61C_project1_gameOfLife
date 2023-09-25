/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"
//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	// 方向数组
	int dir[8][2] = {{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1}};
	// 判断是否为生是根据RBG每个值分开来算的
	int countAliveR = 0;
	int countAliveG = 0;
	int countAliveB = 0;
	int isAliveR = (image->image[row * image->cols + col]->R == 255);
	int isAliveG = (image->image[row * image->cols + col]->G == 255);
	int isAliveB = (image->image[row * image->cols + col]->B == 255);
	for(int i = 0 ;i < 8 ; ++i){
		int newRow = (row + dir[i][0] + image->rows) % image->rows;
		int newCol = (col + dir[i][1] + image->cols) % image->cols;
		if(image->image[newRow * image->cols + newCol]->R == 255){
			++countAliveR;
		}
		if(image->image[newRow * image->cols + newCol]->G == 255){
			++countAliveG;
		}
		if(image->image[newRow * image->cols + newCol]->B == 255){
			++countAliveB;
		}
	}
	Color* newC = (Color* )malloc(sizeof(Color));
	// 定位bit的坐标
	int idxR = 9 * isAliveR + countAliveR;
	int idxG = 9 * isAliveG + countAliveG;
	int idxB = 9 * isAliveB + countAliveB;
	if(rule & (1 << idxR)){
		// 说明当前这个状态可以存活
		newC->R = 255;
	}
	else{
		newC->R = 0;
	}
	if(rule & (1 << idxG)){
		// 说明当前这个状态可以存活
		newC->G = 255;
	}
	else{
		newC->G = 0;
	}
	if(rule & (1 << idxB)){
		// 说明当前这个状态可以存活
		newC->B = 255;
	}
	else{
		newC->B = 0;
	}
	return newC;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image* nextGenerationImage = (Image*)malloc(sizeof(Image));
	Color** pixels = (Color**) malloc(sizeof(Color*) * image->rows * image->cols);
	nextGenerationImage->image = pixels;
	nextGenerationImage->rows = image->rows;
	nextGenerationImage->cols = image->cols;
	for(int i = 0 ; i < image->rows; ++i){
		for(int j = 0; j < image->cols; ++j){
			pixels[i * image->cols + j] = evaluateOneCell(image, i, j ,rule);
		}
	}
	return nextGenerationImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if(argc < 3) {
		printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x Life is 0x1808.");
		exit(-1);
	}
	char * filename = argv[1]; // 储存文件名称
	uint32_t rule = strtol(argv[2],NULL,16);
	Image* originImage = readData(filename);
	Image* nextGenerationImgae = life(originImage, rule);
	// 将解码之后的图像打印在stdout里面
	writeData(nextGenerationImgae);
	// 记得释放内存
	freeImage(originImage);
	freeImage(nextGenerationImgae);
}
