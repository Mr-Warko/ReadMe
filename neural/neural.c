#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void MultMat(double **imput,double **weigth, size_t len,size_t len2, double **array) 
{
 // printf("%zu \n",len3); 
  for(size_t i = 0; i < len;++i)
  { 
     for(size_t j = 0; j <len2 ; ++j)
     { 
        for(size_t k =0; k < len2; ++k)
        { 
          array[i][j] += imput[i][k] * weigth[k][j];
        }
  //      printf("mat value : %f\n", array[i][j]);
     }
  }
}
double sigmoid(double z)
{
  return 1 / (1 +(exp(-z)));
}

double **applySigmo(double **array, size_t len,size_t len2)
{
  for(size_t i = 0; i < len; ++i)
    {
      for(size_t j = 0; j < len2 ; ++j)
	{
	  array[i][j] = sigmoid(array[i][j]);
	  printf("sigmo %f\n",array[i][j]);
	}
    }
  printf("\n 2");
  return array;
}

void init(int size[]) // size -> length = 3
{
  int  a = size[0], b = size[1],  c = size[2], d = size[3];
//  double **biais = malloc(b * sizeof(double*));

  double **imput = malloc(a * sizeof(double*));  
  double **weigth = malloc(b * sizeof(double*));
  double **weigth2 = malloc(a * sizeof(double*));
  double **output = malloc(a * sizeof(double*));
  double **output2 = malloc(a * sizeof(double*));
  srand(time(NULL));
   
  for(int i = 0 ; i < a; ++i)
  {
    imput[i] = (double*) malloc (b * sizeof(double));
    output[i] = (double*) malloc(c * sizeof(double));
    output2[i] = (double*) malloc(c * sizeof(double));
    for(int j = 0; j < b; ++j)
    {  
      imput[i][j] = (double)rand()/(double)RAND_MAX;
      if (imput[i][j] > 0.5)
        imput[i][j] = 1.0;
      else 
        imput[i][j] = 0.0;                                                       //A FIXER
 //     printf("imput value%f\n ", imput[i][j]); 
    }
  }  
  for(int i = 0; i < b; ++i)
  {	  
    weigth[i] = (double*) malloc(a * sizeof(double));
    for(int k = 0 ; k < a; ++k)    
    {
      weigth[i][k] = (double)rand() / (double)RAND_MAX;
   //   printf("weigth value%f\n",weigth[i][k]);
    } 
  }
  for(int i = 0 ; i < c; ++i)
  { 
    weigth2[i] = (double*) malloc(d* sizeof(double));
    for(int j = 0; j < d; ++j)
    {
      weigth2[i][j] = (double)rand() /(double)RAND_MAX;
//      printf("weigth2 value%f\n",weigth2[i][j]);
    }
  }
  MultMat(imput, weigth,a,b,output);
  free(imput);
  free(weigth); 
  applySigmo(output,a,c) ; 
  MultMat(output,weigth2,a,c,output2);
  free(output);
  free(weigth2);
  applySigmo(output2,a,d);  // free ouput2 apres avoir recup valeur
}
int main()
{
  int test[4] = {3,2,3,1}; //{imput,weigth,weight[0]/hidden,weigth2} 
  init(test);
  return 0;
} 
