/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
    	perror("无法打开文件");
    	return NULL; // 返回空指针，表示失败
	}
	// 记住要动态分配内存
	Image *image1 = (Image*) malloc(sizeof(Image));
	char format[3]; //格式类型
	int range = 0;// 颜色范围
	fscanf(file,"%s",format);
	// 判断格式是否有效
	if(format[0] != 'P' || format[1] != '3') {
		printf("wrong ppm format!\n");
		return NULL;
	}
	fscanf(file, "%u %u", &image1->cols, &image1->rows);
	fscanf(file, "%u", &range);
	// 检查输入数据是否符合规范
	if(image1->rows < 0 || image1->cols < 0 || range != 255) {
		printf("wrong ppm format!\n");
		return NULL;
	}
	int totalNumOfPixels = image1->rows * image1->cols;
	// 记住必须要使用动态分配
	Color** pixels = (Color **)malloc(sizeof(Color*) * totalNumOfPixels); //储存所有像素的颜色信息
	image1->image = pixels;
	for(int i = 0 ; i < totalNumOfPixels; ++i){
		image1->image[i] = (Color *)malloc(sizeof(Color));
		Color* pixel = image1->image[i];
		fscanf(file, "%u %u %u",&pixel->R, &pixel->G, &pixel->B);
	}
	// 记得关闭文件流
	fclose(file);
	return image1;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image1)
{
	//YOUR CODE HERE
	printf("P3\n");
	printf("%d %d\n", image1->cols, image1->rows);
	printf("255\n");
	uint8_t row = image1->rows;
	uint8_t col = image1->cols;
	for(int i = 0; i < row; ++i){
		for(int j = 0 ; j < col; ++j){
			if(j != col - 1)
			printf("%3u %3u %3u   ",image1->image[i * col + j]->R,image1->image[i * col + j]->G,image1->image[i * col + j]->B);
			else
			printf("%3u %3u %3u",image1->image[i * col + j]->R,image1->image[i * col + j]->G,image1->image[i * col + j]->B);
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image1)
{
	// 分别把各个成分都free了
	int length = image1->rows * image1->cols;
	for(int i = 0 ; i < length; ++i){
		free(image1->image[i]);
	}
	free(image1->image);
	free(image1);
}
// int main() {
//     char *filename = "/home/bo/CS Leaning/CS61C/project1/fa20-proj1-starter/testInputs/JohnConway.ppm";
//     Image *image = readData(filename);

//     if (image != NULL) {
//         // 处理图像数据
//         // ...

//         // 调用写入函数
//         writeData(image);

//         // 释放内存
//         freeImage(image);
//     } else {
//         printf("读取图像数据失败\n");
//     }
// 	getchar();
//     return 0;
// }