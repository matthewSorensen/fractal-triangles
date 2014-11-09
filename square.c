#include<stdio.h>
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

void vertex(int i, int j, int k, int n){
  float x = (float) i - ((float) n) / 2.0;
  float y = (float) j - ((float) n) / 2.0;
  float z = (float) k - ((float) n) / 2.0;
  printf("vertex %f %f %f\n", x, y, z);
}

void square(int i, int j, int k, int n){
  float half = ((float) n) / -2.0;
  float vect[3] = {half + (float) i, half + (float) j, half + (float) k};
  float delta[18] = {1.0, 0, 1.0, 1.0, 0, 0, 1.0, 1.0, 0,
		    1.0, 1.0, 1.0, 1.0, 1.0, 0, 1.0, 0, 1.0};
  // Print half of the thingy
  for(int rot = 0; rot < 9; rot += 3){
    for(int half = 0; half < 18; half += 9){
      printf("facet normal 0.0 0.0 0.0\n");
      printf("outer loop\n");
      int offset = 0;
      for(offset = 0; offset < 9; offset += 3){
	printf("vertex ");
	for(int ii = 0; ii < 3; ii++){
	  int rotated = rotations[rot + ii];
	  printf("%f ",vect[rotated]+delta[half + offset + rotated]);
	}
	printf("\n");
      }
      printf("end loop\n");
      printf("end facet\n");
    }
  }
}

int main(int argc, char** argv){
  int i;
  int j;
  int k;
  int n = 9;
  // Now we take each cube and generate triangles...

  printf("solid cube\n");

  for(i = -1; i < n; i++){
    for(j = -1; j < n; j++){ 
      for(k = -1; k < n; k++){
	int status = is_in(i,j,k,n);

	if(status ^  is_in(i+1,j,k,n)){
	  square(i,j,k,n);
	}

      }
    }
  }

  printf("endsolid cube\n");

  return 0;
}

