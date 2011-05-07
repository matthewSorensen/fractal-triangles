#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define X_SIZE 400
#define Y_SIZE 400
#define writeHeader(file,x,y) fprintf((file),"P6  %d %d 255 ",(x),(y));






void allocateImageData(size_t width,char** state, char** img_buff){
  *state = (char*) malloc(sizeof(char)*width);
  *img_buff = (char*) malloc(sizeof(char)*width*3+1);
  memset(*state, 0, width);
  memset(*img_buff, 255, width*3);
}


void writeLine(size_t width, char* state, char* buff, FILE* file){
  //Create the new image buffer, into buff
  int i;
  char* dest = buff;
  for(i = 0; i < width; i++){
    int color = state[i] ? 0x000000:0xffFFff; // Where red is the least significant byte, then green, then red.
    *((int*)dest) = color;
    dest += 3;
  }
  // Now we need to write the image data to the file:
  width *= 3;
  size_t rem = width & 3;
  width -= rem;
  fwrite(buff, 4, width >> 2, file);
  fwrite(buff + width, 1, rem, file);
}


int main(){
  int i,j;

  char* state, *image;
  allocateImageData(X_SIZE, &state, &image);
 
  FILE* img = fopen("test.ppm","w");
  writeHeader(img,X_SIZE,Y_SIZE);

 
  for(i=0;i<Y_SIZE;i++){
    writeLine(X_SIZE, state, image, img);
  }
  fclose(img);
  return 0;
}
