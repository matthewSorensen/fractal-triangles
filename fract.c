#include <stdio.h>
#include <stdlib.h>

#define X_SIZE 10
#define Y_SIZE 10

char* state;
char* output;


void writeState(FILE* fp){
  int i;
  for(i=0;i<X_SIZE;i++){
    fputc(state[i],fp);
    fputc(0,fp);
    fputc(0,fp);
  }
}



int main(){
  int i,j;
  FILE* img = fopen("test.ppm","w");
  state  = (char*) malloc(X_SIZE);
  output = (char*) malloc(X_SIZE*3);

  fprintf(img,"P6  %d %d 255 ",X_SIZE,Y_SIZE);
  for(i=0;i<Y_SIZE;i++){
    writeState(img);
  }
  fclose(img);
  return 0;
}
