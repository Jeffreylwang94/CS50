#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int maxCapper(long comparison);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float sum;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            sum =image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            sum = round(sum / 3);
            image[i][j].rgbtBlue = sum;
            image[i][j].rgbtRed = sum;
            image[i][j].rgbtGreen = sum;
        }
    }
    return;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    long reflectRed[width];
    long reflectBlue[width];
    long reflectGreen[width];
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            reflectRed[j]=image[i][j].rgbtRed;
            reflectBlue[j]=image[i][j].rgbtBlue;
            reflectGreen[j]=image[i][j].rgbtGreen;

        }
        for(int k = 0; k<width;k++){
            image[i][width-k-1].rgbtRed=reflectRed[k];
            image[i][width-k-1].rgbtBlue=reflectBlue[k];
            image[i][width-k-1].rgbtGreen=reflectGreen[k];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float totalSumRed;
    float totalSumGreen;
    float totalSumBlue;
    int validPixels=0;
    RGBTRIPLE outputImage[height][width];
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            for(int k = -1; k <= 1; k++ ){
                for(int l = -1; l <= 1; l++){
                    if(i+k<0 || i+k>height-1 || j+l<0 || j+l > width-1){
                    } else {
                        totalSumRed=totalSumRed+image[i+k][j+l].rgbtRed;
                        totalSumBlue=totalSumBlue+image[i+k][j+l].rgbtBlue;
                        totalSumGreen=totalSumGreen+image[i+k][j+l].rgbtGreen;
                        validPixels++;
                    }
                }
            }
            outputImage[i][j].rgbtRed=round(totalSumRed/validPixels);
            outputImage[i][j].rgbtBlue=round(totalSumBlue/validPixels);
            outputImage[i][j].rgbtGreen=round(totalSumGreen/validPixels);
            validPixels=0;
            totalSumRed=0;
            totalSumBlue=0;
            totalSumGreen=0;
        }
    }

    for(int i = 0; i < height; i++){
        for(int j =0; j<width;j++){
            image[i][j]=outputImage[i][j];
        }
    }
    return;
}



// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    double totalSumRedGx, totalSumRedGy, totalSumGreenGx, totalSumGreenGy, totalSumBlueGx, totalSumBlueGy;

    int gxArray[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    int gyArray[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};

    RGBTRIPLE(*framedImage)[width] = calloc(height, width * sizeof(RGBTRIPLE));
   // RGBTRIPLE framedImage[height][width];
//    printf("%lu\n",sizeof(image));

    for(int i = 0; i < height; i++){
        for(int j =0; j< width;j++){
            framedImage[i][j]=image[i][j];
        }
    }


    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){


            for(int k = -1; k <= 1; k++ ){
                for(int l = -1; l <= 1; l++){
                    if( ((i+k) < 0) || ((i+k) > (height-1)) || ((j+l < 0)) || ((j+l) > (width-1))){
                    } else {
                        totalSumRedGx=totalSumRedGx+(image[i+k][j+l].rgbtRed * gxArray[k+1][l+1]);
                        totalSumRedGy=totalSumRedGy+(image[i+k][j+l].rgbtRed * gyArray[k+1][l+1]);
                        totalSumGreenGx=totalSumGreenGx+(image[i+k][j+l].rgbtGreen * gxArray[k+1][l+1]);
                        totalSumGreenGy=totalSumGreenGy+(image[i+k][j+l].rgbtGreen * gyArray[k+1][l+1]);
                        totalSumBlueGx=totalSumBlueGx+(image[i+k][j+l].rgbtBlue * gxArray[k+1][l+1]);
                        totalSumBlueGy=totalSumBlueGy+(image[i+k][j+l].rgbtBlue * gyArray[k+1][l+1]);
                    }
                }
 //               printf("\n");
            }
 //           printf("\n");
 //           printf("\n");
 //           if(totalSumRedGx>0){
//                printf("totalSumRedGx: %f   totalSumRedGy: %f   \n",totalSumRedGx,totalSumRedGy);
//            }

            double maxValRed =round(sqrt((totalSumRedGx*totalSumRedGx)+(totalSumRedGy*totalSumRedGy)));
            double maxValBlue =round(sqrt((totalSumBlueGx*totalSumBlueGx)+(totalSumBlueGy*totalSumBlueGy)));
            double maxValGreen=round(sqrt((totalSumGreenGx*totalSumGreenGx)+(totalSumGreenGy*totalSumGreenGy)));


            framedImage[i][j].rgbtRed=maxCapper(maxValRed);
            framedImage[i][j].rgbtBlue=maxCapper(maxValBlue);
            framedImage[i][j].rgbtGreen=maxCapper(maxValGreen);

            totalSumRedGx=0;
            totalSumBlueGx=0;
            totalSumGreenGx=0;
            totalSumRedGy=0;
            totalSumBlueGy=0;
            totalSumGreenGy=0;

        }
    }

    for(int i = 0; i < height; i++){
        for(int j =0; j< width;j++){
            image[i][j]=framedImage[i][j];
        }
    }

    free(framedImage);
    return;
}

int maxCapper(long comparison){
    if(comparison>255){
        return 255;
    }
    return comparison;
}

