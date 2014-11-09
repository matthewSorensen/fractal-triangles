#include<stdio.h>
#include <string.h>
#include <stdint.h>

unsigned int ones(int x){
  unsigned int acc = 0;
  unsigned int bit = 1;
  
  while(x > 0){
    int q = x / 3;
    int r = x - 3*q;
    
    if(r == 1) acc |= bit;
    bit = bit << 1;
    x = q;
  }
  
  return acc;
}

int in_set(int ones, int x){
  while(x > 0){
    int q = x / 3;
    int r = x - q * 3;
    
    if(r == 1 && (ones & 1))
      return 0;
    ones = ones >> 1;
    x = q;
  }
  return 1;
}



int rotations[18] = {0,1,2,   1,0,2,   2,1,0};
float delta[18] = {0.0, 0.0, 1.0,    0.0, 0.0, 0.0,    0.0, 1.0, 0.0,
		   0.0, 0.0, 1.0,    0.0, 1.0, 0.0,  0.0, 1.0, 1.0};
float normal[3] = {0,0,0};
int normal_rot[18] = {1,0,2, 0,1,2,  0,1,2,1,0,2,  0,1,2,1,0,2 };

void square(int i, int j, int k, int n, FILE* fp, int dir){
  float half = ((float) n) / -2.0;
  float vect[3] = {half + (float) i, half + (float) j, half + (float) k};
  if(dir){dir = 3;}
  // Print half of the thingy
  for(int rot = 0; rot < 9; rot += 3){
    for(int half = 0; half < 18; half += 9){
      // Three zeroes for the normal vector
      fwrite(&normal, sizeof(float), 3, fp);
      for(int offset = 0; offset < 3; offset++){
	int off = 3 * normal_rot[offset + dir + 2*rot];
	// The three vector components, suitably permuted and offset
	for(int ii = 0; ii < 3; ii++){
	  int rotated = rotations[rot + ii];
	  float component = vect[rotated]+delta[half + off + rotated];
	  fwrite(&component, sizeof(float), 1, fp);
	}
      }
      // the attribute thingy
      fputc(0, fp);
      fputc(0, fp);
    }
  }
}

typedef struct {
  char head[80];
  uint32_t count;
} stl_header;

int main(int argc, char** argv){
  int n = 27;
  FILE* fp = fopen("test.stl","w");
  
  // Write a dummy stl header out, just to occupy the correct 
  // amount of space at the start of the file.
  stl_header head;
  memset(&head, 0, sizeof(stl_header));
  fwrite(&head, sizeof(stl_header), 1, fp);
  
  for(int j = 0; j < n; j++){ 
    int zones = ones(j);
    for(int k = 0; k < n; k++){
      int yones = ones(k);
      if(zones & yones) 
	continue;
      yones |= zones;
      int status = 0; 
      for(int i = 0; i < n; i++){
	int next = in_set(yones, i);
	if(status ^  next){
	  square(i,j,k,n,fp, status);
	  head.count += 6;
	}
	status = next;
      }
      if(status){
	square(n,j,k,n,fp, status);
	head.count += 6;
      }
    }
  }

  // Seek to the begining of the file and write out a correct header
  fseek(fp,0,SEEK_SET);
  fwrite(&head, sizeof(stl_header), 1, fp);
  fclose(fp);
  
  return 0;
}

