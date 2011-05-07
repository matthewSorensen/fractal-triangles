#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define X_SIZE 4000
#define Y_SIZE 4000
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


void impulse(size_t width,char* state){
  state[(width>>1)-1] = 1;
}


void binomial(size_t width, char* state, char modulo){
  int i;
  for(i=0;i<width;i++){
    char new = state[i]+state[i+1];
    state[i] = modulo <= new? new-modulo:new;
  }
}


void trinomial (size_t width, char* state, char modulo){
  char old=0;
  int i;
  for(i=0;i<width;i++){
    char tmp = state[i];
    state[i] = (tmp + old + state[i+1]) & 15;
    old = tmp;
  }
}



int main(){
  int i,j;

  char* state, *image;
  allocateImageData(X_SIZE, &state, &image);
  impulse(X_SIZE, state);


  FILE* img = fopen("test.ppm","w");
  writeHeader(img,X_SIZE,Y_SIZE);

 
  for(i=0;i<Y_SIZE;i++){
    writeLine(X_SIZE, state, image, img);
    trinomial(X_SIZE, state,16);
  }
  fclose(img);
  return 0;
}
