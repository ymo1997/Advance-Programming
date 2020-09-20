#include<iostream>
#include<string>
#include<stdio.h>
#include<cmath>
#include"bitmap_image.hpp"
using namespace std;
//some of the codes referenced from the internet.
void readMatrix(int N, int** pixel, const char* fileName){
	FILE *fp = fopen(fileName, "rb");
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			pixel[i][j] = fgetc(fp) - '0';
		}
		fgetc(fp);
	}
	fclose(fp);
}

void saveBmp(int** pixel, int N, string bmpFileName){
	bitmap_image fractal(N, N);
	fractal.clear();
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			fractal.set_pixel(i, j, hsv_colormap[pixel[i][j]]);
		}
	}
	fractal.save_image(bmpFileName);
}

 
int main() {
	int N = 1000;
	int **pixel = (int**)malloc(N * sizeof(int*));
	for(int i = 0; i < N; i++){
		pixel[i] = (int*)malloc(N * sizeof(int));
	}
	string fileName, bmpFileName;

	fileName = "julia_serial.txt";
	bmpFileName = "julia_serial.bmp";
	readMatrix(N, pixel, fileName.c_str());
	saveBmp(pixel, N, bmpFileName);

	fileName = "julia_openmd.txt";
	bmpFileName = "julia_openmd.bmp";
	readMatrix(N, pixel, fileName.c_str());
	saveBmp(pixel, N, bmpFileName);
	free(pixel);

	fileName = "julia_pthreads.txt";
	bmpFileName = "julia_pthreads.bmp";
	readMatrix(N, pixel, fileName.c_str());
	saveBmp(pixel, N, bmpFileName);
}