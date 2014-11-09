#include<stdio.h>

// This becomes somewhat annyoing
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

#define VERT(x,y,z) printf("vertex %f %f %f \n", (double) (x), (double) (y), (double) (z))

int main(int argc, char** argv){
  int i;
  int j;
  int k;
  int n = 27*9;
  // Now we take each cube and generate triangles...

  printf("solid cube\n");

  for(i = -1; i < n; i++){
    for(j = -1; j < n; j++){ 
      for(k = -1; k < n; k++){
	int status = is_in(i,j,k,n);

	if(status ^  is_in(i+1,j,k,n)){
	  // Print half of the thingy
	  printf("facet normal 0.0 0.0 0.0\n");
	  printf("outer loop\n");
	  VERT(i+1, j, k);
	  VERT(i+1, j+1, k);
	  VERT(i+1, j, k+1);
	  printf("end loop\n");
	  printf("end facet\n");
	  // and the other half
	  printf("facet normal 0.0 0.0 0.0\n");
	  printf("outer loop\n");
	  VERT(i+1, j+1, k+1);
	  VERT(i+1, j+1, k);
	  VERT(i+1, j, k+1);
	  printf("end loop\n");
	  printf("end facet\n");
	}


      }
    }
  }


  printf("endsolid cube\n");


  return 0;
}

