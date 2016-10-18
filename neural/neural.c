#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void MultMat(double** imput,double** weigth, size_t len,size_t len2, double** array) 
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
  return 1 / (1 + exp(-z));
}

double** applySigmo(double **array, size_t len,size_t len2)
{
  for(size_t i = 0; i < len; ++i)
    {
      for(size_t j = 0; j < len2 ; ++j)
	{
	  array[i][j] = sigmoid(array[i][j]);
	  printf("Sigmo: %f\n",array[i][j]);
	}
    }
  printf("\n");
  return array;
}

double SigmoPrime(double z)
{
  return exp(-z) / ( pow(1 + exp(-z), 2));
}
double** costFunction(double** array, double** y,  size_t len, size_t len2)
{
  for(size_t i = 0; i < len; ++i)
  {
    for(size_t j = 0; j < len2; ++j)
    {
      array[i][j] = 0.5 * pow((array[i][j] - y[i][j]),2); 
    }
  }
  return array;
}

double** Transpose(double** array, size_t len, size_t len2)
{
  double**Mat = malloc(len2 * sizeof(double*));
  for(size_t i = 0; i < len2; ++i)
  {
    Mat[i] = malloc(len * sizeof(double));
    Mat[i][0] = array[0][i];
  }
  for(size_t i = 1 ; i < len2; ++i)
  {
    for(size_t j = 0; j < len ; ++j)
    {
      Mat[j][i] = array[i][j];
    }
  }
return Mat;
}
void MatImput(double** imput)  // Imput pour la porte Xor
{
  imput[0][0] = 0.0;
  imput[0][1] = 0.0;
  imput[1][0] = 0.0;
  imput[1][1] = 1.0;
  imput[2][0] = 1.0;
  imput[2][1] = 0.0;
  imput[3][1] = 1.0;
  imput[3][2] = 1.0;
 /* y[0][0] = 0.0;  // valeur qu'on doit obtenir.
  y[1][0] = 1.0;
  y[2][0] = 1.0;
  y[3][0] = 0.0 */
}

void init(int size[]) // size -> length = 4
{
  int  a = size[0], b = size[1],  c = size[2], d = size[3];

  double** imput = malloc(a * sizeof(double*));
  double** weigth = malloc(b * sizeof(double*));
  double** weigth2 = malloc(a * sizeof(double*));
  double** hidden = malloc(a * sizeof(double*));
  double** output = malloc(a * sizeof(double*));

  srand(time(NULL));
   
  for(int i = 0 ; i < a; ++i)
  {
    imput[i] = (double*) malloc (b * sizeof(double));
    hidden[i] = (double*) malloc(c * sizeof(double));
    output[i] = (double*) malloc(c * sizeof(double));
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
  MatImput(imput);
  MultMat(imput, weigth,a,b,hidden);
  free(imput);
  free(weigth);
  applySigmo(hidden,a,c);
  MultMat(hidden,weigth2,a,c,output);
  free(hidden);
  free(weigth2);
  applySigmo(output,a,d);  // free ouput2 
}
int main()
{
  int test[4] = {4,2,3,1};
  init(test);
  return 0;
} 
