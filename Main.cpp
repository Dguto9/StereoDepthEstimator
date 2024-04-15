#include "Matrix.h"
#include "Image.h"
#include <cmath>
#include <iostream>

int searchW = 20;
int searchH = 20;

char outPath[] = "C:/Dguto9/Code/stereodepthestimator/images/output/out0.bmp";

int main(int argc, char** argv){
  searchW = searchH = atoi(argv[1]);
  Image left("C:/Dguto9/Code/stereodepthestimator/images/left.bmp");
  Image right("C:/Dguto9/Code/stereodepthestimator/images/right.bmp");

  Matrix gsLeft = (left.pixels[0]+left.pixels[1]+left.pixels[2]) * (1.0f/3.0f);
  Matrix gsRight = (right.pixels[0]+right.pixels[1]+right.pixels[2]) * (1.0f/3.0f);
  Matrix result(gsLeft.rows - (searchH*2), gsLeft.columns - (searchW*2));
  Matrix kernel((searchH*2)+1, (searchW*2)+1);
  for (int posY = searchH; posY < gsLeft.rows - searchH; posY++){
    for (int posX = searchW; posX < gsLeft.columns - searchW; posX++){
      int difference;
      float lowest;
      for (int k = posX - searchW; k < posX + searchW; k++){
        float total = 0;
        for (int i = 0; i < (searchH*2)+1; i++){  //Step through the vertical of the kernel
          for (int j = 0; j < (searchW*2)+1; j++){//Step through the horizontal of the kernel
            if (k + j - searchW >= 0 && k + j - searchW < gsRight.columns){
              kernel.data[j + (i*kernel.columns)] = gsRight.data[k + j - searchW + ((posY + i - searchH)*gsRight.columns)] - gsLeft.data[posX + j - searchW + ((posY + i - searchH)*gsLeft.columns)];
            }
            else{
              kernel.data[j + (i*kernel.columns)] = 0;
            }
            total += fabsf(kernel.data[j + (i*kernel.columns)]);
          }
        }
        if (k == 0){
          lowest = total;
        }
        if (total < lowest){
          lowest = total;
          difference = abs(posX - k);
        }
      }
      result.data[posX - searchW + ((posY - searchH)*result.columns)] = difference;
    }
  }

  float maxVal = 0;
  for (int i = 0; i < result.rows*result.columns; i++){
    maxVal = (result.data[i] > maxVal) ? result.data[i] : maxVal;
  }
  result = result * (1.0f/maxVal);
  
  Image output(result.columns, result.rows);
  
  output.pixels[0].fill(0);
  output.pixels[1].fill(0);
  output.pixels[2].fill(0);
  
  for (int i = 0; i < result.columns*result.rows; i++){
    if (result.data[i] > 0){
      output.pixels[0].data[i] = result.data[i];
    }
    if (result.data[i] < 0){
      output.pixels[2].data[i] = -result.data[i];
    }
  }

  output.saveToBMP("C:/Dguto9/Code/stereodepthestimator/images/output/out.bmp");
  
  return 0;
}