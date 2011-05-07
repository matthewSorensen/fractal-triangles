#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define X_SIZE 10000
#define Y_SIZE 5000
#define writeHeader(file,x,y) fprintf((file),"P6  %d %d 255 ",(x),(y));






void allocateImageData(size_t width,char** state, char** img_buff){
  *state = (char*) malloc(sizeof(char)*width+3);
  *img_buff = (char*) malloc(sizeof(char)*width*3+1);
  memset(*state, 0, width+3);
  memset(*img_buff, 255, width*3);
}



#define RGB(r,g,b) ((b<<16)|(g<<8)|r)

int colors[16] = {RGB(255,255,255),RGB(34,8,118),RGB(35,9,122),RGB(37,10,126),
		  RGB(39,10,130),RGB(40,11,134),RGB(42,12,138),RGB(44,13,142),
		  RGB(45,14,147),RGB(47,15,151),RGB(49,16,155),RGB(50,17,159),
		  RGB(52,17,163),RGB(54,18,167),RGB(55,19,171),RGB(57,20,175)};


void writeLine(size_t width, char* state, char* buff, FILE* file){
  //Create the new image buffer, into buff
  int i;
  char* dest = buff;
  for(i = 0; i < width; i++){
    *((int*)dest) = colors[state[i]&15];
    dest += 3;
  }
  // Now we need to write the image data to the file:
  width *= 3;
  size_t rem = width & 3;
  width -= rem;
  fwrite(buff, 4, width >> 2, file);
  fwrite(buff + width, 1, rem, file);
}

void impulse(size_t width,char* state,int type){
  state[(width>>type)-1] = 1;
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
    char new = tmp + old + state[i+1];
    if(modulo <= new){
      new -= modulo;
      if(modulo <= new)
	new -= modulo;
    }
    state[i] = new;
    old = tmp;
  }
}

void quartic(size_t width, char* state, char modulo){
  int i;
  for(i=0;i<width;i++){
    char new = state[i];
    int j;
    for(j=1; j<4;j++){
      new += state[i+j];
      if(modulo <= new)
	new -= modulo;
    }
    state[i] = new;;
  }
}


void (*triangles[3])(size_t,char*,char) = {binomial,trinomial,quartic};


int main(){
  int i,degree;
  char* state, *image;
  void (*update)(size_t,char*,char);
  FILE* img;


  degree = 4;
  update = triangles[degree-2];

  allocateImageData(X_SIZE, &state, &image);
  impulse(X_SIZE, state, degree & 1);

  img = fopen("test.ppm","w");
  writeHeader(img,X_SIZE,Y_SIZE);
  for(i=0;i<Y_SIZE;i++){
    writeLine(X_SIZE, state, image, img);
    update(X_SIZE, state,32);
  }
  fclose(img);

  return 0;
}
