#include<stdio.h>
#include <string.h>

int is_in2(int x, int y){
  while(x > 0 && y > 0){
    int qx = x / 3;
    int qy = y / 3;
    int rx = x - 3 * qx;
    int ry = y - 3 * qy;
    if(rx == 1 && ry == 1)
      return 0;
    x = qx;
    y = qy;
  }
  return 1;
}
int is_in(int x, int y, int z, int n){
  // also put explicit bounds on the box here, 
  // so as to handle edge cases effectively
  if(x < 0 || y < 0 || z < 0) return 0;
  if(n <= x || n <= y || n <= z) return 0;
  return is_in2(x,y) && is_in2(z,y) && is_in2(x,z);
}


int rotations[9] = {0, 1, 2, 1, 0, 2, 2, 1, 0};
float delta[18] = {1.0, 0, 1.0, 1.0, 0, 0, 1.0, 1.0, 0,
		   1.0, 1.0, 1.0, 1.0, 1.0, 0, 1.0, 0, 1.0};
float normal[3] = {0,0,0};

void square(int i, int j, int k, int n, FILE* fp){
  float half = ((float) n) / -2.0;
  float vect[3] = {half + (float) i, half + (float) j, half + (float) k};
  // Print half of the thingy
  for(int rot = 0; rot < 9; rot += 3){
    for(int half = 0; half < 18; half += 9){
      // Three zeroes for the normal vector
      fwrite(&normal, sizeof(float), 3, fp);
      int offset = 0;
      for(offset = 0; offset < 9; offset += 3){
	// The three vector components, suitably permuted and offset
	for(int ii = 0; ii < 3; ii++){
	  int rotated = rotations[rot + ii];
	  float component = vect[rotated]+delta[half + offset + rotated];
	  fwrite(&component, sizeof(float), 1, fp);
	}
      }
      // the attribute thingy
      fputc(0, fp);
      fputc(0, fp);
    }
  }
}

int main(int argc, char** argv){
  int i;
  int j;
  int k;
  int n = 9;
  // Now we take each cube and generate triangles...
  FILE* fp = fopen("test.stl","w");
  char header[84];
  memset(header, 0, 84);
  int count = 0;
  fwrite(&header,sizeof(char), 84, fp);
  for(i = -1; i < n; i++){
    for(j = -1; j < n; j++){ 
      for(k = -1; k < n; k++){
	int status = is_in(i,j,k,n);

	if(status ^  is_in(i+1,j,k,n)){
	  square(i,j,k,n,fp);
	  count += 6;
	}

      }
    }
  }

  fseek(fp,80,SEEK_SET);
  fwrite(&count, sizeof(int), 1, fp);
  fclose(fp);

  return 0;
}

