#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define writeHeader(file,x,y) fprintf((file),"P6  %d %d 255 ",(x),(y));

void allocateImageData(size_t width,unsigned char** state, unsigned char** img_buff){
  *state = (char*) malloc(sizeof(char)*width+3);
  *img_buff = (char*) malloc(sizeof(char)*width*3+1);
  memset(*state, 0, width+3);
  memset(*img_buff, 255, width*3);
}



#define RGB(r,g,b) ((b<<16)|(g<<8)|r)



/*
// Uncomment this for an incomplete method for coloring pixels.
int colors[16] = {RGB(255,255,255),RGB(34,8,118),RGB(35,9,122),RGB(37,10,126),
		  RGB(39,10,130),RGB(40,11,134),RGB(42,12,138),RGB(44,13,142),
		  RGB(45,14,147),RGB(47,15,151),RGB(49,16,155),RGB(50,17,159),
		  RGB(52,17,163),RGB(54,18,167),RGB(55,19,171),RGB(57,20,175)};
*/
void writeLine(size_t width, unsigned char* state, unsigned char* buff, FILE* file){
  int i;
  unsigned char* dest = buff;
  for(i = 0; i < width; i++){
    *((int*)dest) = state[i]? RGB(0,0,0):RGB(255,255,255); 
    /*    *((int*)dest) = colors[state[i]&15]; //Uncomment this too */
    dest += 3;
  }
  width *= 3;
  size_t rem = width & 3;
  width -= rem;
  fwrite(buff, 4, width >> 2, file);
  fwrite(buff + width, 1, rem, file);
}

void impulse(size_t width, unsigned char* state,int type){
  state[(width>>type)-1] = 1;
}

void binomial(size_t width, unsigned char* state, unsigned char modulo){
  int i;
  for(i=0;i<width;i++){
    state[i] += state[i+1];
    if(modulo <= state[i])
      state[i] -= modulo; 
  }
}

void trinomial (size_t width,unsigned  char* state,unsigned  char modulo){
  unsigned char old=0;
  int i;
  for(i=0;i<width;i++){
    unsigned char tmp = state[i];
    unsigned char new = tmp + old + state[i+1];
    if(modulo <= new){
      new -= modulo;
      if(modulo <= new)
	new -= modulo;
    }
    state[i] = new;
    old = tmp;
  }
}

void quartic(size_t width, unsigned char* state, unsigned char modulo){
  int i;
  for(i=0;i<width;i++){
    unsigned char new = state[i];
    int j;
    for(j=1; j<4;j++){
      new += state[i+j];
      if(modulo <= new)
	new -= modulo;
    }
    state[i] = new;;
  }
}


void (*triangles[3])(size_t,unsigned char*,unsigned char) = {binomial,trinomial,quartic};

int main(int argc, char** argv){
  unsigned int i, wide, high, degree, modulus;
  unsigned char* state, *image;
  void (*update)(size_t,unsigned char*,unsigned char);
  FILE* img;
  
  if(argc < 5){
    puts("usage: <filename.ppm> <multinomial degree> <modulus> <image size, \"500x400\">");
    exit(1);
  }

  degree  = atoi(argv[2]);
  if(4 < degree || degree < 2){
    puts("Invalid degree");
    exit(1);
  }

  modulus = atoi(argv[3]);
  img = fopen(argv[1],"w");
  update = triangles[degree-2];


  i = sscanf(argv[4],"%ux%u",&wide,&high);
  if(i < 2){
    puts("Invalid image dimensions");
    exit(1);
  }

  allocateImageData(wide, &state, &image);
  impulse(wide, state, degree & 1);
  writeHeader(img, wide, high);

  for(i=0; i<high; i++){
    writeLine(wide, state, image, img);
    update(wide, state, modulus);
  }

  fclose(img);
  return 0;
}
