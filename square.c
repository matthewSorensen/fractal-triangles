#include<stdio.h>
#include <string.h>


unsigned int ones(int x, int n){
  unsigned int acc = 0;
  unsigned int bit = 1;

  if(x < 0 || n <= x) return 0xFFFFFFFFFFFF; // ...lots of ones...

  while(x > 0){
    int q = x / 3;
    int r = x - 3*q;

    if(r == 1) acc |= bit;
    bit = bit << 1;
    x = q;
  }

  return acc;
}

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


int rotations[18] = {0, 1, 2,   1, 0, 2,   2, 1, 0};
float delta[18] = {1.0, 0, 1.0, 1.0, 0, 0, 1.0, 1.0, 0,
		   1.0, 0, 1.0, 1.0, 1.0, 0, 1.0, 1.0, 1.0};
float normal[3] = {0,0,0};
int normal_rot[18] = {1,0,2, 0,1,2,  0,1,2, 1,0,2, 0,1,2, 1,0,2 };

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

int in(int zones, int yones, int x, int n){
  if(x < 0 || n <= x) return 0;

  if(zones & yones) return 0;
  zones |= yones;
  while(x > 0){
    int q = x / 3;
    int r = x - q * 3;
    
    if(r == 1 && (zones & 1))
      return 0;
    zones = zones >> 1;
    x = q;
  }
  return 1;
}


int main(int argc, char** argv){
  int n = 27*9;
  // Now we take each cube and generate triangles...
  FILE* fp = fopen("test.stl","w");
  char header[84];
  memset(header, 0, 84);
  int count = 0;
  fwrite(&header,sizeof(char), 84, fp);
  for(int j = 0; j < n; j++){ 
    int zones = ones(j,n);

    for(int k = 0; k < n; k++){
      int yones = ones(k,n);

      if(zones & yones) continue;


      int i = -1;
      int status = in(zones,yones, i, n); // is_in(i,j,k,n);
      for(; i < n; i++){
	int next = in(zones, yones, i+ 1, n);
	if(status ^  next){
	  square(i,j,k,n,fp, status);
	  count += 6;
	}
	status = next;
      }
    }
  }

  fseek(fp,80,SEEK_SET);
  fwrite(&count, sizeof(int), 1, fp);
  fclose(fp);

  return 0;
}

